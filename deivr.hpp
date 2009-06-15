/*
19990318    ljz    Added 'DICOMObject::DeleteVR'
20010730    ljz    Added 'DICOMObject::ReplaceVR'
20010802    ljz    Added 'VR::GetSpecialValueCodeOfVR'
20051217    mvh	   Added 'VR::ReAlloc'
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


class	DICOMObject;

class	VR
	{
	private:
		int		GetSpecialValueCodeOfVR(VR* pVR);

	public:
		UINT16	Group;
		UINT16	Element;
		UINT32	Length;
		void		*Data;
		BOOL		ReleaseMemory;
		BOOL		Valid;
		// void pointers for historical reasons.  SQObjectArray
		// is problem best moved to an Array < DICOMObject * > * in
		// the future once the various system compilers work around
		// the linking problems...  ReferentialIntegrityDatabase should
		// remain void because it is not defined unless the (optional)
		// IOD library is linked in.
		void		*SQObjectArray;
		void		*ReferentialIntegrityDatabase;
		UINT16		TypeCode;
		VR() { Data = NULL; Length = 0; ReleaseMemory = TRUE; TypeCode = 0;
				SQObjectArray = NULL; Valid = FALSE; };
		VR(UINT16 g, UINT16 e, UINT32 l, BOOL	Alloc);
		VR(UINT16 g, UINT16 e, UINT32 l, void	*d);
		VR(UINT16 g, UINT16 e, UINT32 l, void	*d, BOOL);
		~VR();
		UINT		operator	>	(VR	&vr);
		UINT		operator	<	(VR	&vr);
		UINT		operator	==	(VR	&vr);
		BOOL		ReAlloc (UINT32);

		// To support SetIf/Morph

		BOOL		SetIf(VR	*);
		BOOL		Morph(DICOMObject	*);
		BOOL		Reset ();
	};

class	VRGroupPQueue	:
	public	PQueueOfPtr<VR	*>
	{
	public:
		UINT16	Group;
		UINT32	Length;
		VR			*Element0;

	public:
		BOOL	Push(VR	*vr);
		VR		*Pop();
		VR		*GroupLength();
		UINT		operator	>	(VRGroupPQueue	&vrgroup);
		UINT		operator	<	(VRGroupPQueue	&vrgroup);
		UINT		operator	==	(VRGroupPQueue	&vrgroup);
		VRGroupPQueue(UINT16	G)	{ Group = G; Length = 0; };
	};

class	DICOMObject	:
	public	PQueueOfPtr<VRGroupPQueue *>
	{
		VRGroupPQueue			*CurrentGroup, *LastVRG;
		VR				*LastVR;

		UINT16				LastGroup;
		UINT16				LastElement;

	public:
		UINT32				Length;
		BOOL				Packaged;
		Array<VRGroupPQueue *>		VRGroupPQueues;
		Array<DICOMObject *>		*SQObjectArray;
		UINT32				FreeSQObjectSize;
		DICOMObject			*AutoMakeDO;
		UINT32				FixedSize;
		BOOL				UseFixedSize;
	private:
		BOOL				Package();
	public:
		virtual		BOOL		Push(VR	*vr);
		virtual		BOOL		Push(DICOMObject *);	// Sequence Embedding
		virtual		VR		*Pop();
		LE_UINT16			GetUINT16(UINT16, UINT16);
		VR				*GetVR(UINT16, UINT16);
		BOOL				DeleteVR(VR* pVR);
		BOOL				ReplaceVR(VR* pNewVR);
		virtual		BOOL		Reset();
		DICOMObject();
		~DICOMObject();
	};

class	DICOMCommandObject	:
	public	DICOMObject
	{
	};

class	DICOMDataObject	:
	public	DICOMObject
	{
	};
