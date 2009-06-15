/*
19990317	ljz	NKI-specific code
19990903	ljz	Implemented NKI MaxRowColumn VR in DCO
19990906	ljz	Improved implementation of MaxRowColumn
19991022	ljz	Changed the root of NKI-made UIDs to the official one supplied by
			Medical Connections(UK): 1.2.826.0.1.3680043.2.135.1066.xxxxxxxxxx
19991122	ljz	Added NKI-CompressionMode 2: safer, with CRC checks
20000131	ljz	Regardless the FileCompressMode in Dicom.ini, always send images
			compressed (mode=2) when communicating with an NKI application.
20001105 	mvh     Fixed where malloc and new were mixed (vr->data)
20001106        mvh     Use delete [] operation for vr->Data
20001106        mvh     Temporary fix of BC leak in CompressNKI and DecompressNKI
20001120        mvh     Fix bug - vr->data was reallocated without changing vr->length
20001127	ljz	Fix: Crashes happened when more than one 'ServerChildThread' was
			active. m_pDCO is wrongly shared by all threads !!
20010424        mvh     Plugged in faster and safer decompress
20010426	ljz	Fixed leak when connection is aborted
20010429        mvh     Interleaved CRC computation with compress code (factor 2 faster)
                        Added GetADDO to allow read ahead within calling program
20010501        mvh     Merged ljz's change of 20010426                      
20010502        mvh     Added extrabytes pointer to Read and RetrieveOn
20010502        mvh     Added vr(9999,0300)=SliceLimit: send at most so many slices
20010502        mvh     Changed this vr to (9999,0500)=SliceLimit
20011219	ljz	Added extra check in case other manufacturers than NKI use
			VR (0x7fe0, 0x0010), such as ACUSON
20020317        mvh     Replace true by TRUE
20020415        mvh     Added error handling on ProcessDDO
20020429 	mvh	fixed reporting of number of complete slices (now starts with 1)
		        return cancel status if C_STORE failed
20020609        mvh     Added compressor modes 3 and 4
20020609        mvh     Added VR (9999,0600): MaxCompressionLevel (default 2)
                        Note: This level (max of 4) is also used when downsizing
20020613	ljz	Exported DecompressNKI and CompressNKI
20020619	ljz	Damn! ELSCINT uses (0x7fe0, 0x0010) too (see modification on 20011219)
			Extra check on zero-length pixeldata
20020930        mvh     Fixed SliceLimit VR
20021016	mvh	Removed __BORLANDC__ language construct - to fix downsize with BC55
20021115	mvh	Added Generic style retrieve classes; print error messages from c-move
20030522	ljz	Added function ComputeCRC
20030605	ljz	Fix in DecompressNKI: dgate crashed when DCMTK's 'storescu.exe' wants
			to send an NKIcompressed image to dgate; it refuses to send the pixeldata!
20030701        mvh     QualifyOn now also has compression parameter; transfer syntax work in progress
20030703        mvh     Tested recompress functions
20030704        mvh     Disabled decompression in ProcessDDO for non-nki clients; is done by recompress
                        When downsizing use DecompressImage(); also supports JPEG; Removed MaybeRecompress
			Recompression for NKI clients only when downsizing; otherwise as specified in acrnema.map
20030705        mvh     Moved recompression out of ProcessDDO into StandardRetrieveNKI::Read
			Skip recompression to same level as incoming
20030706        mvh     Optional filename parameter for recompress (not used yet); set env for offis tools
20030706	mvh	Attach VRType to PDU's for implicit little endian support
20030709	mvh	Removed DCMDICPATH; is not needed
20030819        mvh     Allow longer filenames
20030905        mvh     Note: recompressfile; DecompressImageFile and CompressJPEGImageFile do not allow spaces in filename
20040401	mvh	Added Changed and ActualMode flags to compress routines; only give compress message if something changed
			Set maxcompression to 4 for NKI send
20040403	mvh	Added own SaveDICOMDataObject as in dgate.cpp
20040406	ljz	Also copy TypeCode in DecompressNKI when converting (0x7fdf, 0x0010) to (0x7fe0, 0x0010)
			Make TypeCode of (0x7fdf, 0x0010) OW instead of OB, because OFFIS refuses to compress OB
20040406	mvh	Always use EXPLICIT syntax for jpeg, fix v2 always use DUMP
			Use -F +ti option for dcmdjpeg for writing to v2: creates raw dump, implicit
			Refuse to jpeg compress V2 files and images != 16 bits; \n is all messages
			Fix error handling of CompressJpegImage
20040530        mvh     Removed unused variable
20040722	mvh	Added "nj" compression: leaves JPEG as is; else MAXNKICOMPRESSION
20041101	ljz	Added built-in JPEG and RLE decompressor
20041102	mvh	Fix problem with external JPEG compression: clear transfer syntax prior 
			to save for OFFIS tools with DICOM_CHAPTER_10_EXPLICIT
20041103	mvh	Added k1,k2,k4,k8 compressors: downsize to 1024,512,256,128 pixels if bigger to start with
20041112	mvh	Called AE SERVER~xx in C-MOVE lets xx override outgoing compression as well 
			(e.g., call CONQUESTSRV1~k4 to always get images downsized to 256x256)
20050102	mvh	Added NOINTJPEG flag to allow compile through total.cxx
20050103	mvh	Changed include file to lower case (for linux)
20050108	mvh	Adapted for linux compile (work in progress)
20050109	mvh	Added configurable TCPIPTimeOut
20050118	mvh	replaced thread local storage under linux with variables in object
20050119	mvh	added rudimentary support to start external (de)compressors under linux
20050121	mvh	Changed filename to lower case
20050122	mvh	Improved external jpeg support under Linux: load dictionary, wait until ready
20050130	mvh	Added To8BitMonochrome and ToGif
20050204	mvh	Small fix in ToGif for non-square images; also interpret RescaleIntercept as float
20050205	mvh	Added counters for compression activity
20050206	mvh	Optimized downsizing: do not decompress if no downsizing to occur
20050206	mvh	Note: replaced thread local storage under linux with variables in object is INCORRECT
			Under linux, the server now can also serve one at a time
			Fix for color to gif; added limited auto scaling in 16 bits to gif
20050211	mvh	Removed need for thread local storage
20051210	mvh	Added "vX" compression (same as as is)
20051217	mvh	Use system() to call on offis apps in linux (now works)
20051217   	mvh   	Use new VR::ReAlloc method to replace data in VR; fixed 2 float warnings
20051229   	mvh   	Fixed several leaks in (jpeg)compression code; changed alloc for nki compression
20051230	mvh	Added +sr option to dcmcjpeg: compress without changing rescaleslope/intercept
			This option is required for our viewers: slices must all have same rescale
20060219	mvh	Removed warning in writegif
20060222	mvh	Added extra checks (not on ID strings) to pass as NKI compressed data
20060311	mvh	dcmcjpeg opts string got truncated (thanks Rainer Libowski)
			fixed unused CompressJPEGImageFile (wrong exe called)
			external decompress temporary files now in printer_files (thanks Rainer Libowski)
20060324	mvh	Added StripGroup2 option to recompress; strip group 2 from outgoing c-stores if not compression "as" or "is"
20060328	mvh	Use IsAbstractSyntaxAccepted(iUID) to test if host accepts image
20060402	mvh	Fix crash when recieving or dropping jpeg compressed color images
			Consider not removing transfer syntax (0x0002, 0x0010) prior to dcmdjpeg/dcmcjpeg
			Would fix problem with external decoding color jpeg images
20060402	mvh	Changed IsAbstractSyntaxAccepted(iUID) test back (thanks Andrej Savelov)
20060405	mvh	That was not the problem
20060618	mvh	Tested IsAbstractSyntaxAccepted - seems to work. Removed need for min()
20060619	mvh	Do not remove transfer syntax before decompression (20060402 suggestion)
			Maintain original object when decompression fails, changed wording of message
			Only complain about internal decompressor for jpeg
20070104	mvh	Export automatic SaveDICOMDataObject
20070113	mvh	Never delete transfer syntax (was left in CompressJpegImage, solves coding error in jpeg store)
	                Tested with $c server debug command
20070210	mvh	Export MaybeDownsize and check on 0 for passed size
20070307	mvh	Added several debug prints in retrieve, e.g. at bail out because connection dropped
20070308	bcb 	removed #include <wait.h> from DARWIN
20070314	mvh	Send 0000,1030 (MoveOriginatorAE) and 0000,1031 (MoveOriginatorMessageID) in SS->Write as result of Move
20070315	mvh	Merged bcb DARWIN stuff; Set TCPIPTimeOut for PDU that moves images
20070316        mvh     Added (color) ToBMP for convert_to_bmp; added level and window to ToGif and ToBMP
20070407        mvh     Use ~p1..9A..Z to override C-MOVE port (adds 1..36 to port number)
20070902        mvh     WriteGif now cpp
20071027	mvh	Added quotes around filenames passed to dcmdjpeg and dcmcjpeg: avoids errors with spaces in paths
20071101	mvh	Enabled JPEG compression and decompression of non-16 bit data
20071104	mvh	Added DecompressNon16BitsJpeg (default TRUE)
20071105	mvh	Restore image when dcmc/djpeg failed to start
20080103	mvh	Added mode 'uj': leave jpeg as is, otherwise uncompress
			Added DecompressImage to recompress()
20080820	mvh	Solaris fix (do not use variable name SS)
*/

#define bool BOOL
#define true TRUE
#define false FALSE

#include "dicom.hpp"
#include "nkiqrsop.hpp"
#include "dgate.hpp"
#include <time.h>

int	TCPIPTimeOut = 300;				// timeout for TCP/IP in seconds

int	NKIPrivateCompress=0;				// counters
int	NKIPrivateDecompress=0;
int	DownSizeImage=0;
int	DecompressJpeg=0;
int	CompressJpeg=0;
int	DecompressedRLE=0;
int	RecompressTime=0;

#define MAXNKICOMPRESSION 4

static const unsigned long CRC32_table[256] = {
      0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
      0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
      0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
      0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
      0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
      0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
      0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
      0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
      0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
      0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
      0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
      0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
      0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
      0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
      0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
      0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
      0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
      0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
      0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
      0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
      0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
      0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
      0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
      0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
      0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
      0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
      0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
      0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
      0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
      0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
      0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
      0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
      0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
      0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
      0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
      0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
      0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
      0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
      0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
      0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
      0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
      0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
      0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

/* Following function used in 'dgate.cpp' when executing the commandline switch
   '-fa'. This is when a dicomfile has to be stored in the dicomserver.
   The problem is that filenames generated in 'dgatefn.cpp' may be not unique
   if many files are dropped in the dicomserver. The static 'counter' intended
   to solve this problem is reset to zero each time dgate is launched.
   Now this 'counter' is initialized with a CRC of the filename.
*/
unsigned int ComputeCRC(char* pcData, int iNbChars)
{ int		i;
  unsigned int	uiCRC = 0;

  for (i=0; i<iNbChars; i++)
    uiCRC = CRC32_table[(unsigned char)uiCRC ^ (unsigned char)pcData[i]] ^ ((uiCRC >> 8) & 0x00FFFFFF);
  return uiCRC;
}

/* Read and cache UseBuiltInDecompressor and DecompressNon16BitsJpeg variable from dicom.ini 
*/

#ifndef	RootConfig
extern	char	RootConfig[];
#endif
#ifndef	ConfigFile
extern	char	ConfigFile[];
#endif

BOOL UseBuiltInDecompressor(BOOL *DecompressNon16BitsJpeg = NULL)
{ char			RootSC[64];
  char			Temp[128];
  static int		res=-1, dn16=1;

  if (res==-1)
  { GetPrivateProfileString ( RootConfig, "MicroPACS", RootConfig,
	  	(char*) RootSC, 64, ConfigFile);

    GetPrivateProfileString ( RootSC, "UseBuiltInDecompressor", "1",
		(char*) Temp, 128, ConfigFile);
    res = atoi(Temp);

    GetPrivateProfileString ( RootSC, "DecompressNon16BitsJpeg", "1",
		(char*) Temp, 128, ConfigFile);
    dn16 = atoi(Temp);
  }

  if (DecompressNon16BitsJpeg) *DecompressNon16BitsJpeg = dn16;
  return(res);
}


/*******************  NKI specific Retrieve Classes *************************/

StandardRetrieveNKI::StandardRetrieveNKI()
{
}

BOOL	StandardRetrieveNKI	::	Read (
	PDU_Service			*PDU,
	DICOMCommandObject		*DCO,
	void				*ExtraBytes)
	{
	UID	MyUID, uid, iUID, AppUID ("1.2.840.10008.3.1.1.1");
	VR	*vr;
	DICOMDataObject	DDO;
	Array < DICOMDataObject *>	ADDO;
	UINT		Index;
	BYTE		IP [ 64 ], Port [ 64 ], ACRNema [ 17 ], MyACR[17], Compress[64];
	StandardStorage	*SStorage;
	DICOMDataObject	*iDDO;
	ExtendedPDU_Service	NewPDU;
	UINT16		Failed;
	int		iVrSliceLimit = 0xffffffff;
	int		status = 0;
	UID		TrnSyntaxUID;
//	int		ContextID;
	char		*AcceptedCompress, *Compression;
	char		Called[20], Calling[20];
	BOOL		StripGroup2;

	if (DCO)
		{
		vr = DCO->GetVR(0x9999, 0x0500);
	    	if (vr)
    			iVrSliceLimit = (unsigned int)(*((unsigned int*)vr->Data));
	  	}

	GetUID(MyUID);

	if( ! PDU )
		return ( FALSE );

	if( ! DCO )
		return ( FALSE );

	vr = DCO->GetVR(0x0000, 0x0002);
	SetUID(uid, vr);
	if (!(MyUID == uid))
		return ( FALSE );

	strcpy(Calling, (char *)(((AAssociateAC *)PDU)->CallingApTitle));
	while (Calling[strlen(Calling)-1]==' ') Calling[strlen(Calling)-1] = 0;

	if (! CMoveRQ :: Read (DCO, PDU, &DDO) )
		{
		SystemDebug.printf("Retrieve: wrong command\n");
		return ( FALSE ); // my SOP, but wrong command
		}

	NewPDU.AttachRTC(&VRType);
	NewPDU.SetTimeOut(TCPIPTimeOut);

	vr = DCO->GetVR(0x0000, 0x0600);
	if(!vr)
		{
		CMoveRSP :: Write ( PDU, DCO, 0xc001 , 0, 0, 0, 0, NULL );
		SystemDebug.printf("Retrieve: move destination missing\n");
		return ( TRUE );
		}

	memset((void*)ACRNema, 0, 17);
	if(vr->Length > 16)
		vr->Length = 16;
	memcpy((void*)ACRNema, vr->Data, (int) vr->Length);
	if(!vr->Length)
		{
		CMoveRSP :: Write ( PDU, DCO, 0xc002 , 0, 0, 0, 0, NULL );
		SystemDebug.printf("Retrieve: move destination empty\n");
		return ( TRUE );
		}
	if(ACRNema[vr->Length-1]==' ')
		ACRNema[vr->Length-1] = '\0';

	if(!QualifyOn(ACRNema, MyACR, IP, Port, Compress))
		{
		CMoveRSP :: Write ( PDU, DCO, 0xc003 , 0, 0, 0, 0, NULL );
		SystemDebug.printf("Retrieve: move destination not allowed or known\n: %s", MyACR);
		return ( TRUE );
		}


	if (! SearchOn (&DDO, &ADDO) )
		{
		CMoveRSP :: Write ( PDU, DCO, 0xc004 , 0, 0, 0, 0, NULL );
		SystemDebug.printf("Retrieve: move search failed\n");
		return ( TRUE );
		}

	if (iVrSliceLimit < 0xffffffff)
		while (ADDO.GetSize() > iVrSliceLimit)
			ADDO.RemoveAt(ADDO.GetSize()-1);

	// If the called AE looks like SERVER~j2, then the last 2 characters override the outgoing compression set in ACRNEMA.MAP
	strcpy(Called, (char *)(((AAssociateAC *)PDU)->CalledApTitle));
	while (Called[strlen(Called)-1]==' ') Called[strlen(Called)-1] = 0;
        Compression = strchr(Called, '~');
	if (!Compression) Compression = (char *)Compress; else Compression++;

	// If the called AE looks like SERVER~p2, then the last character overrides the outgoing port set in ACRNEMA.MAP
        if (Compression[0]=='P' || Compression[0]=='p')			// override port
		{
		sprintf((char *)Port, "%d", atoi((char *)Port)+Compression[1]-'0');
		Compression= (char *)Compress;				// use default compress
		}

	NewPDU.SetRequestedCompressionType((char *)Compression);	// propose compression type

	NewPDU.SetApplicationContext ( AppUID );
	NewPDU.SetLocalAddress ( MyACR );
	NewPDU.SetRemoteAddress ( ACRNema );
	
	// Add all the Abstract Syntaxs we need

	Index = 0;
	while ( Index < ADDO.GetSize() )
		{
		vr = ADDO.Get ( Index ) -> GetVR(0x0008, 0x0016);
		if(!vr)
			{
			delete ADDO.Get ( Index );
			ADDO.RemoveAt ( Index );
			}
		else
			{
			SetUID ( iUID, vr );
			NewPDU.AddAbstractSyntax ( iUID );
			++Index;
			}
		}

        NewPDU.SetTimeOut(TCPIPTimeOut);

	if (!NewPDU.Connect (IP, Port))
		{
		CMoveRSP :: Write ( PDU, DCO, 0xc005 , 0, 0, 0, 0, NULL );
		OperatorConsole.printf("Host '%s' did not accept the connection\n", ACRNema);
          	return ( TRUE );
		}


	Index = 0;
	Failed = 0;

	while ( Index < ADDO.GetSize() )
		{
		vr = ADDO.Get ( Index ) -> GetVR(0x0008, 0x0016);
		SetUID ( iUID, vr );

//		ContextID = NewPDU.GetPresentationContextID(iUID);
//		if ( !ContextID )

		if ( !NewPDU.IsAbstractSyntaxAccepted(iUID) )
			{
			++Failed;
			OperatorConsole.printf("Host '%s' will not accept image\n", ACRNema);
			// Remote end did not accept this UID
			}
		else
			{
			AcceptedCompress = NewPDU.GetAcceptedCompressionType(iUID);
			StripGroup2 = memicmp(AcceptedCompress, "as", 2)!=0 && memicmp(AcceptedCompress, "is", 2)!=0;

			if ( !RetrieveOn (ADDO.Get(Index), &iDDO, &SStorage, DCO, &ADDO, ExtraBytes))
				++Failed;
			else
				{
				// non NKI client
			        if (DCO==NULL)
					recompress(&iDDO, AcceptedCompress, "", StripGroup2);

				// NKI client has 2 vr's to control (re)compression
				// 9999,0600=MaxSupportedCompression (old, nki only)
				// 9999,0700=RequestedCompression (new, nki or jpeg)
			        else
					{
					char mode[16];
					int MaxCompression=4;

					// compression from transfer syntax negotiation
					strcpy(mode, AcceptedCompress);

					// optional requested custom compression
					vr = DCO->GetVR(0x9999, 0x0700);
						if (vr)
						{ 
						strncpy(mode, (const char*)vr->Data, vr->Length);
						mode[vr->Length] = 0;
						}

					vr = DCO->GetVR(0x9999, 0x0600);
					if (vr) MaxCompression = (unsigned int)(*((unsigned int*)vr->Data));
					if (MaxCompression > 4) MaxCompression = 4;

					// limit nki compression to MaxCompression
					if ((mode[0]=='n' || mode[0]=='N') && (mode[1]!='j' && mode[1]!='J'))
						if (atoi(mode+1) > MaxCompression)
							mode[1] = MaxCompression + '0';

					recompress(&iDDO, mode, "", StripGroup2);
					}

				if(!SStorage->Write(&NewPDU, iDDO, DCO->GetVR(0x0000, 0x0110), (unsigned char *)Calling))
					{
					//++Failed;
					// Remote end should accept this image.  if it did
					// not, then just bail out.  Probably means the
					// TCP/IP link has been dropped.
					Failed += (ADDO.GetSize() - Index);

					SystemDebug.printf("Retrieve: remote connection dropped after %d images, %d not sent\n", Index, Failed);

					/* LJ: This also happens when CqDicom has had 'enough'
					   e.g. when retrieving properties. It caused a small
					   leak
					*/
					status = 0xfe00;				// mvh 20020429: return cancel status
					delete iDDO;
					break;
					}
				delete iDDO;
				}
			}

		CMoveRSP :: Write ( PDU,
					DCO,
					0xff00, ADDO.GetSize() - Index - 1,
					(UINT16) Index+1, Failed, 0,			// mvh 20020429: added +1
					ADDO.Get ( Index ));


		delete ADDO.Get(Index);

		++Index;
		}


	CMoveRSP :: Write ( PDU, DCO, status, 0, (UINT16) Index, Failed, 0, NULL );	// mvh 20020429: replaced 0 by status

	// In case we broke out from above..

	while ( Index < ADDO.GetSize() )
		{
		delete ADDO.Get(Index);
		++Index;
		}

	
	return ( TRUE );
	}

BOOL	StandardRetrieveNKI	::	Write (
	PDU_Service		*PDU,
	DICOMDataObject		*DDO,
	BYTE			*ACRNema )
	{
	DICOMCommandObject	*DCO;
	DICOMDataObject		*RDDO;

	if ( ! PDU )
		return ( FALSE );

	if ( ! CMoveRQ :: Write ( PDU, DDO, ACRNema ) )
		return ( FALSE );

	CallBack ( NULL, DDO );

	DCO = new DICOMCommandObject;

	while ( PDU->Read ( DCO ) )
		{
		RDDO = new DICOMDataObject;

		if (! (CMoveRSP :: Read ( DCO, PDU, RDDO) ) )
			{
			return ( FALSE );
			}
		if ( DCO->GetUINT16(0x0000, 0x0800) == 0x0101)
			{
			CallBack ( DCO, NULL );
			delete RDDO;
			if ( DCO->GetUINT16(0x0000, 0x0900) != 0x0000)
				{
				VR *vr;
				while (vr = DCO->Pop())
					{
					delete vr;
					}
				delete DCO;
				return ( FALSE );
				}
			delete DCO;
			return ( TRUE );
			}
		CallBack ( DCO, RDDO );
		delete RDDO;
		delete DCO;
		DCO = new DICOMCommandObject;
		}

	delete DCO;
	return ( FALSE );
	}

BOOL	PatientRootRetrieveNKI	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.826.0.1.3680043.2.135.1066.5.1.4.1.2.1.2" );
	return ( TRUE );
	}

BOOL	StudyRootRetrieveNKI	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.826.0.1.3680043.2.135.1066.5.1.4.1.2.2.2" );
	return ( TRUE );
	}

BOOL	PatientStudyOnlyRetrieveNKI	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.826.0.1.3680043.2.135.1066.5.1.4.1.2.3.2" );
	return ( TRUE );
	}

BOOL	PatientRootRetrieveGeneric	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.840.10008.5.1.4.1.2.1.2" );
	return ( TRUE );
	}

BOOL	StudyRootRetrieveGeneric	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.840.10008.5.1.4.1.2.2.2" );
	return ( TRUE );
	}

BOOL	PatientStudyOnlyRetrieveGeneric	::	GetUID ( UID &uid )
	{
	uid.Set ( "1.2.840.10008.5.1.4.1.2.3.2" );
	return ( TRUE );
	}

/************ NKI specific processing of a loaded image *************************/

typedef struct
{
  unsigned int iOrgSize;	/* NOTE: this is the number of short pixels !!!! */
  unsigned int iMode;
  unsigned int iCompressedSize;	/* in bytes, not pixels */
  unsigned int iOrgCRC;
  unsigned int iCompressedCRC;	/* Excluding this header */
} NKI_MODE2;


/* coder for NKI private compressed pixel data VR 0x7fdf,0x0010
   arguments: dest    = (in) points to area where compressed destination data is written (byte)
              src     = (in) points to uncompressed source data (short)
              npixels = (in) number of pixels to compress
              iMode   = (in) type of compression (1 .. 4)

   The return value is the number of bytes in the compressed data (maximal 3*npixels+10, typical 0.52*npixels)

   if iMode == 1 then
   - The first 4 bytes contain the number of short-int pixels
   - The following 4 bytes contain iMode=1
   - The rest is the compressed image

   if iMode == 2 then
   - The first 4 bytes contain the number of short-int pixels
   - The following 4 bytes contain iMode=2
   - The following 4 bytes contain the size of the compressed image (in bytes)
   - The following 4 bytes contain the CRC of the original image
   - The following 4 bytes contain the CRC of the compressed image
   - The rest is the compressed image
   - The compressed size will be even (padded by a zero if necessary).

   if iMode == 3 then
   - The first 4 bytes contain the number of short-int pixels
   - The following 4 bytes contain iMode=3
   - The rest is the compressed image, including 4 bit differences

   if iMode == 4 then
   - The first 4 bytes contain the number of short-int pixels
   - The following 4 bytes contain iMode=4
   - The following 4 bytes contain the size of the compressed image (in bytes)
   - The following 4 bytes contain the CRC of the original image
   - The following 4 bytes contain 0
   - The rest is the compressed image, including 4 bit differences
   - The compressed size will be even (padded by a zero if necessary).
*/

// optimized settings for the 4 bit run compressor (mode 3 and 4)

#define MINZEROS 5		// shortest RLE (2 byte overhead, but breaks 4bit run)
#define MIN4BIT  6		// shortest 4 bit run (6 bytes compressed to 5 bytes)

// This internal routine converts an 8 bit difference string into a 4 bit one
static signed char *recompress4bit(int n, signed char *dest)
{ signed char *p, *q;
  int val;

  n = n & 0xfe;
  dest -= n;
  p = dest;
  val = (((int)p[0])<<4) | (p[1]&15);
  p += 2;
  *dest++ = 0xc0;
  *dest++ = n;
  q = dest++;
  n -= 2;
  while(n>0)
  { *dest++ = (((int)p[0])<<4) | (p[1]&15);
    p += 2;
    n -= 2;
  }
  q[0] = val;

  return dest;
}

int nki_private_compress(signed char  *dest, short int  *src, int npixels, int iMode)
{ unsigned long		iCRC;
  unsigned long		iCRC2;
  register int		val, i, j;
  NKI_MODE2*		pHeader = (NKI_MODE2*)dest;

  NKIPrivateCompress++;

  /* Up till now only Mode=1 .. 4 are supported */
  if ((iMode < 1) || (iMode > 4))
    return 0;

  /* Create the header */
  pHeader->iOrgSize = npixels;
  pHeader->iMode = iMode;
  switch (iMode)
  { case 1:
      dest += 8;
      break;
    case 2:
      dest += sizeof(NKI_MODE2);
      break;
    case 3:
      dest += 8;
      break;
    case 4:
      dest += sizeof(NKI_MODE2);
      break;
  }

  /* Create the compressed image */

  if (iMode == 1)
  { *(short int *)dest = *src; 
    dest+=2;

    npixels--;

    do
    { val = src[1] - src[0];
      src++;
  
      if (val == 0)                            /* run length-encode zero differences */
      { for (i=2;; i++)
        { if (i>=npixels || src[i-1]!=src[-1] || i==256)
          { if (i==2) 
              *dest++=0;
            else      
            { *dest++  =  0x80;
              *dest++  = (i-1);
              npixels -= (i-2);
              src     += (i-2);
            }
            break;
          }
        }
      }    
      else if (val >= -64 && val <= 63)         /* small difference coded as one byte */
      { *dest = val;
        dest++;
      }
      else if (val >= -0x3F00 && val <= 0x3EFF) /* large differences coded as two bytes */
      { dest[0] = (val>>8) ^ 0x40;
        dest[1] = val;
        dest+=2;
      }
      else                                      /* if very large differences code abs val as three bytes */
      { *dest++ = 0x7F;
        *dest++ = src[0]>>8; 
        *dest++ = src[0];
      }
    }
    while (--npixels);
  }

  else if (iMode == 2)
  { iCRC  = 0;
    iCRC2 = 0;

    *(short int *)dest = val = *src; 
    iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char) val    ] ^ ((iCRC2 >> 8));
    iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
    iCRC  = CRC32_table[(unsigned char)iCRC  ^ (unsigned char) val    ] ^ ((iCRC  >> 8));
    iCRC  = CRC32_table[(unsigned char)iCRC  ^ (unsigned char)(val>>8)] ^ ((iCRC  >> 8));
    dest+=2;
    npixels--;

    do
    { val = src[1] - src[0];
      src++;
      iCRC  = CRC32_table[(unsigned char)iCRC  ^ (unsigned char) src[0]    ] ^ ((iCRC  >> 8));
      iCRC  = CRC32_table[(unsigned char)iCRC  ^ (unsigned char)(src[0]>>8)] ^ ((iCRC  >> 8));

      if (val == 0)                            /* run length-encode zero differences */
      { for (i=2;; i++)
        { if (i>=npixels || src[i-1]!=src[-1] || i==256)
          { if (i==2) 
            { *dest++=0;
              iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ 0    ] ^ ((iCRC2 >> 8));
            }
            else      
            { *dest++  =  0x80;
              iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ 0x80 ] ^ ((iCRC2 >> 8));
              *dest++  = (i-1);
              iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (i-1)] ^ ((iCRC2 >> 8));
              npixels -= (i-2);

              for (j=0; j<i-2; j++)
              { src++;
                iCRC = CRC32_table[(unsigned char)iCRC  ^ (unsigned char) src[0]    ] ^ ((iCRC  >> 8));
                iCRC = CRC32_table[(unsigned char)iCRC  ^ (unsigned char)(src[0]>>8)] ^ ((iCRC  >> 8));
              }
            }
            break;
          }
        }
      }    
      else if (val >= -64 && val <= 63)         /* small difference coded as one byte */
      { *dest = val;
        iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val     ] ^ ((iCRC2 >> 8));
        dest++;
      }
      else if (val >= -0x3F00 && val <= 0x3EFF) /* large differences coded as two bytes */
      { dest[0] = (val>>8) ^ 0x40;
        iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)dest[0] ] ^ ((iCRC2 >> 8));
        dest[1] = val;
        iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val     ] ^ ((iCRC2 >> 8));
        dest+=2;
      }
      else                                      /* if very large differences code abs val as three bytes */
      { dest[0] = 0x7F;
        iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ 0x7f                   ] ^ ((iCRC2 >> 8));
	val     = src[0];
        dest[1] = val>>8; 
        iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
        dest[2] = val;
        iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val     ] ^ ((iCRC2 >> 8));
	dest+=3;
      }
    }
    while (--npixels);

    pHeader->iCompressedSize = dest - (signed char*)pHeader - sizeof(NKI_MODE2);

    /* Pad it to get an even length */
    if (pHeader->iCompressedSize & 1)
    { *dest++ = 0;
      iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ 0] ^ ((iCRC2 >> 8));
      pHeader->iCompressedSize++;
    }

    pHeader->iOrgCRC        = iCRC;
    pHeader->iCompressedCRC = iCRC2;
  }

  /* Create the compressed image - compressor with added 4 bit run */
  
  else if (iMode == 3)
  { int n4bit=0;
    *(short int *)dest = *src; 
    dest+=2;
    npixels--;

    do
    { val = src[1] - src[0];
      src++;
  
      if (val == 0)                            	/* run length-encode zero differences */
      { for (i=2;; i++)
        { if (i>=npixels || src[i-1]!=src[-1] || i==256)
          { if (i<=MINZEROS) 		       	/* too short run -> write zeros */
            { for (j=0; j<i-1; j++)
              { *dest++=0;
                n4bit++;

                if(n4bit>=254)			/* maximum length 4 bit run */
                { dest  = recompress4bit(n4bit, dest);
                  n4bit = 0;
                }
              }
            }
            else      
            { if (n4bit>=MIN4BIT)		/* end (and write) 4 bit run */
                dest  = recompress4bit(n4bit, dest);

              n4bit=0;
              *dest++  =  0x80;
              *dest++  = (i-1);
            }

            npixels -= (i-2);
            src     += (i-2);
            break;
          }
        }
      }    
      else if (val >= -63 && val <= 63)         /* small difference coded as one byte */
      { if (val >= -8 && val <= 7)
        { *dest++ = val;
          n4bit++;

          if(n4bit>=254)			/* maximum length 4 bit run */
          { dest  = recompress4bit(n4bit, dest);
            n4bit=0;
          }
        }
        else if(n4bit>=MIN4BIT)			/* end and write 4 bit run */
        { j = val;
          dest  = recompress4bit(n4bit, dest);
          n4bit=0;
          *dest++ = j;
        }
        else
        { *dest++ = val;			/* end 4 bit run */
           n4bit  = 0;
        }
      }
      else if (val >= -0x3F00 && val <= 0x3EFF) /* large differences coded as two bytes */
      { j = val;

        if(n4bit>=MIN4BIT)			/* end (and write) 4 bit run */
          dest  = recompress4bit(n4bit, dest);

        n4bit=0;
        dest[0] = (j>>8) ^ 0x40;
        dest[1] = j;
        dest+=2;
      }
      else                                      /* if very large differences code abs val as three bytes */
      { j = src[0];

        if(n4bit>=MIN4BIT)			/* end (and write) 4 bit run */
          dest  = recompress4bit(n4bit, dest);

        n4bit=0;
        *dest++ = 0x7F;
        *dest++ = j>>8; 
        *dest++ = j;
      }
    }
    while (--npixels);
  }

  /* Create the compressed image - compressor with added 4 bit run and CRC */
  
  else if (iMode == 4)
  { int n4bit=0;
    iCRC  = 0;
    
    *(short int *)dest = val = *src; 
    iCRC  = CRC32_table[(unsigned char)iCRC  ^ (unsigned char) val    ] ^ ((iCRC  >> 8));
    iCRC  = CRC32_table[(unsigned char)iCRC  ^ (unsigned char)(val>>8)] ^ ((iCRC  >> 8));
    dest+=2;
    npixels--;

    do
    { val = src[1] - src[0];
      src++;
      iCRC  = CRC32_table[(unsigned char)iCRC  ^ (unsigned char) src[0]    ] ^ ((iCRC  >> 8));
      iCRC  = CRC32_table[(unsigned char)iCRC  ^ (unsigned char)(src[0]>>8)] ^ ((iCRC  >> 8));
  
      if (val == 0)                            	/* run length-encode zero differences */
      { for (i=2;; i++)
        { if (i>=npixels || src[i-1]!=src[-1] || i==256)
          { if (i<=MINZEROS) 		       	/* too short run -> write zeros */
            { for (j=0; j<i-1; j++)
              { *dest++=0;
                n4bit++;

                if(n4bit>=254)			/* maximum length 4 bit run */
                { dest  = recompress4bit(n4bit, dest);
                  n4bit = 0;
                }
              }
            }
            else      
            { if (n4bit>=MIN4BIT)		/* end (and write) 4 bit run */
                dest  = recompress4bit(n4bit, dest);

              n4bit=0;
              *dest++  =  0x80;
              *dest++  = (i-1);
            }

            npixels -= (i-2);
            for (j=0; j<i-2; j++)
            { src++;
              iCRC = CRC32_table[(unsigned char)iCRC  ^ (unsigned char) src[0]    ] ^ ((iCRC  >> 8));
              iCRC = CRC32_table[(unsigned char)iCRC  ^ (unsigned char)(src[0]>>8)] ^ ((iCRC  >> 8));
            }
            break;
          }
        }
      }    
      else if (val >= -63 && val <= 63)         /* small difference coded as one byte */
      { if (val >= -8 && val <= 7)
        { *dest++ = val;
          n4bit++;

          if(n4bit>=254)			/* maximum length 4 bit run */
          { dest  = recompress4bit(n4bit, dest);
            n4bit=0;
          }
        }
        else if(n4bit>=MIN4BIT)			/* end and write 4 bit run */
        { j = val;
          dest  = recompress4bit(n4bit, dest);
          n4bit=0;
          *dest++ = j;
        }
        else
        { *dest++ = val;			/* end 4 bit run */
           n4bit  = 0;
        }
      }
      else if (val >= -0x3F00 && val <= 0x3EFF) /* large differences coded as two bytes */
      { j = val;

        if(n4bit>=MIN4BIT)			/* end (and write) 4 bit run */
          dest  = recompress4bit(n4bit, dest);

        n4bit=0;
        dest[0] = (j>>8) ^ 0x40;
        dest[1] = j;
        dest+=2;
      }
      else                                      /* if very large differences code abs val as three bytes */
      { j = src[0];

        if(n4bit>=MIN4BIT)			/* end (and write) 4 bit run */
          dest  = recompress4bit(n4bit, dest);

        n4bit=0;
        *dest++ = 0x7F;
        *dest++ = j>>8; 
        *dest++ = j;
      }
    }
    while (--npixels);

    pHeader->iCompressedSize = dest - (signed char*)pHeader - sizeof(NKI_MODE2);

    /* Pad it to get an even length */
    if (pHeader->iCompressedSize & 1)
    { *dest++ = 0;
      pHeader->iCompressedSize++;
    }

    pHeader->iOrgCRC        = iCRC;
    pHeader->iCompressedCRC = 0;
  }

  return dest - (signed char*)pHeader;
}

int get_nki_private_decompressed_length(signed char *src)
{ int npixels = *(int *)src;
  return 2*npixels;
}

int get_nki_private_compress_mode(signed char *src)
{ int mode = *(int *)(src+4);
  return mode;
}

/* decoder for NKI private compressed pixel data - faster and safe version
   arguments: dest    = (in) points to area where destination data is written (short)
              src     = (in) points compressed source data (byte stream)
              size    = (in) number of bytes source data (safety)

   The return value is the number of pixels that have been processed.
 
   The compressed data looks like:
   (number of pixels)-1 times:
     OR 1 byte   = LL     7  bits signed (difference pixel[1] - pixel[0]);
     OR 2 bytes  = HHLL   15 bits signed (difference pixel[1] - pixel[0]) xored with 0x4000;
     OR 3 bytes  = 7FHHLL 16 bits absolute pixel data if 15 bits difference is exceeded
     OR 2 bytes  = 80NN   run length encode NN zero differences (max 255)
for mode 3 and 4 added:
     OR 2 bytes  = CONN   encode NN 4 bit differences (max 255)

   Performance on typical CT or MRI is >2x compression and a very good speed

   This code is not valid on HIGHENDIAN (high byte first) machines
*/

int nki_private_decompress(short int *dest, signed char *src, int size)
{ int			npixels, retvalue, mode, iMode, val, j;
  NKI_MODE2*		pHeader = (NKI_MODE2*)src;
  unsigned long		iCRC=0, iCRC2=0;
  signed char           *save, *end;

  NKIPrivateDecompress++;

  retvalue = npixels = pHeader->iOrgSize;
  iMode = pHeader->iMode;		// safety: this value is checked in case statement

  if (npixels<1) return 0;		// safety: check for invalid npixels value

  /* Up till now only Mode=1, 2, 3, and 4 are supported */

  switch (iMode)
  { case 1:
      src += 8;				// mode 1 only has 8 bytes header: iOrgSize and iMode
      end  = src + size - 3;		// for overflow check if we are close to end of input buffer
  
      *dest = *(short int *)src;
      src += 2;
      npixels--;

      do
      { if (src > end) 			// check whether the last few messages fit in input buffer
        { if (src<end+3) val = *src;
          else           val = 0;

          if (val >= -64 && val <= 63)      mode = 1;	// 7 bit difference
          else if (val==0x7f)		    mode = 3;	// 16 bit value
          else if ((val&0xff)==0x80)	    mode = 2;	// run length encoding
          else				    mode = 2;

          if (src+mode > end+3) 
            return 0;			// safety: overflow input data
        }
    
        val = *src;

        if (val >= -64 && val <= 63)	// 7 bit difference
        { dest[1] = dest[0] + val;
          dest++;
          src++;
        }
        else if (val==0x7f)		// 16 bit value
        { dest[1] = val = ((int)(((unsigned char *)src)[1])<<8) + ((unsigned char*)src)[2];
          dest++;
          src+=3;
        }
        else if ((val&0xff)==0x80)	// run length encoding
        { mode = ((unsigned char *)src)[1];
          npixels -= mode-1;
          if (npixels<=0) return 0;	// safety: overflow output data
          do 
          { dest[1] = dest[0];
            dest++;
          }
          while (--mode);
          src+=2;
        }
        else
        { signed short diff = ((val^0x40)<<8) + (unsigned char)(src[1]);
          dest[1] = dest[0] + diff;	// 15 bit difference
          dest++;
          src+=2;
        }
      }
      while (--npixels);

      break;

    case 2:
      src += sizeof(NKI_MODE2);
      save = src;
      end  = src + pHeader->iCompressedSize - 3;

      if (end > src + size - 3) 
        end = src + size - 3;		// may occur if pHeader is corrupted

      *dest = val = *(short int *)src;
      iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val] ^ ((iCRC2 >> 8));
      iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
      src+=2;
    
      npixels--;
    
      do
      { if (src > end) 			// check whether the last few messages fit in input buffer
        { if (src<end+3) val = *src;
          else           val = 0;

          if (val >= -64 && val <= 63)      mode = 1;	// 7 bit difference
          else if (val==0x7f)		    mode = 3;	// 16 bit value
          else if ((val&0xff)==0x80)	    mode = 2;	// run length encoding
          else				    mode = 2;

          if (src+mode > end+3) 
            break;			// safety: overflow input data
        }

        val = *src;
    
        if (val >= -64 && val <= 63)	// 7 bits difference
        { dest[1] = val = dest[0] + val;
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val] ^ ((iCRC2 >> 8));
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
          dest++;
          src++;
        }
        else if (val==0x7f)		// 16 bit value
        { dest[1] = val = ((int)(((unsigned char *)src)[1])<<8) + ((unsigned char*)src)[2];
    
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val] ^ ((iCRC2 >> 8));
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
          dest++;
          src+=3;
        }
        else if ((val&0xff)==0x80)	// run length encoding
        { mode = ((unsigned char *)src)[1];
          npixels -= mode-1;
          if (npixels<=0) break;	// safety: overflow output data
          do 
          { dest[1] = val = dest[0];
            iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val] ^ ((iCRC2 >> 8));
            iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
            dest++;
          }
          while (--mode);
          src+=2;
        }
        else
        { signed short diff = ((val^0x40)<<8) + ((unsigned char *)src)[1];
          dest[1] = val = dest[0] + diff;	// 15 bit difference
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val] ^ ((iCRC2 >> 8));
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
          dest++;
          src+=2;
        }
      }
      while (--npixels);
    
      if (iCRC2 != pHeader->iOrgCRC)	// if error in output CRC:
      { src = save;			// check input CRC
        while (src < end)
        { iCRC = CRC32_table[(unsigned char)iCRC ^ (unsigned char)src[0]] ^ ((iCRC >> 8));
          src++;
        }

        if (iCRC != pHeader->iCompressedCRC)
        { //OperatorConsole.printf("NKI decompression: the file is corrupted\n");
          retvalue=0;
        }
        else
        { //OperatorConsole.printf("NKI private decompression: internal error\n"); 
          retvalue=0; 
        }
      }

      break;

    case 3:
      src += 8;				// mode 3 only has 8 bytes header: iOrgSize and iMode
      end  = src + size - 3;		// for overflow check if we are close to end of input buffer
  
      *dest = *(short int *)src;
      src += 2;
      npixels--;

      do
      { if (src > end) 			// check whether the last few messages fit in input buffer
        { if (src<end+3) val = *src;
          else           val = 0;

          if (val >= -63 && val <= 63)      mode = 1;	// 7 bit difference
          else if (val==0x7f)		    mode = 3;	// 16 bit value
          else if ((val&0xff)==0x80)	    mode = 2;	// run length encoding
          else if ((val&0xff)==0xC0)	    mode = 2;	// 4 bit encoding
          else				    mode = 2;

          if (src+mode > end+3) 
            return 0;			// safety: overflow input data
        }
    
        val = *src;

        if (val >= -63 && val <= 63)	// 7 bit difference
        { dest[1] = dest[0] + val;
          dest++;
          src++;
        }
        else if (val==0x7f)		// 16 bit value
        { dest[1] = val = ((int)(((unsigned char *)src)[1])<<8) + ((unsigned char*)src)[2];
          dest++;
          src+=3;
        }
        else if ((val&0xff)==0x80)	// run length encoding
        { mode = ((unsigned char *)src)[1];
          npixels -= mode-1;
          if (npixels<=0) return 0;	// safety: overflow output data
          do 
          { dest[1] = dest[0];
            dest++;
          }
          while (--mode);
          src+=2;
        }
        else if ((val&0xff)==0xC0)	// 4 bit run
        { mode = ((unsigned char *)src)[1];
          npixels -= mode-1;
          mode/=2;
          src+=2;
          if (npixels<=0) return 0;	// safety: overflow output data
          do 
          { val = *src++;
            dest[1] = dest[0] + (val>>4);
            dest++;
            if (val&8) val |= 0xfffffff0; else val &= 0x0f;
            dest[1] = dest[0] + val;
            dest++;
          }
          while (--mode);
        }
        else
        { signed short diff = ((val^0x40)<<8) + (unsigned char)(src[1]);
          dest[1] = dest[0] + diff;	// 15 bit difference
          dest++;
          src+=2;
        }
      }
      while (--npixels);

      break;

    case 4:
      src += sizeof(NKI_MODE2);
      save = src;
      end  = src + pHeader->iCompressedSize - 3;
  
      if (end > src + size - 3) 
        end = src + size - 3;		// may occur if pHeader is corrupted

      *dest = val = *(short int *)src;
      iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val] ^ ((iCRC2 >> 8));
      iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
      src += 2;
      npixels--;

      do
      { if (src > end) 			// check whether the last few messages fit in input buffer
        { if (src<end+3) val = *src;
          else           val = 0;

          if (val >= -63 && val <= 63)      mode = 1;	// 7 bit difference
          else if (val==0x7f)		    mode = 3;	// 16 bit value
          else if ((val&0xff)==0x80)	    mode = 2;	// run length encoding
          else if ((val&0xff)==0xC0)	    mode = 2;	// 4 bit encoding
          else				    mode = 2;

          if (src+mode > end+3) 
            return 0;			// safety: overflow input data
        }
    
        val = *src;

        if (val >= -63 && val <= 63)	// 7 bit difference
        { dest[1] = val = dest[0] + val;
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val] ^ ((iCRC2 >> 8));
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
          dest++;
          src++;
        }
        else if (val==0x7f)		// 16 bit value
        { dest[1] = val = ((int)(((unsigned char *)src)[1])<<8) + ((unsigned char*)src)[2];
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val] ^ ((iCRC2 >> 8));
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
          dest++;
          src+=3;
        }
        else if ((val&0xff)==0x80)	// run length encoding
        { mode = ((unsigned char *)src)[1];
          npixels -= mode-1;
          if (npixels<=0) return 0;	// safety: overflow output data
          do 
          { dest[1] = val = dest[0];
            iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val] ^ ((iCRC2 >> 8));
            iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
            dest++;
          }
          while (--mode);
          src+=2;
        }
        else if ((val&0xff)==0xC0)	// 4 bit run
        { mode = ((unsigned char *)src)[1];
          npixels -= mode-1;
          mode/=2;
          src+=2;
          if (npixels<=0) return 0;	// safety: overflow output data
          do 
          { val = *src++;
            dest[1] = j = dest[0] + (val>>4);
            iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)j] ^ ((iCRC2 >> 8));
            iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(j>>8)] ^ ((iCRC2 >> 8));
            dest++;
            if (val&8) val |= 0xfffffff0; else val &= 0x0f;
            dest[1] = j = dest[0] + val;
            iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)j] ^ ((iCRC2 >> 8));
            iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(j>>8)] ^ ((iCRC2 >> 8));
            dest++;
          }
          while (--mode);
        }
        else
        { signed short diff = ((val^0x40)<<8) + (unsigned char)(src[1]);
          dest[1] = val = dest[0] + diff;	// 15 bit difference
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)val] ^ ((iCRC2 >> 8));
          iCRC2 = CRC32_table[(unsigned char)iCRC2 ^ (unsigned char)(val>>8)] ^ ((iCRC2 >> 8));
          dest++;
          src+=2;
        }
      }
      while (--npixels);

      if (iCRC2 != pHeader->iOrgCRC)	// if error in output CRC:
       retvalue=0;

      break;

    default:
      //OperatorConsole.printf("NKI private decompression: unsupported mode\n"); 
      return 0;
  }

  return retvalue;
}

BOOL DecompressNKI(DICOMDataObject* pDDO)
{ VR*		pVR;
  VR*		pNewVR;
  signed char*	pCompressed;
  int		iDecompressedLength, iCompressedLength;

  pVR = pDDO->GetVR(0x7fdf, 0x0010);
  if (pVR)
  { /* Are the 'normal' pixel-data also present? If so, then 'our' tag is not
       made by us!
    */
    pNewVR = pDDO->GetVR(0x7fe0, 0x0010);
    if (pNewVR)
      return TRUE;

    /* Acuson and Elscint use this tag: test whether we or they made it */
    if ((pVR->Length > 6) &&
        ((strncmp((char*)pVR->Data, "ACUSON", 6) == 0) ||
         (strncmp((char*)pVR->Data, "ELSCIN", 6) == 0)))
      return TRUE;

    /* no data in our tag (always starts with 2 dwords): cannot be compresssed */
    if (pVR->Length<=8)
      return TRUE;

    /* check compression level: if invalid it cannot be our data */
    if (pVR->Length>8 && ((unsigned int*)pVR->Data)[1]>MAXNKICOMPRESSION)
      return TRUE;

    /* Decompress it */
    pCompressed         = ((signed char *)(pVR->Data));
    if (!pCompressed)
      return FALSE;
    
    iCompressedLength   = pVR->Length;
    iDecompressedLength = get_nki_private_decompressed_length(pCompressed);
    if (!iDecompressedLength)
      return FALSE;

    pNewVR = new VR(0x7fe0, 0x0010, iDecompressedLength, TRUE);
    if (!pNewVR) 
      return FALSE;

    if (!nki_private_decompress((short *)(pNewVR->Data), pCompressed, iCompressedLength))
    { delete pNewVR;
      return FALSE;
    }

    /* Push the new VR, and delete the old one */
    pDDO->Push(pNewVR);
    pDDO->DeleteVR(pVR);
  }
  return TRUE;
}

BOOL CompressNKI(DICOMDataObject* pDDO, int CompressMode /* = 2 */)
{ VR*		pVR;
  VR*		pNewVR;
  signed char*	pCompressed;
  int		iCompressedLength;

  /* First check whether the image already contains 0x7fdf, 0x0010.
     If so, compression was already done, or 'our' VR is occupied.
     Also do not compress the 'ugly' variable-length sequence of pixeldata.
  */
  pVR = pDDO->GetVR(0x7fdf, 0x0010);
  if (pVR)
    return TRUE;

  /* Pixeldata present? */
  pVR = pDDO->GetVR(0x7fe0, 0x0010);
  if (pVR && (pVR->Length != 0xffffffff) && (pVR->Length != 0x0))
  { /* Compress it */
    pCompressed = (signed char*) malloc((pVR->Length/2) * 3 + 10);
    if (!pCompressed)
      return FALSE;

    iCompressedLength = nki_private_compress(pCompressed, (short int *)(pVR->Data), pVR->Length/2, CompressMode);
    if (iCompressedLength & 0x01)
      pCompressed[iCompressedLength++] = 0;

    /* Create  a new VR, and delete the old one */
    pNewVR = new VR(0x7fdf, 0x0010, iCompressedLength, pCompressed);
    pNewVR->TypeCode = pVR->TypeCode;
    pDDO->Push(pNewVR);
    pDDO->DeleteVR(pVR);
  }
  return TRUE;
}

/* Downsizing of pixeldata only when VR (0x9999, 0x0200) is present in the
   DicomCommandObject.
   NOTE: this VR is NKI specific.
*/

int MaybeDownsize(DICOMDataObject* pDDO, DICOMCommandObject* pDCO, int size)
{ int			i, j;
  VR*			pVR;
  unsigned int		iMaxRowsColumns;
  unsigned short	iRows, iColumns, iNewRows, iNewColumns, iDownsizeFactor;
  char			s[256];
  char*			pSeparator;
  float			fSpacingX, fSpacingY;
  int			iImageSize, iNbBytesPerPixel, iNbPixels, iSkip;
  char*			pcSrc;
  char*			pcDest;
  short*		psSrc;
  short*		psDest;
  int*			piSrc;
  int*			piDest;

  if (pDCO)
  { pVR = pDCO->GetVR(0x9999, 0x0200);	/* MaxRowsColumns */
    if (!pVR)
      return TRUE;			/* Nothing to do... */
    iMaxRowsColumns = (unsigned int)(*((unsigned int*)pVR->Data));
  }
  else
    iMaxRowsColumns = size;

  if (!iMaxRowsColumns)
    return TRUE;			/* Downsizing to zero doesn't make sense */

  /* OK. Now check whether all necessary PixelData-info is present in pDDO */
  pVR = pDDO->GetVR(0x0028, 0x0010);	/* Rows */
  if (!pVR)
    return TRUE;
  iRows = *((unsigned short*)pVR->Data);

  pVR = pDDO->GetVR(0x0028, 0x0011);	/* Columns */
  if (!pVR)
    return TRUE;
  iColumns = *((unsigned short*)pVR->Data);
  iNbPixels = iRows * iColumns;

  if ((!iNbPixels) ||				/* Image must make sense */
      ((iRows <= iMaxRowsColumns) &&
       (iColumns <= iMaxRowsColumns)))		/* Image is already 'small' enough */
    return TRUE;

  DownSizeImage++;

  if (iRows >= iColumns)
  { iDownsizeFactor = iRows / iMaxRowsColumns;
    if (iRows % iMaxRowsColumns)
      iDownsizeFactor ++;
  }
  else
  { iDownsizeFactor = iColumns / iMaxRowsColumns;
    if (iColumns % iMaxRowsColumns)
      iDownsizeFactor ++;
  }

  pVR = pDDO->GetVR(0x0028, 0x0030);	/* Pixelspacing */
  if (!pVR)
    return TRUE;
  strncpy(s, (const char*)pVR->Data, pVR->Length);
  s[pVR->Length] = 0;
  fSpacingX = (float)(atof(s));
  pSeparator = strchr(s, '\\');
  if (pSeparator)
    fSpacingY = (float)(atof(pSeparator + 1));
  else
    fSpacingY = fSpacingX;
  
  if (!DecompressNKI(pDDO))		/* Pixeldata must first be decompressed */
    return FALSE;

  pVR = pDDO->GetVR(0x7fe0, 0x0010);	/* Pixeldata */
  if (!pVR)
    return TRUE;
  iImageSize = pVR->Length;
  if ((iImageSize < iNbPixels) ||
      (iImageSize % iNbPixels))
    return TRUE;			/* Image doesn't make sense */
  iNbBytesPerPixel = iImageSize / iNbPixels;
  if (iNbBytesPerPixel > 4)
    return TRUE;			/* Image is too beautiful to downsize! */

  /* LETS DO IT !
     The downsized image is stored in the same memory-block as the original one:
     Just be sure to adjust the length-indicator. The image will be (re)compressed anyway
     later on.
  */

  iNewRows    = iRows / iDownsizeFactor;
  if (!iNewRows)
    iNewRows++;
  iNewColumns = iColumns / iDownsizeFactor;
  if (!iNewColumns)
    iNewColumns++;
  switch (iNbBytesPerPixel)
  { case 1:
      pcSrc  = (char*)pVR->Data;
      pcDest = pcSrc;
      for (i=0; i<iNewRows; i++)
      { for (j=0; j<iNewColumns; j++)
	{ *pcDest++ = *pcSrc;
	  pcSrc += iDownsizeFactor;
	}
	pcSrc = (char*)pVR->Data + iDownsizeFactor * iColumns * (i + 1);
      }
      break;
    case 2:
      psSrc  = (short*)pVR->Data;
      psDest = psSrc;
      for (i=0; i<iNewRows; i++)
      { for (j=0; j<iNewColumns; j++)
	{ *psDest++ = *psSrc;
	  psSrc += iDownsizeFactor;
	}
	psSrc = (short*)pVR->Data + iDownsizeFactor * iColumns * (i + 1);
      }
      break;
    case 3:
      pcSrc  = (char*)pVR->Data;
      pcDest = pcSrc;
      iSkip = (iDownsizeFactor - 1) * 3;
      for (i=0; i<iNewRows; i++)
      { for (j=0; j<iNewColumns; j++)
	{ *pcDest++ = *pcSrc++;
	  *pcDest++ = *pcSrc++;
	  *pcDest++ = *pcSrc++;
	  pcSrc += iSkip;
	}
	pcSrc = (char*)pVR->Data + iDownsizeFactor * 3 * iColumns * (i + 1);
      }
      break;
    case 4:
      piSrc  = (int*)pVR->Data;
      piDest = piSrc;
      for (i=0; i<iNewRows; i++)
      { for (j=0; j<iNewColumns; j++)
	{ *piDest++ = *piSrc;
	  piSrc += iDownsizeFactor;
	}
	piSrc = (int*)pVR->Data + iDownsizeFactor * iColumns * (i + 1);
      }
      break;
  }

  /* FINALLY: adjust Rows, Columns, Pixelspacing and (the length of) PixelData */
  pVR = pDDO->GetVR(0x0028, 0x0010);	/* Rows */
  *((unsigned short*)pVR->Data) = iNewRows;

  pVR = pDDO->GetVR(0x0028, 0x0011);	/* Columns */
  *((unsigned short*)pVR->Data) = iNewColumns;

  pVR = pDDO->GetVR(0x0028, 0x0030);	/* Pixelspacing */
  if (pSeparator)
    sprintf(s, "%g\\%g", fSpacingX * iDownsizeFactor, fSpacingY * iDownsizeFactor);
  else
    sprintf(s, "%g", fSpacingX * iDownsizeFactor);
  j = strlen(s);
  if (j & 0x01) s[j++] = 0;	// mvh 20001120 - added initialize of new letter of s

  pVR->ReAlloc(j);
  memcpy(pVR->Data, s, j);

  pVR = pDDO->GetVR(0x7fe0, 0x0010);	/* Pixeldata */
  pVR->Length = iNewRows * iNewColumns * iNbBytesPerPixel;
  return 2;			// Downsizing actually occurred
}

/* Test if downsizing will occur */

static int TestDownsize(DICOMDataObject* pDDO, DICOMCommandObject* pDCO, int size)
{ VR*			pVR;
  unsigned int		iMaxRowsColumns;
  unsigned short	iRows, iColumns;
  int			iNbPixels;

  if (pDCO)
  { pVR = pDCO->GetVR(0x9999, 0x0200);	/* MaxRowsColumns */
    if (!pVR)
      return FALSE;			/* Nothing to do... */
    
    iMaxRowsColumns = (unsigned int)(*((unsigned int*)pVR->Data));
    if (!iMaxRowsColumns)
      return FALSE;			/* Downsizing to zero doesn't make sense */
  }
  else
    iMaxRowsColumns = size;

  /* OK. Now check whether all necessary PixelData-info is present in pDDO */
  pVR = pDDO->GetVR(0x0028, 0x0010);	/* Rows */
  if (!pVR)
    return FALSE;
  iRows = *((unsigned short*)pVR->Data);

  pVR = pDDO->GetVR(0x0028, 0x0011);	/* Columns */
  if (!pVR)
    return FALSE;
  iColumns = *((unsigned short*)pVR->Data);
  iNbPixels = iRows * iColumns;

  if ((!iNbPixels) ||				/* Image must make sense */
      ((iRows <= iMaxRowsColumns) &&
       (iColumns <= iMaxRowsColumns)))		/* Image is already 'small' enough */
    return FALSE;

  return TRUE;				/* image would be downsized */
}

BOOL ProcessDDO(DICOMDataObject** pDDO, DICOMCommandObject* pDCO)
{ VR  *pVR;
  int dum;
  
  if (!pDCO)				// non-nki clients
  { // De-compression is left to recompress in StandardRetrieveNKI::Read 
    return TRUE; 
  }

  /* pDCO possibly contains specifications of how the NKI clients
     want the data the be sent/processed. Only downsize is 
     implemented for now.
  */

  pVR = pDCO->GetVR(0x9999, 0x0200);	// MaxRowsColumns
  if (pVR)
  { if (TestDownsize(*pDDO, pDCO, 0))
    { DecompressImage(pDDO, &dum);	// will also decompress JPEG images

      if (!MaybeDownsize(*pDDO, pDCO, 0))  // downsize
        return FALSE;
    }
  }

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// JPEG and NKI utility compression code
///////////////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>

// Runs external executable hidden
static BOOL ExecHidden(
	char	*ProcessBinary,					// name of executable
	char	*Arg1, char *Arg2, char *Arg3, char *Arg4)	// arguments
	{
	PROCESS_INFORMATION	PINFO;
	STARTUPINFO		SINFO;
	SECURITY_ATTRIBUTES	SA;
	HANDLE			hMap;
	char			CommandLine[1024];

	memset((void*)&SINFO, 0, sizeof(STARTUPINFO));
	SINFO.cb = sizeof(STARTUPINFO);
	SINFO.wShowWindow = SW_HIDE;

	SA.nLength = sizeof (SECURITY_ATTRIBUTES);
	SA.lpSecurityDescriptor = NULL;

	sprintf(CommandLine, "%s %s \"%s\" \"%s\" %s", ProcessBinary, Arg1, Arg2, Arg3, Arg4);
	if(!CreateProcess(ProcessBinary, CommandLine, NULL, NULL, FALSE,
		DETACHED_PROCESS, NULL /* "DCMDICTPATH=offis.dic\0\0" */, NULL, &SINFO, &PINFO))
		{
		OperatorConsole.printf("***Failed to create process: %s - error %d\n", ProcessBinary, GetLastError());
		return ( FALSE );
		}
	else
		{
	  	WaitForSingleObject(PINFO.hProcess, INFINITE);
		CloseHandle(PINFO.hThread);
		CloseHandle(PINFO.hProcess);
		}

	return ( TRUE );
	}
#else

#ifndef DARWIN
#include <wait.h>
#endif //DARWIN

static BOOL ExecHidden(
	char	*ProcessBinary,					// name of executable
	char	*Arg1, char *Arg2, char *Arg3, char *Env)	// arguments
	{
        char line[256];
        sprintf(line, "export DCMDICTPATH=dicom.dic;%s %s \"%s\" \"%s\"", ProcessBinary, Arg1, Arg2, Arg3);
        system(line);
        return TRUE;
	}
#endif

// Replaces PDU.SaveDICOMDataObject; automatically detects transfer syntax to use
void SaveDICOMDataObject(char *Filename, DICOMDataObject* pDDO)
	{
	int len, has_dcm_extension=0, UseChapter10=FALSE, bForcedImplicit=FALSE;
	VR *pVR;
	char s[256];
	PDU_Service PDU;

	PDU.AttachRTC(&VRType);

	len = strlen(Filename);
	if (len>4)
		has_dcm_extension = (stricmp(Filename+len-4, ".dcm")==0);

	/* Type of save depends on file name extension is .dcm (UseChapter10) 
	   and on the transfer syntax (not ImplicitLittleEndian, not
	   ExplicitLittleEndian and not ExplicitBigEndian)
	*/
	pVR = pDDO->GetVR(0x0002, 0x0010);	// TransferSyntaxUID
	if (pVR && pVR->Data)
		{
		strncpy(s, (char*)pVR->Data, pVR->Length);
		s[pVR->Length] = 0;
		if ((strcmp(s, "1.2.840.10008.1.2")   != 0) &&
		    (strcmp(s, "1.2.840.10008.1.2.1") != 0) &&
		    (strcmp(s, "1.2.840.10008.1.2.2") != 0))
			{
			if (!has_dcm_extension)
				{
				OperatorConsole.printf("Recompress: cannot rewrite jpeg/rle image in v2 format\n");
				Filename[0]=0;
				delete pDDO;
				return;
				}
			UseChapter10 = has_dcm_extension;
			}
		else
			{
			UseChapter10 = has_dcm_extension;
			//bForcedImplicit = TRUE; // 20040406
			}
		}
	else
		UseChapter10 = has_dcm_extension;

	if (!UseChapter10)
		PDU.SaveDICOMDataObject(Filename, ACRNEMA_VR_DUMP, pDDO);
	else if (bForcedImplicit)
	 	PDU.SaveDICOMDataObject(Filename, DICOM_CHAPTER_10_IMPLICIT, pDDO);
	else
	 	PDU.SaveDICOMDataObject(Filename, DICOM_CHAPTER_10_EXPLICIT, pDDO);
	}

#ifndef NOINTJPEG

/***********************************************************************************/
/*
   Code from CqGlobal.h		(CqDicom.dll)
*/
/***********************************************************************************/

#define DCM_E_OK				0
#define DCM_E_MEMORY				5
#define DCM_E_UNSUPPORTED_COMPRESSION		26
#define DCM_E_UNSUPPORTED_FORMAT		28

typedef struct
{
	char	szModality[16];
	char	szSeriesInstanceUID[256];
	float	fSpacingX;
	float	fSpacingY;
	int	iDimX;
	int	iDimY;
	int	iNbFrames;
	int	iNbTimeSequences;
	char	szPatientPosition[16];
	int	iDataSize;
	BOOL	bNkiCompressed;
	int	iPixelRepresentation;
	int	iBitsStored;
	float	fRescaleIntercept;
	float	fRescaleSlope;
	float	fDoseGridScaling;
	BOOL	bRtImagePosOK;
	float	fRtImagePosX;
	float	fRtImagePosY;
	char	szPhotometricInterpretation[20];
} SLICE_INFO;

/***********************************************************************************/
/*
   Code from CqJPEG.cpp			(CqDicom.dll)
*/
/***********************************************************************************/

/* There was no way to incorporate following functions in one file */
extern int DecompressJPEG16(SLICE_INFO* pSliceInfo, VR* pSequence,
	void** ppResult, int* piResultSize);
extern int DecompressJPEG12(SLICE_INFO* pSliceInfo, VR* pSequence,
	void** ppResult, int* piResultSize);
extern int DecompressJPEG8(SLICE_INFO* pSliceInfo, VR* pSequence,
	void** ppResult, int* piResultSize);

/* Forward declaration of RLE decompressor */
static int DecompressRLE(SLICE_INFO* pSliceInfo, VR* pSequence,
	void** ppResult, int* piResultSize);


#define		Uint8	unsigned char
#define		Uint16	unsigned short
#define		Uint32	unsigned int


/* Following two functions are to find out which version of the ijg libraries
   to use in order to decompress.
*/
static Uint16 readUint16(const Uint8* pData)
{ return (((Uint16)(*pData) << 8) | ((Uint16)(*(pData+1))));
}

static Uint8 scanJpegDataForBitDepth(
  const Uint8 *data,
  const Uint32 fragmentLength)
{
  Uint32 offset = 0;
  while(offset+4 < fragmentLength)
  {
    switch(readUint16(data+offset))
    {
      case 0xffc0: // SOF_0: JPEG baseline
        return data[offset+4];
        /* break; */
      case 0xffc1: // SOF_1: JPEG extended sequential DCT
        return data[offset+4];
        /* break; */
      case 0xffc2: // SOF_2: JPEG progressive DCT
        return data[offset+4];
        /* break; */
      case 0xffc3 : // SOF_3: JPEG lossless sequential
        return data[offset+4];
        /* break; */
      case 0xffc5: // SOF_5: differential (hierarchical) extended sequential, Huffman
        return data[offset+4];
        /* break; */
      case 0xffc6: // SOF_6: differential (hierarchical) progressive, Huffman
        return data[offset+4];
        /* break; */
      case 0xffc7: // SOF_7: differential (hierarchical) lossless, Huffman
        return data[offset+4];
        /* break; */
      case 0xffc8: // Reserved for JPEG extentions
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xffc9: // SOF_9: extended sequential, arithmetic
        return data[offset+4];
        /* break; */
      case 0xffca: // SOF_10: progressive, arithmetic
        return data[offset+4];
        /* break; */
      case 0xffcb: // SOF_11: lossless, arithmetic
        return data[offset+4];
        /* break; */
      case 0xffcd: // SOF_13: differential (hierarchical) extended sequential, arithmetic
        return data[offset+4];
        /* break; */
      case 0xffce: // SOF_14: differential (hierarchical) progressive, arithmetic
        return data[offset+4];
        /* break; */
      case 0xffcf: // SOF_15: differential (hierarchical) lossless, arithmetic
        return data[offset+4];
        /* break; */
      case 0xffc4: // DHT
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xffcc: // DAC
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xffd0: // RST m
      case 0xffd1:
      case 0xffd2:
      case 0xffd3:
      case 0xffd4:
      case 0xffd5:
      case 0xffd6:
      case 0xffd7: 
        offset +=2;
        break;
      case 0xffd8: // SOI
        offset +=2;
        break;
      case 0xffd9: // EOI
        offset +=2;
        break;
      case 0xffda: // SOS
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xffdb: // DQT
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xffdc: // DNL
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xffdd: // DRI
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xffde: // DHP
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xffdf: // EXP
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xffe0: // APPn
      case 0xffe1:
      case 0xffe2:
      case 0xffe3:
      case 0xffe4:
      case 0xffe5:
      case 0xffe6:
      case 0xffe7:
      case 0xffe8:
      case 0xffe9:
      case 0xffea:
      case 0xffeb:
      case 0xffec:
      case 0xffed:
      case 0xffee:
      case 0xffef: 
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xfff0: // JPGn
      case 0xfff1:
      case 0xfff2:
      case 0xfff3:
      case 0xfff4:
      case 0xfff5:
      case 0xfff6:
      case 0xfff7:
      case 0xfff8:
      case 0xfff9:
      case 0xfffa:
      case 0xfffb:
      case 0xfffc:
      case 0xfffd: 
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xfffe: // COM
        offset += readUint16(data+offset+2)+2;
        break;
      case 0xff01: // TEM
        break;
      default: 
        if ((data[offset]==0xff) && (data[offset+1]>2) && (data[offset+1] <= 0xbf)) // RES reserved markers
        { 
          offset += 2;
        }
        else return 0; // syntax error, stop parsing
        break;
    }
  } // while
  return 0; // no SOF marker found
}


/* This JPEG decompression function accepts a dicom VR with pixeldata-sequence,
   and replaces the sequence by a 'normal' VR.
   returns:
   - DCM_E_UNSUPPORTED_FORMAT when the sequence was not understood
   - DCM_E_UNSUPPORTED_COMPRESSION when the data are not supported by the ijg lib
   - DCM_E_MEMORY when out of memory
   - DCM_E_OK
*/
static int DecompressJPEG(SLICE_INFO* pSliceInfo, VR* pSequence)
{ int				rc = DCM_E_UNSUPPORTED_FORMAT;
  Array<DICOMDataObject*>*	pADDO;
  DICOMDataObject*		pDDO;
  VR*				pVR;
  int				iNbBitsPerSample;
  void*				pResult = NULL;
  int				iSize = 0;

  /* Is the dicom-sequence OK? */
  pADDO = (Array<DICOMDataObject*>*)pSequence->SQObjectArray;
  if (!pADDO)
    return rc;
  pDDO = pADDO->Get(1);
  if (!pDDO)
    return rc;
  pVR  = pDDO->GetVR(0xfffe, 0xe000);
  if (!pVR)
    return rc;
  if ((!pVR->Data) || (pVR->Length <= 0))
    return rc;

  /* Does ijg support the compression? */
  iNbBitsPerSample = scanJpegDataForBitDepth((unsigned char*)pVR->Data, pVR->Length);
  if ((iNbBitsPerSample <= 0) || (iNbBitsPerSample > 16))
  { /* It is not a supported JPEG image. Maybe it is RLE compressed */
    DecompressedRLE++;
    rc = DecompressRLE(pSliceInfo, pSequence, &pResult, &iSize);
    if (rc != DCM_E_OK)
      return rc;
  }
  else
  { /* Depending on iNbBitsPerSample, a different ijg libs/includes are used */
    DecompressJpeg++;

    if (iNbBitsPerSample > 12)
      rc = DecompressJPEG16(pSliceInfo, pSequence, &pResult, &iSize);
    else if (iNbBitsPerSample > 8)
      rc = DecompressJPEG12(pSliceInfo, pSequence, &pResult, &iSize);
    else
      rc = DecompressJPEG8(pSliceInfo, pSequence, &pResult, &iSize);
  }


  /* When OK, replace the VR */
  if (rc == DCM_E_OK)
  { /* TO DO: add consistency checks iSize <--> DicomTags */
    /* Cleanup the sequence, and make a normal VR */
//    delete[] pADDO;
//    pSequence->SQObjectArray = NULL;
    pSequence->Reset();
    pSequence->Data = pResult;
    pSequence->ReleaseMemory = TRUE;
    if (iSize & 0x01)
    { ((char*)pResult)[iSize] = 0;
      iSize++;
    }
    pSequence->Length = iSize;
  }

  return rc;
}

static int DecompressRLE(SLICE_INFO* pSliceInfo, VR* pSequence,
	void** ppResult, int* piResultSize)
{ int		rc = DCM_E_UNSUPPORTED_COMPRESSION;
  int		i, n, iNbFrames, iNbRows, iNbRleSegments;
  Array<DICOMDataObject*>*	pADDO;
  DICOMDataObject*	pDDO;
  VR*			pVR;
  char		*pIn, *pOut;
  int*		piHeader;
  int		iCompressedSize, iDecompressedSize, iNbCompressed, iNbDecompressed;
  char		cCurrent;
  char		*pR, *pG, *pB;
  bool		bIsRGB = false;

  pADDO = (Array<DICOMDataObject*>*)pSequence->SQObjectArray;
  pDDO = pADDO->Get(1);
  pVR  = pDDO->GetVR(0xfffe, 0xe000);

  /* The RLE data should start with 16 INT32 (Dicom chapter 5 annex G):
     - 1st contains how many RLE segments there are (15 max)
     - 2nd-16th contain offsets of each RLE segment
     All non-used segments have offset 0 in this small table
     Since this 16 INT32 header has a size of 64 bytes, the first offset should
     be 64. All integers are stored in little-endian format.
  */
  /* Check the header */
  if ((!pVR->Data) || (pVR->Length < 64))
    return rc;
  piHeader = (int*)pVR->Data;
  iNbRleSegments = piHeader[0];
  if ((iNbRleSegments < 1) || (iNbRleSegments > 15))
    return rc;
  if (piHeader[1] != 64)
    return rc;
  for (i=2; i<=iNbRleSegments; i++)
    if (piHeader[i] < piHeader[i-1])
      return rc;
  for (i=iNbRleSegments + 1; i<=15; i++)
    if (piHeader[i] != 0)
      return rc;

  /* OK, it probably is RLE-compressed */

  /* Find out how big the output is */
  iDecompressedSize = pSliceInfo->iDimX * pSliceInfo->iDimY *
	(pSliceInfo->iBitsStored / 8);
  if (pSliceInfo->iNbFrames)
    iDecompressedSize *= pSliceInfo->iNbFrames;
  if (strncmp(pSliceInfo->szPhotometricInterpretation, "RGB", 3) == 0)
    bIsRGB = true;
  /* Of all color-representations, just support RGB for now */

  if (bIsRGB)
    iDecompressedSize = iDecompressedSize * 3;
  pOut = (char*)malloc(iDecompressedSize);
  memset(pOut, 0xff, iDecompressedSize);
  if (!pOut)
    return DCM_E_MEMORY;

  iNbCompressed   = 0;
  iNbDecompressed = 0;
  iCompressedSize = pVR->Length - 64;
  pIn = ((char*)piHeader) + 64;
  while (iNbDecompressed < iDecompressedSize)
  { cCurrent = pIn[iNbCompressed++];
    if (cCurrent >= 0)
    { if ((iNbCompressed + ((int)cCurrent) + 1 <= iCompressedSize) &&
	  (iNbDecompressed + ((int)cCurrent) + 1 <= iDecompressedSize))
      {	memcpy(pOut + iNbDecompressed, pIn + iNbCompressed, ((int)cCurrent) + 1);
	iNbCompressed   += ((int)cCurrent) + 1;
	iNbDecompressed += ((int)cCurrent) + 1;
      }
      else
	break;
    }
    else if (cCurrent == -128)
      iNbCompressed = iNbCompressed;
    else
    { n = ((int)-cCurrent) + 1;
      if (iNbDecompressed + n > iDecompressedSize)
	break;
      for (i=0; i<n; i++)
	pOut[iNbDecompressed + i] = pIn[iNbCompressed];
      iNbDecompressed += n;
      iNbCompressed++;
    }
  }

  if (bIsRGB)
  { /* Convert the R, G and B image planes to triplets */
    pR = pOut;
    pOut = (char*)malloc(iDecompressedSize);
    if (!pOut)
    { free(pR);
      return DCM_E_MEMORY;
    }
    pG = pR + iDecompressedSize / 3;
    pB = pG + iDecompressedSize / 3;
    for (i=0; i<iDecompressedSize / 3; i++)
    { *pOut++ = pR[i];
      *pOut++ = pG[i];
      *pOut++ = pB[i];
    }
    pOut -= iDecompressedSize;
    free(pR);
  }


  rc = DCM_E_OK;

  if (rc == DCM_E_OK)
  { *ppResult = pOut;
    *piResultSize = iDecompressedSize;
  }
  else
  { *ppResult = NULL;
    *piResultSize = 0;
  }
  return rc;
}

/***********************************************************************************/
/*
   JPEG decompression interface for dgate
*/
/***********************************************************************************/

static int dcmdjpeg(DICOMDataObject* pDDO)
{
  /* Functions above (from CqDicom.dll) will do the work.
     Here:
     - Get the necessary parameters from pDDO, and put them in SLICE_INFO structure
     - Adjust some dicom-tags
  */
  char		s[100];
  int		rc;
  VR*		pVR;
  SLICE_INFO	SliceInfo;
  SLICE_INFO*	pSliceInfo;

  pSliceInfo = &SliceInfo;
  memset(pSliceInfo, 0x00, sizeof(SLICE_INFO));

  /* Rows */
  pVR = pDDO->GetVR(0x0028, 0x0010);
  if (pVR)
    pSliceInfo->iDimY = (int)(*((unsigned short*)pVR->Data));
  else
    return DCM_E_UNSUPPORTED_FORMAT;

  /* Columns */
  pVR = pDDO->GetVR(0x0028, 0x0011);
  if (pVR)
    pSliceInfo->iDimX = (int)(*((unsigned short*)pVR->Data));
  else
    return DCM_E_UNSUPPORTED_FORMAT;

  /* BitsStored */
  pVR = pDDO->GetVR(0x0028, 0x0101);
  if (pVR)
    pSliceInfo->iBitsStored = (int)(*((unsigned short*)pVR->Data));
  else
    pSliceInfo->iBitsStored = 16; /* Default 16 bits */

  /* NumberOfFrames */
  pVR = pDDO->GetVR(0x0028, 0x0008);
  if (pVR && pVR->Length)
  { strncpy(s, (const char*)pVR->Data, pVR->Length);
    s[pVR->Length] = 0;
    pSliceInfo->iNbFrames = atoi(s);
    pSliceInfo->iNbTimeSequences = 1;
    /*  NumberOfSlices */
    pVR = pDDO->GetVR(0x0054, 0x0081);
    if (pVR && pVR->Length)
    { /* Seems like a 3D time-sequence:
	 - Keep on using the iNbFrames member to designate the number of slices
	 - Use the iNbTimeSequences member for the number of volumes
      */
      int iNbSlices, iNbVolumes;

      iNbSlices  = (int)(*((unsigned short*)pVR->Data));
      if (iNbSlices > 0)
      { iNbVolumes = pSliceInfo->iNbFrames / iNbSlices;
	if (iNbVolumes * iNbSlices == pSliceInfo->iNbFrames)
	{ pSliceInfo->iNbFrames = iNbSlices;
	  pSliceInfo->iNbTimeSequences = iNbVolumes;
	}
      }
    }
  }
//  else
//    return DCM_E_UNSUPPORTED_FORMAT;

  /* PhotometricInterpretation */
  pVR = pDDO->GetVR(0x0028, 0x0004);
  if (pVR && pVR->Data)
    strncpy(pSliceInfo->szPhotometricInterpretation,
	(char*)pVR->Data, pVR->Length<19 ? pVR->Length : 19);

  if (memicmp((char *)pSliceInfo->szPhotometricInterpretation, "MONO", 4)!=0)
    return DCM_E_UNSUPPORTED_FORMAT;	// mvh 20040602

  /* PixelData */
  pVR = pDDO->GetVR(0x7fe0, 0x0010);

  rc = DecompressJPEG(pSliceInfo, pVR);
  if (rc != DCM_E_OK)
    return rc;

  /* Some tags need to been changed */

  /*
     The Attribute Lossy Image Compression (0028,2110) conveys that the Image has undergone
     lossy compression. It provides a means to record that the Image has been compressed (at a
     point in its lifetime) with a lossy algorithm and changes have been introduced into the pixel data.
     Once the value has been set to '01', it shall not be reset.
     Note: If an image is compressed with a lossy algorithm, the attribute Lossy Image Compression
     (0028,2110) is set to '01'. Subsequently, if the image is decompressed and transferred in
     uncompressed format, this attribute value remains '01'.
  */

  /* It seems we only have to change the TransferSyntaxUID to LittleEndianExplicit */
  pVR = new VR(0x0002, 0x0010, 20, (void*)"1.2.840.10008.1.2.1", FALSE);
  pDDO->ReplaceVR(pVR);
  delete pVR;

  return rc;
}

#endif

/***********************************************************************************/
/*

*/
/***********************************************************************************/

// create tempory filename in printer_files directory for (de)compression

static void NewTempFile(char *name)
	{
	int i;
	char name2[70];

	name[0]=0;
	GetPhysicalDevice("MAG0", name);
	strcat(name, "printer_files");

#ifndef WIN32
	mkdir(name, 0777);
#else
	mkdir(name);
#endif

	i = strlen(name);
	name[i]   = PATHSEPCHAR;
	name[i+1] = 0;

	GenUID(name2);
	strcat(name2, ".dcm");

	strcat(name, name2);
	}

// Decompress JPEG or NKI coded image file
BOOL
DecompressImageFile(char *file, int *Changed)
	{
	DICOMDataObject		*pDDO;
	PDU_Service		PDU;
	char			name[1024];
	VR			*pVR;
	BOOL			nki, jpeg, color=FALSE;
	char			option[20];
	int			len;

        *Changed = 0;

	PDU.AttachRTC(&VRType);

	pDDO = PDU.LoadDICOMDataObject(file);
	if(!pDDO)
		{
		OperatorConsole.printf("***[DecompressImageFile] %s -FAILED: Error on Load\n", file);
		return FALSE;
		}

	// is it nki compressed ?
	nki = (pDDO->GetVR(0x7fdf, 0x0010))!=NULL && (pDDO->GetVR(0x7fe0, 0x0010))==NULL;

	// is it jpeg compressed ?
	pVR = pDDO->GetVR(0x7fe0, 0x10);
	jpeg = (pVR && pVR->SQObjectArray);

        pVR = pDDO->GetVR(0x0028, 0x0004);
	if (pVR) color = memicmp((char*)pVR->Data, "MONO", 4)!=0;
#ifndef NOINTJPEG
	if (color && UseBuiltInDecompressor() && jpeg) 
		OperatorConsole.printf("[DecompressImageFile]: internal decompressor does not support color jpeg\n");
#endif

	if (!nki && !jpeg)		// not compressed
		{
		delete pDDO;
		return ( TRUE );
		}

	if (nki && !jpeg)		// nki compressed
		{
		if (!DecompressNKI(pDDO))
			{ 
			OperatorConsole.printf("***[DecompressImageFile]: failed to NKI decompress DICOM object %s\n", file);
			delete pDDO;
		        return FALSE;
			}

		SaveDICOMDataObject(file, pDDO);
		delete pDDO;
                *Changed = 1;
		return ( TRUE );
		}

#ifndef NOINTJPEG
        if(!UseBuiltInDecompressor() || color)
#endif
		{
	        DecompressJpeg++;

		NewTempFile(name);


// mvh 20040602: why is this deleted at all?
//	        pVR = pDDO->GetVR(0x0002, 0x0010);
//        	if (pVR) pDDO->DeleteVR(pVR);

		PDU.SaveDICOMDataObject(name, DICOM_CHAPTER_10_EXPLICIT, pDDO);

		option[0]=0;
		len = strlen(file);
		if (len>3)
			if (stricmp(file+len-3, ".v2")==0) 
				strcpy(option, "-F +ti");

#ifdef WIN32		
		if (!ExecHidden("dcmdjpeg.exe", option, name, file, ""))
#else
		if (!ExecHidden("dcmdjpeg", option, name, file, "DCMDICTPATH=dicom.dic\0"))
#endif
			{			// if executable fails incoming file will not change
			unlink(name);
			delete pDDO;
			return ( FALSE );
			}
	
		unlink(name);
		}
#ifndef NOINTJPEG
	else
	        if (dcmdjpeg(pDDO) != DCM_E_OK)
			{
			OperatorConsole.printf("***[DecompressImageFile]: JPEG decompression error\n");
			delete pDDO;
			return FALSE;
			}
#endif

	SaveDICOMDataObject(file, pDDO);
	delete pDDO;
        *Changed = 1;
	return ( TRUE );
	}

// Decompress NKI and/or JPEG coded image
BOOL
DecompressImage(DICOMDataObject **pDDO, int *Changed)
	{
	PDU_Service		PDU;
	char			name[1024], name2[1024];
	VR*			pVR;
	BOOL			nki, jpeg, color=FALSE;

        *Changed = 0;

	PDU.AttachRTC(&VRType);

	// is it nki compressed ?
	nki = ((*pDDO)->GetVR(0x7fdf, 0x0010))!=NULL && ((*pDDO)->GetVR(0x7fe0, 0x0010))==NULL;

	// is it jpeg compressed ?
	pVR = (*pDDO)->GetVR(0x7fe0, 0x10);
	jpeg = (pVR && pVR->SQObjectArray);

        pVR = (*pDDO)->GetVR(0x0028, 0x0004);
	if (pVR) color = memicmp((char*)pVR->Data, "MONO", 4)!=0;
#ifndef NOINTJPEG
	if (color && UseBuiltInDecompressor() && jpeg) 
		OperatorConsole.printf("[DecompressImage]: internal decompressor does not support color jpeg, using external decompressor\n");
#endif

	if (!nki && !jpeg) 		// not compressed
		return TRUE;

	if (nki && !jpeg)		// nki compressed
		{
		DecompressNKI(*pDDO);
                *Changed = 1;
		return ( TRUE );
		}

#ifndef NOINTJPEG
        if(!UseBuiltInDecompressor() || color)
#endif
		{
	        DecompressJpeg++;

		NewTempFile(name);
		NewTempFile(name2);

// mvh 20040602: why is this deleted at all?
//	        pVR = (*pDDO)->GetVR(0x0002, 0x0010);
//     		if (pVR) (*pDDO)->DeleteVR(pVR);

		PDU.SaveDICOMDataObject(name, DICOM_CHAPTER_10_EXPLICIT, *pDDO);
		delete *pDDO;
		*pDDO=NULL;

#ifdef WIN32		
		if (!ExecHidden("dcmdjpeg.exe", "-F +ti", name, name2, ""))
#else
		if (!ExecHidden("dcmdjpeg", "-F +ti", name, name2, "DCMDICTPATH=dicom.dic\0"))
#endif
		
			{
			*pDDO = PDU.LoadDICOMDataObject(name);
			unlink(name);
			return FALSE;
			}

		*pDDO = PDU.LoadDICOMDataObject(name2);
		if(!*pDDO)
			{
			OperatorConsole.printf("***[DecompressImage]: Error on load after external decompression, image not decompressed\n");
			*pDDO = PDU.LoadDICOMDataObject(name);	// mvh 20060402: restore original object
			unlink(name);
			//return FALSE;
			return TRUE;
			}

		unlink(name);
		unlink(name2);
		}
#ifndef NOINTJPEG
	else
	        if (dcmdjpeg(*pDDO) != DCM_E_OK)
			{
			OperatorConsole.printf("***[DecompressImage]: JPEG decompression error\n");
			return FALSE;
			}
#endif

        *Changed = 1;
	return ( TRUE );
	}

// Compress NKI coded image (FileCompressMode must be in [0..4], 0=uncompressed)
BOOL
CompressNKIImageFile(char *file, int FileCompressMode, int *ActualMode)
	{
	DICOMDataObject		*pDDO;
	PDU_Service		PDU;
	int			dum;

        *ActualMode = -1;

	PDU.AttachRTC(&VRType);

	pDDO = PDU.LoadDICOMDataObject(file);

	if(!pDDO)
		{
		OperatorConsole.printf("***[CompressNKIImageFile] %s -FAILED: Error on Load\n", file);
		return FALSE;
		}

	DecompressImage(&pDDO, &dum);	// make sure it is not NKI or JPEG compressed
        if (dum) *ActualMode = 0;

	if (FileCompressMode>0 && FileCompressMode<5)
		if (!CompressNKI(pDDO, FileCompressMode))
			{ 
			OperatorConsole.printf("***[CompressNKIImageFile]: failed to compress DICOM object %s\n", file);
			delete pDDO;
	        	return FALSE;
			}
	 
	SaveDICOMDataObject(file, pDDO);

	delete pDDO;
        *ActualMode = FileCompressMode;
	return ( TRUE );
	}

// Downsize image (FileCompressMode must be in ['1','2', '4','8'], '1' = to 1024)
BOOL
DownSizeImageFile(char *file, int FileCompressMode, int *ActualMode)
	{
	DICOMDataObject		*pDDO;
	PDU_Service		PDU;
	int			dum, rc, size;

        *ActualMode = -1;

	PDU.AttachRTC(&VRType);

	switch(FileCompressMode)
		{ 
		case '1': size = 1024; break;
		case '2': size =  512; break;
		case '4': size =  256; break;
		case '8': size =  128; break;
                default : size =65536; break;
		}

	pDDO = PDU.LoadDICOMDataObject(file);

	if(!pDDO)
		{
		OperatorConsole.printf("***[CompressNKIImageFile] %s -FAILED: Error on Load\n", file);
		return FALSE;
		}

	if (TestDownsize(pDDO, NULL, size))
		{
		DecompressImage(&pDDO, &dum);	// make sure it is not NKI or JPEG compressed
        	if (dum) *ActualMode = 0;

	        rc = MaybeDownsize(pDDO, NULL, size);

		if (!rc)
			{ 
			OperatorConsole.printf("***[CompressNKIImageFile]: failed to downsize DICOM object %s\n", file);
			delete pDDO;
		       	return FALSE;
			}
	 
		SaveDICOMDataObject(file, pDDO);

		if (rc==2) *ActualMode = FileCompressMode;
		}

	delete pDDO;
	return ( TRUE );
	}

// Compress JPEG coded image file (FileCompressMode must be in [0..6], 0=uncompressed)
BOOL
CompressJPEGImageFile(char *file, int FileCompressMode, int *ActualMode)
	{
	DICOMDataObject		*pDDO;
	PDU_Service		PDU;
	char			opts[20];
	char			name[1024];
	int			dum, len;
	VR*			pVR;

	len = strlen(file);
	if (len>3)
		if (stricmp(file+len-3, ".v2")==0)
			{
			OperatorConsole.printf("[CompressJPEGImageFile]: cannot compress v2 file\n");
			return FALSE;
			}

        *ActualMode = -1;

	if (FileCompressMode=='0')
		{
		*ActualMode = '0';
		return DecompressImageFile(file, &dum);
		} 

	CompressJpeg++;

	PDU.AttachRTC(&VRType);

	NewTempFile(name);

	pDDO = PDU.LoadDICOMDataObject(file);
	if(!pDDO)
		{
		OperatorConsole.printf("***[CompressJPEGImageFile] %s -FAILED: Error on Load\n", file);
		return FALSE;
		}

	if ((pDDO->GetVR(0x7fdf, 0x0010)) != NULL)
		if (!DecompressNKI(pDDO))
			{
			OperatorConsole.printf("***[CompressJPEGImageFile]: failed to NKI decompress DICOM object %s\n", file);
			delete pDDO;
	        	return FALSE;
			}

	pVR = pDDO->GetVR(0x0028, 0x0100);
	if (pVR) 
		{
		BOOL DecompressNon16BitsJpeg;
		UseBuiltInDecompressor(&DecompressNon16BitsJpeg);

		int BitsAllocated = *((unsigned short*)pVR->Data);
		if (BitsAllocated!=16 && !DecompressNon16BitsJpeg)
			{
			OperatorConsole.printf("[CompressJPEGImageFile]: JPEG compression only supported for 16 bits data\n");
			delete pDDO;
	        	return FALSE;
			}
		}
	
	switch(FileCompressMode)
		{
		case '1': strcpy(opts, "+e1 +un +sr"); break;	// default lossless SV1 "1.2.840.10008.1.2.4.70" = jpeg 14 (33%)
		case '2': strcpy(opts, "+el +un +sr"); break;	// 	   lossless SV6 "1.2.840.10008.1.2.4.57" = jpeg 14 (33%)
		case '3': strcpy(opts, "+eb +un +sr"); break;	// baseline   (8 bits ) "1.2.840.10008.1.2.4.50" = jpeg 1  (15%)
		case '4': strcpy(opts, "+ee +un +sr"); break;	// extended   (12 bits) "1.2.840.10008.1.2.4.51" = jpeg 4  (15%)
		case '5': strcpy(opts, "+es +un +sr"); break;	// spectral sel 	"1.2.840.10008.1.2.4.53" = jpeg 8  (15%)
		case '6': strcpy(opts, "+ep +un +sr"); break;	// progressive		"1.2.840.10008.1.2.4.55" = jpeg 12 (14%)

		default:strcpy(opts, "+e1 +un +sr"); break;	// default = lossless SV1
		}
	 
	pVR = pDDO->GetVR(0x7fe0, 0x0010);
	if (pVR)
		pVR->TypeCode = 'OW';

// mvh 20040602: why is this deleted at all?
//	pVR = pDDO->GetVR(0x0002, 0x0010);
//	if (pVR) pDDO->DeleteVR(pVR);

	PDU.SaveDICOMDataObject(name, DICOM_CHAPTER_10_EXPLICIT, pDDO);
	delete pDDO;

#ifdef WIN32		
	if (!ExecHidden("dcmcjpeg.exe", opts, name, file, ""))
#else
	if (!ExecHidden("dcmcjpeg", opts, name, file, "DCMDICTPATH=dicom.dic\0"))
#endif
		{			// if executable fails file did not change
		unlink(name);
		return ( FALSE );
		}

	unlink(name);
        *ActualMode = FileCompressMode;
	return ( TRUE );
	}

// Compress JPEG coded image (FileCompressMode must be in [0..6], 0=uncompressed)
BOOL
CompressJPEGImage(DICOMDataObject **pDDO, int FileCompressMode, int *ActualMode)
	{
	PDU_Service		PDU;
	char			opts[20];
	char			name[1024], name2[1024];
	int			dum;
	VR*			pVR;

        *ActualMode = -1;

	if (FileCompressMode=='0')
		{
                *ActualMode = '0';
		return DecompressImage(pDDO, &dum);
		}

	CompressJpeg++;

	PDU.AttachRTC(&VRType);

	NewTempFile(name);
	NewTempFile(name2);

	if (((*pDDO)->GetVR(0x7fdf, 0x0010)) != NULL)
		if (!DecompressNKI(*pDDO))
			{
			OperatorConsole.printf("***[CompressJPEGImage]: failed to NKI decompress DICOM object\n");
	        	return FALSE;
			}

	pVR = (*pDDO)->GetVR(0x0028, 0x0100);
	if (pVR) 
		{
		BOOL DecompressNon16BitsJpeg;
		UseBuiltInDecompressor(&DecompressNon16BitsJpeg);

		int BitsAllocated = *((unsigned short*)pVR->Data);
		if (BitsAllocated!=16 && !DecompressNon16BitsJpeg)
			{
			OperatorConsole.printf("[CompressJPEGImage]: JPEG compression only supported for 16 bits data\n");
	        	return FALSE;
			}
		}

	switch(FileCompressMode)
		{
		case '1': strcpy(opts, "+e1 +un +sr"); break;	// default lossless SV1 "1.2.840.10008.1.2.4.70" = jpeg 14 (33%)
		case '2': strcpy(opts, "+el +un +sr"); break;	// 	   lossless SV6 "1.2.840.10008.1.2.4.57" = jpeg 14 (33%)
		case '3': strcpy(opts, "+eb +un +sr"); break;	// baseline   (8 bits ) "1.2.840.10008.1.2.4.50" = jpeg 1  (15%)
		case '4': strcpy(opts, "+ee +un +sr"); break;	// extended   (12 bits) "1.2.840.10008.1.2.4.51" = jpeg 4  (15%)
		case '5': strcpy(opts, "+es +un +sr"); break;	// spectral sel 	"1.2.840.10008.1.2.4.53" = jpeg 8  (15%)
		case '6': strcpy(opts, "+ep +un +sr"); break;	// progressive		"1.2.840.10008.1.2.4.55" = jpeg 12 (14%)

		default:strcpy(opts, "+e1 +un +sr"); break;	// default = lossless SV1
		}
	 
	pVR = (*pDDO)->GetVR(0x7fe0, 0x0010);
	if (pVR)
		pVR->TypeCode = 'OW';

// mvh 20040602: why is this deleted at all?
//	pVR = (*pDDO)->GetVR(0x0002, 0x0010);
//	if (pVR) (*pDDO)->DeleteVR(pVR);

	PDU.SaveDICOMDataObject(name, DICOM_CHAPTER_10_EXPLICIT, *pDDO);
	delete *pDDO;
	*pDDO = NULL;

#ifdef WIN32		
	if (!ExecHidden("dcmcjpeg.exe", opts, name, name2, ""))
#else
	if (!ExecHidden("dcmcjpeg", opts, name, name2, "DCMDICTPATH=dicom.dic\0"))
#endif
		{
		*pDDO = PDU.LoadDICOMDataObject(name);	// get back original image
		unlink(name);
		return FALSE;
		}

	*pDDO = PDU.LoadDICOMDataObject(name2);
	if(!*pDDO)
		{
		OperatorConsole.printf("***[CompressJPEGImage]: Error on load after: dcmcjpeg %s %s out.dcm)\n", opts, name);
		*pDDO = PDU.LoadDICOMDataObject(name);	// get back original image
		return FALSE;
		}

	unlink(name);
	unlink(name2);
        *ActualMode = FileCompressMode;
	return ( TRUE );
	}

BOOL recompressFile(char *File, char *Compression)
	{
	int 	status = 0;
	BOOL	rc = FALSE;

	if (stricmp(Compression, ""     )==0) return TRUE;
	if (memicmp(Compression, "as", 2)==0) return TRUE;
	if (stricmp(Compression, "is"   )==0) return TRUE;

	if      (memicmp(Compression, "un", 2)==0) rc = DecompressImageFile(File, &status);
	else if (memicmp(Compression, "j",  1)==0) rc = CompressJPEGImageFile(File, Compression[1], &status);
	else if (memicmp(Compression, "nj", 2)==0) 
		{
		DICOMDataObject*	pDDO;
		PDU_Service		PDU;
		char s[64];
		VR *vr;

		PDU.AttachRTC(&VRType);
		pDDO = PDU.LoadDICOMDataObject(File);

		s[0] = 0;
		vr = pDDO->GetVR(0x0002, 0x0010);
		if (vr && vr->Data)
			strncpy(s, (char*)vr->Data, vr->Length); s[vr->Length] = 0;

		delete pDDO;

		// leave JPEG as is
		if (strcmp(s, "1.2.840.10008.1.2.4.70")==0) return TRUE;
		if (strcmp(s, "1.2.840.10008.1.2.4.57")==0) return TRUE;
		if (strcmp(s, "1.2.840.10008.1.2.4.50")==0) return TRUE;
		if (strcmp(s, "1.2.840.10008.1.2.4.51")==0) return TRUE;
		if (strcmp(s, "1.2.840.10008.1.2.4.53")==0) return TRUE;
		if (strcmp(s, "1.2.840.10008.1.2.4.55")==0) return TRUE;

		if (!DecompressImageFile(File, &status)) return FALSE;
		rc = CompressNKIImageFile (File, MAXNKICOMPRESSION, &status);
		}
	else if (memicmp(Compression, "uj", 2)==0) 
		{
		DICOMDataObject*	pDDO;
		PDU_Service		PDU;
		char s[64];
		VR *vr;

		PDU.AttachRTC(&VRType);
		pDDO = PDU.LoadDICOMDataObject(File);

		s[0] = 0;
		vr = pDDO->GetVR(0x0002, 0x0010);
		if (vr && vr->Data)
			strncpy(s, (char*)vr->Data, vr->Length); s[vr->Length] = 0;

		delete pDDO;

		// leave JPEG as is
		if (strcmp(s, "1.2.840.10008.1.2.4.70")==0) return TRUE;
		if (strcmp(s, "1.2.840.10008.1.2.4.57")==0) return TRUE;
		if (strcmp(s, "1.2.840.10008.1.2.4.50")==0) return TRUE;
		if (strcmp(s, "1.2.840.10008.1.2.4.51")==0) return TRUE;
		if (strcmp(s, "1.2.840.10008.1.2.4.53")==0) return TRUE;
		if (strcmp(s, "1.2.840.10008.1.2.4.55")==0) return TRUE;

		rc = DecompressImageFile(File, &status);
		}
	else if (memicmp(Compression, "n",  1)==0) 
		{
		if (!DecompressImageFile(File, &status)) return FALSE;
		if (Compression[1]=='0') rc = TRUE;
		else                     rc = CompressNKIImageFile (File, Compression[1]-'0', &status);
		}
	else if (memicmp(Compression, "k",  1)==0) 
		{
		if (!DecompressImageFile(File, &status)) return FALSE;
		if (Compression[1]=='0') rc = TRUE;
		else                     rc = DownSizeImageFile (File, Compression[1], &status);
		}

        if (rc && status>0)
          OperatorConsole.printf("[recompressFile]: recompressed with mode = %s\n", Compression);

	return rc;
	}
	
static void Strip2(DICOMDataObject *pDDO)
{ 	DICOMObject	DO2;
	VR		*vr;

	while(vr=pDDO->Pop())
		{
	  	if (vr->Group==2) delete vr;
		else DO2.Push(vr);
		}

	pDDO->Reset();
	while(vr=DO2.Pop()) pDDO->Push(vr);
}

// use Filename to optionally forbid illegal compressions (e.g. nki->.dcm & jpeg->.v2)
BOOL recompress(DICOMDataObject **pDDO, char *Compression, char *Filename, BOOL StripGroup2)
	{
	int 	status = 0;
	BOOL	rc = FALSE;

	// no strip group 2 for these options!
	if (stricmp(Compression, ""     )==0) return TRUE;
	if (memicmp(Compression, "as", 2)==0) return TRUE;
	if (stricmp(Compression, "is"   )==0) return TRUE;
	if (memicmp(Compression, "v" , 1)==0) return TRUE;

	int t = time(NULL);

	if (memicmp(Compression, "un", 2)==0) 
		rc = DecompressImage(pDDO, &status);

	else if (memicmp(Compression, "j",  1)==0) 
		{
		char s[64];
		VR *vr = (*pDDO)->GetVR(0x0002, 0x0010);
		if (vr && vr->Data)
			{
			strncpy(s, (char*)vr->Data, vr->Length); s[vr->Length] = 0;

			// skip recompression to original level
			if ((strcmp(s, "1.2.840.10008.1.2.4.70")==0 && Compression[1]=='1') ||
			    (strcmp(s, "1.2.840.10008.1.2.4.57")==0 && Compression[1]=='2') ||
			    (strcmp(s, "1.2.840.10008.1.2.4.50")==0 && Compression[1]=='3') ||
			    (strcmp(s, "1.2.840.10008.1.2.4.51")==0 && Compression[1]=='4') ||
			    (strcmp(s, "1.2.840.10008.1.2.4.53")==0 && Compression[1]=='5') ||
			    (strcmp(s, "1.2.840.10008.1.2.4.55")==0 && Compression[1]=='6'))
			    	{
			    	if (StripGroup2) Strip2(*pDDO);
			    	return TRUE;
			    	}
			}
	
		if (!DecompressImage(pDDO, &status)) return FALSE;
		rc = CompressJPEGImage(pDDO, Compression[1], &status);
		}

	else if (memicmp(Compression, "nj",  2)==0) 
		{
		char s[64];
		VR *vr = (*pDDO)->GetVR(0x0002, 0x0010);
		if (vr && vr->Data)
			{
			strncpy(s, (char*)vr->Data, vr->Length); s[vr->Length] = 0;

			// leave JPEG as is
			if ((strcmp(s, "1.2.840.10008.1.2.4.70")==0) ||
			    (strcmp(s, "1.2.840.10008.1.2.4.57")==0) ||
			    (strcmp(s, "1.2.840.10008.1.2.4.50")==0) ||
			    (strcmp(s, "1.2.840.10008.1.2.4.51")==0) ||
			    (strcmp(s, "1.2.840.10008.1.2.4.53")==0) ||
		            (strcmp(s, "1.2.840.10008.1.2.4.55")==0))
			    	{
			    	if (StripGroup2) Strip2(*pDDO);
			    	return TRUE;
			    	}
			}
	
		if (!DecompressImage(pDDO, &status)) return FALSE;
		rc = CompressNKI(*pDDO, MAXNKICOMPRESSION);
		status = rc;
		}

	else if (memicmp(Compression, "uj",  2)==0) 
		{
		char s[64];
		VR *vr = (*pDDO)->GetVR(0x0002, 0x0010);
		if (vr && vr->Data)
			{
			strncpy(s, (char*)vr->Data, vr->Length); s[vr->Length] = 0;

			// leave JPEG as is
			if ((strcmp(s, "1.2.840.10008.1.2.4.70")==0) ||
			    (strcmp(s, "1.2.840.10008.1.2.4.57")==0) ||
			    (strcmp(s, "1.2.840.10008.1.2.4.50")==0) ||
			    (strcmp(s, "1.2.840.10008.1.2.4.51")==0) ||
			    (strcmp(s, "1.2.840.10008.1.2.4.53")==0) ||
		            (strcmp(s, "1.2.840.10008.1.2.4.55")==0))
			    	{
			    	if (StripGroup2) Strip2(*pDDO);
			    	return TRUE;
			    	}
			}
	
		rc = DecompressImage(pDDO, &status);
		}

	else if (memicmp(Compression, "n",  1)==0) 
		{
		// skip recompression to original level
		signed char *p;
		VR *vr = (*pDDO)->GetVR(0x7fdf, 0x0010);
		if (vr)
			{
    			p = ((signed char *)(vr->Data));
		    	if (p && get_nki_private_compress_mode(p)==Compression[1]-'0')
			    	{
			    	if (StripGroup2) Strip2(*pDDO);
			    	return TRUE;
			    	}
			}
    
		if (!DecompressImage(pDDO, &status)) return FALSE;

		if (Compression[1]=='0') rc = TRUE;
		else                     
			{
			rc = CompressNKI(*pDDO, Compression[1]-'0');
			status = rc;
			}
		}

	else if (memicmp(Compression, "k",  1)==0) 
		{
		int size;

		switch (Compression[1])
			{ 
			case '1': size = 1024; break;
			case '2': size =  512; break;
			case '4': size =  256; break;
			case '8': size =  128; break;
                	default : size =65536; break;
			}

		if (TestDownsize(*pDDO, NULL, size))
			{
			if (!DecompressImage(pDDO, &status)) return FALSE;

			switch (Compression[1])
				{
				case '1': rc = MaybeDownsize(*pDDO, NULL, 1024); break;
				case '2': rc = MaybeDownsize(*pDDO, NULL, 512); break;
				case '4': rc = MaybeDownsize(*pDDO, NULL, 256); break;
				case '8': rc = MaybeDownsize(*pDDO, NULL, 128); break;
				}
			if (rc==2) status = 1;
			}
		else
			status = 1;
		}

	if (StripGroup2) Strip2(*pDDO);

        if (rc && status>0)
          OperatorConsole.printf("[recompress]: recompressed with mode = %s (strip=%d)\n", Compression, StripGroup2);

	RecompressTime += time(NULL)-t;

	return rc;
	}

///////////////////////////////////////////////////////////////////////////////////////////////
// PDU with dgate compression styles
///////////////////////////////////////////////////////////////////////////////////////////////

BOOL
ExtendedPDU_Service	::	AddTransferSyntaxs(PresentationContext &PresContext)
	{
	UID		uid;
	TransferSyntax	TrnSyntax;

	// as-is not correctly implemented (hard to do, since each image will require other compression)
	// will now tranmsit over ImplicitLittleEndian - OK for NKI clients

	if (memicmp(RequestedCompressionType, "as", 2)==0 || memicmp(RequestedCompressionType, "is", 2)==0)
		{
		OperatorConsole.printf("Warning: use of as is compression for outgoing associations not fully implemented\n");
		}
			
	// only jpeg requires other tranmission syntaxes

	if (RequestedCompressionType[0]=='j' || RequestedCompressionType[0]=='J')
		{
		switch(RequestedCompressionType[1])
			{
			case '1': // lossless
				  uid.Set("1.2.840.10008.1.2.4.70"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  break;

			case '2': // losless sv 6
				  uid.Set("1.2.840.10008.1.2.4.57"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  uid.Set("1.2.840.10008.1.2.4.70"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  break;

			case '3': // lossy baseline (8bit) + extended(12 bit)
			case '4': uid.Set("1.2.840.10008.1.2.4.51"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  uid.Set("1.2.840.10008.1.2.4.50"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  break;

			case '5': // spectral selection
				  uid.Set("1.2.840.10008.1.2.4.53"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  uid.Set("1.2.840.10008.1.2.4.51"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  uid.Set("1.2.840.10008.1.2.4.50"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  break;

			case '6': // progressive
				  uid.Set("1.2.840.10008.1.2.4.55"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  uid.Set("1.2.840.10008.1.2.4.51"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  uid.Set("1.2.840.10008.1.2.4.50"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  break;

			default:  // lossless
				  uid.Set("1.2.840.10008.1.2.4.70"); TrnSyntax.Set(uid);
				  PresContext.AddTransferSyntax(TrnSyntax);
				  break;
			}
		}

	uid.Set("1.2.840.10008.1.2");		// ImplicitLittleEndian
	TrnSyntax.Set(uid);
	PresContext.AddTransferSyntax(TrnSyntax);

	return ( TRUE );
	}

BOOL
ExtendedPDU_Service	::	SetRequestedCompressionType(char *type)
	{
	strcpy(RequestedCompressionType, type);
	return ( TRUE );
	}

char*
ExtendedPDU_Service	::	GetAcceptedCompressionType(UID uid)
	{
	UID TrnSyntaxUID;
	int ContextID = GetPresentationContextID(uid);
	char *p;

	if ( !ContextID )
		{
		strcpy(AcceptedCompressionType, "");
		return ( AcceptedCompressionType );
		}

	GetTransferSyntaxUID(ContextID, TrnSyntaxUID);

	if (RequestedCompressionType[0]!='j' && RequestedCompressionType[0]!='J')
		{
		strcpy(AcceptedCompressionType, RequestedCompressionType);
		return ( AcceptedCompressionType );
		}

	p = (char *)TrnSyntaxUID.GetBuffer(1);
	     if (strcmp(p, "1.2.840.10008.1.2.4.70")==0) strcpy(AcceptedCompressionType, "j1");
	else if (strcmp(p, "1.2.840.10008.1.2.4.57")==0) strcpy(AcceptedCompressionType, "j2");
	else if (strcmp(p, "1.2.840.10008.1.2.4.50")==0) strcpy(AcceptedCompressionType, "j3");
	else if (strcmp(p, "1.2.840.10008.1.2.4.51")==0) strcpy(AcceptedCompressionType, "j4");
	else if (strcmp(p, "1.2.840.10008.1.2.4.53")==0) strcpy(AcceptedCompressionType, "j5");
	else if (strcmp(p, "1.2.840.10008.1.2.4.55")==0) strcpy(AcceptedCompressionType, "j6");
	else strcpy(AcceptedCompressionType, "un");

	OperatorConsole.printf("Accepted compression: %s\n", AcceptedCompressionType);

	return ( AcceptedCompressionType );
	}

/* Downsize and convert image to 8 bit monochrome or RGB */

static BOOL To8bitMonochromeOrRGB(DICOMDataObject* pDDO, int size, int *Dimx, int *Dimy, unsigned char **out, int RGBout=0, int level=0, int window=0)
{ int			i, j;
  VR*			pVR;
  unsigned int		iMaxRowsColumns;
  unsigned short	iRows, iColumns, iNewRows, iNewColumns, iDownsizeFactor;
  int			iImageSize, iNbBytesPerPixel, iNbPixels, iSkip, dum;
  char*			pcSrc;
  unsigned char*	pcDest;
  short*		psSrc;
  int*			piSrc;
  int			r, g, b;
  int                   RescaleIntercept = -1024;

  iMaxRowsColumns = size;

  /* OK. Now check whether all necessary PixelData-info is present in pDDO */
  pVR = pDDO->GetVR(0x0028, 0x0010);	/* Rows */
  if (!pVR)
    return FALSE;
  iRows = *((unsigned short*)pVR->Data);

  pVR = pDDO->GetVR(0x0028, 0x0011);	/* Columns */
  if (!pVR)
    return FALSE;
  iColumns = *((unsigned short*)pVR->Data);
  iNbPixels = iRows * iColumns;

  if (!iNbPixels) 			/* Image must make sense */
    return FALSE;

  pVR = pDDO->GetVR(0x0028, 0x1052);	/* RescaleIntercept */
  if ( pVR)
  { char s[256];
    strncpy(s, (const char*)pVR->Data, pVR->Length);
    s[pVR->Length] = 0;
    RescaleIntercept = atoi(s);
  }

  if (iRows >= iColumns)
  { iDownsizeFactor = iRows / iMaxRowsColumns;
    if (iRows % iMaxRowsColumns)
      iDownsizeFactor ++;
  }
  else
  { iDownsizeFactor = iColumns / iMaxRowsColumns;
    if (iColumns % iMaxRowsColumns)
      iDownsizeFactor ++;
  }

  if (iDownsizeFactor<1) iDownsizeFactor=1;

  if (!DecompressImage(&pDDO, &dum))	/* make sure it is not NKI or JPEG compressed */
    return FALSE;

  pVR = pDDO->GetVR(0x7fe0, 0x0010);	/* Pixeldata */
  if (!pVR)
    return TRUE;
  iImageSize = pVR->Length;
  if ((iImageSize < iNbPixels) ||
      (iImageSize % iNbPixels))
    return FALSE;			/* Image doesn't make sense */

  iNbBytesPerPixel = iImageSize / iNbPixels;
  if (iNbBytesPerPixel > 4)
    return FALSE;			/* Image is too beautiful to convert! */

  /* LETS DO IT !
     The downsized image is stored in the same memory-block as the original one:
     Just be sure to adjust the length-indicator. The image will be (re)compressed anyway
     later on.
  */

  iNewRows    = iRows / iDownsizeFactor;
  if (!iNewRows)
    iNewRows++;
  iNewColumns = iColumns / iDownsizeFactor;
  if (!iNewColumns)
    iNewColumns++;

  if (RGBout & (iNewColumns&3)!=0) 
    iNewColumns &= 0xfffffffc;	// make multiple of 4 (truncate some columns if needed)

  *Dimy = iNewRows;
  *Dimx = iNewColumns;

  *out = (unsigned char *)malloc(iNewRows * iNewColumns * (RGBout?3:1));
  pcDest = *out;

  switch (iNbBytesPerPixel + 10 * RGBout)
  { case 1:
      pcSrc  = (char*)pVR->Data;
      for (i=0; i<iNewRows; i++)
      { for (j=0; j<iNewColumns; j++)
	{ *pcDest++ = *pcSrc;
	  pcSrc += iDownsizeFactor;
	}
	pcSrc = (char*)pVR->Data + iDownsizeFactor * iColumns * (i + 1);
      }
      break;

    case 2:
    { int max=0;
      psSrc  = (short*)pVR->Data;

      if (window)
      { for (i=0; i<iNewRows; i++)
        { for (j=0; j<iNewColumns; j++)
	  { r = *psSrc + RescaleIntercept + 1024;
            r = 255 * (r - level + window/2) / window;
            if (r>255) r=255;
            if (r<0)   r=0;
            *pcDest++ = r;
	    psSrc += iDownsizeFactor;
  	  }
	  psSrc = (short*)pVR->Data + iDownsizeFactor * iColumns * (i + 1);
        }
      }
      else
      { for (i=0; i<iNewRows; i++)
        { for (j=0; j<iNewColumns; j++)
  	  { r = *psSrc + RescaleIntercept + 1024;
            if (r > max) max=r;
            if (r>2047) r=2047;
            if (r<0)    r=0;
            r = r/8;
            *pcDest++ = r;
  	    psSrc += iDownsizeFactor;
  	  }
  	  psSrc = (short*)pVR->Data + iDownsizeFactor * iColumns * (i + 1);
        }
  
        // auto scale if too dark or too bright
  
        if (max < 1024 || max > 4095)
        { int factor = (int)((max / 256.0) + 0.5);
          if (factor==0) factor=1;
  
          pcDest = *out;
          psSrc  = (short*)pVR->Data;
          for (i=0; i<iNewRows; i++)
          { for (j=0; j<iNewColumns; j++)
  	    { r = *psSrc + RescaleIntercept + 1024;
              r = r/factor;
              if (r>255) r=255;
              if (r<0)   r=0;
              *pcDest++ = r;
  	      psSrc += iDownsizeFactor;
    	    }
    	    psSrc = (short*)pVR->Data + iDownsizeFactor * iColumns * (i + 1);
          }
        }
      }

      break;
    }

    case 3:
      pcSrc  = (char*)pVR->Data;
      iSkip = (iDownsizeFactor - 1) * 3;
      for (i=0; i<iNewRows; i++)
      { for (j=0; j<iNewColumns; j++)
	{ r = *pcSrc++;
	  g = *pcSrc++;
	  b = *pcSrc++;
          *pcDest++ = (r+g+b)/3;
	  pcSrc += iSkip;
	}
	pcSrc = (char*)pVR->Data + iDownsizeFactor * 3 * iColumns * (i + 1);
      }
      break;

    case 4:
      piSrc  = (int*)pVR->Data;
      if (window)
      { for (i=0; i<iNewRows; i++)
        { for (j=0; j<iNewColumns; j++)
  	  { r = *piSrc + RescaleIntercept + 1024;
            r = 255 * (r - level + window/2) / window;
            if (r>255) r=255;
            if (r<0)   r=0;
            *pcDest++ = r;
	    piSrc += iDownsizeFactor;
	  }
	  piSrc = (int*)pVR->Data + iDownsizeFactor * iColumns * (i + 1);
        }
      }
      else
      { for (i=0; i<iNewRows; i++)
        { for (j=0; j<iNewColumns; j++)
  	  { *pcDest++ = (*piSrc + RescaleIntercept+1024) / 8;
    	    piSrc += iDownsizeFactor;
	  }
	  piSrc = (int*)pVR->Data + iDownsizeFactor * iColumns * (i + 1);
        }
      }
      break;

    case 11:
      for (i=0; i<iNewRows; i++)
      { pcSrc = (char*)pVR->Data + iDownsizeFactor * iColumns * (iNewRows-1-i);
        for (j=0; j<iNewColumns; j++)
	{ *pcDest++ = *pcSrc;
	  *pcDest++ = *pcSrc;
	  *pcDest++ = *pcSrc;
	  pcSrc += iDownsizeFactor;
	}
      }
      break;

    case 12:
    { int max=0;
      if (window)
      { for (i=0; i<iNewRows; i++)
        { psSrc = (short*)pVR->Data + iDownsizeFactor * iColumns * (iNewRows-1-i);
          for (j=0; j<iNewColumns; j++)
	  { r = *psSrc + RescaleIntercept + 1024;
            r = 255 * (r - level + window/2) / window;
            if (r>255) r=255;
            if (r<0)   r=0;
            *pcDest++ = r;
            *pcDest++ = r;
            *pcDest++ = r;
	    psSrc += iDownsizeFactor;
  	  }
        }
      }
      else
      { for (i=0; i<iNewRows; i++)
        { psSrc = (short*)pVR->Data + iDownsizeFactor * iColumns * (iNewRows-1-i);
          for (j=0; j<iNewColumns; j++)
  	  { r = *psSrc + RescaleIntercept + 1024;
            if (r > max) max=r;
            if (r>2047) r=2047;
            if (r<0)    r=0;
            r = r/8;
            *pcDest++ = r;
            *pcDest++ = r;
            *pcDest++ = r;
  	    psSrc += iDownsizeFactor;
  	  }
        }
  
        // auto scale if too dark or too bright
  
        if (max < 1024 || max > 4095)
        { int factor = (int)((max / 256.0) + 0.5);
          if (factor==0) factor=1;
  
          pcDest = *out;
          for (i=0; i<iNewRows; i++)
          { psSrc = (short*)pVR->Data + iDownsizeFactor * iColumns * (iNewRows-1-i);
            for (j=0; j<iNewColumns; j++)
  	    { r = *psSrc + RescaleIntercept + 1024;
              r = r/factor;
              if (r>255) r=255;
              if (r<0)   r=0;
              *pcDest++ = r;
              *pcDest++ = r;
              *pcDest++ = r;
  	      psSrc += iDownsizeFactor;
    	    }
          }
        }
      }

      break;
    }

    case 13:
      iSkip = (iDownsizeFactor - 1) * 3;
      for (i=0; i<iNewRows; i++)
      { pcSrc = (char*)pVR->Data + iDownsizeFactor * 3 * iColumns * (iNewRows-1-i);
        for (j=0; j<iNewColumns; j++)
	{ *pcDest++ = *pcSrc++;
	  *pcDest++ = *pcSrc++;
	  *pcDest++ = *pcSrc++;
	  pcSrc += iSkip;
	}
      }
      break;

    case 14:
      if (window)
      { for (i=0; i<iNewRows; i++)
        { piSrc = (int*)pVR->Data + iDownsizeFactor * iColumns * (iNewRows-1-i);
          for (j=0; j<iNewColumns; j++)
  	  { r = *piSrc + RescaleIntercept + 1024;
            r = 255 * (r - level + window/2) / window;
            if (r>255) r=255;
            if (r<0)   r=0;
            *pcDest++ = r;
  	    *pcDest++ = r;
	    *pcDest++ = r;
	    piSrc += iDownsizeFactor;
	  }
        }
      }
      else
      { for (i=0; i<iNewRows; i++)
        { piSrc = (int*)pVR->Data + iDownsizeFactor * iColumns * (iNewRows-1-i);
          for (j=0; j<iNewColumns; j++)
  	  { *pcDest++ = (*piSrc + RescaleIntercept+1024) / 8;
  	    *pcDest++ = (*piSrc + RescaleIntercept+1024) / 8;
	    *pcDest++ = (*piSrc + RescaleIntercept+1024) / 8;
	    piSrc += iDownsizeFactor;
	  }
        }
      }
      break;
  }

  return TRUE;
}

int WriteGIF(FILE *fp, unsigned char *pic, int ptype, int w, int h, 
                       unsigned char *rmap, unsigned char *gmap, unsigned char *bmap,
                       int numcols, int colorstyle, char *comment);

BOOL ToGif(DICOMDataObject* pDDO, char *filename, int size, int append, int level, int window)
{ int dimx, dimy;
  unsigned char *out;
  FILE *f;
  unsigned char lut[256];
  int i;

  for(i=0; i<256; i++) lut[i]=i;

  if (To8bitMonochromeOrRGB(pDDO, size, &dimx, &dimy, &out, 0, level, window))
  { if (append) f = fopen(filename, "ab");
    else        f = fopen(filename, "wb");
    if (!f)
    { free(out);
      return FALSE;
    }

    WriteGIF(f, out, 0, dimx, dimy, lut, lut, lut, 256, 0, "ConquestDICOMServer");
    free(out);
    fclose(f);
    return TRUE;
  }

  return FALSE;
}

#pragma pack(1)
typedef struct  
{       char	bfType1;		/* Must be 'B' */
        char	bfType2;		/* Must be 'M' */
        long	bfSize;			/* Size of the file */
        short	bfReserved1;		/* Must be 0 */
        short	bfReserved2;		/* Must be 0 */
        long	bfOffBits;		/* Offset of the ImageBits */

	long	biSize;			/* size of all biXX (40) */
	long	biWidth;
	long	biHeight;
	short	biPlanes;
	short	biBitCount;
	long	biCompression;
	long	biSizeImage;
	long	biXPelsPerMeter;
	long	biYPelsPerMeter;
	long	biClrUsed;
	long	biClrImportant;
} BMP_HEADER;	/* 24BITS */
#pragma pack()

BOOL ToBMP(DICOMDataObject* pDDO, char *filename, int size, int append, int level, int window)
{ int dimx, dimy;
  unsigned char *out;
  FILE *f;
  int i, iDataSize;
  BMP_HEADER bmp;

  if (To8bitMonochromeOrRGB(pDDO, size, &dimx, &dimy, &out, 1, level, window))
  { iDataSize = dimx*dimy*3;	// must be multiple of 4: OK because above routine made dimx multiple of 4

    memset((void*)&bmp, 0, sizeof(BMP_HEADER));
    bmp.bfType1 = 'B';
    bmp.bfType2 = 'M';
    bmp.bfSize = iDataSize + sizeof(BMP_HEADER);
    bmp.bfOffBits = sizeof(BMP_HEADER);

    bmp.biSize = 40;
    bmp.biWidth = dimx;
    bmp.biHeight = dimy;
    bmp.biPlanes = 1;
    bmp.biBitCount = 24;
    bmp.biSizeImage = iDataSize;
    bmp.biXPelsPerMeter = 0x0ece;
    bmp.biYPelsPerMeter = 0x0ece;

    if (append) f = fopen(filename, "ab");
    else        f = fopen(filename, "wb");
    if (!f)
    { free(out);
      return FALSE;
    }

    fwrite((void*)&bmp, sizeof(BMP_HEADER), 1, f);
    fwrite(out,         dimx*dimy*3,        1, f);
    
    free(out);
    fclose(f);
    return TRUE;
  }

  return FALSE;
}
