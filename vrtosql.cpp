/*
20000629	ljz	Logging of trouble now starts with '***'
20010330	ljz	Added a few debug printf's
20010420	ljz	Fixed memory leaks in all query levels
20020519        mvh     Clear BindField result strings before reading (NULL does not read)
20021028	mvh	Restructured queries to give lowest level of de-normalized databases
                        Fixed sorting on PatientName in denormalized study query
20021030	mvh	Reversed this again apart from study level because SQL server becomes very slow
20030113        mvh     Added PatientQuerySortOrder etc overrides for sorting
20030114        mvh     Added in querycolumns (even if akreday present); 
                        tested for SQL server, requires testing for other datasources
20030122      ljz+mvh   Remove unused entries from Tables string in queries
20040930        mvh     Started adapt such that query strings etc and not limited in length
			For now: query string max 8192 (here and in odbci.cpp); sql statement max 16384 (in odbci.hpp)
			Enough for query with one multiple values UID of about 100 images (list truncated if too long)
			Added safestrcat protection of SearchString
20041003        mvh     Truncated debug prints, malloc strings at image level
                        Analysed string lenghts; limited 'records =' debug log
20041013	mvh	Used MAXQUERYLENGTH
20050107	mvh	Removed UNIX flags: solve difference in database interface
20050206	mvh	Image query can send filename and device in 0x9999,0x800/0x0801
20050401	mvh	Added QueryOnModalityWorkList; - todo - put selected items into sequence 0040,0100
20050404	mvh	Take query for Scheduled Procedure Step from sequence, put results into sequence
20050414	mvh	Made sequence unfolding for worklist more generic - accepts any number of and nested N=1 sequences
			This is correct behavior for query input, but limited for query results
20050417	mvh	Removed unused variable
20050831	mvh	Fixes in worklist code: is ok for all OFFIS tests except 1 (empty seq not returned) and 
			10 (undef non-empty seq takes one entry from higher level) - good enough for initial release?
20050901	mvh	Fix for test 10: allow undefined sequence in query
20050907	mvh	Merged change by Hans-Peter Hellemann: fix missing max() macro
20051229	mvh	Debug log show records of all queries. DumpVR only left in worklistquery and shows sequence layout
20051230	mvh	Removed 2 forgotten dumpvr's
20060103	mvh	Added debug info from testing phase inside nested sequences for modality query
20060211	mvh	Added empty required modality sequences 8,1110 and 8,1120 when not there
20060224	mvh	Fixed modality worklist query: empty result sequences were not handled correctly (Thanks Tony Tong)
20060311	mvh	Worklist change was tested OK with Agfa (thanks Frank Grogan), cleaned debug log a bit
20060607	mvh	Fix crash when coding empty sequence as last item in worklist query (thanks Francois Piette)
20061219	mvh	Small fix in layout debug log
20071118   	mvh    	Adapted for 64 bits (use SQLLEN for BindField)
20080817	mvh	Fixed bug found by larc: worklist sql syntax error (fields start with ,) when 1st item is sequence
		mvh	Fixed bug found by Alberto Smulders: sometimes sequence level in response would not come down
			The problem was that a higher levels the reported sequence level was inconsistent with the 1st level
			Changed CodeSequence stop criterium and level coding for deeper levels; now works with varies sequence combination
			Added WorkListReturnsISO_IR_100 flag (default 1)
20080818    	mvh 	DbaseIII check now uses DB flags, not PATHSEP in datasource name
20080901	mvh	Implemented 'Number of Patient Related Studies' (0x0020, 0x1200) etc
20080902	mvh	Fixed that option for VirtualServers; added EnableComputedFields 
*/

#ifndef	WHEDGE
#	include	"dgate.hpp"
#else
#	include	"master.h"
#endif

extern char	PatientQuerySortOrder[];
extern char	StudyQuerySortOrder[];
extern char	SeriesQuerySortOrder[];
extern char	ImageQuerySortOrder[];
extern int 	WorkListReturnsISO_IR_100;
extern int 	EnableComputedFields;

void safestrcat(char *result, char *tocat, int maxlen)
{ int len = strlen(result), len2 = strlen(tocat);
  if (len+len2 < maxlen)
    strcpy(result+len, tocat);
}

// from dgate.cpp
int VirtualQuery(DICOMDataObject *DDO, char *Level, int N, Array < DICOMDataObject  *> *pADDO);
void RemoveQueryDuplicates(char *Level, Array < DICOMDataObject * > *ADDO);

// compute items like 'Number of Patient Related Studies' and generate VR with the result
static VR *ComputeField(DICOMDataObject *DDO, int group, int element)
	{
	Array < DICOMDataObject * > ADDO;
	char count[16];
	char *Level;
	VR *vr;
	int save = DebugLevel, count1, count2, sources=0;
	
	// level for VirtualQueries to other servers
	if (element==0x1200) Level = "STUDY";
	if (element==0x1202) Level = "SERIES";
	if (element==0x1206) Level = "SERIES";
	if (element==0x1204) Level = "IMAGE";
	if (element==0x1208) Level = "IMAGE";
	if (element==0x1209) Level = "IMAGE";
        
	// make that a UNIQUE query will result the correct count
	if (element==0x1200 && DDO->GetVR(0x0020, 0x000d)==NULL) DDO->Push(new VR(0x0020, 0x000d, 0, FALSE));
	if (element==0x1202 && DDO->GetVR(0x0020, 0x000e)==NULL) DDO->Push(new VR(0x0020, 0x000e, 0, FALSE));
	if (element==0x1206 && DDO->GetVR(0x0020, 0x000e)==NULL) DDO->Push(new VR(0x0020, 0x000e, 0, FALSE));
	if (element==0x1204 && DDO->GetVR(0x0008, 0x0018)==NULL) DDO->Push(new VR(0x0008, 0x0018, 0, FALSE));
	if (element==0x1208 && DDO->GetVR(0x0008, 0x0018)==NULL) DDO->Push(new VR(0x0008, 0x0018, 0, FALSE));
	if (element==0x1209 && DDO->GetVR(0x0008, 0x0018)==NULL) DDO->Push(new VR(0x0008, 0x0018, 0, FALSE));

	// query other virtualfor servers
	for (int i=0; i<10; i++)
		sources += VirtualQuery(DDO, Level, i, &ADDO)!=0;
	count1 = ADDO.GetSize();

	// query this server
	DebugLevel=0;
	if (element==0x1200) QueryOnStudy  (DDO, &ADDO); // Number of Patient Related Studies
	if (element==0x1202) QueryOnSeries (DDO, &ADDO); // Number of Patient Related Series
	if (element==0x1206) QueryOnSeries (DDO, &ADDO); // Number of Study Related Series
	if (element==0x1204) QueryOnImage  (DDO, &ADDO); // Number of Patient Related Instances
	if (element==0x1208) QueryOnImage  (DDO, &ADDO); // Number of Study Related Instances
	if (element==0x1209) QueryOnImage  (DDO, &ADDO); // Number of Series Related Instances
	DebugLevel=save;

	// count number of servers accessed
	count2 = ADDO.GetSize()-count1;
	sources += count2!=0;

	if (sources>1)	// more than one source; data can be duplicated
		RemoveQueryDuplicates(Level, &ADDO);

	// create result VR
	sprintf(count, "%d", ADDO.GetSize());
	if (strlen(count)&1) strcat(count, " ");
	vr = new VR( group, element, strlen(count), TRUE);
	memcpy(vr->Data, count, strlen(count));

        // free arrays
	for (int i=0; i<ADDO.GetSize(); i++)
		while(vr=ADDO.Get(i)->Pop()) delete vr;

	while (ADDO.GetSize())
		{
		delete ADDO.Get(0);
		ADDO.RemoveAt(0);
		}
	
	return vr;
	}

BOOL	QueryOnPatient (
	DICOMDataObject *DDO,
	Array < DICOMDataObject *> *ADDO)
	{
	UINT				Index, CIndex, CCIndex;
	UINT32				VRLength;
	DICOMDataObject			*RDDO;
	Database			DB;
	char				*SQLResultString;
	Array < char * >		SQLResult, SQLResultPatient;
	Array < VR * >			EMaskPatient, EMask;
	Array < SQLLEN *>		SQLResultLength;
	Array < DBENTRY * >		DBQPatient;
	Array < DBENTRY * >		DBQMaster;
	DBENTRY				*TempDBEPtr;
	char				*DBR;
	SQLLEN				*DBL;
	VR				*vr;
	VR				VRPatientName;
	DBENTRY				*DBEntryPatientName;
	char				SortOrder[128];
	char				*Sorting;
	BOOL				DoSort;
	BOOL				SendAE = FALSE;
	char				TempString [ 8192 ];
	char				SearchString [ 8192 ];
	char				ColumnString [ 4096 ];
	char				Tables [ 256 ];
        char				*Sort=NULL;
      	DICOMDataObject 		*qStudies = NULL;
      	DICOMDataObject 		*qSeries = NULL;
      	DICOMDataObject 		*qSops = NULL;

	SystemDebug.printf("Query On Patient\n");

	// First.  Check that all the asked-for elements are actually in
	// the Patient/Study database.  If they are not, well, then we
	// return FALSE.
	DoSort = FALSE;
	while (vr = DDO->Pop())
		{
		if(vr->Element == 0x0000)
			{
			delete	vr;
			continue;	// discard length codes
			}
		if(vr->Group == 0x0008)
			if(vr->Element == 0x0052)
				{
				delete	vr;
				continue;	// discard model level
				}

		if(vr->Group == 0x0008)
			if(vr->Element == 0x0054)
				{
				SendAE = TRUE;
				delete vr;
				continue;	// discard it (but send it)
				}

		if(vr->Group == 0x0020 && EnableComputedFields)
			if(vr->Element == 0x1200)
				{
				qStudies = new DICOMDataObject;
				delete	vr;
				continue;	// discard 'Number of' items (but send them)
				}

		if(vr->Group == 0x0020 && EnableComputedFields)
			if(vr->Element == 0x1202)
				{
				qSeries = new DICOMDataObject;
				delete	vr;
				continue;	// discard 'Number of' items (but send them)
				}

		if(vr->Group == 0x0020 && EnableComputedFields)
			if(vr->Element == 0x1204)
				{
				qSops = new DICOMDataObject;
				delete	vr;
				continue;	// discard 'Number of' items (but send them)
				}

		if(!VerifyIsInDBE(vr, PatientDB, TempDBEPtr))
			{
			SystemDebug.printf("Failed on VR Search: %4.4x %4.4x\n",
				vr->Group, vr->Element);
			SystemDebug.printf("(testing phase) - ignored\n");
			delete	vr;
			continue;

			while(EMaskPatient.GetSize())
				{
				delete EMaskPatient.Get(0);
				EMaskPatient.RemoveAt(0);
				}
			while(SQLResultPatient.GetSize())
				{
				delete SQLResultPatient.Get(0);
				SQLResultPatient.RemoveAt(0);
				}

			return ( FALSE );
			}
		else
			{
			if(vr->Group == 0x0010)
				if(vr->Element == 0x0010)
					DoSort = TRUE;
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);
			SQLResultPatient.Add ( SQLResultString );
			EMaskPatient.Add ( vr );
			DBQPatient.Add ( TempDBEPtr );	
			}
		}

	if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
		{
		DB.PrintLastError();
		return ( FALSE );	// failed open
		}

	// Prepare the query string.
	// from EMasks, and SQLResults

	SearchString[0] = '\0';
	ColumnString[0] = '\0';

	Index = 0;CIndex = 0;CCIndex = 0;
	while ( Index < SQLResultPatient.GetSize() )
		{
		SQLResultString = SQLResultPatient.Get(Index);
		if(BuildSearchString(&DB, PatientDB, PatientTableName, EMaskPatient.Get(Index),
			SQLResultString, TempString, sizeof(TempString)-512))
			{
			if(CIndex++)
				safestrcat(SearchString, " and ", sizeof(SearchString));
			safestrcat(SearchString, TempString, sizeof(SearchString));
			}
		BuildColumnString(PatientDB, PatientTableName, EMaskPatient.Get(Index), TempString);
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, TempString);
		EMask.Add(EMaskPatient.Get(Index));
		DBQMaster.Add(DBQPatient.Get(Index));
		++Index;++CCIndex;
		}

        if (PatientQuerySortOrder[0]) 
		{
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, PatientQuerySortOrder);
		}

        sprintf(Tables, "%s",
	  	PatientTableName);
        
	SystemDebug.printf("Issue Query on Columns: %s\n", ColumnString);
	SystemDebug.printf("Values: %.1000s\n", SearchString);
	SystemDebug.printf("Tables: %.1000s\n", Tables);

	while(SQLResultPatient.GetSize())
		{
		delete SQLResultPatient.Get(0);
		SQLResultPatient.RemoveAt(0);
		}

	VRPatientName.Group = 0x0010;
	VRPatientName.Element = 0x0010;
	DBEntryPatientName = FindDBE(&VRPatientName);
	if(DBEntryPatientName)
		{
		sprintf(SortOrder, "%s.%s",
			PatientTableName,
			DBEntryPatientName->SQLColumn);
		if(DoSort)
			Sorting = SortOrder;
		else
			Sorting = NULL;
		}
	else
		Sorting = NULL;

        Sort = Sorting;
        if (PatientQuerySortOrder[0]) Sort = PatientQuerySortOrder;

	SystemDebug.printf("Sorting (%s) DoSort := %d\n", Sort, DoSort);

	if(strlen(SearchString))
		{
		if (!DB.QueryDistinct ( Tables, ColumnString, SearchString, Sort) )
			{
			DB.PrintLastError();
			return ( FALSE );	// failed query
			}
		}
	else
		if (!DB.QueryDistinct ( Tables, ColumnString, NULL, Sort))
			{
			DB.PrintLastError();
			return ( FALSE );	// failed query
			}

	Index = 0;
	while ( Index < CCIndex )
		{
		DBR = new char[255];
		DBR[0] = 0;			// in case a field is NULL it does not read
		SQLResult.Add(DBR);
		DBL = new SQLLEN;
		SQLResultLength.Add(DBL);
		if(!DB.BindField (Index+1, SQL_C_CHAR,
			SQLResult.Get(Index), 255,
			SQLResultLength.Get(Index)))
			{
			SystemDebug.printf("Column Number : %d\n", Index+1);
			DB.PrintLastError();
			while(SQLResult.GetSize())
				{
				delete SQLResult.Get(0);
				SQLResult.RemoveAt(0);
				}
			while(SQLResultLength.GetSize())
				{
				delete SQLResultLength.Get(0);
				SQLResultLength.RemoveAt(0);
				}
			return ( FALSE );	// failed to bind column
			}
		++Index;
		}
	while (DB.NextRecord())
		{
		RDDO = new DICOMDataObject;
		Index = 0;
		while ( Index < CCIndex )
			{
			VRLength = SQLRealSize(SQLResult.Get(Index),
							*SQLResultLength.Get(Index));
			vr = ConstructVRFromSQL (
				DBQMaster.Get(Index),
				EMask.Get(Index)->Group,
				EMask.Get(Index)->Element,
				VRLength,
				SQLResult.Get(Index));

			if (qStudies) 
				qStudies->Push(ConstructVRFromSQL (DBQMaster.Get(Index), EMask.Get(Index)->Group, EMask.Get(Index)->Element, VRLength, SQLResult.Get(Index)));
			if (qSeries) 
				qSeries->Push(ConstructVRFromSQL (DBQMaster.Get(Index), EMask.Get(Index)->Group, EMask.Get(Index)->Element, VRLength, SQLResult.Get(Index)));
			if (qSops) 
				qSops->Push(ConstructVRFromSQL (DBQMaster.Get(Index), EMask.Get(Index)->Group, EMask.Get(Index)->Element, VRLength, SQLResult.Get(Index)));

			RDDO->Push(vr);
			++Index;
			}

		if(SendAE)
			RDDO->Push(ConstructAE());

		if (qStudies)
			{
			RDDO->Push(ComputeField(qStudies, 0x0020, 0x1200));
			qStudies->Reset();
			}

		if (qSeries)
			{
			RDDO->Push(ComputeField(qSeries, 0x0020, 0x1202));
			qSeries->Reset();
			}

		if (qSops)
			{
			RDDO->Push(ComputeField(qSops, 0x0020, 0x1204));
			qSops->Reset();
			}

		ADDO->Add(RDDO);
		}

	SystemDebug.printf("Records = %d\n", ADDO->GetSize());

	while(SQLResult.GetSize())
		{
		delete SQLResult.Get(0);
		SQLResult.RemoveAt(0);
		}
	while(SQLResultLength.GetSize())
		{
		delete SQLResultLength.Get(0);
		SQLResultLength.RemoveAt(0);
		}

	/* LJ: Leak!! */
	while(EMaskPatient.GetSize())
		{
		delete EMaskPatient.Get(0);
		EMaskPatient.RemoveAt(0);
		}

	if (qStudies) delete qStudies;
	if (qSeries) delete qSeries;
	if (qSops) delete qSops;

	return ( TRUE );
	}

BOOL	QueryOnStudy (
	DICOMDataObject *DDO,
	Array < DICOMDataObject *> *ADDO)
	{
	UINT				Index, CIndex, CCIndex;
	UINT32				VRLength;
	DICOMDataObject			*RDDO;
	Database			DB;
	char				*SQLResultString;
	Array < char * >		SQLResult, SQLResultStudy, SQLResultPatient;
	Array < DBENTRY * >		DBQPatient, DBQStudy;
	Array < DBENTRY * >		DBQMaster;
	Array < VR * >			EMaskPatient, EMaskStudy, EMask;
	Array < SQLLEN *>		SQLResultLength;
	DBENTRY				*TempDBEPtr;
	char				*DBR;
	SQLLEN				*DBL;
	VR				*vr;
	BOOL				SendAE = FALSE;
	char				TempString [ 8192 ];
	char				SearchString [ 8192 ];
	char				ColumnString [ 4096 ];
	char				Tables [ 256 ];
	VR				VRPatientName;
	DBENTRY				*DBEntryPatientName;
	char				SortOrder[128];
	char				*Sorting;
	BOOL				DoSort;
        char				*Sort=NULL;
      	DICOMDataObject 		*qSeries = NULL;
      	DICOMDataObject 		*qSops = NULL;

	SystemDebug.printf("Query On Study\n");

	// First.  Check that all the asked-for elements are actually in
	// the Patient/Study database.  If they are not, well, then we
	// return FALSE.

	DoSort = FALSE;

	while (vr = DDO->Pop())
		{
		if(vr->Element == 0x0000)
			{
			delete	vr;
			continue;	// discard length codes
			}

		if(vr->Group == 0x0008)
			if(vr->Element == 0x0052)
				{
				delete	vr;
				continue;	// discard model level
				}

		if(vr->Group == 0x0008)
			if(vr->Element == 0x0054)
				{
				SendAE = TRUE;
				delete vr;
				continue;	// discard it (but send it)
				}

		if(vr->Group == 0x0020 && EnableComputedFields)
			if(vr->Element == 0x1206)
				{
				qSeries = new DICOMDataObject;
				delete	vr;
				continue;	// discard 'Number of' items (but send them)
				}

		if(vr->Group == 0x0020 && EnableComputedFields)
			if(vr->Element == 0x1208)
				{
				qSops = new DICOMDataObject;
				delete	vr;
				continue;	// discard 'Number of' items (but send them)
				}

		if(vr->Group == 0x0010)
			if(vr->Element == 0x0010)
				DoSort = TRUE;

		if(VerifyIsInDBE(vr, StudyDB, TempDBEPtr))
			{
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);
			SQLResultStudy.Add ( SQLResultString );
			EMaskStudy.Add ( vr );
			DBQStudy.Add ( TempDBEPtr );
			}
		else if(VerifyIsInDBE(vr, PatientDB, TempDBEPtr))
			{
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);
			SQLResultPatient.Add ( SQLResultString );
			EMaskPatient.Add ( vr );
			DBQPatient.Add ( TempDBEPtr );
			}
		else
			{
			SystemDebug.printf("Failed on VR Search: %4.4x %4.4x\n",
				vr->Group, vr->Element);
			SystemDebug.printf("(testing phase) - ignored\n");
			delete	vr;
			continue;
			}
		}

	if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
		{
		DB.PrintLastError();
		return ( FALSE );	// failed open
		}

	// Prepare the query string.
	// from EMasks, and SQLResults

	SearchString[0] = '\0';
	ColumnString[0] = '\0';

	Index = 0;CIndex = 0;CCIndex = 0;
	while ( Index < SQLResultPatient.GetSize() )
		{
		SQLResultString = SQLResultPatient.Get(Index);
		if(BuildSearchString(&DB, PatientDB, PatientTableName, EMaskPatient.Get(Index),
			SQLResultString, TempString, sizeof(TempString)-512))
			{
			if(CIndex++)
				safestrcat(SearchString, " and ", sizeof(SearchString));
			safestrcat(SearchString, TempString, sizeof(SearchString));
			}
		BuildColumnString(PatientDB, PatientTableName, EMaskPatient.Get(Index), TempString);
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, TempString);
		EMask.Add(EMaskPatient.Get(Index));
		DBQMaster.Add(DBQPatient.Get(Index));
		++Index;++CCIndex;
		}
	Index = 0;
	while ( Index < SQLResultStudy.GetSize() )
		{
		SQLResultString = SQLResultStudy.Get(Index);
		if(BuildSearchString(&DB, StudyDB, StudyTableName, EMaskStudy.Get(Index),
			SQLResultString, TempString, sizeof(TempString)-512))
			{
			if(CIndex++)
				safestrcat(SearchString, " and ", sizeof(SearchString));
			safestrcat(SearchString, TempString, sizeof(SearchString));
			}
		BuildColumnString(StudyDB, StudyTableName, EMaskStudy.Get(Index), TempString);
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, TempString);
		EMask.Add(EMaskStudy.Get(Index));
		DBQMaster.Add(DBQStudy.Get(Index));
		++Index;++CCIndex;
		}

	// Join only required when patient items asked (i.e., not for denormalized databases)
	if (SQLResultPatient.GetSize())
		{
		if(CIndex++)
			safestrcat(SearchString, " and ", sizeof(SearchString));
		// Join Study into Patient
		sprintf(TempString, "%s.%s = %s.%s",
			StudyTableName, StudyDB [ LastDBE(StudyDB) ].SQLColumn,
			PatientTableName, StudyDB [ LastDBE(StudyDB) ].SQLColumn);
		safestrcat(SearchString, TempString, sizeof(SearchString));
		}

        if (StudyQuerySortOrder[0]) 
		{
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, StudyQuerySortOrder);
		}

	if (SQLResultPatient.GetSize())
	  sprintf(Tables, "%s, %s",
	  	StudyTableName, PatientTableName);
	else
	  sprintf(Tables, "%s",
	  	StudyTableName);
        
	SystemDebug.printf("Issue Query on Columns: %s\n", ColumnString);
	SystemDebug.printf("Values: %.1000s\n", SearchString);
	SystemDebug.printf("Tables: %.1000s\n", Tables);

	while(SQLResultStudy.GetSize())
		{
		delete SQLResultStudy.Get(0);
		SQLResultStudy.RemoveAt(0);
		}
	while(SQLResultPatient.GetSize())
		{
		delete SQLResultPatient.Get(0);
		SQLResultPatient.RemoveAt(0);
		}

	// Issue Query

	VRPatientName.Group = 0x0010;
	VRPatientName.Element = 0x0010;
	DBEntryPatientName = FindDBE(&VRPatientName);
	if(DBEntryPatientName)
		{
		// preferably sort on denormalized database entries
		if (DBEIndex(StudyDB, &VRPatientName))
			sprintf(SortOrder, "%s.%s",
				StudyTableName,
				DBEntryPatientName->SQLColumn);
		else
			sprintf(SortOrder, "%s.%s",
				PatientTableName,
				DBEntryPatientName->SQLColumn);
		if(DoSort)
			Sorting = SortOrder;
		else
			Sorting = NULL;
		}
	else
		Sorting = NULL;

        Sort = Sorting;
        if (StudyQuerySortOrder[0]) Sort = StudyQuerySortOrder;

	SystemDebug.printf("Sorting (%s) DoSort := %d\n", Sort, DoSort);

	if(strlen(SearchString))
		{
		if (!DB.QueryDistinct ( Tables, ColumnString, SearchString, Sort) )
			{
			DB.PrintLastError();
			return ( FALSE );	// failed query
			}
		}
	else
		if (!DB.QueryDistinct ( Tables, ColumnString, NULL, Sort))
			{
			DB.PrintLastError();
			return ( FALSE );	// failed query
			}

	Index = 0;
	while ( Index < CCIndex )
		{
		DBR = new char[255];
		DBR[0] = 0;			// in case a field is NULL it does not read
		SQLResult.Add(DBR);
		DBL = new SQLLEN;
		SQLResultLength.Add(DBL);
		if(!DB.BindField (Index+1, SQL_C_CHAR,
			SQLResult.Get(Index), 255,
			SQLResultLength.Get(Index)))
			{
			SystemDebug.printf("Column Number : %d\n", Index+1);
			DB.PrintLastError();
			while(SQLResult.GetSize())
				{
				delete SQLResult.Get(0);
				SQLResult.RemoveAt(0);
				}
			while(SQLResultLength.GetSize())
				{
				delete SQLResultLength.Get(0);
				SQLResultLength.RemoveAt(0);
				}
			return ( FALSE );	// failed to bind column
			}
		++Index;
		}
	while (DB.NextRecord())
		{
		RDDO = new DICOMDataObject;
		Index = 0;
		while ( Index < CCIndex )
			{
			VRLength = SQLRealSize(SQLResult.Get(Index),
							*SQLResultLength.Get(Index));
			vr = ConstructVRFromSQL (
				DBQMaster.Get(Index),
				EMask.Get(Index)->Group,
				EMask.Get(Index)->Element,
				VRLength,
				SQLResult.Get(Index));
			RDDO->Push(vr);

			if (qSeries) 
				qSeries->Push(ConstructVRFromSQL (DBQMaster.Get(Index), EMask.Get(Index)->Group, EMask.Get(Index)->Element, VRLength, SQLResult.Get(Index)));
			if (qSops) 
				qSops->Push(ConstructVRFromSQL (DBQMaster.Get(Index), EMask.Get(Index)->Group, EMask.Get(Index)->Element, VRLength, SQLResult.Get(Index)));
			
			++Index;
			}

		if(SendAE)
			RDDO->Push(ConstructAE());

		if (qSeries)
			{
			RDDO->Push(ComputeField(qSeries, 0x0020, 0x1206));
			qSeries->Reset();
			}

		if (qSops)
			{
			RDDO->Push(ComputeField(qSops, 0x0020, 0x1208));
			qSops->Reset();
			}

		ADDO->Add(RDDO);
		}

	SystemDebug.printf("Records = %d\n", ADDO->GetSize());

	while(SQLResult.GetSize())
		{
		delete SQLResult.Get(0);
		SQLResult.RemoveAt(0);
		}
	while(SQLResultLength.GetSize())
		{
		delete SQLResultLength.Get(0);
		SQLResultLength.RemoveAt(0);
		}

	/* LJ: Leak!! */
	while(EMaskStudy.GetSize())
		{
		delete EMaskStudy.Get(0);
		EMaskStudy.RemoveAt(0);
		}
	while(EMaskPatient.GetSize())
		{
		delete EMaskPatient.Get(0);
		EMaskPatient.RemoveAt(0);
		}

	if (qSeries) delete qSeries;
	if (qSops) delete qSops;

	return ( TRUE );
	}

BOOL	QueryOnSeries (
	DICOMDataObject *DDO,
	Array < DICOMDataObject *> *ADDO)
	{
	UINT				Index, CIndex, CCIndex;
	UINT32				VRLength;
	DICOMDataObject			*RDDO;
	Database			DB;
	char				*SQLResultString;
	Array < char * >		SQLResult, SQLResultStudy, SQLResultPatient,
					SQLResultSeries;
	Array < DBENTRY * >		DBQPatient, DBQStudy, DBQSeries;
	Array < DBENTRY * >		DBQMaster;
	Array < VR * >			EMaskPatient, EMaskStudy, EMaskSeries, EMask;
	Array < SQLLEN *>		SQLResultLength;
	DBENTRY				*TempDBEPtr;
	char				*DBR;
	SQLLEN				*DBL;
	VR				*vr;
	BOOL				SendAE = FALSE;
	char				TempString [ 8192 ];
	char				SearchString [ 8192 ];
	char				ColumnString [ 4096 ];
	char				Tables [ 256 ];
        char				*Sort=NULL;
      	DICOMDataObject 		*qSops = NULL;

	SystemDebug.printf("Query On Series\n");

	// First.  Check that all the asked-for elements are actually in
	// the Patient/Study database.  If they are not, well, then we
	// return FALSE.

	while (vr = DDO->Pop())
		{
		if(vr->Element == 0x0000)
			{
			delete	vr;
			continue;	// discard length codes
			}

		if(vr->Group == 0x0008)
			if(vr->Element == 0x0052)
				{
				delete	vr;
				continue;	// discard model level
				}

		if(vr->Group == 0x0008)
			if(vr->Element == 0x0054)
				{
				SendAE = TRUE;
				delete vr;
				continue;	// discard it (but send it)
				}

		if(vr->Group == 0x0020 && EnableComputedFields)
			if(vr->Element == 0x1209)
				{
				qSops = new DICOMDataObject;
				delete	vr;
				continue;	// discard 'Number of' items (but send them)
				}

		if(VerifyIsInDBE(vr, StudyDB, TempDBEPtr))
			{
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);
			SQLResultStudy.Add ( SQLResultString );
			EMaskStudy.Add ( vr );
			DBQStudy.Add ( TempDBEPtr );
			}
		else if(VerifyIsInDBE(vr, PatientDB, TempDBEPtr))
			{
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);
			SQLResultPatient.Add ( SQLResultString );
			EMaskPatient.Add ( vr );
			DBQPatient.Add ( TempDBEPtr );
			}
		else if(VerifyIsInDBE(vr, SeriesDB, TempDBEPtr))
			{
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);
			SQLResultSeries.Add ( SQLResultString );
			EMaskSeries.Add ( vr );
			DBQSeries.Add ( TempDBEPtr );
			}
		else
			{
			SystemDebug.printf("Failed on VR Search: %4.4x %4.4x\n",
						vr->Group, vr->Element);
			SystemDebug.printf("(testing phase) - ignored\n");
			delete	vr;
			continue;
			}
		}

	if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
		{
		DB.PrintLastError();
		return ( FALSE );	// failed open
		}

	// Prepare the query string.
	// from EMasks, and SQLResults

	SearchString[0] = '\0';
	ColumnString[0] = '\0';

	Index = 0;CIndex = 0;CCIndex = 0;
	while ( Index < SQLResultSeries.GetSize() )
		{
		SQLResultString = SQLResultSeries.Get(Index);
		if(BuildSearchString(&DB, SeriesDB, SeriesTableName, EMaskSeries.Get(Index),
			SQLResultString, TempString, sizeof(TempString)-512))
			{
			if(CIndex++)
				safestrcat(SearchString, " and ", sizeof(SearchString));
			safestrcat(SearchString, TempString, sizeof(SearchString));
			}
		BuildColumnString(SeriesDB, SeriesTableName, EMaskSeries.Get(Index), TempString);
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, TempString);
		EMask.Add(EMaskSeries.Get(Index));
		DBQMaster.Add(DBQSeries.Get(Index));
		++Index;++CCIndex;
		}
	Index = 0;
	while ( Index < SQLResultPatient.GetSize() )
		{
		SQLResultString = SQLResultPatient.Get(Index);
		if(BuildSearchString(&DB, PatientDB, PatientTableName, EMaskPatient.Get(Index),
			SQLResultString, TempString, sizeof(TempString)-512))
			{
			if(CIndex++)
				safestrcat(SearchString, " and ", sizeof(SearchString));
			safestrcat(SearchString, TempString, sizeof(SearchString));
			}
		BuildColumnString(PatientDB, PatientTableName, EMaskPatient.Get(Index), TempString);
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, TempString);
		EMask.Add(EMaskPatient.Get(Index));
		DBQMaster.Add(DBQPatient.Get(Index));
		++Index;++CCIndex;
		}
	Index = 0;
	while ( Index < SQLResultStudy.GetSize() )
		{
		SQLResultString = SQLResultStudy.Get(Index);
		if(BuildSearchString(&DB, StudyDB, StudyTableName, EMaskStudy.Get(Index),
			SQLResultString, TempString, sizeof(TempString)-512))
			{
			if(CIndex++)
				safestrcat(SearchString, " and ", sizeof(SearchString));
			safestrcat(SearchString, TempString, sizeof(SearchString));
			}
		BuildColumnString(StudyDB, StudyTableName, EMaskStudy.Get(Index), TempString);
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, TempString);
		EMask.Add(EMaskStudy.Get(Index));
		DBQMaster.Add(DBQStudy.Get(Index));
		++Index;++CCIndex;
		}

	if (SQLResultPatient.GetSize())
		{
		if(CIndex++)
			safestrcat(SearchString, " and ", sizeof(SearchString));
		// Join Study into Patient
		sprintf(TempString, "%s.%s = %s.%s",
			StudyTableName, StudyDB [ LastDBE(StudyDB) ].SQLColumn,
			PatientTableName, StudyDB [ LastDBE(StudyDB) ].SQLColumn);
		safestrcat(SearchString, TempString, sizeof(SearchString));
		}

	if (SQLResultStudy.GetSize() || SQLResultPatient.GetSize())
		{
		if(CIndex++)
			safestrcat(SearchString, " and ", sizeof(SearchString));
		// Join Series into Study
		sprintf(TempString, "%s.%s = %s.%s",
			SeriesTableName, SeriesDB [ LastDBE(SeriesDB) ].SQLColumn,
			StudyTableName, SeriesDB [ LastDBE(SeriesDB) ].SQLColumn);
		safestrcat(SearchString, TempString, sizeof(SearchString));
		}

        if (SeriesQuerySortOrder[0]) 
		{
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, SeriesQuerySortOrder);
		}

	if (SQLResultPatient.GetSize())
	  sprintf(Tables, "%s, %s, %s",
	  	SeriesTableName, StudyTableName, PatientTableName);
	else if (SQLResultStudy.GetSize())
	  sprintf(Tables, "%s, %s",
	  	SeriesTableName, StudyTableName);
	else
	  sprintf(Tables, "%s",
	  	SeriesTableName);
        
	SystemDebug.printf("Issue Query on Columns: %s\n", ColumnString);
	SystemDebug.printf("Values: %.1000s\n", SearchString);
	SystemDebug.printf("Tables: %.1000s\n", Tables);

	while(SQLResultSeries.GetSize())
		{
		delete SQLResultSeries.Get(0);
		SQLResultSeries.RemoveAt(0);
		}
	while(SQLResultStudy.GetSize())
		{
		delete SQLResultStudy.Get(0);
		SQLResultStudy.RemoveAt(0);
		}
	while(SQLResultPatient.GetSize())
		{
		delete SQLResultPatient.Get(0);
		SQLResultPatient.RemoveAt(0);
		}

	// Issue Query

        if (SeriesQuerySortOrder[0]) Sort = SeriesQuerySortOrder;

	if(strlen(SearchString))
		{
		if (!DB.QueryDistinct ( Tables, ColumnString, SearchString, Sort) )
			{
			DB.PrintLastError();
			return ( FALSE );	// failed query
			}
		}
	else
		if (!DB.QueryDistinct ( Tables, ColumnString, NULL, Sort))
			{
			DB.PrintLastError();
			return ( FALSE );	// failed query
			}

	Index = 0;
	while ( Index < CCIndex )
		{
		DBR = new char[255];
		DBR[0] = 0;			// in case a field is NULL it does not read
		SQLResult.Add(DBR);
		DBL = new SQLLEN;
		SQLResultLength.Add(DBL);
		if(!DB.BindField (Index+1, SQL_C_CHAR,
			SQLResult.Get(Index), 255,
			SQLResultLength.Get(Index)))
			{
			SystemDebug.printf("***Column Number : %d\n", Index+1);
			DB.PrintLastError();
			while(SQLResult.GetSize())
				{
				delete SQLResult.Get(0);
				SQLResult.RemoveAt(0);
				}
			while(SQLResultLength.GetSize())
				{
				delete SQLResultLength.Get(0);
				SQLResultLength.RemoveAt(0);
				}
			return ( FALSE );	// failed to bind column
			}
		++Index;
		}
	while (DB.NextRecord())
		{
		RDDO = new DICOMDataObject;
		Index = 0;
		while ( Index < CCIndex )
			{
			VRLength = SQLRealSize(SQLResult.Get(Index),
							*SQLResultLength.Get(Index));
			vr = ConstructVRFromSQL (
				DBQMaster.Get(Index),
				EMask.Get(Index)->Group,
				EMask.Get(Index)->Element,
				VRLength,
				SQLResult.Get(Index));
			RDDO->Push(vr);

			if (qSops) 
				qSops->Push(ConstructVRFromSQL (DBQMaster.Get(Index), EMask.Get(Index)->Group, EMask.Get(Index)->Element, VRLength, SQLResult.Get(Index)));
			
			++Index;
			}

		if(SendAE)
			RDDO->Push(ConstructAE());

		if (qSops)
			{
			RDDO->Push(ComputeField(qSops, 0x0020, 0x1209));
			qSops->Reset();
			}

		ADDO->Add(RDDO);
		}

	SystemDebug.printf("Records = %d\n", ADDO->GetSize());

	while(SQLResult.GetSize())
		{
		delete SQLResult.Get(0);
		SQLResult.RemoveAt(0);
		}
	while(SQLResultLength.GetSize())
		{
		delete SQLResultLength.Get(0);
		SQLResultLength.RemoveAt(0);
		}

	/* LJ: Leak!! */
	while(EMaskSeries.GetSize())
		{
		delete EMaskSeries.Get(0);
		EMaskSeries.RemoveAt(0);
		}
	while(EMaskStudy.GetSize())
		{
		delete EMaskStudy.Get(0);
		EMaskStudy.RemoveAt(0);
		}
	while(EMaskPatient.GetSize())
		{
		delete EMaskPatient.Get(0);
		EMaskPatient.RemoveAt(0);
		}

	if (qSops) delete qSops;

	return ( TRUE );

	}

#define MAXLENGTH (MAXQUERYLENGTH-10000)

BOOL	QueryOnImage (
	DICOMDataObject *DDO,
	Array < DICOMDataObject *> *ADDO)
	{
	UINT				Index, CIndex, CCIndex;
	UINT32				VRLength;
	DICOMDataObject			*RDDO;
	Database			DB;
	char				*SQLResultString;
	Array < char * >		SQLResult, SQLResultStudy, SQLResultPatient,
					SQLResultSeries, SQLResultImage;
	Array < DBENTRY * >		DBQPatient, DBQStudy, DBQSeries, DBQImage;
	Array < DBENTRY * >		DBQMaster;
	Array < VR * >			EMaskPatient, EMaskStudy, EMaskSeries, EMaskImage, EMask;
	Array < SQLLEN *>		SQLResultLength;
	DBENTRY				*TempDBEPtr;
	char				*DBR;
	SQLLEN				*DBL;
	VR				*vr;
	BOOL				SendAE = FALSE, SendObjectFile = FALSE;
	char				*TempString;
	char				*SearchString;
	char				ColumnString [ 4096 ];
	char				Tables [ 256 ];
        char				*Sort=NULL;

// Analysis of length of strings:
//
// let us assume max #fields to query is 100 (>> total fields in DB)
// max size columnstring is about 100 x (maxfieldname+2) = 1200 -> no problem
// max size tempstring is length of a single field = 'value' string
// max size searchstring is total (length of field = 'value' strings + 5)
//                          + 172 (for joins)
// it is no problem here to malloc these strings at really large size
// values add up to max 1000 if no multiple used, fields = add up to max 3000
// if multiple is used at one level (reasonable), max 67 per entry
// Assume max slices is 4000 for 1 series, max searchstring: 
// 4000 + 4000*67 = 272000, max tempstring: 30 + 4000*67 = 268030, 
// set lets say 300 kb each. This value is defined in MAXLENGTH

// Most likely the max at lower query levels are much smaller, and 8 kb 
// will be fine at these levels (allows multiplicity of 59)

        TempString = NULL;
        SearchString = NULL;

        TempString = (char *)malloc(MAXLENGTH+1);
        SearchString = (char *)malloc(MAXLENGTH+1);

	if (TempString==NULL || SearchString==NULL)
        	{ 
		free(TempString);
          	free(SearchString);
          	return FALSE;
        	}

	SystemDebug.printf("Query On Image\n");

	while (vr = DDO->Pop())
		{
		if(vr->Element == 0x0000)
			{
			delete	vr;
			continue;	// discard length codes
			}
		if(vr->Group == 0x0008)
			if(vr->Element == 0x0052)
				{
				delete	vr;
				continue;	// discard model level
				}
		if(vr->Group == 0x0008)
			if(vr->Element == 0x0054)
				{
				SendAE = TRUE;
				delete vr;
				continue;	// discard it (but send it)
				}
		if(vr->Group == 0x9999)
			if(vr->Element == 0x0800)
				{
				SendObjectFile = TRUE;
				delete vr;
				continue;	// discard it (but send it)
				}
		if(vr->Group == 0x9999)
			if(vr->Element == 0x0801)
				{
				SendObjectFile = TRUE;
				delete vr;
				continue;	// discard it (but send it)
				}
		if(VerifyIsInDBE(vr, StudyDB, TempDBEPtr))
			{
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);
			SQLResultStudy.Add ( SQLResultString );
			EMaskStudy.Add ( vr );
			DBQStudy.Add ( TempDBEPtr );
			}
		else if(VerifyIsInDBE(vr, PatientDB, TempDBEPtr))
			{
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);
			SQLResultPatient.Add ( SQLResultString );
			EMaskPatient.Add ( vr );
			DBQPatient.Add ( TempDBEPtr );
			}
		else if(VerifyIsInDBE(vr, SeriesDB, TempDBEPtr))
			{
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);
			SQLResultSeries.Add ( SQLResultString );
			EMaskSeries.Add ( vr );
			DBQSeries.Add ( TempDBEPtr );
			}
		else if(VerifyIsInDBE(vr, ImageDB, TempDBEPtr))
			{
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);
			SQLResultImage.Add ( SQLResultString );
			EMaskImage.Add ( vr );
			DBQImage.Add(TempDBEPtr);
			}
		else
			{
			SystemDebug.printf("Failed on VR Search: %4.4x %4.4x\n",
						vr->Group, vr->Element);
			SystemDebug.printf("(testing phase) - ignored\n");
			delete	vr;
			continue;
			}
		}

	if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
		{
		DB.PrintLastError();
		free(TempString);
          	free(SearchString);
          	return FALSE;
        	}

	// Prepare the query string.
	// from EMasks, and SQLResults

	SearchString[0] = '\0';
	ColumnString[0] = '\0';

	Index = 0;CIndex = 0;CCIndex = 0;
	while ( Index < SQLResultImage.GetSize() )
		{
		SQLResultString = SQLResultImage.Get(Index);
		if(BuildSearchString(&DB, ImageDB, ImageTableName, EMaskImage.Get(Index),
			SQLResultString, TempString, MAXLENGTH-4000))
			{
			if(CIndex++)
				safestrcat(SearchString, " and ", MAXLENGTH);
			safestrcat(SearchString, TempString, MAXLENGTH);
			}
		BuildColumnString(ImageDB, ImageTableName, EMaskImage.Get(Index), TempString);
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, TempString);
		EMask.Add(EMaskImage.Get(Index));
		DBQMaster.Add(DBQImage.Get(Index));
		++Index;++CCIndex;
		}
	Index = 0;
	while ( Index < SQLResultSeries.GetSize() )
		{
		SQLResultString = SQLResultSeries.Get(Index);
		if(BuildSearchString(&DB, SeriesDB, SeriesTableName, EMaskSeries.Get(Index),
			SQLResultString, TempString, MAXLENGTH-4000))
			{
			if(CIndex++)
				safestrcat(SearchString, " and ", MAXLENGTH);
			safestrcat(SearchString, TempString, MAXLENGTH);
			}
		BuildColumnString(SeriesDB, SeriesTableName, EMaskSeries.Get(Index), TempString);
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, TempString);
		EMask.Add(EMaskSeries.Get(Index));
		DBQMaster.Add(DBQSeries.Get(Index));
		++Index;++CCIndex;
		}
	Index = 0;
	while ( Index < SQLResultPatient.GetSize() )
		{
		SQLResultString = SQLResultPatient.Get(Index);
		if(BuildSearchString(&DB, PatientDB, PatientTableName, EMaskPatient.Get(Index),
			SQLResultString, TempString, MAXLENGTH-4000))
			{
			if(CIndex++)
				safestrcat(SearchString, " and ", MAXLENGTH);
			safestrcat(SearchString, TempString, MAXLENGTH);
			}
		BuildColumnString(PatientDB, PatientTableName, EMaskPatient.Get(Index), TempString);
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, TempString);
		EMask.Add(EMaskPatient.Get(Index));
		DBQMaster.Add(DBQPatient.Get(Index));
		++Index;++CCIndex;
		}
	Index = 0;
	while ( Index < SQLResultStudy.GetSize() )
		{
		SQLResultString = SQLResultStudy.Get(Index);
		if(BuildSearchString(&DB, StudyDB, StudyTableName, EMaskStudy.Get(Index),
			SQLResultString, TempString, MAXLENGTH-4000))
			{
			if(CIndex++)
				safestrcat(SearchString, " and ", MAXLENGTH);
			safestrcat(SearchString, TempString, MAXLENGTH);
			}
		BuildColumnString(StudyDB, StudyTableName, EMaskStudy.Get(Index), TempString);
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, TempString);
		EMask.Add(EMaskStudy.Get(Index));
		DBQMaster.Add(DBQStudy.Get(Index));
		++Index;++CCIndex;
		}

	if (SQLResultPatient.GetSize())
		{
		if(CIndex++)
			safestrcat(SearchString, " and ", MAXLENGTH);
		// Join Study into Patient
		sprintf(TempString, "%s.%s = %s.%s",
			StudyTableName, StudyDB [ LastDBE(StudyDB) ].SQLColumn,
			PatientTableName, StudyDB [ LastDBE(StudyDB) ].SQLColumn);
		safestrcat(SearchString, TempString, MAXLENGTH);
		}

	if (SQLResultStudy.GetSize() || SQLResultPatient.GetSize())
		{
		// Join Series into Study
		if (CIndex++)
			safestrcat(SearchString, " and ", MAXLENGTH);
		sprintf(TempString, "%s.%s = %s.%s",
			SeriesTableName, SeriesDB [ LastDBE(SeriesDB) ].SQLColumn,
			StudyTableName, SeriesDB [ LastDBE(SeriesDB) ].SQLColumn);
		safestrcat(SearchString, TempString, MAXLENGTH);
		}

	if (SQLResultSeries.GetSize() || SQLResultStudy.GetSize() || SQLResultPatient.GetSize())
		{
		if(CIndex++)
			safestrcat(SearchString, " and ", MAXLENGTH);
		// Join Image into Series
		sprintf(TempString, "%s.%s = %s.%s",
			ImageTableName, ImageDB [ LastDBE(ImageDB) ].SQLColumn,
			SeriesTableName, ImageDB [ LastDBE(ImageDB) ].SQLColumn);
		safestrcat(SearchString, TempString, MAXLENGTH);
		}

        if (ImageQuerySortOrder[0]) 
		{
		if(CCIndex)
			strcat(ColumnString, ", ");
		strcat(ColumnString, ImageQuerySortOrder);
		}

	if (SQLResultPatient.GetSize())
	  sprintf(Tables, "%s, %s, %s, %s",
	  	ImageTableName, SeriesTableName, StudyTableName, PatientTableName);
	else if (SQLResultStudy.GetSize())
	  sprintf(Tables, "%s, %s, %s",
	  	ImageTableName, SeriesTableName, StudyTableName);
	else if (SQLResultSeries.GetSize())
	  sprintf(Tables, "%s, %s",
	  	ImageTableName, SeriesTableName);
        else
	  sprintf(Tables, "%s",
	  	ImageTableName);

	if (SendObjectFile)
		{
		strcat(ColumnString, ",");
		strcat(ColumnString, ImageTableName);
		strcat(ColumnString, ".ObjectFile");
		strcat(ColumnString, ",");
		strcat(ColumnString, ImageTableName);
		strcat(ColumnString, ".DeviceName");
		}

	SystemDebug.printf("Issue Query on Columns: %s\n", ColumnString);
	SystemDebug.printf("Values: %.1000s\n", SearchString);
	SystemDebug.printf("Tables: %.1000s\n", Tables);

	while(SQLResultImage.GetSize())
		{
		delete SQLResultImage.Get(0);
		SQLResultImage.RemoveAt(0);
		}
	while(SQLResultSeries.GetSize())
		{
		delete SQLResultSeries.Get(0);
		SQLResultSeries.RemoveAt(0);
		}
	while(SQLResultStudy.GetSize())
		{
		delete SQLResultStudy.Get(0);
		SQLResultStudy.RemoveAt(0);
		}
	while(SQLResultPatient.GetSize())
		{
		delete SQLResultPatient.Get(0);
		SQLResultPatient.RemoveAt(0);
		}

	// Issue Query
        if (ImageQuerySortOrder[0]) Sort = ImageQuerySortOrder;

	if(strlen(SearchString))
		{
		if (!DB.QueryDistinct ( Tables, ColumnString, SearchString, Sort) )
			{
			DB.PrintLastError();
			free(TempString);
        	  	free(SearchString);
			return ( FALSE );	// failed query
			}
		}
	else
		if (!DB.QueryDistinct ( Tables, ColumnString, NULL, Sort))
			{
			DB.PrintLastError();
			free(TempString);
        	  	free(SearchString);
			return ( FALSE );	// failed query
			}

	Index = 0;
	while ( Index < CCIndex )
		{
		DBR = new char[255];
		DBR[0] = 0;			// in case a field is NULL it does not read
		SQLResult.Add(DBR);
		DBL = new SQLLEN;
		SQLResultLength.Add(DBL);
		if(!DB.BindField (Index+1, SQL_C_CHAR,
			SQLResult.Get(Index), 255,
			SQLResultLength.Get(Index)))
			{
			SystemDebug.printf("Column Number : %d\n", Index+1);
			DB.PrintLastError();
			while(SQLResult.GetSize())
				{
				delete SQLResult.Get(0);
				SQLResult.RemoveAt(0);
				}
			while(SQLResultLength.GetSize())
				{
				delete SQLResultLength.Get(0);
				SQLResultLength.RemoveAt(0);
				}
			free(TempString);
        	  	free(SearchString);
			return ( FALSE );	// failed to bind column
			}
		++Index;
		}

	if (SendObjectFile)			// need two more columns
		for (int i=0; i<2; i++)
		{
		DBR = new char[255];
		DBR[0] = 0;			// in case a field is NULL it does not read
		SQLResult.Add(DBR);
		DBL = new SQLLEN;
		SQLResultLength.Add(DBL);
		if(!DB.BindField (Index+1, SQL_C_CHAR,
			SQLResult.Get(Index), 255,
			SQLResultLength.Get(Index)))
			{
			SystemDebug.printf("Column Number : %d\n", Index+1);
			DB.PrintLastError();
			while(SQLResult.GetSize())
				{
				delete SQLResult.Get(0);
				SQLResult.RemoveAt(0);
				}
			while(SQLResultLength.GetSize())
				{
				delete SQLResultLength.Get(0);
				SQLResultLength.RemoveAt(0);
				}
			free(TempString);
        	  	free(SearchString);
			return ( FALSE );	// failed to bind column
			}
		++Index;
		}

	while (DB.NextRecord())
		{
		RDDO = new DICOMDataObject;
		Index = 0;
		while ( Index < CCIndex )
			{
			VRLength = SQLRealSize(SQLResult.Get(Index),
							*SQLResultLength.Get(Index));
			vr = ConstructVRFromSQL (
				DBQMaster.Get(Index),
				EMask.Get(Index)->Group,
				EMask.Get(Index)->Element,
				VRLength,
				SQLResult.Get(Index));
			RDDO->Push(vr);
			++Index;
			}

		if ( SendObjectFile )	// filename in 0x9999,0x0800; devicename in 0x9999,0x801
			{
			VRLength = SQLRealSize(SQLResult.Get(Index),
							*SQLResultLength.Get(Index));
			vr = ConstructVRFromSQL (
				NULL,
				0x9999,
				0x800,
				VRLength,
				SQLResult.Get(Index));
			RDDO->Push(vr);
			++Index;

			VRLength = SQLRealSize(SQLResult.Get(Index),
							*SQLResultLength.Get(Index));
			vr = ConstructVRFromSQL (
				NULL,
				0x9999,
				0x801,
				VRLength,
				SQLResult.Get(Index));
			RDDO->Push(vr);
			++Index;
			}

		if(SendAE)
			RDDO->Push(ConstructAE());

		ADDO->Add(RDDO);
		}

	SystemDebug.printf("Records = %d\n", ADDO->GetSize());

	while(SQLResult.GetSize())
		{
		delete SQLResult.Get(0);
		SQLResult.RemoveAt(0);
		}
	while(SQLResultLength.GetSize())
		{
		delete SQLResultLength.Get(0);
		SQLResultLength.RemoveAt(0);
		}

	/* LJ: Leak!! */
	while(EMaskImage.GetSize())
		{
		delete EMaskImage.Get(0);
		EMaskImage.RemoveAt(0);
		}
	while(EMaskSeries.GetSize())
		{
		delete EMaskSeries.Get(0);
		EMaskSeries.RemoveAt(0);
		}
	while(EMaskStudy.GetSize())
		{
		delete EMaskStudy.Get(0);
		EMaskStudy.RemoveAt(0);
		}
	while(EMaskPatient.GetSize())
		{
		delete EMaskPatient.Get(0);
		EMaskPatient.RemoveAt(0);
		}

	free(TempString);
        free(SearchString);
	return ( TRUE );
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Modality worklist query (out of database)
///////////////////////////////////////////////////////////////////////////////////////////////////

// unfold a query DDO with sequences in it to a flat query
// DDO: 	input DICOM sequence
// DB:  	database definition
// EMask: 	output array of VR's found in sequence and in DB
// SQLResult:	field='value' string to build query of VR's found in sequence and in DB
// DBQ:		points to field definition of VR's found in sequence and in DB
// Levels:	nesting in sequences of VR's found in sequence and in DB
// level:	nesting of this sequence

static void ProcessQuery(DICOMDataObject *DDO, DBENTRY *DB, Array <VR *> *EMask, Array <char *> *SQLResult, 
			Array <DBENTRY *> *DBQ, Array <int> *Levels, int level)
	{
	VR *vr;
	DBENTRY	*TempDBEPtr;
	char *SQLResultString;

	while (vr = DDO->Pop())
		{
		if(vr->Element == 0x0000)
			{
			delete	vr;
			continue;	// discard length codes
			}

		if (vr->SQObjectArray)
			{
			Array < DICOMDataObject	* > *aSQArray;
			aSQArray = (Array < DICOMDataObject * > *) vr->SQObjectArray;
			if ( aSQArray->GetSize()<=1 )
				{
				char *c = NULL; SQLResult->Add ( c );			// NULL, vr, level, NULL: deeper sequence
				EMask->Add ( vr );
				int l = level; Levels->Add ( l );
				DBENTRY *d=NULL; DBQ->Add ( d );	

				ProcessQuery(aSQArray->Get(0), DB, EMask, SQLResult, DBQ, Levels, level+1);
				continue;		
				}
			continue;		
			}				

		if(!VerifyIsInDBE(vr, DB, TempDBEPtr))
			{
			SystemDebug.printf("Failed on VR Search: %4.4x %4.4x\n",
				vr->Group, vr->Element);
			SystemDebug.printf("(testing phase) - ignored\n");
			delete	vr;
			continue;
			}
		else
			{
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);

			SQLResult->Add ( SQLResultString );				// field=value, vr, level, DBE: vr
			EMask->Add ( vr );
			Levels->Add ( level );
			DBQ->Add ( TempDBEPtr );	
			}

		}
	}

// Build a N=1 sequence out of a flat data query into vr as from emask - taking all items at the same level as sequence
// EMask: 		input array of VR's found in sequence in query and in DB
// SQLResult:		input array of read value strings (NULL is sequence starter)
// SQLResultLength: 	input array of read value lenghts
// DBQMaster:		input array of field definitions
// Levels:		input array of nesting level in sequences, sequence stops when decreases, nests when increases
// DDO: 		DICOM object to append sequence too
// Index:		Index in array of sequence fields: array[Index]: vr of sequence, array[Index+N]: vr's of sequence contents

static void CodeSequence(Array < VR * >*EMask, Array <char *> *SQLResult, Array <SQLLEN *> *SQLResultLength, Array <DBENTRY *> *DBQMaster, 
			Array <int> *Levels, DICOMDataObject *DDO, UINT *Index)
	{
	UINT32	VRLength;
	VR 	*vr, *vrs;
	int	level, ind = *Index;

	level = Levels->Get(*Index);

	SystemDebug.printf("Coding sequence (%04x,%04x), level=%d\n", EMask->Get(*Index)->Group, EMask->Get(*Index)->Element, level);

	if (*Index+1 >= Levels->GetSize() || Levels->Get(*Index+1) <= level) 
		{ 
		SystemDebug.printf("(sequence is empty)\n");
		vrs = new VR(EMask->Get(*Index)->Group, EMask->Get(*Index)->Element, 0, NULL, FALSE);
		DDO->Push(vrs);
		(*Index)++;
		return;	// allow empty sequence
		}

	Array < DICOMDataObject * > *SQE = new Array < DICOMDataObject * >;
	vrs = new VR(EMask->Get(*Index)->Group, EMask->Get(*Index)->Element, 0, NULL, FALSE);
	vrs->SQObjectArray = (void*) SQE;

	DICOMDataObject *D = new DICOMDataObject;

	(*Index)++;

	while ( *Index < EMask->GetSize() )
		{
		if (Levels->Get(*Index)<=level) break;
		if (SQLResult->Get(*Index))
			{
			VRLength = SQLRealSize(SQLResult->Get(*Index), *SQLResultLength->Get(*Index));
			vr = ConstructVRFromSQL (
				DBQMaster->Get(*Index),
				EMask->Get(*Index)->Group,
				EMask->Get(*Index)->Element,
				VRLength,
				SQLResult->Get(*Index));
			DumpVR(vr, NULL, level-1);
			D->Push(vr);
			(*Index)++;
			}
		else
			CodeSequence(EMask, SQLResult, SQLResultLength, DBQMaster, Levels, D, Index);
		}

	SQE->Add(D);
	DDO->Push(vrs);

//	SystemDebug.printf("end coding sequence (%04x,%04x), level=%d\n", EMask->Get(ind)->Group, EMask->Get(ind)->Element, level);
	}

BOOL	QueryOnModalityWorkList (
	DICOMDataObject *DDO,
	Array < DICOMDataObject *> *ADDO)
	{
	UINT				Index, CIndex, CCIndex, FIndex;
	UINT32				VRLength;
	DICOMDataObject			*RDDO;
	Database			DB;
	char				*SQLResultString;
	Array < char * >		SQLResult, SQLResultWorkList;
	Array < VR * >			EMaskWorkList, EMask;
	Array < SQLLEN *>		SQLResultLength;
	Array < DBENTRY * >		DBQWorkList;
	Array < DBENTRY * >		DBQMaster;
	Array < int >			Levels;
	DBENTRY				*TempDBEPtr;
	char				*DBR;
	SQLLEN				*DBL;
	VR				*vr, *vrs;
	VR				VRPatientName;
	DBENTRY				*DBEntryPatientName;
	char				SortOrder[128];
	char				*Sorting;
	BOOL				DoSort;
	BOOL				SendAE = FALSE;
	BOOL				SendScheduledProcedureStep = FALSE;
	char				TempString [ 8192 ];
	char				SearchString [ 8192 ];
	char				ColumnString [ 4096 ];
	char				Tables [ 256 ];
        char				*Sort=NULL;

	SystemDebug.printf("Query On Modality WorkList\n");
	// First.  Check that all the asked-for elements are actually in the database.  
	DoSort = FALSE;
	while (vr = DDO->Pop())
		{
		if(vr->Element == 0x0000)
			{
			delete	vr;
			continue;	// discard length codes
			}
		if(vr->Group == 0x0008)
			if(vr->Element == 0x0052)
				{
				delete	vr;
				continue;	// discard model level
				}
		if(vr->Group == 0x0008)
			if(vr->Element == 0x0054)
				{
				SendAE = TRUE;
				delete vr;
				continue;	// discard it (but send it)
				}

		if (vr->SQObjectArray)
			{
			Array < DICOMDataObject	* > *aSQArray;
			aSQArray = (Array < DICOMDataObject * > *) vr->SQObjectArray;
			if ( aSQArray->GetSize()<=1 )
				{
				char *c=NULL; SQLResultWorkList.Add ( c );	// NULL, vr, 1, NULL: code sequence
				EMaskWorkList.Add ( vr );
				int l=1; Levels.Add ( l );
				DBENTRY *d=NULL; DBQWorkList.Add ( d );	

				ProcessQuery(aSQArray->Get(0), WorkListDB, &EMaskWorkList, &SQLResultWorkList, &DBQWorkList, &Levels, 2);
				continue;		
				}
			delete vr;
			continue;		
			}				

		if(!VerifyIsInDBE(vr, WorkListDB, TempDBEPtr))
			{
			SystemDebug.printf("Failed on VR Search: %4.4x %4.4x\n",
				vr->Group, vr->Element);
			SystemDebug.printf("(testing phase) - ignored\n");
			delete	vr;
			continue;
			}
		else
			{
			if(vr->Group == 0x0010)
				if(vr->Element == 0x0010)
					DoSort = TRUE;
			SQLResultString = SetString(vr, NULL, 0);
			DICOM2SQLQuery(SQLResultString);

			SQLResultWorkList.Add ( SQLResultString );		// field=value, vr, 1, DBE: code VR
			EMaskWorkList.Add ( vr );
			int l=1; Levels.Add ( l );
			DBQWorkList.Add ( TempDBEPtr );	
			}
		}

//	Index = 0;
//	while ( Index < Levels.GetSize() )
//		{
//		SystemDebug.printf("VR %4.4x %4.4x, level %d\n", EMaskWorkList.Get(Index)->Group, EMaskWorkList.Get(Index)->Element, Levels.Get(Index));
//		Index++;
//		}

	if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
		{
		DB.PrintLastError();
		return ( FALSE );	// failed open
		}

	// Prepare the query string.
	// from EMasks, and SQLResults

	SearchString[0] = '\0';
	ColumnString[0] = '\0';

	Index = 0;CIndex = 0;CCIndex = 0;
	while ( Index < SQLResultWorkList.GetSize() )
		{
		SQLResultString = SQLResultWorkList.Get(Index);
		if (SQLResultString)						// flat query - but sequence starters (NULL) are not queried
			{
			if(BuildSearchString(&DB, WorkListDB, WorkListTableName, EMaskWorkList.Get(Index),
				SQLResultString, TempString, sizeof(TempString)-512))
				{
				if(CIndex++)
					safestrcat(SearchString, " and ", sizeof(SearchString));
				safestrcat(SearchString, TempString, sizeof(SearchString));
				}
			BuildColumnString(WorkListDB, WorkListTableName, EMaskWorkList.Get(Index), TempString);
			if(ColumnString[0])
				strcat(ColumnString, ", ");
			strcat(ColumnString, TempString);
			}
		EMask.Add(EMaskWorkList.Get(Index));
		DBQMaster.Add(DBQWorkList.Get(Index));
		++Index;++CCIndex;
		}

        sprintf(Tables, "%s",
	  	WorkListTableName);
        
	SystemDebug.printf("Issue Query on Columns: %s\n", ColumnString);
	SystemDebug.printf("Values: %.1000s\n", SearchString);
	SystemDebug.printf("Tables: %.1000s\n", Tables);

	// Issue Query

	// (optionally) control sorting
	VRPatientName.Group = 0x0010;
	VRPatientName.Element = 0x0010;
	DBEntryPatientName = FindDBE(&VRPatientName);
	if(DBEntryPatientName)
		{
		sprintf(SortOrder, "%s.%s",
			WorkListTableName,
			DBEntryPatientName->SQLColumn);
		if(DoSort)
			Sorting = SortOrder;
		else
			Sorting = NULL;
		}
	else
		Sorting = NULL;

        Sort = Sorting;

	SystemDebug.printf("Sorting (%s) DoSort := %d\n", Sort, DoSort);

	if(strlen(SearchString))
		{
		if (!DB.QueryDistinct ( Tables, ColumnString, SearchString, Sort) )
			{
			DB.PrintLastError();
			return ( FALSE );	// failed query
			}
		}
	else
		if (!DB.QueryDistinct ( Tables, ColumnString, NULL, Sort))
			{
			DB.PrintLastError();
			return ( FALSE );	// failed query
			}

	// Allocate memory for and bind all fields that will actually be read (no sequence starters)
	Index = 0;
	FIndex = 1;
	while ( Index < CCIndex )
		{
		SQLResultString = SQLResultWorkList.Get(Index);
		if (SQLResultString)			// Sequence starters have a NULL here
			{
			DBR = new char[255];
			DBR[0] = 0;			// Initialize memory: in case a field is NULL it does not read
			SQLResult.Add(DBR);
			DBL = new SQLLEN;
			SQLResultLength.Add(DBL);
			if(!DB.BindField (FIndex, SQL_C_CHAR,
				SQLResult.Get(Index), 255,
				SQLResultLength.Get(Index)))
				{
				SystemDebug.printf("Column Number : %d\n", Index+1);
				DB.PrintLastError();
				while(SQLResult.GetSize())
					{
					delete SQLResult.Get(0);
					SQLResult.RemoveAt(0);
					}
				while(SQLResultLength.GetSize())
					{
					delete SQLResultLength.Get(0);
					SQLResultLength.RemoveAt(0);
					}
				while(SQLResultWorkList.GetSize())
					{
					delete SQLResultWorkList.Get(0);
					SQLResultWorkList.RemoveAt(0);
					}
				return ( FALSE );	// failed to bind column
				}
			FIndex++;
			}
		else
			{
			DBR = NULL;
			SQLResult.Add(DBR);
			DBL = NULL;
			SQLResultLength.Add(DBL);
			}
		++Index;
		}

	// Create dicom object out of each database record
	while (DB.NextRecord())
		{
		RDDO = new DICOMDataObject;
		Index = 0;
		while ( Index < CCIndex )
			{
			SQLResultString = SQLResultWorkList.Get(Index);
			if (!SQLResultString)		// sequence starter has a NULL here: build a sequence (with 1 item)
				CodeSequence(&EMask, &SQLResult, &SQLResultLength, &DBQMaster, &Levels, RDDO, &Index);

			else				// build a vr
				{
				VRLength = SQLRealSize(SQLResult.Get(Index), *SQLResultLength.Get(Index));
				vr = ConstructVRFromSQL (
					DBQMaster.Get(Index),
					EMask.Get(Index)->Group,
					EMask.Get(Index)->Element,
					VRLength,
					SQLResult.Get(Index));
	
				DumpVR(vr);
				RDDO->Push(vr);
				++Index;
				}
			}

		vr = RDDO->GetVR(0x0008, 0x1110);
		if(!vr)
			{
			VR *vrNew = new VR(0x0008, 0x1110, 0, NULL, FALSE);
			RDDO->Push(vrNew);
			}

		vr = RDDO->GetVR(0x0008, 0x1120);
		if(!vr)
			{
			VR *vrNew = new VR(0x0008, 0x1120, 0, NULL, FALSE);
			RDDO->Push(vrNew);
			}

		if (WorkListReturnsISO_IR_100)
			{
			vr = RDDO->GetVR(0x0008, 0x0005);
			if(!vr)
				{
				VR *vrNew = new VR(0x0008, 0x0005, 10, TRUE);
				strcpy((char*)vrNew->Data, "ISO IR_100");
				RDDO->Push(vrNew);
				}
			}

		if(SendAE)
			RDDO->Push(ConstructAE());
		ADDO->Add(RDDO);
		}

	SystemDebug.printf("Records = %d\n", ADDO->GetSize());

	while(SQLResultWorkList.GetSize())
		{
		delete SQLResultWorkList.Get(0);
		SQLResultWorkList.RemoveAt(0);
		}
	while(SQLResult.GetSize())
		{
		delete SQLResult.Get(0);
		SQLResult.RemoveAt(0);
		}
	while(SQLResultLength.GetSize())
		{
		delete SQLResultLength.Get(0);
		SQLResultLength.RemoveAt(0);
		}
	while(EMaskWorkList.GetSize())
		{
		delete EMaskWorkList.Get(0);
		EMaskWorkList.RemoveAt(0);
		}
	while(Levels.GetSize())
		{
		Levels.RemoveAt(0);
		}

	return ( TRUE );
	}

