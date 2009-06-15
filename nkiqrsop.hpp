#ifndef	_NKIQRSOP_H_
#	define	_NKIQRSOP_H_

/*
19990317	ljz	NKI-specific code
20001128	ljz	Fix: Crashes happened when more than one 'ServerChildThread' was
			active. m_pDCO was wrongly shared by all threads !!
20010429        mvh     Added GETADDO to allow optional read ahead withing calling program
20010502        mvh     Added extrabytes pointer to Read and RetrieveOn
20020415        mvh     ProcessDDO now returns status (to allow compression error check)
20020613	ljz	Added prototypes for DecompressNKI and CompressNKI
20021115	mvh	Added Generic retrieve classes
20030522	ljz	Added prototype of ComputeCRC
20030701        mvh     QualifyOn now also has compression parameter
20030702        mvh     added ExtendedPDU_Service
20030704        mvh     Changed ProcessDDO parameter to **DDO (for recompress)
20050118	mvh	replaced thread local storage under linux with variables in object
20050121	mvh	Changed filename to lower case
mvh 20050211: Removed need for thread local storage
*/


class	StandardRetrieveNKI	:
	public	CMoveRQ,
	public	CMoveRSP
	{
//#ifndef WIN32  
//	DICOMCommandObject         *gl_pThreadDCO;
//  	Array < DICOMDataObject *> *gl_pThreadADDO;
//#endif
	public:
		StandardRetrieveNKI();
		virtual	BOOL	GetUID (UID	&uid) { return (uGetUID(uid)); };
		virtual	BOOL	uGetUID ( UID &) = 0;
		virtual	BOOL	SearchOn ( DICOMDataObject	*,
					Array < DICOMDataObject *> *) = 0;
		virtual	BOOL	RetrieveOn (	DICOMDataObject *,
						DICOMDataObject **,
						StandardStorage	**,
						DICOMCommandObject	   *,
					        Array < DICOMDataObject *> *,
						void *) = 0;
		virtual	BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE *, BYTE * ) = 0;
		virtual	BOOL	CallBack (	DICOMCommandObject	*,
						DICOMDataObject	* ) = 0;
		BOOL	Read (	PDU_Service *, DICOMCommandObject *, void *ExtraBytes );
		BOOL	Write ( PDU_Service	*, DICOMDataObject	*, BYTE	*);
//		DICOMCommandObject 	   *GetDCO (void);
//		Array < DICOMDataObject *> *GetADDO(void);
	};


class	PatientRootRetrieveNKI	:
			public	StandardRetrieveNKI
	{
	public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };

		inline	BOOL	Read (	PDU_Service		*PDU,
					DICOMCommandObject	*DCO,
					void 			*ExtraBytes )
			{ return ( StandardRetrieveNKI :: Read ( PDU, DCO, ExtraBytes ) ); };
		BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO,
						BYTE	*ACRNema)
			{ return ( StandardRetrieveNKI :: Write ( PDU,
						DDO, ACRNema )); };
	};


class	StudyRootRetrieveNKI	:
			public	StandardRetrieveNKI
	{
	public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };
		
		inline	BOOL	Read (	PDU_Service		*PDU,
					DICOMCommandObject	*DCO,
					void 			*ExtraBytes )
			{ return ( StandardRetrieveNKI :: Read ( PDU, DCO, ExtraBytes ) ); };
		BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO,
						BYTE	*ACRNema)
			{ return ( StandardRetrieveNKI :: Write ( PDU,
						DDO, ACRNema )); };
	};


class	PatientStudyOnlyRetrieveNKI	:
			public	StandardRetrieveNKI
	{
	public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };
		
		inline	BOOL	Read (	PDU_Service	*PDU,
					DICOMCommandObject	*DCO,
					void 			*ExtraBytes )
			{ return ( StandardRetrieveNKI :: Read ( PDU, DCO, ExtraBytes ) ); };
		BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO,
						BYTE	*ACRNema)
			{ return ( StandardRetrieveNKI :: Write ( PDU,
						DDO, ACRNema )); };
	};
						

class	PatientRootRetrieveGeneric	:
			public	StandardRetrieveNKI
	{
	public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };

		inline	BOOL	Read (	PDU_Service		*PDU,
					DICOMCommandObject	*DCO,
					void 			*ExtraBytes )
			{ return ( StandardRetrieveNKI :: Read ( PDU, DCO, ExtraBytes ) ); };
		BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO,
						BYTE	*ACRNema)
			{ return ( StandardRetrieveNKI :: Write ( PDU,
						DDO, ACRNema )); };
	};


class	StudyRootRetrieveGeneric	:
			public	StandardRetrieveNKI
	{
	public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };
		
		inline	BOOL	Read (	PDU_Service		*PDU,
					DICOMCommandObject	*DCO,
					void 			*ExtraBytes )
			{ return ( StandardRetrieveNKI :: Read ( PDU, DCO, ExtraBytes ) ); };
		BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO,
						BYTE	*ACRNema)
			{ return ( StandardRetrieveNKI :: Write ( PDU,
						DDO, ACRNema )); };
	};


class	PatientStudyOnlyRetrieveGeneric	:
			public	StandardRetrieveNKI
	{
	public:
		BOOL	GetUID ( UID & );
		BOOL	uGetUID ( UID &uid ) { return ( GetUID(uid) ); };
		
		inline	BOOL	Read (	PDU_Service	*PDU,
					DICOMCommandObject	*DCO,
					void 			*ExtraBytes )
			{ return ( StandardRetrieveNKI :: Read ( PDU, DCO, ExtraBytes ) ); };
		BOOL	Write (	PDU_Service	*PDU,
						DICOMDataObject	*DDO,
						BYTE	*ACRNema)
			{ return ( StandardRetrieveNKI :: Write ( PDU,
						DDO, ACRNema )); };
	};

class	ExtendedPDU_Service	:
	public		PDU_Service
	{
		BOOL	AddTransferSyntaxs(PresentationContext &);	// override original function
		char	RequestedCompressionType[64];
		char	AcceptedCompressionType[64];
	public:
		BOOL	SetRequestedCompressionType(char *type);
		char*	GetAcceptedCompressionType(UID uid);
	};

BOOL ProcessDDO(DICOMDataObject** pDDO, DICOMCommandObject* pDCO);
BOOL DecompressNKI(DICOMDataObject* pDDO);
BOOL CompressNKI(DICOMDataObject* pDDO, int iCompressMode = 2);
unsigned int ComputeCRC(char* pcData, int iNbChars);

#endif
