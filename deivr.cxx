/*
19990318    ljz    Added 'DICOMObject::DeleteVR'
20001106    mvh    Used delete [] operator for vr->Data
20010426    ljz    Fixed small leaks in 'DICOMObject::DeleteVR'
20010730    ljz    Added 'DICOMObject::ReplaceVR'. Is able to replace VRs inside
		   sequences.
20010802    ljz    Added 'VR::GetSpecialValueCodeOfVR'
20011121    ljz    Added VR_SPECIAL_RGB_TO_MONO. Used by a conversion-tool to change
		   ultrasound Kretz RGB images to monochrome that VariSeed (brachytherapy)
		   understands.
20011121    ljz    Added VR_SPECIAL_RGB_TO_MONO_PLUS. Same as above, but tries to preserve
		   colored drawings.
20051217    mvh	   Added 'VR::ReAlloc'
20051217    mvh    Use malloc/free instead of new char[] and delete []
20051229    mvh    Removed unnecessary reset() in DeleteVR
20071031    mvh    Temp printf memory error messages
20071102    mvh    Send allocation errors to DicomError
20080313    ljz    Removed some warnings
*/


/****************************************************************************
          Copyright (C) 1995, University of California, Davis

          THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND THE UNIVERSITY
          OF CALIFORNIA DOES NOT MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS
          PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR
          USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY
          SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF
          THE SOFTWARE IS WITH THE USER.

          Copyright of the software and supporting documentation is
          owned by the University of California, and free access
          is hereby granted as a license to use this software, copy this
          software and prepare derivative works based upon this software.
          However, any distribution of this software source code or
          supporting documentation or derivative works (source code and
          supporting documentation) must include this copyright notice.
****************************************************************************/

/***************************************************************************
 *
 * University of California, Davis
 * UCDMC DICOM Network Transport Libraries
 * Version 0.1 Beta
 *
 * Technical Contact: mhoskin@ucdavis.edu
 *
 ***************************************************************************/

#	include	"dicom.hpp"
//#	include	"mini.hpp"

/********************************
 *
 * Error handler
 *
 ********************************/

DeepErrorCall DicomErrorHandler = NULL;

void SetDicomErrorHandler(DeepErrorCall handler)
	{ 
	DicomErrorHandler = handler;
	}

void DicomError(int error, char *message, int info)
	{ 
	if (DicomErrorHandler) 
		DicomErrorHandler(error, message, info);
	else
		{
		printf("DICOM ERROR ");
		printf(message, info);
		}
	}

/********************************
 *
 * Base VR Type
 *
 ********************************/
VR	::	VR(UINT16 g, UINT16 e, UINT32 l, BOOL	Alloc)
	{
	BYTE	*bptr = NULL;
	BOOL	cLast = FALSE;

	if ( l & 0x01 )
		{
		++l;
		cLast = TRUE;
		}


	Group = g;
	Element = e;
	Length = l;
	if(Alloc)
		{
		Data = malloc(Length);
		if (!Data) DicomError(DCM_ERROR_MEMORY, "VR: out of memory allocating %d bytes\n", Length);
		}
	else
		Data = NULL;

	if ( cLast )
		{
		bptr = (BYTE*)Data;
		bptr[Length-1] = '\0';
		}
	ReleaseMemory = TRUE;
	SQObjectArray = NULL;
	TypeCode = 0;
	Valid = FALSE;
	}

VR	::	VR(UINT16 g, UINT16 e, UINT32 l, void	*d)
	{
	BYTE	*bptr = NULL;
	BOOL	cLast = FALSE;

/*	if ( l & 0x01 )
		{
		++l;
		cLast = TRUE;
		}*/

	Group = g;
	Element = e;
	Length = l;
//	if(Length&1)
//		++Length;
	Data = d;
	if ( cLast )
		{
		bptr = (BYTE*)Data;
		bptr[Length-1] = '\0';
		}
	ReleaseMemory = TRUE;
	SQObjectArray = NULL;
	TypeCode = 0;
	Valid = FALSE;
	}

VR	::	VR(UINT16 g, UINT16 e, UINT32 l, void	*d, BOOL	mFlag)
	{
	BYTE	*bptr = NULL;
	BOOL	cLast = FALSE;

/*	if ( l & 0x01 )
		{
		++l;
		cLast = TRUE;
		}*/

	Group = g;
	Element = e;
	Length = l;
//	if(Length&1)
//		++Length;
	Data = d;
	if ( cLast )
		{
		bptr = (BYTE*)Data;
		bptr[Length-1] = '\0';
		}
	ReleaseMemory = mFlag;
	SQObjectArray = NULL;
	TypeCode = 0;
	Valid = FALSE;
	}

VR	::	~VR()
	{
	Array<DICOMObject	*>	*ArrayDO;
/*
	printf("~VR()\n\tGroup = %4.4x Element = %4.4x Data = %x Length = %d\n",
		Group, Element, Data, Length);
	printf("\tReleasememory = %d, SQObjectArray = %x, RID = %x Valid = %d\n",
		ReleaseMemory, SQObjectArray, ReferentialIntegrityDatabase, Valid);
	fflush(stdout);
*/
	if(ReleaseMemory)
		if(Data)
//			delete [] Data;
			free(Data);
	if(SQObjectArray)
		{
		ArrayDO = (Array<DICOMObject *> *) SQObjectArray;
		while(ArrayDO->GetSize())
			{
			delete ArrayDO->Get(0);
			ArrayDO->RemoveAt(0);
			}
		delete ArrayDO;
		SQObjectArray = NULL;
		}
		
	}


UINT	VR	::	operator	>	(VR	&vr)
	{
	if(Group > vr.Group)
		return ( 1 );
	if(Group == vr.Group)
		{
		if(Element > vr.Element)
			return ( 1 );
		}
	return ( 0 );
	}

UINT	VR	::	operator	<	(VR	&vr)
	{
	if(Group < vr.Group)
		return ( 1 );
	if(Group == vr.Group)
		{
		if(Element < vr.Element)
			return ( 1 );
		}
	return ( 0 );
	}

UINT	VR	::	operator	==	(VR	&vr)
	{
	if(Group == vr.Group)
	if(Element == vr.Element)
			return ( 1 );
	return ( 0 );
	}

/*****************************************************
 *
 * VR Group P-Queues
 *
 *****************************************************/

BOOL	VRGroupPQueue	::	Push(VR	*vr)
	{
//	printf("[VRGroupPQueue : Push(%x)]\n", vr);
	if(vr->Group!=Group)
		return ( FALSE );
	Length += vr->Length + sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32);
	if(vr->Length&0x01)
		++Length;	// Transfer Syntax Encodeder will +1 here
	PQueueOfPtr<VR *> :: Push ( vr );
	return ( TRUE );
	}

VR	*	VRGroupPQueue	::	Pop()
	{
	VR	*tv;

//	printf("[VRGroupPQueue : Pop()]\n");
	if ( !PQueueOfPtr<VR *> :: GetSize())
		return ( (VR *) NULL );
	tv = PQueueOfPtr<VR *> :: Pop ();
	if(!tv)
		return ( tv );
	Length -= (tv->Length + sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32));
	if(tv->Length&0x01)
		--Length;	// See Push
	return ( tv );
	}

VR	*	VRGroupPQueue	::	GroupLength()
	{
	LE_UINT32	*puint32;

	Element0 = new VR ( Group, 0x0000, 4, TRUE);
	if(!Element0)
		return ( NULL );
	puint32 = (LE_UINT32 *) Element0->Data;
	(*puint32) = (LE_UINT32) Length;// + sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32) + sizeof(UINT32);
	Element0->TypeCode = 'UL';
	return ( Element0 );
	}

UINT	VRGroupPQueue	::	operator	>	(VRGroupPQueue	&vrgroup)
	{
	if(Group > vrgroup.Group)
		return ( 1 );
	return ( 0 );
	}

UINT	VRGroupPQueue	::	operator	<	(VRGroupPQueue	&vrgroup)
	{
	if(Group < vrgroup.Group)
		return ( 1 );
	return ( 0 );
	}

UINT	VRGroupPQueue	::	operator	==	(VRGroupPQueue	&vrgroup)
	{
	if(Group == vrgroup.Group)
		return ( 1 );
	return ( 0 );
	}

/*****************************************************
 *
 * DICOM Objects
 *
 *****************************************************/

DICOMObject	::	DICOMObject()
	{
	Packaged = FALSE;
	Length = 0;
	VRGroupPQueues.ClearType = FALSE;
	SQObjectArray = NULL;
	LastVR = NULL;
	LastVRG = NULL;
	FreeSQObjectSize = 0;
	AutoMakeDO = NULL;
	FixedSize = 0;
	UseFixedSize = FALSE;
	}
	
DICOMObject	::	~DICOMObject()
 	{
	VR	*VRPtr;

	if ( !Packaged)
		if ( !Package() )
			return;

	if ( Packaged )
		{
		while ( VRPtr = this->Pop())
			delete VRPtr;
		return;
		}
	while ( VRGroupPQueues.GetSize() )
		VRGroupPQueues.RemoveAt(0);
	}

BOOL	DICOMObject	::	Push(DICOMObject	*SQDO)
	{
	Array<DICOMObject *>	*ArrayPtr;

	if ( Packaged )
		return ( FALSE );
	if ( ! LastVR )
		{
		return ( FALSE );	// not-supported yet
		if (!SQObjectArray)
			{
			SQObjectArray = new Array<DICOMObject *>;
			if ( !SQObjectArray )
				return ( FALSE );
			}
		SQObjectArray->Add ( SQDO );
		FreeSQObjectSize += SQDO->Length;
		return ( TRUE );
		}
	if (!LastVR->SQObjectArray)
		{
		ArrayPtr = new Array<DICOMObject *>;
		if (!ArrayPtr)
			return ( FALSE );
		LastVR->SQObjectArray = (void *) ArrayPtr;
		// "new sequence"
		LastVRG->Length += (2 * (sizeof(UINT16) + sizeof(UINT16) +
						sizeof(UINT32) ));
		Length += (2 * (sizeof(UINT16) + sizeof(UINT16) +
						sizeof(UINT32) ));
		}
	ArrayPtr = (Array<DICOMObject *> *) LastVR->SQObjectArray;
	ArrayPtr->Add ( SQDO );	
	LastVRG->Length += SQDO->Length + 
			(2 * (sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32)));
	Length += SQDO->Length + 
			(2 * (sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32)));
	return ( TRUE );
	}

BOOL	DICOMObject	::	Push(VR	*vr)
	{
	UINT			Index;
 	VRGroupPQueue	*VRGroupPQueuePtr;
	BOOL			rBOOL;

//	printf("[Push]\n");
	if ( Packaged )
		return ( FALSE );	// already Pop() from this object

	if(vr->Length == 0xffffffff)
		{
//		fprintf(stdout, "Attemping to push -1 length\n");
		vr->Length = 0;
		}
	if(AutoMakeDO)
		{
		rBOOL = AutoMakeDO->Push ( vr );
		if ( rBOOL )
			LastVRG->Length += vr->Length + sizeof(UINT16) +
						sizeof(UINT16) + sizeof(UINT32);
		return ( rBOOL );
		}

	Index = 0;
	if(vr->Element == 0x0000)
		{
//		fprintf(stderr, "Attemping to push GLC: %d\n",
//			(*((UINT32 *) vr->Data)));
		delete vr;
		return ( TRUE );	// ignore Group Length Codes (auto calculated)
		}
	LastVR = vr;
	if(LastVRG)
		if(LastVRG->Group == vr->Group)
			{
			Length += vr->Length + sizeof(UINT16) + sizeof(UINT16) +
						sizeof(UINT32);
			if(vr->Length&0x01)
				++Length;
			return ( LastVRG->Push ( vr ));
			}
//	printf("[VRGroupPQueues.GetSize() : %d]\n", VRGroupPQueues.GetSize());
	while ( Index < VRGroupPQueues.GetSize())
		{
		VRGroupPQueuePtr = VRGroupPQueues.Get(Index);
		if(!VRGroupPQueuePtr)
			return ( FALSE );
		if(VRGroupPQueuePtr->Group == vr->Group)
			{
			LastVRG = VRGroupPQueuePtr;
			Length += vr->Length + sizeof(UINT16) + sizeof(UINT16) +
						sizeof(UINT32);
			if(vr->Length&0x01)
				++Length;
//			printf("[VRGroupPQueuePtr->Group : %d]\n", VRGroupPQueuePtr->Group);
			return ( VRGroupPQueuePtr->Push ( vr ));
			}
		++Index;
		}

	VRGroupPQueuePtr = new VRGroupPQueue(vr->Group);
	if(!VRGroupPQueuePtr)
		return ( FALSE );

	Length += sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32) +
			sizeof(UINT32);
	Length += vr->Length + sizeof(UINT16) + sizeof(UINT16) +
			sizeof(UINT32);
	if(vr->Length&0x01)
		++Length;
	LastVRG = VRGroupPQueuePtr;	
//	printf("[VRGroupPQueues.Push]\n");
	VRGroupPQueues.Add(VRGroupPQueuePtr);
	return ( VRGroupPQueuePtr->Push(vr) );
	}

VR	*	DICOMObject	::	Pop()
	{
	VR	*VRPtr;

	//return ( (VR *) NULL );

	if(!Packaged)
		if(!Package())
			return ( (VR *) NULL );
	if(!Packaged)
		return ( (VR *) NULL );

	if ( ! CurrentGroup )
		{
//		printf("[Pop : CurrentGroup = NULL]\n");
		if ( !PQueueOfPtr<VRGroupPQueue *>::GetSize())
			{
			Packaged = FALSE;
			LastVRG = NULL;
			LastVR = NULL;
			return ( (VR *) NULL );
			}
		CurrentGroup = PQueueOfPtr<VRGroupPQueue *>::Pop();
		if ( ! CurrentGroup )
			return ( (VR *) NULL );
		Length -= ( sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32) +
				sizeof(UINT32) );
	
		return ( CurrentGroup->GroupLength() );
		}
	if ( ! ( VRPtr = CurrentGroup->Pop() ) )
		{
		delete CurrentGroup;
		CurrentGroup = NULL;
		return ( this->Pop() );
		}
	Length -= (VRPtr->Length + sizeof(UINT16) + sizeof(UINT16) +
			sizeof(UINT32) );
	if(VRPtr->Length&0x01)
		--Length;	// See Push
	return ( VRPtr );
	}

// this needs to be replaced with a more efficient algorithm in the
// future, but it will do for now.
VR		*DICOMObject	::	GetVR(UINT16	g, UINT16	e)
	{
	VR				*vr;
	UINT			Index,Index2;
		VRGroupPQueue	*VRGroupPQueuePtr;

	Index = 0;

	while ( Index < VRGroupPQueues.GetSize())
		{
		VRGroupPQueuePtr = VRGroupPQueues.Get(Index);
		if(!VRGroupPQueuePtr)
			return ( FALSE );
		if(VRGroupPQueuePtr->Group == g)
			{
			Index2 = 0;
			while ( Index2 < VRGroupPQueuePtr->GetSize() )
				{
				vr = VRGroupPQueuePtr->Get(Index2);
				if(!vr)
					return ( 0 );
				if(vr->Element == e)
					{
					return ( vr );
					}
				++Index2;
				}
			return ( NULL );
			}
		++Index;
		}
	return ( NULL );
	}
		
	
LE_UINT16	DICOMObject	::	GetUINT16(UINT16	g, UINT16	e)
	{
	VR		*vr;
	LE_UINT16	Val;

	vr = this->GetVR ( g, e );
	if(!vr)
		return(0);
		
	if ( vr->Data )
		if(vr->Length == sizeof(UINT16))
			{
			memcpy(&Val, vr->Data, 2);
			return ( Val );
			}
	return ( 0 );
	}


BOOL	DICOMObject::DeleteVR(VR* pVR)
{
  VR			*vr;
  UINT			Index,Index2;
  VRGroupPQueue		*VRGroupPQueuePtr;

  if ( Packaged )
    return ( FALSE );	// already Pop() from this object

  /* Search the pVR */
  Index = 0;
  while ( Index < VRGroupPQueues.GetSize())
  { VRGroupPQueuePtr = VRGroupPQueues.Get(Index);
    if(!VRGroupPQueuePtr)
      return FALSE;

    if(VRGroupPQueuePtr->Group == pVR->Group)
    { Index2 = 0;
      while ( Index2 < VRGroupPQueuePtr->GetSize() )
      { vr = VRGroupPQueuePtr->Get(Index2);
	if(!vr)
	  return FALSE;

	if(vr->Element == pVR->Element)
	{ /* OK, the pVR has been found.
	     Remove from Array of VrPointers */

	  VRGroupPQueuePtr->RemoveAt(Index2);

	  VRGroupPQueuePtr->Length -= (pVR->Length + sizeof(UINT16) + sizeof(UINT16) +
			sizeof(UINT32) );
	  if (VRGroupPQueuePtr->Length & 0x01)
	    VRGroupPQueuePtr->Length--;

	  Length -= (pVR->Length + sizeof(UINT16) + sizeof(UINT16) +
			sizeof(UINT32) );
	  if (Length & 0x01)
	    Length--;

	  /* Maybe remove from Array of GroupPointers */
	  if (!VRGroupPQueuePtr->Length)
	  { VRGroupPQueues.RemoveAt(Index);
	    delete VRGroupPQueuePtr;		/* LJ: leak */
	    Length -= (sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32));
	  }

	  /* Get rid of the data */
	  delete vr;				/* LJ: leak */
	  return TRUE;
	}
	++Index2;
      }
      return FALSE;
    }
    ++Index;
  }
  return FALSE;
}

BOOL DICOMObject::ReplaceVR(VR* pVR)
{
  VR*			vr;
  UINT			Index, Index1, Index2;
  VRGroupPQueue*	VRGroupPQueuePtr;
  Array<DICOMDataObject*>*	pADDO;

  if ( Packaged )
    return ( FALSE );	// already Pop() from this object
  /* Search the pVR */
  Index = 0;
  while ( Index < VRGroupPQueues.GetSize())
  { VRGroupPQueuePtr = VRGroupPQueues.Get(Index);
    if(!VRGroupPQueuePtr)
      return FALSE;
    Index1 = 0;
    while ( Index1< VRGroupPQueuePtr->GetSize() )
    { vr = VRGroupPQueuePtr->Get(Index1);
      if(!vr)
	return FALSE;
      if (vr->SetIf(pVR))
      { /* OK, the pVR has been found */
      }
      else if (vr->SQObjectArray)
      { /* Loop over all DICOMDataObjects in this sequence */
	pADDO = (Array<DICOMDataObject*>*)vr->SQObjectArray;
	Index2 = 0;
	while (Index2< pADDO->GetSize())
	{ pADDO->Get(Index2)->ReplaceVR(pVR);
	  ++Index2;
	}
      }
      ++Index1;
    }
    ++Index;
  }
  return FALSE;
}


BOOL	DICOMObject	::	Reset()
	{
	VR	*vr;

	while ( vr = Pop() )
		delete vr;

	LastVRG = NULL;
	LastVR = NULL;

	return ( TRUE );
	}

BOOL	DICOMObject	::	Package()
	{
	UINT	Index;
	
	CurrentGroup = NULL;
	Index = 0;
//	printf("[start: Package()]\n");
	while ( Index < VRGroupPQueues.GetSize())
		{
		PQueueOfPtr<VRGroupPQueue *> :: Push (VRGroupPQueues.Get(Index) );
		++Index;
		}
	while ( VRGroupPQueues.GetSize() )
		VRGroupPQueues.RemoveAt(0);
		
//	printf("[Packaged : %d Groups]\n", Index);
	Packaged = TRUE;
	if ( Index )
		return ( TRUE );
	Packaged = FALSE;
	return ( FALSE );
	}




#define VR_SPECIAL_NONE			0x00
#define VR_SPECIAL_FIX_TMS		0x01
#define VR_SPECIAL_RGB_TO_MONO		0x02
#define VR_SPECIAL_RGB_TO_MONO_PLUS	0x03
#define VR_SPECIAL_UNKNOWN		0xFF

int VR::GetSpecialValueCodeOfVR(VR* pVR)
{  char*	pcData;

  /* This private function is called by SetIf.
     It analyzes the data of the VR. If it contains a special code, a string between
     '[[' and ']]', it will return the code; otherwise zero.
  */
  if ((pVR->Length < 4) || (!pVR->Data))
    return VR_SPECIAL_NONE;
  pcData = (char*)pVR->Data;
  if ((pcData[0] != '[') || (pcData[1] != '['))
    return VR_SPECIAL_NONE;

  if ((pVR->Length == 12) && (strncmp(pcData, "[[FIX_TMS]]", 11) == 0))
    return VR_SPECIAL_FIX_TMS;
  if ((pVR->Length == 20) && (strncmp(pcData, "[[RGB_TO_MONO_PLUS]]", 20) == 0))
    return VR_SPECIAL_RGB_TO_MONO_PLUS;
  if ((pVR->Length == 16) && (strncmp(pcData, "[[RGB_TO_MONO]]", 15) == 0))
    return VR_SPECIAL_RGB_TO_MONO;

  return VR_SPECIAL_UNKNOWN;
}


BOOL VR::SetIf(VR	*vr)
{ Array<DICOMObject*>	*ArrayDO;
  int			i, iSecialCodeOfVR;
  BOOL			rc = FALSE;
  char*			pCopy;
  unsigned char*	pRGB;
  unsigned char*	pMono;
  int			iNewLength;
  int			iNewValue, iPad = 0;

  if (!vr)
    return rc;
  if ((vr->Group==Group) && (vr->Element==Element))
  {
    iSecialCodeOfVR = GetSpecialValueCodeOfVR(vr);
    switch (iSecialCodeOfVR)
    { case VR_SPECIAL_NONE:
      /* Just replace the VR */
      if(ReleaseMemory)
//        delete [] Data;
	  free(Data);
      if(SQObjectArray)
      { ArrayDO = (Array<DICOMObject *> *) SQObjectArray;
	while(ArrayDO->GetSize())
	{ delete ArrayDO->Get(0);
	  ArrayDO->RemoveAt(0);
	}
	delete ArrayDO;
	SQObjectArray = NULL;
      }
      Length = vr->Length;
      if (vr->Length)
      { Data = malloc(vr->Length);
        if (!Data) DicomError(DCM_ERROR_MEMORY, "VR:SetIf out of memory allocating %d bytes\n", Length);
	memcpy(Data, vr->Data, vr->Length);
	ReleaseMemory = TRUE;
      }
      else
	ReleaseMemory = FALSE;
      rc = TRUE;
      break;

      case VR_SPECIAL_FIX_TMS:
      /* Change PatientName like 'LastName^FirstName^MiddleName' into
	 'LastName,FirstName,MiddleName'.
      */
      if (Length > 0)
      { pCopy = (char*)malloc(Length+2);
        if (!pCopy) DicomError(DCM_ERROR_MEMORY, "VR:SetIf out of memory allocating %d bytes\n", Length+2);
	memcpy(pCopy, (char*)Data, Length);
	pCopy[Length] = 0;
	/* Maybe padded with a space */
	if ((pCopy[Length-1] == ' ') || (pCopy[Length-1] == '^'))
	  pCopy[Length-1] = 0;
	if ((Length > 1) && (pCopy[Length-2] == '^'))
	  pCopy[Length-2] = 0;
	for (i=0; i<(int)(strlen(pCopy)); i++)
	  if (pCopy[i] == '^')
	    pCopy[i] = ',';
	/* May pad it again */
	if (strlen(pCopy) & 0x01)
	  strcat(pCopy, " ");
	strncpy((char*)Data, pCopy, strlen(pCopy));
	Length = strlen(pCopy);
	free(pCopy);
      }
      rc = TRUE;
      break;

      case VR_SPECIAL_RGB_TO_MONO:
      /* Convert triplets of bytes to just bytes (the mean of the RGB channels) */
      if (Length >= 3)
      { iNewLength = Length / 3;
	if (iNewLength & 0x01)
	  iPad = 1;
        pMono = (unsigned char*)malloc(iNewLength + iPad);
        if (!pMono) DicomError(DCM_ERROR_MEMORY, "VR:SetIf out of memory allocating %d bytes\n", iNewLength + iPad);
	pRGB = (unsigned char*)Data;
	for (i=0; i<iNewLength; i++)
	{ iNewValue = (pRGB[i * 3] + pRGB[i * 3 + 1] + pRGB[i * 3 + 2]) / 3; 
	  pMono[i] = (unsigned char)iNewValue;
	}
	if (iPad)
	  pMono[iNewLength] = 0;
	Length = iNewLength + iPad;
	free(Data);
	Data = pMono;
      }
      rc = TRUE;
      break;

      case VR_SPECIAL_RGB_TO_MONO_PLUS:
      /* Convert triplets of bytes to just bytes only when the value of those three
	 are equal (a B&W image). However, 'real' color (contours e.g.) are converted
	 to very white (255).
      */
      if (Length >= 3)
      { iNewLength = Length / 3;
	if (iNewLength & 0x01)
	  iPad = 1;
        pMono = (unsigned char*)malloc(iNewLength + iPad);
        if (!pMono) DicomError(DCM_ERROR_MEMORY, "VR:SetIf out of memory allocating %d bytes\n", iNewLength + iPad);
	pRGB = (unsigned char*)Data;
	for (i=0; i<iNewLength; i++)
	{ if ((pRGB[i * 3] == pRGB[i * 3 + 1]) && (pRGB[i * 3] == pRGB[i * 3 + 2])) 
	    pMono[i] = pRGB[i * 3];
	  else
	    pMono[i] = 255;
	}
	if (iPad)
	  pMono[iNewLength] = 0;
	Length = iNewLength + iPad;
	free(Data);
	Data = pMono;
      }
      rc = TRUE;
      break;
    }
  }
  return rc;
}


BOOL
VR	::	Morph (
	DICOMObject	*DDO)
	{
	Array<DICOMObject	*>	*ArrayDO;

	if (!Length)
		if(!Valid)
			return ( FALSE );

	VR	*vr = new VR(Group, Element, 0, (BOOL) FALSE);
	vr->Length = Length;
	vr->Data = Data;
	vr->ReleaseMemory = ReleaseMemory;
	Length = 0;
	Data = NULL;
	ReleaseMemory = FALSE;
	DDO->Push(vr);

	if(SQObjectArray)
		{
		ArrayDO = (Array<DICOMObject *> *) SQObjectArray;
		while(ArrayDO->GetSize())
			{
			DDO->Push(ArrayDO->Get(0));
			ArrayDO->RemoveAt(0);
			}
		delete ArrayDO;
		SQObjectArray = NULL;
		}
	return ( TRUE );
	}

BOOL
VR	::	Reset ()
	{
	Array<DICOMObject	*>	*ArrayDO;

	if(ReleaseMemory)
		if(Data)
//			delete [] Data;
			free(Data);
	if(SQObjectArray)
		{
		ArrayDO = (Array<DICOMObject *> *) SQObjectArray;
		while(ArrayDO->GetSize())
			{
			delete ArrayDO->Get(0);
			ArrayDO->RemoveAt(0);
			}
		delete ArrayDO;
		SQObjectArray = NULL;
		}
	ReleaseMemory = FALSE;
	Data = NULL;
	Length = 0;
	return ( TRUE );
	}


BOOL
VR	::	ReAlloc (UINT32 N)
	{
        Reset();

	if (N&1) N++;
	ReleaseMemory = TRUE;
	Data = malloc(N);
        if (!Data) DicomError(DCM_ERROR_MEMORY, "VR:ReAlloc out of memory allocating %d bytes\n", N);
	Length = N;
	return ( Data!=NULL );
	}
