/*
 * Reads DICOMObject's from disk.  As well as makes Windows BITMAP
 * Structures.
 *
 * Bugs:	Does not handle sequence objects
 *			Does not support the recent change to DICOM Chapter 10
 *			Only works on 32 bit little-endian architecture
 * Notes:
 *
 *			Eventually this routine will be not be needed.  I will
 *	add Disk I/O to the PDU_Service class, which will be a much cleaner,
 *	and much more portable solution.
 * Mark Oskin
 *
 */

/*
19980409 mvh     Added NKI private decompression engine VR 0x7fdf,0x0010
19980410 mvh     Added run length encoding of zero differences
19980410 ljz+mvh Fix 16 bit absolute value decode
19980415 ljz+mvh Fix leak on decompress
19980625 mvh     Added compression in SaveDicomDataObject(C10)
19980703 mvh+ljz Made DObjectSerialize thread save (removed statics)
19990105 ljz     Fix: SaveDICOMDataObjectC10 created VR 0x0002 0x0013 twice
                 Added comment in SaveDICOMDataObjectC10; CleanUp
                 Put the nki-routines also in dgate.hpp
                 Added LoadImplicitLittleEndianFile
19990317 ljz     Decompression removed from LoadImplicitLittleEndianFile; added a
                 VR-sizelimit of that reader
19990827 mvh     NOTE: LoadImplicitLittleEndianFile allocates 100 MB and crashes when passing
                 chapter 10 file (happens when extension is wrongly .v2)
19990830 ljz     Fixed problem above: 'LoadImplicitLittleEndianFile' uses
		 'PDU.LoadDICOMDataObject' in such cases.
19991117 ljz     Added parameter FileCompressMode to in nki_private_compress call
20000629 ljz     Logging of trouble now starts with '***'
20001104 mvh     Renamed _LittleEndianUID to _LittleEndianUID_space
20001105 mvh     Fixed where malloc and new were mixed (vr->data)
20001106 mvh     Use delete [] operation for vr->Data
20001128 ljz     Fix in 'Implicit length' code  in LoadImplicitLittleEndian
20010429 mvh     Use faster and safer decompressor
20020412 ljz     Added class CBufferedIO for faster reading from DVD (stream functions
		 do not seem to be cacheing...)
20020415 mvh     Removed diagnostic time code (did not compile on ms4.2)
                 Made buffer size dependent on iVrSizelimit 
                 (i.e, use smaller for regen, slightly faster)
20030706 mvh	 Attach VRType to PDU's for implicit little endian support
20041130 mvh	 Documented crash on win2000 server in code - remain to be fixed
20050107 mvh     Adapted for linux compile
20051219 mvh     Use ReAlloc instead of new BYTE[] to fill VR data
20070308 bcb     And little/big endian.
*/


#ifdef WIN32
#include	<io.h>
#else
#include	<unistd.h>
#endif

#include	<fcntl.h>
#include	<sys/stat.h>

#ifndef	WHEDGE
#	include	"dgate.hpp"
#else
#	include	"master.h"
#endif

typedef	struct	_DEIVR
	{
	UINT16		Group;
	UINT16		Element;
	UINT32		Length;
	}	DEIVR;


// Serializes the input of a DICOM Data object 1 byte at a time.  Very usefull
// routine to use if memory constraints are present

typedef	struct 	_DOBJECTSERSTATE
	{
	int	state;
	UINT16	stuint16;
	UINT32	stuint32;
	DEIVR	deivr;
	VR	*vr;
	UINT	sDIndex;
	}	DOBJECTSERSTATE;

DICOMDataObject	*
DObjectSerialize ( DICOMDataObject  *DDO, unsigned	char	ch,
			BOOL	LFrame, DOBJECTSERSTATE *doss)
	{

//	NOTE: THIS CODE WAS NOT THREADSAVE DUE TO THE USE OF STATIC VARIABLES !
//	static	int	state;
//	static	UINT16	stuint16;
//	static	UINT32	stuint32;
//	static	DEIVR	deivr;
//	static	VR	*vr;
//	static	UINT	sDIndex;

	UINT32	tuint32;

	if(!DDO)	// re-intialize state engine
		{
		doss->state = 0;
		return (new DICOMDataObject);
		}
	
	tuint32 = ch;

	switch ( doss->state )
		{
		case	0:	// base state
			doss->stuint16 = (UINT16) (tuint32 << 0);
			doss->state = 1;
			return (DDO);

		case	1:
			doss->stuint16 = doss->stuint16 | (UINT16) (tuint32 << 8);
			doss->deivr.Group = doss->stuint16;
			doss->state = 2;
			return ( DDO );

		case	2:
			doss->stuint16 = (UINT16) (tuint32 << 0);
			doss->state = 3;
			return ( DDO );

		case	3:
			doss->stuint16 = doss->stuint16 | (UINT16) (tuint32 << 8);
			doss->deivr.Element = doss->stuint16;
			doss->state = 4;
			return ( DDO );

		case	4:
			doss->stuint32 = (tuint32 << 0);
			doss->state = 5;
			return ( DDO );

		case	5:
			doss->stuint32 = doss->stuint32 | (tuint32 << 8);
			doss->state = 6;
			return ( DDO );

		case	6:
			doss->stuint32 = doss->stuint32 | (tuint32 << 16);
			doss->state = 7;
			return ( DDO );

		case	7:
			doss->stuint32 = doss->stuint32 | (tuint32 << 24);
			doss->deivr.Length = doss->stuint32;
			doss->sDIndex = 0;
			if(doss->deivr.Group==0x7fe0 && doss->deivr.Element==0x0010)
				if(LFrame)
					{
					return ( NULL );
					}
			if(doss->deivr.Group==0x7fdf && doss->deivr.Element==0x0010)
				if(LFrame)
					{
					return ( NULL );
					}

			doss->vr = new VR(doss->deivr.Group, doss->deivr.Element, doss->deivr.Length, TRUE);

			if(!doss->deivr.Length)
				{
				DDO->Push(doss->vr);
				doss->state = 0;
				}
			else
				doss->state = 8;
			return ( DDO );

		case	8:
			if(!doss->vr->Data)
				return ( NULL );	// error

			((BYTE *) doss->vr->Data ) [doss->sDIndex] = ch;
			++(doss->sDIndex);

			if(doss->sDIndex == doss->deivr.Length)
				{
				if(doss->deivr.Group==0x7fdf && doss->deivr.Element==0x0010)
					{ 
					VR *v;
					signed char *CompressedData = ((signed char *)(doss->vr->Data));
      					int UncompressedLength = get_nki_private_decompressed_length(CompressedData);
					v = new VR(0x7fe0, 0x0010, UncompressedLength, TRUE);
      					nki_private_decompress((short *)(v->Data), CompressedData, v->Length);
					DDO->Push(v);
					delete doss->vr; // we stored a copy
					}
				else
					DDO->Push(doss->vr);

				doss->state = 0;
				}
			return ( DDO );
		}
	return ( NULL );	// error
	}



DICOMDataObject	*
LoadDICOMDataObject(char	*filename)
	{
	DICOMDataObject	*DDO;
	UINT	MemBlock, Block;
	UINT32	TCount, Index;
	unsigned	char	*mem;
	FILE	*fp;
        DOBJECTSERSTATE doss;

	MemBlock = 50000;

	fp = fopen(filename, "rb");
	if(!fp)
		{
		return ( FALSE );
		}

	mem = (unsigned	char *) malloc(MemBlock);
	if(!mem)
		{
		fclose(fp);
		return ( FALSE );
		}

	DDO = DObjectSerialize(NULL, 0, FALSE, &doss);
	if(!DDO)
		{
		free(mem);
		return ( FALSE );
		}
	TCount = 0;
	while( TRUE )
		{
		Block = fread(mem, 1, MemBlock, fp);

		Index = 0;
		if(!TCount)
			{
			if(!strncmp((char*) &mem[128], "DICM", 4))
				Index = 128 + 4;
			}
		TCount += Block;
		while(Index < Block)
			{
			DObjectSerialize(DDO, mem[Index], FALSE, &doss);
			++Index;
			}
		if(Block!=MemBlock)
			break;
		}
	free(mem);
	fclose ( fp );
	return ( DDO );
	}


static BOOL
SaveDDO(DICOMDataObject* DDOPtr, FILE* fp, int FileCompressMode)
{
	VR	*TempVR;
#if NATIVE_ENDIAN == BIG_ENDIAN //Big Endian like Apple power pc
	UINT16	beGroup, beElement;
	UINT32	beLength;
#endif //Big Endian

	while(TempVR = DDOPtr->Pop())
	{
		if(TempVR->Group==0x7fe0 && TempVR->Element==0x0010 && FileCompressMode)
		{
			int DataLength;
			short CompressedGroup = 0x7fdf, CompressedElement = 0x0010;
			signed char *CompressedData;

			CompressedData = new signed char [(TempVR->Length/2) * 3 + 10];

			DataLength = nki_private_compress(CompressedData, (short int *)(TempVR->Data), TempVR->Length/2, FileCompressMode);
			if (!CompressedData)
			{
				OperatorConsole.printf("***Out of memory NKI-compress");
				return FALSE;
			}

			if (DataLength&1)
				CompressedData[DataLength++] = 0;

#if NATIVE_ENDIAN == LITTLE_ENDIAN //Little Endian
			fwrite(&CompressedGroup,   1, 2, fp);
			fwrite(&CompressedElement, 1, 2, fp);
			fwrite(&DataLength,        1, 4, fp);
#else //Big Endian like Apple power pc
			beGroup = SwitchEndian((UINT16) CompressedGroup);
			beElement = SwitchEndian((UINT16) CompressedElement);
			beLength = SwitchEndian((UINT32) DataLength);
			fwrite(&beGroup,   1, 2, fp);
			fwrite(&beElement, 1, 2, fp);
			fwrite(&beLength,  1, 4, fp);
#endif //Big Endian

			if (fwrite(CompressedData, 1, DataLength, fp) != (unsigned int)DataLength)
			{
				free(CompressedData);
				OperatorConsole.printf("***Failed to save compressed pixel data (disk full?)");
				return FALSE;
			}

			delete [] CompressedData;
		}
		else
		{
#if NATIVE_ENDIAN == LITTLE_ENDIAN //Little Endian
			fwrite(&TempVR->Group, 1, 2, fp);
			fwrite(&TempVR->Element, 1, 2, fp);
			fwrite(&TempVR->Length, 1, 4, fp);
#else //Big Endian like Apple power pc
			beGroup = SwitchEndian((UINT16) TempVR->Group);
			beElement = SwitchEndian((UINT16) TempVR->Element);
			beLength = SwitchEndian((UINT32) TempVR->Length);
			fwrite(&beGroup,   1, 2, fp);
			fwrite(&beElement, 1, 2, fp);
			fwrite(&beLength,  1, 4, fp);
#endif //Big Endian
			if(TempVR->Length)
			{
				if(!TempVR->Data)
				{
					OperatorConsole.printf("***Missing data in VR (0x%04x,0x%04x)",
						TempVR->Group, TempVR->Element);
					return FALSE;
				}
				fwrite(TempVR->Data, 1, TempVR->Length, fp);
			}
		}
		delete TempVR;
	}
	return TRUE;	
}

BOOL
SaveDICOMDataObject(DICOMDataObject *DDOPtr, char *filename, int FileCompressMode)
{
	FILE*	fp;
	BOOL	rc;

	fp = fopen(filename, "wb");
	if(!fp) return FALSE;
	rc = SaveDDO(DDOPtr, fp, FileCompressMode);
	fclose(fp);
	return rc;	
}

#	define	_LittleEndianUID_space	"1.2.840.10008.1.2 "
#	define	_ImplementationUID	"none yet"
#	define	_ImplementationVersion	"0.1AlphaUCDMC "
#	define	_SourceApplicationAE	"none            "

BOOL
SaveDICOMDataObjectC10(DICOMDataObject *DDOPtr, char *filename, int FileCompressMode)
{
	VR		*vr, *vr1;
	UINT16	tuint16;
	FILE	*fp;
	char	s[140];
	BOOL	rc;

	vr = DDOPtr->GetVR(0x0002, 0x0001);
	if(!vr)
	{
		// This does not contain the C10 Header yet, so we need to
		// constuct it
		// FileMetaInformationVersion
		vr = new VR(0x0002, 0x0001, 0x0002, TRUE);
		tuint16 = 0x0001;
		memcpy(vr->Data, (void*)&tuint16, 2);
		DDOPtr->Push(vr);
		// SOPClassUID
		vr = DDOPtr->GetVR(0x0008, 0x0016);
		if(vr)
		{
			// MediaStorageSOPClassUID
			vr1 = new VR(0x0002, 0x0002, vr->Length, TRUE);
			memcpy(vr1->Data, vr->Data, vr->Length);
			DDOPtr->Push(vr1);
		}
		// SOPInstanceUID
		vr = DDOPtr->GetVR(0x0008, 0x0018);
		if(vr)
		{
			// MediaStorageSOPInstanceUID
			vr1 = new VR(0x0002, 0x0003, vr->Length, TRUE);
			memcpy(vr1->Data, vr->Data, vr->Length);
			DDOPtr->Push(vr1);
		}
		// TransferSyntaxUID
		vr = new VR(0x0002, 0x0010, strlen(_LittleEndianUID_space), TRUE);
		memset(vr->Data, 0, vr->Length);
		memcpy(vr->Data, (void*)_LittleEndianUID_space, strlen(_LittleEndianUID_space));
		DDOPtr->Push(vr);
		// ImplementationClassUID
		vr = new VR(0x0002, 0x0012, strlen(_ImplementationUID), TRUE);
		memset(vr->Data, 0, vr->Length);
		memcpy(vr->Data, (void*)_ImplementationUID, strlen(_ImplementationUID));
		DDOPtr->Push(vr);
		// ImplementationVersionName
		vr = new VR(0x0002, 0x0013, strlen(_ImplementationVersion), TRUE);
		memset(vr->Data, 0, vr->Length);
		memcpy(vr->Data, (void*)_ImplementationVersion, strlen(_ImplementationVersion));
		DDOPtr->Push(vr);
		// SourceApplicationEntityTitle
		vr = new VR(0x0002, 0x0016, strlen(_SourceApplicationAE), TRUE);
		memset(vr->Data, 0, vr->Length);
		memcpy(vr->Data, (void*)_SourceApplicationAE, strlen(_SourceApplicationAE));
		DDOPtr->Push(vr);
	}

	
	fp = fopen(filename, "wb");
	if(!fp) return FALSE;
	memset((void*)s, 0, 128);
	strcpy(&s[128], "DICM");
	fwrite(s, 128 + 4, 1, fp);
	rc = SaveDDO(DDOPtr, fp, FileCompressMode);
	fclose(fp);
	return rc;
}




static BOOL
OrgLoadImplicitLittleEndian(DICOMDataObject* pDDO, FILE* fp, unsigned int iVrSizeLimit)
{
	DICOMDataObject*	pNewDDO;
	VR*					pVR;
	char				Buf[2 + 2 + 4];

	while (fread(Buf, 1, sizeof(Buf), fp) == sizeof(Buf))
	{
		/* Group, Element and Size could be read */
		pVR = new VR;
		if (!pVR)
			return FALSE;
#if NATIVE_ENDIAN == LITTLE_ENDIAN //Little Endian
		pVR->Group   = *((unsigned short*) Buf);
		pVR->Element = *((unsigned short*)(Buf + 2));
		pVR->Length  = *((unsigned int*)  (Buf + 2 + 2));
#else //Big Endian like Apple power pc
		pVR->Group   = SwitchEndian( *((UINT16*) Buf));
		pVR->Element = SwitchEndian( *((UINT16*)(Buf + 2)));
		pVR->Length  = SwitchEndian( *((UINT32*)  (Buf + 2 + 2)));
#endif //Big Endian

		if (pVR->Group == 0xfffe)
		{
			/* A deliminator */
			if ((pVR->Element == 0xe0dd) || (pVR->Element == 0xe00d))
			{
				delete pVR;
				return TRUE;
			}
			if (pVR->Length == 0xffffffff)
			{
				/* Implicit length... Go until deliminator */
				pVR->Length = 0;
				delete pVR;
				pNewDDO = new DICOMDataObject;
				if (OrgLoadImplicitLittleEndian(pNewDDO, fp, iVrSizeLimit))
				{
					pDDO->Push(pNewDDO);
					continue;
				}
				else
				{
					delete pNewDDO;
					return FALSE;
				}
			}
			if (pVR->Element == 0xe000)
			{
				/* Sequence begin ? */
				pVR->Length = 0;
				delete pVR;
				pNewDDO = new DICOMDataObject;
				if (OrgLoadImplicitLittleEndian(pNewDDO, fp, iVrSizeLimit))
				{
					pDDO->Push(pNewDDO);
					continue;
				}
				else
				{
					delete pNewDDO;
					return FALSE;
				}
			}
		}
		if (pVR->Length == 0xffffffff)
		{
			pVR->Length = 0;
			pDDO->Push(pVR);
			if (!OrgLoadImplicitLittleEndian(pDDO, fp, iVrSizeLimit))
				return FALSE;
			continue;
		}
		/* Check whether the current VR has to be read.
		   NKI DicomNodes can restrict what has to be read
		   Following code assumes that reading is finished when pixeldata
		   are encountered. (Maybe a problem here!!!)
		*/
		if (pVR->Length > iVrSizeLimit)
		{
			if (((pVR->Group == 0x7fdf) || (pVR->Group == 0x7fe0)) &&
				(pVR->Element == 0x0010))
			{
				/* Ready !? */
				pVR->Length = 0;
				delete pVR;
				return TRUE;
			}
			else
			{	/* Read it, throw it away and continue */
//				pVR->Data = new char [pVR->Length];
				pVR->ReAlloc(pVR->Length);
				if (!pVR->Data)
					return FALSE;
				fread(pVR->Data, 1, pVR->Length, fp);
				delete pVR;
				continue;
			}
		}
		if (pVR->Length)
		{
//			pVR->Data = new char [pVR->Length];
			pVR->ReAlloc(pVR->Length);
			if (!pVR->Data)
				return FALSE;
			fread(pVR->Data, 1, pVR->Length, fp);
/*			if ((pVR->Group == 0x7fdf) && (pVR->Element == 0x0010))
			{ 
				VR* v;
				signed char *CompressedData = ((signed char *)(pVR->Data));
				int UncompressedLength = get_nki_private_decompressed_length(CompressedData);
				v = new VR(0x7fe0, 0x0010, UncompressedLength, TRUE);
     				nki_private_decompress((short *)(v->Data), CompressedData);
				delete pVR;
				pVR = v;
			}
*/		}
		else
			pVR->Data = NULL;
		pDDO->Push(pVR);
	}
	return TRUE;
}

DICOMDataObject*
OrgLoadImplicitLittleEndianFile(char* filename, unsigned int iVrSizeLimit)
{
	DICOMDataObject*	pDDO;
	FILE*			fp;
	char			Buf[128 + 4];
	PDU_Service		PDU;

	PDU.AttachRTC(&VRType);

	fp = fopen(filename, "rb");
	if(!fp)
	{
		return NULL;
	}
	/* Find out whether marker 'DICM' exists */
	if (fread(Buf, 1, 128 + 4, fp) != 128 + 4)
	{
		fclose(fp);
		return NULL;
	}
	else
	{
		if (strncmp(Buf + 128, "DICM", 4) != 0)
		{
			fseek(fp, 0, SEEK_SET);
			/* We are at beginning of the VRs */
			pDDO = new DICOMDataObject;
			OrgLoadImplicitLittleEndian(pDDO, fp, iVrSizeLimit);
			fclose ( fp );
			return pDDO;
		}
		else
		{
			/* The function 'LoadImplicitLittleEndianFile' is not intended for
			   DICM files, however don't be fuzzy about it... */
			fclose(fp);
			pDDO = PDU.LoadDICOMDataObject(filename);
			return pDDO;
		}
	}
}


/*-----------------------------------------------------------------------*/

class CBufferedIO
{
  private:
    char*	m_pBuf;
    int		m_iBufSize;
    int		m_iCurPos;
    int		m_iNbBytesInBuffer;
    int		m_iNbBlocksRead;
  public:
    CBufferedIO(int iBufSize = 0x4000);
    ~CBufferedIO();
    int  read (int handle, void *buffer, unsigned int count);
    long lseek(int handle, long offset, int origin);
};

CBufferedIO::CBufferedIO(int iBufSize)
{
  m_pBuf             = NULL;
  m_iBufSize         = iBufSize;
  m_iCurPos          = 0;
  m_iNbBytesInBuffer = 0;
  m_iNbBlocksRead    = 0;
};

CBufferedIO::~CBufferedIO()
{
  if (m_pBuf)
    free(m_pBuf);
};

int CBufferedIO::read (int handle, void *buffer, unsigned int count)
{ int		iNbBytesCopied = 0;

  if (!m_pBuf)
  { if (m_iBufSize <= 0)
      return -1;
    m_pBuf = (char*)malloc(m_iBufSize);
    if (!m_pBuf)
      return -1;
  }

  while (count - iNbBytesCopied > m_iNbBytesInBuffer - m_iCurPos)
  { memcpy(buffer, m_pBuf + m_iCurPos, m_iNbBytesInBuffer - m_iCurPos);
    buffer = (char*)buffer + m_iNbBytesInBuffer - m_iCurPos;
    iNbBytesCopied += m_iNbBytesInBuffer - m_iCurPos;
    m_iCurPos = 0;
    m_iNbBytesInBuffer = ::read(handle, m_pBuf, m_iBufSize);
    m_iNbBlocksRead ++;
    if (m_iNbBytesInBuffer <= 0)
      return iNbBytesCopied;
  }

/* crash probably when read failed of file after:

2004-11-30 09:46:48 [RT8769103] Sending file : y:\dv0_0001\20001314\1.2.840.113619.2.22.287.1.369.2.20000619.221935_0002_000014_96263112199.V2

 0001:0001fb70       ?read@CBufferedIO@@QAEHHPAXI@Z 00420b70 f loadddo.obj
 0001:0001fc40       ?lseek@CBufferedIO@@QAEJHJH@Z 00420c40 f loadddo.obj


function: <nosymbols>
        00420c0d 5b               pop     ebx
        00420c0e c20c00           ret     0xc
        00420c11 8b44241c         mov     eax,[esp+0x1c]         ss:0b6c89cf=????????
        00420c15 8b7500           mov     esi,[ebp+0x0]          ss:02b85cb6=????????
        00420c18 8bd0             mov     edx,eax
        00420c1a 037508           add     esi,[ebp+0x8]          ss:02b85cb6=????????
        00420c1d 2bd3             sub     edx,ebx
        00420c1f 8b7c2418         mov     edi,[esp+0x18]         ss:0b6c89cf=????????
        00420c23 8bca             mov     ecx,edx
        00420c25 c1e902           shr     ecx,0x2
FAULT ->00420c28 f3a5            rep  movsd ds:023c2000=???????? es:1f838480=00000000
        00420c2a 8bca             mov     ecx,edx
        00420c2c 83e103           and     ecx,0x3
        00420c2f f3a4             rep     movsb         ds:023c2000=?? es:1f838480=00
        00420c31 8bc8             mov     ecx,eax
        00420c33 2bcb             sub     ecx,ebx
        00420c35 014d08           add     [ebp+0x8],ecx          ss:02b85cb6=????????
        00420c38 5d               pop     ebp
        00420c39 5f               pop     edi
        00420c3a 5e               pop     esi
        00420c3b 5b               pop     ebx
        00420c3c c20c00           ret     0xc

*/

  memcpy(buffer, m_pBuf + m_iCurPos, count - iNbBytesCopied);
  m_iCurPos += count - iNbBytesCopied;
  return count;
}

long CBufferedIO::lseek(int handle, long offset, int origin)
{ long	rc;

  /* Just handle the special case for the 'DICM' test */
  if ((origin == SEEK_SET) &&
      (offset < m_iNbBytesInBuffer) &&
      (m_iNbBlocksRead == 1))
  { m_iCurPos = offset;
    rc = offset;
  }
  else
  { /* Make sure that above special case does not re-appear */
    m_iNbBlocksRead = 999;
    if (origin == SEEK_CUR)
      /* Adjust the file-position to where the caller thinks it is */
      offset = offset - (m_iNbBytesInBuffer - m_iCurPos);
    rc = ::lseek(handle, offset, origin);
    m_iCurPos          = 0;
    m_iNbBytesInBuffer = 0;
  }
  return rc;
}

/*-----------------------------------------------------------------------*/


static BOOL
LoadImplicitLittleEndian(DICOMDataObject* pDDO, CBufferedIO* pBufferedIO,
	int handle, unsigned int iVrSizeLimit)
{
	DICOMDataObject*	pNewDDO;
	VR*					pVR;
	char				Buf[2 + 2 + 4];

	while (pBufferedIO->read(handle, Buf, sizeof(Buf)) == sizeof(Buf))
	{
		/* Group, Element and Size could be read */
		pVR = new VR;
		if (!pVR)
			return FALSE;
#if NATIVE_ENDIAN == LITTLE_ENDIAN //Little Endian
		pVR->Group   = *((unsigned short*) Buf);
		pVR->Element = *((unsigned short*)(Buf + 2));
		pVR->Length  = *((unsigned int*)  (Buf + 2 + 2));
#else //Big Endian like Apple power pc
		pVR->Group   = SwitchEndian( *((UINT16*) Buf));
		pVR->Element = SwitchEndian( *((UINT16*)(Buf + 2)));
		pVR->Length  = SwitchEndian( *((UINT32*)  (Buf + 2 + 2)));
#endif //Big Endian

		if (pVR->Group == 0xfffe)
		{
			/* A deliminator */
			if ((pVR->Element == 0xe0dd) || (pVR->Element == 0xe00d))
			{
				delete pVR;
				return TRUE;
			}
			if (pVR->Length == 0xffffffff)
			{
				/* Implicit length... Go until deliminator */
				pVR->Length = 0;
				delete pVR;
				pNewDDO = new DICOMDataObject;
				if (LoadImplicitLittleEndian(pNewDDO, pBufferedIO, handle, iVrSizeLimit))
				{
					pDDO->Push(pNewDDO);
					continue;
				}
				else
				{
					delete pNewDDO;
					return FALSE;
				}
			}
			if (pVR->Element == 0xe000)
			{
				/* Sequence begin ? */
				pVR->Length = 0;
				delete pVR;
				pNewDDO = new DICOMDataObject;
				if (LoadImplicitLittleEndian(pNewDDO, pBufferedIO, handle, iVrSizeLimit))
				{
					pDDO->Push(pNewDDO);
					continue;
				}
				else
				{
					delete pNewDDO;
					return FALSE;
				}
			}
		}
		if (pVR->Length == 0xffffffff)
		{
			pVR->Length = 0;
			pDDO->Push(pVR);
			if (!LoadImplicitLittleEndian(pDDO, pBufferedIO, handle, iVrSizeLimit))
				return FALSE;
			continue;
		}
		/* Check whether the current VR has to be read.
		   NKI DicomNodes can restrict what has to be read
		   Following code assumes that reading is finished when pixeldata
		   are encountered. (Maybe a problem here!!!)
		*/
		if (pVR->Length > iVrSizeLimit)
		{
			if (((pVR->Group == 0x7fdf) || (pVR->Group == 0x7fe0)) &&
				(pVR->Element == 0x0010))
			{
				/* Ready !? */
				pVR->Length = 0;
				delete pVR;
				return TRUE;
			}
			else
			{	/* Read it, throw it away and continue */
//				pVR->Data = new char [pVR->Length];
				pVR->ReAlloc(pVR->Length);
				if (!pVR->Data)
					return FALSE;
				pBufferedIO->read(handle, pVR->Data, pVR->Length);
				delete pVR;
				continue;
			}
		}
		if (pVR->Length)
		{
//			pVR->Data = new char [pVR->Length];
			pVR->ReAlloc(pVR->Length);
			if (!pVR->Data)
				return FALSE;
			pBufferedIO->read(handle, pVR->Data, pVR->Length);
		}
		else
			pVR->Data = NULL;
		pDDO->Push(pVR);
	}
	return TRUE;
}


DICOMDataObject*
LoadImplicitLittleEndianFile(char* filename, unsigned int iVrSizeLimit)
{
	DICOMDataObject*	pDDO;
	int			handle;
	char			Buf[128 + 4];
	PDU_Service		PDU;
	unsigned int		iLastTime, iCurTime;
	CBufferedIO*		pBufferedIO;

	PDU.AttachRTC(&VRType);

#ifdef WIN32	
	handle = sopen( filename, O_RDONLY | O_BINARY, SH_DENYNO, S_IREAD);
#else
	handle = open( filename, O_RDONLY);
#endif

	if (handle == -1)
	{
		return NULL;
	}

	pBufferedIO = new CBufferedIO(iVrSizeLimit < 0x1000 ? 0x1000 : 0x8000);
	/* Find out whether marker 'DICM' exists */
	if (pBufferedIO->read(handle, Buf, 128 + 4) != 128 + 4)
	{
		delete pBufferedIO;
		close(handle);
		return NULL;
	}
	else
	{
		if (strncmp(Buf + 128, "DICM", 4) != 0)
		{
			pBufferedIO->lseek(handle, 0, SEEK_SET);
			/* We are at beginning of the VRs */
			pDDO = new DICOMDataObject;
			LoadImplicitLittleEndian(pDDO, pBufferedIO, handle, iVrSizeLimit);
//iLastTime = timeGetTime();
			delete pBufferedIO;
			close(handle);
//iCurTime = timeGetTime();
//printf("fopen=%d\r\n", iCurTime - iLastTime);
			return pDDO;
		}
		else
		{
			/* The function 'LoadImplicitLittleEndianFile' is not intended for
			   DICM files, however don't be fuzzy about it... */
			delete pBufferedIO;
			close(handle);
			pDDO = PDU.LoadDICOMDataObject(filename);
			return pDDO;
		}
	}
}



DICOMDataObject	*
LoadDICOMDataObjectFrame(char	*filename)
	{
	DICOMDataObject	*DDO;
	UINT	MemBlock, Block;
	UINT32	TCount, Index;
	unsigned	char	*mem;
	FILE	*fp;
	DOBJECTSERSTATE doss;

	MemBlock = 50000;

	fp = fopen(filename, "rb");
	if(!fp)
		{
		return ( FALSE );
		}

	mem = (unsigned	char *) malloc(MemBlock);
	if(!mem)
		{
		fclose(fp);
		return ( FALSE );
		}

	DDO = DObjectSerialize(NULL, 0, FALSE, &doss);
	if(!DDO)
		{
		free(mem);
		return ( FALSE );
		}
	TCount = 0;
	while( TRUE )
		{
		Block = fread(mem, 1, MemBlock, fp);

		Index = 0;
		if(!TCount)
			{
			if(!strncmp((char*) &mem[128], "DICM", 4))
				Index = 128 + 4;
			}
		TCount += Block;
		while(Index < Block)
			{
			if(!DObjectSerialize(DDO, mem[Index], TRUE, &doss))
				{
				free(mem);
				fclose(fp);
				return(DDO);
				}
			++Index;
			}
		if(Block!=MemBlock)
			break;
		}
	free(mem);
	fclose ( fp );
	return ( DDO );
	}

