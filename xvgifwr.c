/*
20050129	mvh	Adapted for use in the Conquest DICOM server (see //)
20050130	mvh	Made it ansi C
20050826	mvh	Made it reentrant to avoid gif file corruption in server
20051219	mvh	Fixed leak
*/

/*
 * xvgifwr.c  -  handles writing of GIF files.  based on flgife.c and
 *               flgifc.c from the FBM Library, by Michael Maudlin
 *
 * Contains: 
 *   WriteGIF(fp, pic, ptype, w, h, rmap, gmap, bmap, numcols, colorstyle,
 *            comment)
 *
 * Note: slightly brain-damaged, in that it'll only write non-interlaced 
 *       GIF files (in the interests of speed, or something)
 *
 */



/*****************************************************************
 * Portions of this code Copyright (C) 1989 by Michael Mauldin.
 * Permission is granted to use this file in whole or in
 * part for any purpose, educational, recreational or commercial,
 * provided that this copyright notice is retained unchanged.
 * This software is available to all free of charge by anonymous
 * FTP and in the UUNET archives.
 *
 *
 * Authors:  Michael Mauldin (mlm@cs.cmu.edu)
 *           David Rowley (mgardi@watdcsu.waterloo.edu)
 *
 * Based on: compress.c - File compression ala IEEE Computer, June 1984.
 *
 *	Spencer W. Thomas       (decvax!harpo!utah-cs!utah-gr!thomas)
 *	Jim McKie               (decvax!mcvax!jim)
 *	Steve Davies            (decvax!vax135!petsd!peora!srd)
 *	Ken Turkowski           (decvax!decwrl!turtlevax!ken)
 *	James A. Woods          (decvax!ihnp4!ames!jaw)
 *	Joe Orost               (decvax!vax135!petsd!joe)
 *****************************************************************/
 

#include <stdio.h>
#include <string.h>

#define MONO(rd,gn,bl) ( ((int)(rd)*11 + (int)(gn)*16 + (int)(bl)*5) >> 5)
#define XV_BITS	12    /* BITS was already defined on some systems */
#define MSDOS	1
#define HSIZE  5003            /* 80% occupancy */
#define MAXCODE(n_bits)     ( (1 << (n_bits)) - 1)

typedef long int        count_int;
typedef unsigned char	byte;
typedef unsigned long	u_long;
typedef unsigned char   char_type;

struct aap
{ unsigned long cur_accum;
  int cur_bits;
  int n_bits;                    /* number of bits/code */
  int maxcode;                   /* maximum code, given n_bits */
  int free_ent;              /* first unused entry */
  int clear_flg;
  int g_init_bits;
  FILE *g_outfile;
  int ClearCode;
  int EOFCode;
  int a_count;
  char accum[ 256 ];
  byte pc2nc[256],r1[256],g1[256],b1[256];
};

static void putword   (int, FILE *);
static void compress  (int, FILE *, byte *, int, struct aap *);
static void output    (int, struct aap *);
static void cl_block  (count_int *, struct aap *);
static void cl_hash   (count_int, count_int *);
static void char_out  (int, struct aap *);
static void flush_char(struct aap *);

/*************************************************************/
int WriteGIF(FILE *fp, byte *pic, int ptype, int w, int h, byte *rmap, byte *gmap, byte *bmap, 
             int numcols, int colorstyle, char *comment)
{ int   RWidth, RHeight;
  int   LeftOfs, TopOfs;
  int   ColorMapSize, InitCodeSize, Background, BitsPerPixel;
  int   i,j,nc;
  byte *pic8;
  byte  rtemp[256],gtemp[256],btemp[256];
  int   Interlace;

  struct aap *a = (struct aap *)malloc(sizeof(struct aap));
  memset(a, 0, sizeof(struct aap));

//  if (ptype == PIC24) {  /* have to quantize down to 8 bits */
//    pic8 = Conv24to8(pic, w, h, 256, rtemp,gtemp,btemp);
//    if (!pic8) FatalError("Unable to malloc in WriteGIF()");
//    rmap = rtemp;  gmap = gtemp;  bmap = btemp;  numcols=256;
//  }
//  else 
    pic8 = pic;

  Interlace = 0;
  Background = 0;

  for (i=0; i<256; i++) { a->pc2nc[i] = a->r1[i] = a->g1[i] = a->b1[i] = 0; }

  /* compute number of unique colors */
  nc = 0;

  for (i=0; i<numcols; i++) {
    /* see if color #i is already used */
    for (j=0; j<i; j++) {
      if (rmap[i] == rmap[j] && gmap[i] == gmap[j] && 
	  bmap[i] == bmap[j]) break;
    }

    if (j==i) {  /* wasn't found */
      a->pc2nc[i] = nc;
      a->r1[nc] = rmap[i];
      a->g1[nc] = gmap[i];
      a->b1[nc] = bmap[i];
      nc++;
    }
    else a->pc2nc[i] = a->pc2nc[j];
  }


  /* figure out 'BitsPerPixel' */
  for (i=1; i<8; i++)
    if ( (1<<i) >= nc) break;
  
  BitsPerPixel = i;

  ColorMapSize = 1 << BitsPerPixel;
	
  RWidth  = w;
  RHeight = h;
  LeftOfs = TopOfs = 0;
	
  if (BitsPerPixel <= 1) InitCodeSize = 2;
                    else InitCodeSize = BitsPerPixel;

  if (!fp) {
    fprintf(stderr,  "WriteGIF: file not open for writing\n" );
//    if (ptype == PIC24) free(pic8);
    free(a);
    return (1);
  }

//  if (DEBUG) 
//    fprintf(stderr,"WrGIF: pic=%lx, w,h=%dx%d, numcols=%d, Bits%d,Cmap=%d\n",
//	    (u_long) pic8, w,h,numcols,BitsPerPixel,ColorMapSize);

  if (comment && strlen(comment) > (size_t) 0)
    fwrite("GIF89a", (size_t) 1, (size_t) 6, fp);    /* the GIF magic number */
  else
    fwrite("GIF87a", (size_t) 1, (size_t) 6, fp);    /* the GIF magic number */

  putword(RWidth, fp);           /* screen descriptor */
  putword(RHeight, fp);

  i = 0x80;	                 /* Yes, there is a color map */
  i |= (8-1)<<4;                 /* OR in the color resolution (hardwired 8) */
  i |= (BitsPerPixel - 1);       /* OR in the # of bits per pixel */
  fputc(i,fp);          

  fputc(Background, fp);         /* background color */

  fputc(0, fp);                  /* future expansion byte */


  if (colorstyle == 1) {         /* greyscale */
    for (i=0; i<ColorMapSize; i++) {
      j = MONO(a->r1[i], a->g1[i], a->b1[i]);
      fputc(j, fp);
      fputc(j, fp);
      fputc(j, fp);
    }
  }
  else {
    for (i=0; i<ColorMapSize; i++) {       /* write out Global colormap */
      fputc(a->r1[i], fp);
      fputc(a->g1[i], fp);
      fputc(a->b1[i], fp);
    }
  }

  if (comment && strlen(comment) > (size_t) 0) {   /* write comment blocks */
    char *sp;
    int   i, blen;

    fputc(0x21, fp);     /* EXTENSION block */
    fputc(0xFE, fp);     /* comment extension */

    sp = comment;
    while ( (blen=strlen(sp)) > 0) {
      if (blen>255) blen = 255;
      fputc(blen, fp);
      for (i=0; i<blen; i++, sp++) fputc(*sp, fp);
    }
    fputc(0, fp);    /* zero-length data subblock to end extension */
  }


  fputc( ',', fp );              /* image separator */

  /* Write the Image header */
  putword(LeftOfs, fp);
  putword(TopOfs,  fp);
  putword(RWidth,   fp);
  putword(RHeight,  fp);
  if (Interlace) fputc(0x40, fp);   /* Use Global Colormap, maybe Interlace */
            else fputc(0x00, fp);

  fputc(InitCodeSize, fp);
  compress(InitCodeSize+1, fp, pic8, w*h, a);

  fputc(0,fp);                      /* Write out a Zero-length packet (EOF) */
  fputc(';',fp);                    /* Write GIF file terminator */

//  if (ptype == PIC24) free(pic8);

  free(a);
  if (ferror(fp)) return -1;
  return (0);
}


/******************************/
static void putword(int w, FILE *fp)
{
  /* writes a 16-bit integer in GIF order (LSB first) */
  fputc(w & 0xff, fp);
  fputc((w>>8)&0xff, fp);
}




/***********************************************************************/

/*
 * To save much memory, we overlay the table used by compress() with those
 * used by decompress().  The tab_prefix table is the same size and type
 * as the codetab.  The tab_suffix table needs 2**BITS characters.  We
 * get this from the beginning of htab.  The output stack uses the rest
 * of htab, and contains characters.  There is plenty of room for any
 * possible stack (stack used to be 8000 characters).
 */

/*
 * block compression parameters -- after all codes are used up,
 * and compression rate changes, start over.
 */

/*
 * compress stdin to stdout
 *
 * Algorithm:  use open addressing double hashing (no chaining) on the 
 * prefix code / next character combination.  We do a variant of Knuth's
 * algorithm D (vol. 3, sec. 6.4) along with G. Knott's relatively-prime
 * secondary probe.  Here, the modular division first probe is gives way
 * to a faster exclusive-or manipulation.  Also do block compression with
 * an adaptive reset, whereby the code table is cleared when the compression
 * ratio decreases, but after the table fills.  The variable-length output
 * codes are re-sized at this point, and a special CLEAR code is generated
 * for the decompressor.  Late addition:  construct the table according to
 * file size for noticeable speed improvement on small files.  Please direct
 * questions about this implementation to ames!jaw.
 */


/********************************************************/
static void compress(int init_bits, FILE *outfile, byte *data, int   len, struct aap *a)
{
  register long fcode;
  register int i = 0;
  register int c;
  register int ent;
  register int disp;
  register int hsize_reg;
  register int hshift;

  unsigned short codetab [HSIZE];
  count_int      htab [HSIZE];

  /*
   * Set up the globals:  g_init_bits - initial number of bits
   *                      g_outfile   - pointer to output file
   */
  a->g_init_bits = init_bits;
  a->g_outfile   = outfile;

  /* initialize 'compress' globals */
  memset((char *) htab,    0, sizeof(htab));
  memset((char *) codetab, 0, sizeof(codetab));
  a->free_ent = 0;
  a->clear_flg = 0;
  a->cur_accum = 0;
  a->cur_bits = 0;

  /*
   * Set up the necessary values
   */
  a->clear_flg = 0;
  a->maxcode = MAXCODE(a->n_bits = a->g_init_bits);

  a->ClearCode = (1 << (init_bits - 1));
  a->EOFCode = a->ClearCode + 1;
  a->free_ent = a->ClearCode + 2;

  a->a_count = 0;
  ent = a->pc2nc[*data++];  len--;

  hshift = 0;
  for ( fcode = (long)HSIZE;  fcode < 65536L; fcode *= 2L )
    hshift++;
  hshift = 8 - hshift;                /* set hash code range bound */

  hsize_reg = HSIZE;
  cl_hash( (count_int) hsize_reg, htab);       /* clear hash table */

  output(a->ClearCode, a);
    
  while (len) {
    c = a->pc2nc[*data++];  len--;

    fcode = (long) ( ( (long) c << XV_BITS) + ent);
    i = (((int) c << hshift) ^ ent);    /* xor hashing */

    if ( htab[i] == fcode ) {
      ent = codetab[i];
      continue;
    }

    else if ( (long)htab[i] < 0 )      /* empty slot */
      goto nomatch;

    disp = hsize_reg - i;           /* secondary hash (after G. Knott) */
    if ( i == 0 )
      disp = 1;

probe:
    if ( (i -= disp) < 0 )
      i += hsize_reg;

    if ( htab[i] == fcode ) {
      ent = codetab[i];
      continue;
    }

    if ( (long)htab[i] >= 0 ) 
      goto probe;

nomatch:
    output(ent, a);
    ent = c;

    if ( a->free_ent < (1<<XV_BITS) ) {
      codetab[i] = a->free_ent++; /* code -> hashtable */
      htab[i]    = fcode;
    }
    else
      cl_block(htab, a);
  }

  /* Put out the final code */
  output(ent, a);
  output(a->EOFCode, a);
}


/*****************************************************************
 * TAG( output )
 *
 * Output the given code.
 * Inputs:
 *      code:   A n_bits-bit integer.  If == -1, then EOF.  This assumes
 *              that n_bits =< (long)wordsize - 1.
 * Outputs:
 *      Outputs code to the file.
 * Assumptions:
 *      Chars are 8 bits long.
 * Algorithm:
 *      Maintain a BITS character long buffer (so that 8 codes will
 * fit in it exactly).  Use the VAX insv instruction to insert each
 * code in turn.  When the buffer fills up empty it and start over.
 */

static
unsigned long masks[] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
                                  0x001F, 0x003F, 0x007F, 0x00FF,
                                  0x01FF, 0x03FF, 0x07FF, 0x0FFF,
                                  0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };

static void output(int code, struct aap *a)
{
  a->cur_accum &= masks[a->cur_bits];

  if (a->cur_bits > 0)
    a->cur_accum |= ((long)code << a->cur_bits);
  else
    a->cur_accum = code;
	
  a->cur_bits += a->n_bits;

  while( a->cur_bits >= 8 ) {
    char_out( (int) (a->cur_accum & 0xff), a );
    a->cur_accum >>= 8;
    a->cur_bits -= 8;
  }

  /*
   * If the next entry is going to be too big for the code size,
   * then increase it, if possible.
   */

  if (a->free_ent > a->maxcode || a->clear_flg) {

    if( a->clear_flg ) {
      a->maxcode = MAXCODE (a->n_bits = a->g_init_bits);
      a->clear_flg = 0;
    }
    else {
      a->n_bits++;
      if ( a->n_bits == XV_BITS )
	a->maxcode = (1<<XV_BITS);
      else
	a->maxcode = MAXCODE(a->n_bits);
    }
  }
	
  if( code == a->EOFCode ) {
    /* At EOF, write the rest of the buffer */
    while( a->cur_bits > 0 ) {
      char_out( (int)(a->cur_accum & 0xff), a );
      a->cur_accum >>= 8;
      a->cur_bits -= 8;
    }

    flush_char(a);
	
    fflush( a->g_outfile );

  }
}


/********************************/
static void cl_block (count_int *htab, struct aap *a)             /* table clear for block compress */
{
  /* Clear out the hash table */

  cl_hash ( (count_int)HSIZE, htab );
  a->free_ent = a->ClearCode + 2;
  a->clear_flg = 1;

  output(a->ClearCode, a);
}


/********************************/
static void cl_hash(register count_int hsize, count_int *htab) /* reset code table */
{
  register count_int *htab_p = htab+hsize;
  register long i;
  register long m1 = -1;

  i = hsize - 16;
  do {                            /* might use Sys V memset(3) here */
    *(htab_p-16) = m1;
    *(htab_p-15) = m1;
    *(htab_p-14) = m1;
    *(htab_p-13) = m1;
    *(htab_p-12) = m1;
    *(htab_p-11) = m1;
    *(htab_p-10) = m1;
    *(htab_p-9) = m1;
    *(htab_p-8) = m1;
    *(htab_p-7) = m1;
    *(htab_p-6) = m1;
    *(htab_p-5) = m1;
    *(htab_p-4) = m1;
    *(htab_p-3) = m1;
    *(htab_p-2) = m1;
    *(htab_p-1) = m1;
    htab_p -= 16;
  } while ((i -= 16) >= 0);

  for ( i += 16; i > 0; i-- )
    *--htab_p = m1;
}


/******************************************************************************
 *
 * GIF Specific routines
 *
 ******************************************************************************/

/*
 * Add a character to the end of the current packet, and if it is 254
 * characters, flush the packet to disk.
 */
static void char_out(int c, struct aap *a)
{
  a->accum[ a->a_count++ ] = c;
  if( a->a_count >= 254 ) 
    flush_char(a);
}

/*
 * Flush the packet to disk, and reset the accumulator
 */
static void flush_char(struct aap *a)
{
  if( a->a_count > 0 ) {
    fputc(a->a_count, a->g_outfile );
    fwrite(a->accum, (size_t) 1, (size_t) a->a_count, a->g_outfile );
    a->a_count = 0;
  }
}	
