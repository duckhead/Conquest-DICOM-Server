/*
MvH 19980327: Disable logfile for speed and save disk
MvH 19980327: -! Enables logging to Personal pacs console; RunServer works; -p breaks
MvH 19980327: All printf's to systemdebug
MvH 19980320: Integrated dumppacs code here for convenience
MvH 19980320: Fixed -p option; SaveToDisk returns filename, printed in stats
MvH 19980320: Documented and added print of usage
MvH 19980404: Added test options, SQL config option and command line specified console pipe/udp
MvH 19980409: added FileCompressMode option; use local save to disk code instead of pdu member
MvH+LJZ 19980409: Return on failed GenerateFileName in SaveToDisk
MvH 19980409: Cleaned up nki private compression code and allow >15 bit differences
MvH 19980410: Added run length encoding for equal pixels to compressor; fix 3-byte case
MvH 19980415: Cleaned up the messages; -! does not enable debug; do not enable accessupdates (use aroute)
MvH 19980618: Some work on manual and database layout listing
MvH 19980621: Added -z option: zap patient
MvH 19980624: Comm test also produces systemdebug and long text output; added \n to some output
MvH 19980625: SaveDicomDataObject in loadddo.cpp supports NKI compression
MvH 19980704: Added some test code for counting disk space and renaming device for patient
MvH 19980705: Added CACHE devices in free store check; added FindPhysicalDevice (searches for file)
MvH 19980709: Added archiving options: -as, -au, -ab, -ac, -ad
MvH 19981217: Temporary use PDU.Save... code for testing DICOM objects (does not compress)
MvH 19981218: Temp use Pdu.Load as well (no compression)
MvH 19981228: Disabled ljz's code until it has been completed
ljz 19990108: Reenabled ljz's code: Replace uncompressed by compressed pixeldata VR in
              SaveToDisk (it seemed to work anyway...)
MvH 19990109: Removed some commented out code; added comments for alternative load and save routines
	      Added Regen of specified device
MvH 19990110: Added n, for MAGn, as parameter in -as and -ab archiving options (default all)
	      exit(1) on failure
MvH 19990111: Fixed exit code on -ar option
MvH 19990117: If the filename is *.dcm (depends on FileNameSyntax) use uncompressed chapter 10 format
ljz 19990317: Added NKI-private stuff
MvH 19990628: Temporary enabled debug in -!nnnn option
MvH 19990630: Added MIRROR option; disabled debug in -!nnnn option
MvH 19990707: Fixed MIRROR option; added MIRROR message; added check on FindPhysicalDevice result
ljz 19990817: Added ability to spawn ExportConverters at the end of SaveToDisk
MvH 19990827: Optimized read of slices: cache a DB to pass to GetFileName (10% faster)
ljz 19991117: Added parameter FileCompressMode to in nki_private_compress call
mvh 20000318: Added private command object 9999, 0300 = print to console
              Added private command object 9999, 0400 = force C-Echo to be 'silent' (no log)
              Display C-Move Destination; print time info about threads
ljz 20000323: Serious fix: MainThread 'hangs' when several associations occur simultaneously, caused
	      by compiler-optimisation. Solution: DriverApp.Lock should be 'volatile'.
mvh 20000329: On request by ljz: designed crash of server if silent VR receives 'DEAD'
ljz 20000501: Compare dicom modality with UPPER-case ini-file setting (ExportConverters)
ljz 20000221: Initialize TroubleLogFile and UserLogFile; log C-STORE and C-MOVE
ljz 20000703: Logging of trouble now starts with '***'
mvh 20000703: Exclude archival messages from trouble log; removed force dcm code
mvh 20001104: Added WINAPI to thread routines
mvh 20001105: Replaced malloc by new where a mix could occur (vr->data)
              Made compilable for MSC and BC and WC using some #ifdefs
              Changed order of verification and store; better localized new/delete DDO more
mvh 20001106: Run-time load odbcinst routine for BC and WC, use spawn... i/s/o _spawn...           
mvh 20001106: Use delete [] operation for vr->Data
mvh 20010328: Added -av: verify mirror disk
mvh 20010415: Added DeleteSeries, DeleteStudy and DeleteImage routines and -f options
mvh 20010416: Added -f options: -feFile = enter MAGN file into DB, -fdFile = delete file from DB
	      -faFile = add (also makes copy) file into server, -f?=ask UID of file, 
	      -fu = generate UID, -fcNEWPATID,file = modify patient ID of image file (moves file!)
	      NOTE: -fc is irreversible in the sense that if a portion of a study/series is changed
              and changed back it winds up as a different study/series. This occurs because the 
	      UIDs must be changed for each image modification. This problem does not occur when 
	      a whole series or study is changed
mvh 20010417: Simplified code to change uids to allow easy extension to referenced SOPs
              Check after load on common UIDs to avoid accepting dropped non-dicom files
              NOTE: ChangeUIDinDDO should also change sequences and referenced SOPs (0008,1155)
mvh 20010418: Fix in above change
mvh 20010418: Changed names of database fields in UIDMODS - did not work on msaccess
mvh 20010419: Added -fk option: copy image file to other patient ID (as -fc but no delete)
              Added version number 1.3.9 to command line help
mvh 20010429: Added -fz option: zap file from server and database
mvh 20010430: Some work on read ahead thread: note that thread must not use any pointer copy!
mvh 20010502: Made read ahead thread working; fixed thread counter; fix odbc option: strings too short
mvh 20020506: Fixed handle leak in read ahead thread
mvh 20020507: Fixed memory leak in read ahead thread; kbhit() test #ifdef DO_LEAK_DETECTION
mvh 20010509: Added RenameDevice is -ae option
ljz 20010730: Replaced parameter argv[0] of ExportConverters to proper value
mvh 20010829: Added ExportStationName as export filter; default Export... value set to "*"
              Made export comparisons case insensitive; added special 'forward to AE' ExportConverter
              Version to 1.3.10
mvh 20010831: Added extended syntax for exportconverters with %f=filename, %m=modality, 
              %s=stationname, %b=file base name, %p=file path; 
              Also allows 'copy %f destination', but for windows NT only (uses cmd.exe)
              Added ExportFilter option that uses an SQL statement to filter.
mvh 20010901: Added a simple queue to process exportconverters asynchroniously
mvh 20010901: Create a queue for each converter; queue mirror copy requests
mvh 20010902: Added %o = SOP; %n = newline; added the following hard coded converters: 
              append "text" to file; write "text" to file; copy file to file; copy file to dir
ljz 20010904: Re-entered change of 20010730
ljz 20010905: Fix: After 'SaveToDisk', do not free pDDO
mvh 20011109: Fix: made ExportConverter stuff case insensitive
              Note: empty filter tests for empty contents - this is not the same as '*'
              Released 1.3.10
mvh 20011110: Note: in release 1.3.10, the " to " in e.g., write "%f%n" to file must be lowercase
mvh 20011115: Made " to" case insensitive; Added *** before some error messages for better logging
              Removed cache db: would hang the server if SQL has gone down and up.
              Now use one DB per thread which is passed to the read ahead thread = fast too!
ljz 20011207: Added extra errormessage when creation of datasource fails
mvh 20020109: Disabled this code for watcom or borland (instead of run-time loading an address)
mvh 20020314: Implemented ignore of C-CANCEL request for ADAC NM (was error before)
              Blocked out SQLInstallerError call; does not compile on clinical MS4.2
mvh 20020413: Added some notes about thread safety and db's: The serverchild is not thread 
              safe when reading and writing is done on the same association.
mvh 20020415: Added -atDEVICE option: sanity check of images on disk; version to 1.3.11
mvh 20020416: Made UID generator configurable through UIDPrefix in dicom.ini; made time print unsigned
mvh 20020422: Sleazy fix of ModifyPatID: time(null) created same UID: now wait 1 s per changed file
mvh 20020426: Generate filename no longer gives an error: is done at higher level
mvh 20020428: Only serious error messages use *** in message
mvh 20020428: Added FAILSAFE_STORAGE: tries to save incoming files even when DB errors occur
mvh 20020429: Put SaveToDisk in myUnknownStorage: pass DB failures to caller
              Disabled FAILSAFE_STORAGE: any storage failures should now be detected by client
mvh 20020508: Do not rewrite image not on MAG but it is not an error (warning is generated earlier)
mvh 20020520: Added -gSERVER,DATE option: grab images on date from remote server
mvh 20020521: fix displaying junk filename when rewriting image not on MAG 
ljz 20020522: Fix in SetStringVR: strings must be padded with a space
mvh 20020529: -i and -r now have mode parameter (0=normal and default, 1=simple, e.g., for DBASEIII)
ljz 20020531: In 'QualifyOn', support case-sensitive call to 'GetACRNema', but make AE-Title
              uppercase after retrieving IP and port.
ljz 20020613: Better handling of Compression ON/OFF in 'SaveToDisk'
	      Fixed crash in DeleteImageFile (twice free of VR->Data)
mvh 20020613: compress to specified mode, check (de)compress result, decompresses data before 
              forward; added 'export compressed to AE' export; -fa has optional ,NEWPATID syntax; 
mvh 20020802: Simplified rmdir code and allow multiple subdirectories in rmdir and mkdir code
              Version to 1.3.12 (unreleased)
mvh 20020804: Use PATHSEPCHAR where possible
mvh 20020807: Changed SaveToDisk order: only make patientdir when DB save succeeded
mvh 20020812: Added skeleton of printer support
mvh 20020816: Added color imagebox; Save incoming print DDO as file
mvh 20020819: UIDs of imageboxes end on rows.cols.position for easy print handling
mvh 20020819: Fix in delete unexistent mirror files for changepatientUID
              Extract pixel data from sequence in ImageBox; added sleep(10) in -fk option
mvh 20020819: Added better UID generation (but gets close to 64 chars)
mvh 20020821: Fix printer UID generation; display page orientation and film #; shorter info
mvh 20021016: Added (indexed) patid to exportfilter query and getfilename for speed; 
              The option tests for rev5 db or higher; increase 64 strings to 66;
              Note: the GrabImagesFromServer option does not yet pass patid to the query 
	      (i.e., will be slow with non-odbc driver)
mvh 20021017: Control NeedPack from here; added -c option: set UID counter at startup
              Removed Sleep from anonimize options; do not re-create UIDMODS
mvh 20021018: Pass NoKill through GenerateFilename for interactive DGATE
mvh 20021028: Fix query on study in PatientStudyOnlyQuery (did an image query)
mvh 20021105: Revamped read-ahead thread logic to try fix rare crash where ratd->DDOOutPtr became NULL
mvh 20021115: Use Generic style retrieve classes with NKIqrsop code instead of older qrsop code
              Advantages: read ahead thread used, and allows print of error messages from C-MOVE
              Required changes such that ratd->pcdo can be NULL.
mvh 20021121: Disabled read-ahead thread for time being
mvh 20021212: Also loop ServerApp.Server(Port) in threaded mode server
mvh 20021213: Re-entered fix by Hanne Kooy: compressed forwarding had extra ;
mvh 20021213: Re-enabled use of generic query classes with read-ahead thread; 
	      but made read-ahead thread configurable with "EnableReadAheadThread"
              Made grab query faster by including date - note grab is independent of FixPhilips
mvh 20021215: Found read-ahead thread problem reported by Aaron Cann - missing Sleep in wait for slices
mvh 20021223: Version to 1.4.1 pre-release; added in dgatefn forbidden filename chars suggest by Thomas Buerer
mvh 20030113: Removed loop ServerApp.Server(Port): gave endless loop error messages
ljz 20030120: Added cleanup-code when DO_LEAK_DETECTION is defined for easy leak checking
mvh+ljz 20030205: Fix bug found by Frank-Andre Siebert: read ahead thread crashed when ID or SOP is missing
                  Also fix 2 other potential missing VR problems
mvh 20030228: Fixed bug reported by Jeff Bacbcock: series retrieve from E-film crashed (patid not passed)
ljz 20030424: Version to 1.3.12
	      Changed calls to obsolete PDU.Write(&DDO)
	      In 'SaveToDisk' the call to PDU.SaveDICOMDataObject now has parameterDICOM_CHAPTER_10_EXPLICIT
	      (which in writes IMPLICIT files iff not compressed)
ljz 20030606: Fixed incremental counter when creating UIDs or generating filenames
	      Fix: QueryRetrieveLevel is mandatory for a QueryResponse
mvh 20030625: Fix NGET of BasicPrinterSOPClass
ljz 20030625: Fix in above fix
mvh 20030627: Adapted for MSC4.2; Finally really fix NGET of BasicPrinterSOPClass (wrong UID used)
mvh 20030629: Added -nd, -nc, -jd and -jc options: (de)compress NKI; (de)compress JPEG (used OFFIS executables)
mvh 20030630: Started on compression for dropped, incoming, transmitted; NKI vr 0x9999,0x700=custom compression
              Added 'forward compressed as xx to ...'
mvh 20030701: QualifyOn now also has compression parameter
mvh 20030703: Moved compression code to nkiqrsop; dgate -j-xx and -j*xx = test compression options
	      Removed "forward compressed to" because "forward compressed as .. to" is better.
	      Note that FileCompressMode is now obsolete. KNOWN BUG: JPEG compressed images may be 
	      rewritten as V2 (when written originally as NKI) which are then not correctly read.
mvh 20030704: Made that FileCompressMode parameter still works even though is obsolete
              Moved faulty delete DDO after SOPUnknownStorage.Read
mvh 20030705: Added check of Tranfer Syntax next to extension for save type; ProcessDDO takes **DDO
mvh 20030706: Use dgate.dic; Attach VRType to PDU's for implicit little endian support; small fix in dump code
mvh 20030709: Printer support fix; version to 1.4.2; added SendUpperCaseAE parameter (default 0)
ljz 20030730: Force ImplicitLittleEndian, ExplicitLittleEndian and ExplicitBigEndian objects that must
              be stored in Chapter10 format, to ImplicitLittleEndian.
mvh 20030815: Version to 1.4.3
mvh 20030905: Allow longer filenames
mvh 20030910: Added check on rights to write (trouble) log file and printer_files in MAG0
mvh 20030921: Allow write to any MIRROR if MAG is full; added -ff# option: delete until # MB free
mvh 20030922: Added StorageFailedErrorCode (default 0x110 = 272 decimal = failed processing)
              Added ExportCallingAE and ExportCalledAE export filters (also %u=SCU, %c=calledAE in converter)
mvh 20040401: Version to 1.4.4; pass status pointer to compress routines
mvh 20040402: Delete DDOptr when SaveToDisk cannot write but does not give an error
              Added "Cannot rewrite jpeg/rle image in v2 format" check
	      Use system DSN for ODBC (easier login)
mvh 20040406: Always use EXPLICIT syntax for jpeg, fix v2 always use DUMP
mvh 20040426: Version to 1.4.4a
mvh 20040520: Version to 1.4.4b
mvh 20040523: Added patient ID to speed GetFileName for single slice move
ljz 20040602: Fix: 'forward compressed as' always failed
mvh 20040528: Set NeedPack (controls DbaseIII indexing) for regen, archive, maintenance; Version to 1.4.4c
mvh 20040528: Added built-in maintenance commands through silent VR in ECHO; pass patid in DeleteImageFile
mvh 20040530: extract (DBF only) to database like XDicomImages in same directory; Version to 1.4.5
mvh 20040601: Added deletestudy, deleteseries and packdbf maintenance commands
mvh 20040605: packdbf maintenance command sets NeedPack to 3: forces pack
mvh 20040606: fix rewrite to not MAG (JUKEBOX) was NOT ignored
mvh 20040607: Merged fix ljz 20040602; do not allow rename and delete with empty name
mvh 20040610: Added maintenance server tasks for archival and regeneration
mvh 20040612: Added indexdbf maintenance server task
mvh 20040614: dgate command line maintenance options use NeedPack=5: non-threaded index generation
	      Added -am option: move (all) device data; to move selected data use -amMAG0.Archiving,MAG1
mvh 20040624: Version to 1.4.5a
mvh 20040626: Pass study and series UID to getfilename; version to 1.4.5b; Fixed leak of pats array
mvh 20040713: Version to 1.4.6
mvh 20040805: Fix in C-MOVE for single file (fix from ljz); version to 1.4.6b
ljz 20040909: Increased length of ExportFilter (was 64, now 510)
mvh 20041020: Added mergeseries(file) and mergestudy(file) maintenance commands, silenttext to 1024
mvh 20041101: Added deletesopfromdb maintenance command; version to 1.4.7
mvh 20041103: called, e.g., AE~nj overrides IncomingCompression to nj
mvh 20041108: Added $c test compression mode (alt-drop files with $c as patient ID tests compression modes)
mvh 20041109: Added $tSERVER test compressed forward; some fixes in forward compressed to (server name read wrong)
              $s tests all filename syntaxes
mvh 20041112: Added $rN N times repeated entry test, and $tN which enters image N time each on its own thread
mvh 20041128: Added -^file: log to file (timestamped) instead of to server console; -#file: same with debug
mvh 20041129: Added silent commands debuglog_on and log_on to set logging to UDP or file
	      Added silent command read_amap to re-read acrnema.map without restarting the server
mvh 20050102: Small modifications to allow compile through total.cxx
mvh 20050107: Added code to hold and retry queued forwards and mirror copies when failed:
              After failed export/copy, start storing entries in file ExportFailuresN (CopyFailures)
	      if a new export/copy is requested >1 minute after last failed one, try again
              if queue is empty, >100 seconds after last fail, try export/copy stored ones
              processing queued entries has priority over processing stored entries
mvh 20050107: Version to 1.4.8; modifications for linux compile
mvh 20050108: Added server PORT# to fail files, so that multiple servers on different ports do not share files
mvh 20050109: Added configurable TCPIPTimeOut, FailHoldOff, RetryDelay and QueueSize
mvh 20050109: Adapted thread code for linux; still need to implement WaitForSingleObject
mvh 20050111: Added ctype.h (for mingw)
mvh 20050118: Linux: crudely implemented WaitForSingleObject; detach pthreads to avoid leaks; 
              crudely implemented export converter apps
mvh 20050119: Added grabimagesfromserver, regendir and makespace server commands
	      dgate -- sends server commands to other running server; adapted manual
mvh 20050121: Small updates to manual
mvh 20050124: Replaced BackGroundExec with simpler system() for Linux; added query server command, e.g.,
	      dgate "--query:DICOMpatients|patientid,patientname,patientsex|patientsex='F'|%s %s %s|t.t"
              Added read_ini: server command to reread dicom.ini; changes most config except:
              TCPPort (busy), UseBuiltInDecompressor (cached) and FileNameSyntax (cached)
mvh 20050130: added get_param, get_freestore, get_amap, get_sqldef that respond through echo-response
	      also: get_sop, get_transfer, get_application, get_localae, get_remoteae, get_dic, forward
	      added: dump_header (to file), convert_to_gif (to file); adapted query (to file) 
	      where file="cgi" for stdout in cgi/mime mode, file="" to send to stdout as is
	      most GUI commands can now pass PatientID|SOPInstanceUID instead of filename of dicom object
              added put_amap, write_amap, put_sop, put_transfer, put_application, put_localae, put_remoteae, 
	      put_param. Note: get/put_param work through disk (read_ini activates), get/put_amap through 
              memory (write_amap writes to disk), get/put_sop etc read from memory and write to mem and disk.
mvh 20050204: Added delete_param, delete_amap and fix cgi output (double \n)
mvh 20050204  most GUI commands can now pass PatientID|?*:SOPInstanceUID; added delete sop etc commands
	      More fixes in cgi data passing VR vr() is mostly wrong; use VR *vr=new VR()
mvh 20050205: Allow up to 16 output fields in query:, repeat queried fields over all 16 outputs
	      Added counters for server status; start on display_status server command
mvh 20050206: More work at display_status, renamed testtable to xtesttable: default not opened
	      Access ObjectFile and DeviceName through query instead of with GetFileName up to 3 x faster!!
	      Also fixes potential leak in read ahead thread: pats, series and studies not always freed
	      Allow up to 32 cgi query outputs, tidied dump layout, start on web interface (put dgate and ini in cgi-bin)
mvh 20050207: Added query2 option: limits number of output rows as asked; gui command not counted as association
              Some more work for web interface
mvh 20050210: Adapted web interface queries such that they run on sql server
mvh 20050212: Removed need for thread local storage; automatically adapt dgate extension in cgi scripts depending in OS
mvh 20050213: Added more parameters to server configuration web page
mvh 20050214: Added update button to server config (updateconfig page to be implemented)
ljz 20050215: Fix in LoadForGUI: Drag 'n' drop of files in the server did not work anymore
              Fixed dgate crash in case of failure in 'recompress()' (pDDO=NULL when OFFIS failes)
	      mvh merged
mvh 20050327: Added rudimentary gz unzip support for dropped files (uses 7za.exe)
mvh 20050401: Start on Modality WorkList Support
mvh 20050404: List End/Start sequence db types (only supported for worklist)
mvh 20050411: Fix incorrect space after filename and device in C-MOVE internal query
mvh 20050414: Added addrecord server command: for now only option to enter worklist entries
mvh 20050603: Fix servercommand query(2) for empty where part
mvh 20050825: Fix web interface queries for sql server (were OK for dbase)
mvh 20050826: More work on web interface and added worklist there; deleterecord command
mvh 20050827: Fixes in addrecord and query commands, editworklist,saveworklist,deleteworklist
mvh 20050829: More fixes in web interface, disable caching for most/all? of it
mvh 20050831: Allow command line length of up to 4096, allow edit all worklist database fields
mvh 20050901: Removed invalid cache-control: cache; added WorkListMode:0= off, 1=optional, 2=compulsory
mvh 20050902: Read db config for worklist edit page in web interface; use generic webpages for db edit
	      Added HL7Tag in DumpDD
mvh 20050903: Added 'simple' loadhl7: server task implementation and HL7->DICOM translation
              Also addimagefile will enter HL7 files into the server, i.e., drag and drop works
              Fix some web queries for sql server with denormalized tables
mvh 20050904: Some more tweaks to hl7 code; max query formats to 48; max format to 8192
mvh 20050905: Auto split HL7 subfields and repeats without using a list
mvh 20050907: Merged change by Hans-Peter Hellemann: fixes extension problem for linux web 
              interface for some browsers
mvh 20050908: Protect convert_to_gif when file not found
mvh 20051121: Recompute SQLResultLength because it does not work in PostGres BindField
mvh 20051123: put in #ifdef POSTGRES
mvh 20051208: put in #if 0 (fixed in odbci.cpp); start on virtual server: will grab images from 
	      other server as needed in read-ahead thread: when in db but not on disk; for now 
	      use rat also for single image access to avoid code duplication
mvh 20051209: Start on virtual query: image (!) query is passed to other server, results populate db
mvh 20051210: Added "vX" incomingcompression (stores dbf but not image; forwarding disabled)
              Added virtual queries on all queries and moves: now works as transparant file cache
	      Added VirtualServerFor0..9 configuration entries: server merges own data with these AE's
	      Added CacheVirtualData control; if set data not deleted after use (db kept for now)
mvh 20051211: Fix: override query level in virtual query
mvh 20051212: Store virtual server # in device; saved on query, used when retrieving, stripped when storing
mvh 20051214: Use different sleep lenghts under linux - not all sleep(0); small fix in virtual server
mvh 20051215: Implemented a linux version of Sleep using nanosleep
mvh 20051217: Use new VR::ReAlloc method to replace data in VR; added quit: command
mvh 20051218: Use ReplaceVR instead of Push in anonymize etc; also changes in sequences
              Version to 1.4.10; show version and exe date in startup log
              Do not start mirror copy queue unless it is needed; show some thread startup info
              Added -b = NOTHREAD: run server as single process, useful for debugging and profiling
              Fixed doc of --regendir: command
mvh 20051219: Fixed several small leaks - as tested with Linux malloc_stats
mvh 20051222: Started on bypass server for virtual query - not functional yet (cleanup required)
              Removed debugVR logic: errors should now list DCO
mvh 20051228: Fix virtual device logic for VirtualServerFor0; 
              Cleanup vr's and duplicates when bypassing server for virtual query, seems functional
mvh 20051229: NonDestructiveDumpDICOMObject now also list sequences; 
	      DebugLevel>0 lists DCO, >1 lists query DDO; fixed leaks in virtual query and some others
mvh 20051230: Added --debuglevel: command, small fix in NonDestructiveDumpDICOMObject
mvh 20060103: Small fix in virtual query; disabled virtual query for worklist; debuglevel 3 shows some query results
mvh 20060123: Added patient/study/seriesfinder (server parameter ignored for now, later for virtual servers etc)
mvh 20060125: Added serieslister (server parameter ignored for now, later for virtual servers etc)
mvh 20060128: Renamed to imagelister, added serieslister
mvh 20060131: Sort filenames from imagelister on ImageNumber (0020,0013)
mvh 20060226: Version to 1.4.11
mvh 20060228: Small fix in isxdigit thanks to ljz
mvh 20060311: Use ReplaceVR in virtual query - level was sent twice (thanks David Lang)
	      Added WEBReadOnly flag
mvh 20060317: Support calledae and callingae in filenamesyntax - for virtualquery called is always MYACRNEMA
mvh 20060324: Strip group 2 from outgoing c-stores if not compression "as" or "is"
mvh 20060328: Improved forwarder, always propose Verification and use IsAbstractSyntaxAccepted(iUID) to test if host accepts image
mvh 20060402: Fix hang condition when a file failed to read in the read-ahead thread
mvh 20060402: Do not remove transfer syntax prior to dcmdjpeg/dcmcjpeg (here in test recompress only)
mvh 20060405: Tried fix for multiple moves on same assoc (thanks Andrej Savelov)
mvh 20060405: Added message when IOD fails to read
mvh 20060603: Fix crash when started as dgate -v: empty statbuf
mvh 20060607: Finally fixed multiple moves on same assoc (thanks Andrej Savelov and Fred Röhner)
mvh 20060618: Renamed dfilecopy and define ODBC_ADD_SYS_DSN if needed
mvh 20060619: Set version to 1.4.12alpha1
mvh 20060621: Added clonedb: command; transfer FULL db from other server to this one using virtualquery
mvh 20060628: AddToDatabase has JustAdd parameter: skip one unnecessary query
mvh 20060701: Speeded filter string handling
mvh 20060702: Pass DB to GenerateFilename to avoid zillions of db open and closes
mvh 20060704: Print number of records found in all queries
mvh 20060708: Set version to 1.4.12alpha
mvh 20061126: Moved servertask temporary files to MAG0/printer_files; protected query: and query2 against empty input
mvh 20061126: Experimental forwarding on same association: close unused association after 5 seconds
              Clean collected sopclasses after one hour of inactivity
	      Note 1: there is a problem with AReleaseRQ: do not use in this task
	      Note 2: maximal 20 forwarders have a remaining association
mvh 20061127: Added ForwardAssociationLevel (PATIENT, SERIES, STUDY, IMAGE, SOPCLASS; default STUDY),
	      ForwardAssociationRefreshDelay (default 3600 s), and ForwardAssociationCloseDelay (default 5 s)
              Limit maximum number of ExportConverters to MAXExportConverters=20 (for static arrays used in forwarders)
	      Forwarders keep association open as long as UID at ForwardAssociationLevel does not change.
              Show filename in forward log; document where fixkodak should operate for virtual queries
mvh 20061128: Added ForwardAssociationLevel GLOBAL: use association until timout or incompatible
              Close ForwardAssociation after send when ForwardAssociationLevel is IMAGE
mvh 20061129: VirtualServerForN may be "server,FIXKODAK" to clean queries (add 0) and their response (remove 0)
	      Hardcoded filters for exportconverters (modality, called, calling, station) can now match e.g., RT*
mvh 20061130: Pass series and study in virtualserver outgoing c-move
mvh 20061213: Fixed nasty bug in query2 and TestFilter: Query without bindfield overwrites previous bound strings!
	      Maybe was problem causing crash on linux with query2: and problem with option -g reportedly stopping incorrectly
mvh 20061219: Version to 1.4.12
mvh 20061231: Added ForwardAssociationRelease to allow testing reconnect problem (default 0 = just hangup)
mvh 20070103: FixKodak blocks *xxxx wildcard virtual queries on date (not allowed in Kodak)
mvh 20070103: Version to 1.4.12b; start on ImportConverters e.g., <set 0008,1040 to "%V0008,0080">
mvh 20070104: ImportConverters %QP %QS %QE %QW; <destroy> <delete xxxx,yyyy> <save to filename>
              Added %% and %i to Export and Import converters
mvh 20070105: <set 0008,1040 if "...">, <setifempty 0008,1040 to "...">, <setifempty 0008,1040 if "...">
              Import/export converters only report when syntax OK and report when action is void
	      Added %Vxxxx,yyyy to export converters and %d(date/time) to import and export converters
              An import/export converter can thus be used to extensively log incoming images
              Added import converters: <stop>, <if "string">, <ifempty "string">, <nop> (also export)
              Added general purpose variables x, y, z (read as %x, %y, %z) to import converters
	      Definition of number of ImportConverters now unnecessary.
              Note: importconverter and exportconverter strings may be changed run-time in dicom.ini
mvh 20070105: Some fixes in dgate --archiving options manual; fixed dgate --restoremagflags:
mvh 20070107: Fix web access broken by change in odbci for dbaseIII; query must now always include spaces around =
mvh 20070113: Do not store NKI compressed data in v2 format: decompress and give error message
              Recompress to NKI format now enables StripGroup2; Tested with $c server debug command
mvh 20070117: Allow multiple rules per Export or ImportConverter separated by ; and zero or more spaces
              <nop> statement prints itself on console: useful for logging anything that follows
mvh 20070117: Now maintain multiple connections per ExportConverter line (max 20x20 forwards)
	      Added <stop>, <silentstop>, <if> and <ifempty> to ExportConverters; but they only affect single rule 
	      (while these would cross over to the subsequent rules in ImportConverters)
mvh 20070122: Now also show free space on mirror devices
mvh 20070123: Fixed TestFilter for patientid with '
mvh 20070125: Removed <if> and added <ifnotempty>, <ifequal>, <ifnotequal>, <ifmatch>, <ifnotmatch>, 
	      <ifnumequal>, <ifnumnotequal>, <ifnumgreater>, <ifnumless> as im/exportconverters
              Added %xxx[first,last] substring operator; and protect it against overrange
              <ifxxx> no longer crosses over importconverters; added {} block for im/exportconverters
              E.g.: ifequal "%V0008,0020[0,3]", "2004"; { forward to CONQUESTSRV3; forward to CONQUESTSRV4; } nop test
	      Note: 1) all (also ifxxxx) statements end with ; 2) statements may begin with { or }; 
              3) extra spaces only allowed after ;{} and single space after ,
mvh 20070127: Fix {} in exportconverters.
              Release 1.4.12b to fix more or less critical odbci error
mvh 20070131: Fix in TestFilter (quotes double); Version to 1.4.12c
mvh 20070201: Added <storage MAGn> import converter: sets device to store this (new) image to (default MAG0)
              Fixed importconverters stop and destroy: did not break current line
mvh 20070204: Added extra format starting with # to ImageFileLister: e.g. #%s:%s prints MAG0:filename
mvh 20070206: Added create database: dgate -esapw name user password 
mvh 20070207: Fix query string bug on applyworklist found by capolan1
mvh 20070207: dgate -e name root <blank> works for mysql: root is used as admin and login
              Also implemented dgate -esapw name sa sapw where sapw may be empty
mvh 20070210: Added CheckDuplicates to UpdateOrAddToTable; added @ in imagefilelister: replace \ by /
              Start on ocx based viewer integrated in cgi interface; used dicom over http:
              Small change in gif export; added * as separator in query/2
mvh 20070211: Added WebScriptAddress (default http://server/cgi-bin/dgate), WebMAG0Address (defaults http://server/mag0)
              and WebServerFor (default 127.0.0.1). Fix in cgi query parser: now loads dsize and size correctly
              Version to 1.4.12d
mvh 20070212: Added WebCodeBase, show version in status_display, added config of which dicom server the web server
	      connects to, added [webdefaults] section, added iconsize parameter
mvh 20070213: Added : as format in imagefilelister: use to list pat:sopuid
              Fixed sorting problem by using adapted imagefilelister for seriesviewer
	      Fixed default for WebCodebase; Version to 1.4.12e; 
	      Use correct case of sql table names in cgi interface: required by mysql on linux
	      Unsolved: n3 does not always decompress correctly in dview.pas see p20401977:i47, changed default to n4
mvh 20070215: Fixed logging and added *** Association rejected message when association is rejected
mvh 20070304: Added <defer> exportconverter: retry later as if connection unavailable
mvh 20070305: Note: n3 problem fixed in dview.pas, version to 1.4.13alpha
	      Allow merging of up to 1000 UIDs: set SilentText to 64000 and *uids[1000] (thanks mp)
              Allow named pipes for log_on and debuglog_on
mvh 20070307: Work on extending extract: allow e.g. SQlite -> dbaseIII
bcb 20070308: Made changes for big endian and Apple's DARWIN
mvh 20070314: Fisished extract: allow e.g. SQlite -> dbaseIII in directory MAGO dbase
mvh 20070315: Merged DARWIN stuff; Set TCPIPTimeOut for PDU that moves images
mvh 20070316: Display defer import converter; Clumsily added <compression> import converter and 
              improved <storage> import converter; Made <storage> thread safe
	      Failed recompress() no longer reason to fail drag and drop; add convert_to_bmp
	      Use | instead of , to separate patientid from file in addimagefile:
              Distributed , code to each of the server commands to make it more , safe
              Note: it is NOT a good idea to use a , in a flexible filenamesyntax
	      Added level & window to convert_to_gif and convert_to_bmp; longer log lines for dump VR
mvh 20070317: <prefetch> using new prefetch_queue, mayprocess and into_queue_unique, <preforward>, <preget>
mvh 20070324: preget now <preretrieve> and avoids looping by checking orgmessageid!=0xfbad
              I.e., data collected by <preretrieve> does not go through import/exportconverters
              Added delayed exportconverters <forward patient>, <forward study>, <forward series> with optional compression
              Removed "forward compressed to" (without as section) as is never used
	      Version to 1.4.13beta; delay to 10 minutes
mvh 20070326: Fixed leak in <forward patient> etc. preretrieve also checking orgmoveae
mvh 20070330: Change by Mark Pearson: read dicom.ini from same dir as exe in unix mode; merged more Darwin stuff
	      Also taken over bcb's fix for the finder: studymodality was asked at patient level
              Also put in a more ENxxxx as command and messageid
ljz 20070404: Added CommaInFilenameWorkAround()
mvh 20070407: Use reentrant ctime_r when available; moved static SOPPatientRootQuery etc to stack to fix 
	      reentrancy problem: concurrent outgoing c-move of CT and MRI on different associations would 
	      share PDU causing sopclass not accepted on one. Also seems to reduce linux crash problems on 
	      multiple inbound (4 or more) C-stores. Also moved CreateBasicFilmSessionRequest etc to fix 
	      potential reentrance problem for printing.
mvh 20070410: Merged CommaInFilenameWorkAround()
mvh 20070413: "extract:" will just create dbase III clone of full patient DB; added "prefetch" server command
mvh 20070415: Added Prefetcher option to dicom.ini: is set will aggresively preread queried patient or studies
mvh 20070416: Prefetcher prints more information; use windows calls in windows for faster preread
mvh 20070705: extract: uses DT_DBASEIIINOINDEX to disable indexing and packing when extracting from e.g. sqlite to dbaseiii
              Note: extract: non dbaseIII goes to MAG0\dbase, dbaseIII goes to DataSource
mvh 20070706: Added <between "9", "17"> (9:00 to 16:59) or <between "17", "9"> (17:00 to 08:59) import/exportconverter
mvh 20070709: Fixed print of clone db done
mvh 20070720: Fixed problem where waiting "forward ... to " task caused high CPU load
mvh 20070901: Added dgate --checklargestmalloc:
mvh 20071027: Added ForwardCollectDelay for delayed forwarding (default 600 sec)
mvh 20071030: Fill in called and calling AE for SaveToDisk for e.g., dropped files etc
	      Fix %u and %c in Export and ImportConverters: called and calling AE
	      Log flow control Export and ImportConverters only in debug mode
	      Added retrying to delayed forwards or prefetches
mvh 20071102: SetDicomErrorHandler: orderly shutdown of server on serious DICOM library errors
	      Fixed a bug in ForwardAssociationLevel: last UID was lost when reading INI file to szTemp
	      Added MaximumExportRetries and MaximumDelayedFetchForwardRetries (default=0 is no maximum)
mvh 20071103: Version to 1.4.13; defer resets queue fails -> keeps on retrying
mvh 20071104: Use local as AE for imagelister in web page instead of CONQUESTSRV1
	      Enable remote servers (use AE instead of 'local') in finder and lister commands
	      Fix crash when dgate -- option gets empty filename: now lists to stdout
mvh 20071114: Put --checklargestmalloc: in dgate -?
mvh 20071118: Adapted for 64 bits (use SQLLEN for BindField)
mvh 20071120: Fixed KodakFixer for empty patient ID (crashed on 64 bits only)
              Added [modality xx] and [date yyyymmdd-yyyymmdd] clauses to export [patient|study|series] to AE
mvh 20071121: Added %V*GGGG,EEEE: search VR in any sequence; version to 1.4.13a
              Added [now -ddd+ddd] and [age -ddd+ddd] clauses to export [patient|study|series] to AE
	      No retry on remote dicom error in forward xxx to: would retry zero records response
              Use study date for data, age and now clauses
mvh 20071123: Made forward command case insensitive; added forward image; added get [patient|study|series|image] from AE
	      The latter option makes preretrieve obsolete
mvh 20071124: Added [sop xxx] clause to [forward|get]
              Reorganized exportconverter code: all converters now do % substitution; reduced code duplication
              Added reading of sequence items like in %V/300c,0060.0/0008,1155 or %V/300c,0060/0008,1155
	      Real life example (must all be one one line, max 512 characters): 
              ifnotequal "%m", "RTPLAN"; stop; 						nop "tests modality = RTPLAN";
	      ifnotequal "%V*300a,00b2[0,1]", "A2"; stop; 				nop "test machine name = A2"
	      forward to XVI_A2; 							nop "forwards the current recieved RTPLAN";
	      get study modality CT from NKIPACS; 					nop "collects associated CT";
	      get study modality RTSTRUCT sop %V/300c,0060.0/0008,1155 from NKIPACS; 	nop "collects associated RTSTRUCT";
	      forward study modality CT imagetype *AXIAL* to XVI_A2; 			nop "forwards associated CT";
	      forward study modality CT series %V(/300c,0060/0008,1155)/3006,0010/3006,0012/3006,0014/0020,000e     
	      forward study modality RTSTRUCT sop %V/300c,0060.0/0008,1155 to XVI_A2; 	nop "forwards associated RTSTUCT";
mvh 20071125: Fixed syntax for %V/gggg,eeee.i/*gggg,eeee (search all groups under a specified one)
mvh 20071126: Start on StorageCommitment
mvh 20080103: Switched to ms7 compiler; fixed forward and get date, modality, sop filters: would not supress the UIDs
mvh 20080107: Fixed malformed web viewer header: worked for IE but not for other browsers
	      Check presence of dicom.sql in web pages that need it
	      Deal with spaces in patient IDs in web pages
mvh 20080126: Check presence of dgate.dic when starting server
mvh 20080129: Fix KodakFixer: wrote one byte outside VR data (pr)
mvh 20080129: Added [imagetype xxxx] clause to [forward|get] (pr)
mvh 20080130: Small fix in above
mvh 20080205: Added experimental PadAEWithZeros flag; version to 1.4.14alpha
mvh 20080210: Added experimental IgnoreOutOfMemoryErrors;
	      Added "delete [patient|study|series|image] [date yyyymmdd-yyyymmdd] [now -ddd+ddd] [age -ddd+ddd] 
	      [modality mm] [sop xxxx] [imagetype xxxx]" export converter
mvh 20080221: Added [seriesdesc xxxxx] filter and "process [patient|study|series|image] [by command]"
mvh 20080222: Small fix in imagetype filter code; process by clause not part of unique check
mvh 20080302: Fixed case of DICOM table names
mvh 20080316: Added dgate --deletestudies:date(range) and dgate --movepatient/study/studies/series
mvh 20080318: Added [series xxxx] and [study xxxxx] filters; work on web page
	      --deleteseries/study and --moveseries/study allow patid:uid as parameter
	      Web interface can now push and delete on series and study level, but
	      delete is disabled in readonly web interface
	      Version to 1.4.14beta
mvh 20080319: Delete in browsers fixed?; added %v(desc)desc: search VR in given sop of same patient
	      Example: %V(/300c,0060/0008,1155)/3006,0010/3006,0012/3006,0014/0020,000e
              Gives referenced CT series UID in referenced structure set from forwarded RTPLAN
mvh 20080321: Fixes in browser; added graphic web setting; may be gif or bmp for now; 
	      Fix %V for non-existing items
mvh 20080322: Added dgate --anonymize that uses general scripting
mvh 20080323: Small fix in (local) for DcmMove: overwrote destination
mvh+pr 20080404: Fixes in series and study filter (rethink naming)
	      pr added value* match for in match() routine
mvh 20080618: Added extra logging in IARQ()
mvh 20080620: Only show extra logging in IARQ when assoc rejected
mvh 20080816: IARQ shows which sops are accepted; and differentiates no sop from connection terminated
mvh 20080817: Small layout fixes; <process with> import converter; show PDU length in log
	      Check saving of file succeeded (thanks Jeff Bellamy and Alberto Smulders)
mvh 20080819: Added experimental DT_MSTR: multi-value entry, e.g. query on 'PET' matches 'PET\CT'
map 20080819: added --get_ini_param (same as get_param) and get_ini_num to list parameter by index
map 20080819: Added get_ini to list all parameters in DICOM.INI
mvh 20080819: Added code by map but kept original get_param for old web cgi client compatibility
              Version to 1.4.14
mvh 20080820: Solaris 10 fixes, and general UNIX fixes (map): configfiles may not be in current dir
mvh 20080821: Default of ForwardAssociationRelease to 1; added sleep export converter
mvh 20080823: Added study UID search to PatientStudyFinder; fixed DcmMove: would call local when remote adressed
	      Added rudimentary query/move web pages
mvh 20080825: query/move looks better; added "thumbs" column to local series lister; config patched to readonly
	      added background color and display of icon (to be placed in WebCodeBase)
mvh 20080826: One more fix in DcmMove, still called wrong server sometimes
	      Do not use 0xfbad as message ID for DcmMove from web page; webreadonly defaults to true
              CGI viewer configurable: seriesviewer, seriesviewer2, noviewer, aiviewer
mvh 20080827: Added slice number and repeat output data in ImagefileLister and SeriesUIDLister for more flexible formatting
              Added flexviewer: reads code from dicom.ini in web server directory
mvh 20080831: Translate yyyy and yyyymm to date range in finder; further only accept valid date and date ranges
              Added dgate --get_amaps: lists all non wildcard entries from acrnema.map
              Fixed handling of patient ID with spaces in ImageFileLister mode :
              All undefined cgi modes can come from the cgi dicom.ini; added special and cgi script variables
	      Lines can start with -- to start dgate, with # as comment, any other passed as HTML
	      Added source parameter: flex pages can be read from script file
              Added serversideviewer: requires javascript only, nothing to install
mvh 20080901: Fixed some leaks in printing and --commands; fixed --get_ini: formatting
mvh 20080902: Added header option for web scripts; added some CGI environment variables
*/

#define VERSION "1.4.14"
                    
//#define DO_LEAK_DETECTION	1
//#define DO_VIOLATION_DETECTION	1
//#define FAILSAFE_STORAGE

// max ExportConverters lines AND max forwards per line
#define MAXExportConverters 20

#	include	"dgate.hpp"
#	include	"nkiqrsop.hpp"
#	include	<time.h>
#ifdef WIN32
#	include <io.h>
#	include <process.h>
#	include <conio.h>
#else
#	include <unistd.h>
#	include <pthread.h>
#ifndef DARWIN
#	include <malloc.h>
#endif //DARWIN
#endif
#	include <stdio.h>
#	include <ctype.h>
#	include <fcntl.h>
#	include <sys/stat.h>

#ifdef WIN32
#if !defined(__BORLANDC__) && !defined(__WATCOMC__)
#	include	<odbcinst.h>
#endif

#if defined(DO_LEAK_DETECTION) || defined(DO_VIOLATION_DETECTION)
#	include <crtdbg.h>
#endif

#endif

#ifndef ODBC_ADD_SYS_DSN
#define  ODBC_ADD_SYS_DSN 4         // add a system DSN
#endif

BOOL	NoThread=FALSE;					// debug non-threaded

#ifndef UNIX  	// Win32...
#	include	<process.h>
#	define THREAD_RETURN_FALSE FALSE
#	define THREAD_RETURN_TRUE TRUE
	typedef BOOL ThreadRoutineType;
	typedef int ThreadRoutineArgType;
#else		// UNIX...
#	include <pthread.h>
#	include <unistd.h>
#	include <stdlib.h>
#	include <string.h>
#	include <signal.h>
#	include <sys/types.h>
#	include <sys/stat.h>
//#	include <fstream.h>
#	include "npipe.hpp"
#	define THREAD_RETURN_FALSE ((void *)NULL)
#	define THREAD_RETURN_TRUE ((void *)1)
//#	define closesocket(s) close(s)
#	define CloseHandle(h)
void Sleep(int h)
{ struct timespec t;
  if (NoThread) return;
  t.tv_sec =(h/1000);
  t.tv_nsec=(h%1000)*1000000;
  nanosleep(&t, NULL);
}
#	define eof(h) FALSE
	typedef void *	ThreadRoutineType;
	typedef void * ThreadRoutineArgType;
#endif

// This file compiles into two different "modes".  The first (DEBUG_MODE) defined will
// compile into a Windows NT console application.  Just run this exactly like
// any other NT console app (it provides everything the service does).  The second
// mode (DEBUG_MODE undefined) is the service.

#define	DEBUG_MODE

// IPC Block is a block of data which is passed from parent to child with the socket#
#define IPCBlockMagic 0xafaced0a
typedef	struct	_IPCBlock
	{
#ifdef	UNIX
	int			Magic;
#endif
	int			Socketfd;
	}	IPCBlock;

BOOL	RunServer = TRUE;				// reset for regen etc

char	RootConfig[64]	=	"sscscp";		// main entry in ini file
#ifndef	UNIX
char	ConfigFile[512]	=	".\\dicom.ini";		// name ini file
char	DicomDict[64]	=	".\\dgate.dic";		// name dicomdictionary file
#else
char	ConfigFile[512]	=	"dicom.ini";
char	DicomDict[64]	=	"dgate.dic";
#endif

#ifndef UNIX						// for routing IODs
char	AutoRoutePipe[128] = "\\\\.\\pipe\\AutoRoutePipe";
char	AutoRouteExec[128] = "aroute.exe";
#else
char	AutoRoutePipe[128] = "/tmp/AutoRoutePipe";
char	AutoRouteExec[128] = "aroute";
#endif

extern	Array < ACRNemaAddress * > ACRNemaAddressArray; // for -a listing
int	DebugLevel = 0;					// increased after error
RTC	VRType(TRUE);					// VR translation table
extern	int FileCompressMode;
extern unsigned int gl_iFileCounter;

int	ThreadCount = 0;				// Counts total number of associations
int	EnableReadAheadThread=1;			// to enable/disable read ahead thread
int	WorkListMode = 0;				// use worklist to update incoming data

char	DroppedFileCompression[16];			// compression of dropped files
char	IncomingCompression[16];			// compression of incoming images
char	TransmitCompression[16];			// compression of transmitted images
char	DefaultNKITransmitCompression[16];		// default compression of transmitted images using NKI C-MOVE
char	ArchiveCompression[16];				// compression of images to be archived

int     StorageFailedErrorCode = 0x110;			// returned when C-STORE fails (default failed processing)
int	FailHoldOff = 60;				// holdoff in seconds aftre last export/copy failure
int	RetryDelay = 100;				// delay in seconds aftre last export/copy failure for retry
int	QueueSize = 128;				// number of entries in export/copy queue (2k per entry in memory)
int     ForwardCollectDelay = 600;			// delay for forward patient to etc ....

char	VirtualServerFor[11][48];			// AE's of servers providing data for this one (cache function) + parameters
int	CacheVirtualData=1;				// if set, keep (cache) passed through data

// counters for status display
int     StartTime;					// when dgate is started
int	TotalTime=0;					// total run time in s
int	LoadTime=0;					// total time loading
int	ProcessTime=0;					// total time processing (downsize)
int	SaveTime=0;					// total time saving

int     ImagesSent=0;					// how many images were sent
int	ImagesReceived=0;				// idem recieved
int	ImagesSaved=0;					// idem saved
int	ImagesForwarded=0;				// idem forwarded
int	ImagesExported=0;				// executable as export converted
int	ImagesCopied=0;					// copy as export converter

int	IncomingAssociations=0;				// accepted incoming associations (start thread)
int	EchoRequest=0;					// c-echo requests
int	C_Find_PatientRoot=0;
int	C_Move_PatientRootNKI=0;
int	C_Move_PatientRoot=0;
int	C_Find_StudyRoot=0;
int	C_Move_StudyRootNKI=0;
int	C_Move_StudyRoot=0;
int	C_Find_PatientStudyOnly=0;
int	C_Find_ModalityWorkList=0;
int	C_Move_PatientStudyOnlyNKI=0;
int	C_Move_PatientStudyOnly=0;
int	UnknownRequest=0;

int	CreateBasicFilmSession=0;			// printer actions
int	DeleteBasicFilmSession=0;
int	ActionBasicFilmSession=0;
int	SetBasicFilmSession=0;
int	CreateBasicFilmBox=0;
int	ActionBasicFilmBox=0;
int	SetBasicFilmBox=0;
int	DeleteBasicFilmBox=0;
int	SetBasicGrayScaleImageBox=0;
int	SetBasicColorImageBox=0;

int	GuiRequest=0;					// server command requests from GUI
int	ImagesToGifFromGui=0;
int	ImagesToDicomFromGui=0;
int	ExtractFromGui=0;
int	QueryFromGui=0;
int	DeleteImageFromGui=0;
int	DeletePatientFromGui=0;
int	DeleteStudyFromGui=0;
int	DeleteStudiesFromGui=0;
int	DeleteSeriesFromGui=0;
int	MovePatientFromGui=0;
int	MoveStudyFromGui=0;
int	MoveStudiesFromGui=0;
int	MoveSeriesFromGui=0;
int	AddedFileFromGui=0;
int	DumpHeaderFromGui=0;
int	ForwardFromGui=0;
int	GrabFromGui=0;

extern int	DatabaseOpen;				// database activity
extern int	DatabaseClose;
extern int	DatabaseQuery;
extern int	DatabaseAddRecord;
extern int	DatabaseDeleteRecord;
extern int	DatabaseNextRecord;
extern int	DatabaseCreateTable;
extern int	DatabaseUpdateRecords;
extern int	QueryTime;

extern int	SkippedCachedUpdates;			// entering into database levels
extern int	UpdateDatabase;
extern int	AddedDatabase;

extern int	NKIPrivateCompress;			// compression activity
extern int	NKIPrivateDecompress;
extern int	DownSizeImage;
extern int	DecompressJpeg;
extern int	CompressJpeg;
extern int	DecompressedRLE;
extern int      RecompressTime;

#ifndef ctime_r
#define ctime_r(a, b) ctime(a)
#endif

#ifndef localtime_r
#define localtime_r(a, b) memcpy(b, localtime(a), sizeof(struct tm))
#endif

/* error handling for out of memory and such */

static void MyDcmError(int error, char *message, int count)
	{
	char buffer[300];
	char szRootSC[64];

 	strcpy(buffer, "***");
	sprintf(buffer+3, message, count);
	strcat(buffer, "\n");
	OperatorConsole.printf(buffer);

	if (GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig, szRootSC, 64, ConfigFile))
		{
		GetPrivateProfileString(szRootSC, "IgnoreOutOfMemoryErrors", "0", buffer, 64, ConfigFile);
		if (atoi(buffer)) return;
		}

	OperatorConsole.printf("***A fatal error occurred (out of memory) - closing server\n");
	exit(1);
	}

void StatusDisplay(FILE *f)
{ char TimeString[80], buf[64];
  time_t T=StartTime;
  Database DB;
  int i;

  strcpy(TimeString, ctime_r(&T, buf));
  TimeString[strlen(TimeString)-1] = '\0';

  fprintf(f, "DICOM server '%s' (version %s, port %s) was started on %s\n", MYACRNEMA, VERSION, Port, TimeString);
  fprintf(f, "Run time (s) total %d, query %d, load %d, save %d, compress %d, process %d\n", TotalTime, QueryTime/1000, LoadTime, SaveTime, RecompressTime, ProcessTime);
  fprintf(f, "Associations=%d; Images sent=%d, recieved=%d, forwarded=%d\n", IncomingAssociations, ImagesSent, ImagesReceived, ImagesForwarded);
  fprintf(f, "Images printed=%d, in color=%d\n", SetBasicGrayScaleImageBox+SetBasicColorImageBox, SetBasicColorImageBox);
  fprintf(f, "Activity: Echo:%d, Find:%d, Move:%d, Unknown:%d\n", EchoRequest, C_Find_PatientRoot+C_Find_StudyRoot+C_Find_PatientStudyOnly+C_Find_ModalityWorkList,
  	C_Move_PatientRootNKI+C_Move_PatientRoot+C_Move_StudyRootNKI+C_Move_StudyRoot+C_Move_PatientStudyOnlyNKI+C_Move_PatientStudyOnly, UnknownRequest);
  fprintf(f, "Images (de)compressed: NKI %d, JPEG %d, RLE %d, Downsize %d\n",
	NKIPrivateCompress+NKIPrivateDecompress, DecompressJpeg+CompressJpeg, DecompressedRLE, DownSizeImage);
  for (i=0; i<MAGDevices; i++)
    fprintf (f, "Space on MAG%d  : %d MByte\n", i, CheckFreeStoreOnMAGDevice(i));
  for (i=0; i<CACHEDevices; i++)
    fprintf (f, "Space on CACHE%d: %d MByte\n", i, CheckFreeStoreOnCACHEDevice(i));
  if (DB.Open ( DataSource, UserName, Password, DataHost ) )
  { for (i=0; i<10; i++)
      if (DB.dbase_handles[i]) 
        fprintf(f, "Database %s has %d records\n", DB.dbase_handles[i]->filename, DB.dbase_handles[i]->reccount);
  };
};

// Dump an association request for the status display

static BOOL AbstractSyntaxEnabled(CheckedPDU_Service &p, AbstractSyntax	&AbsSyntax)
	{
	UINT	Index;

	if ( ! p.SOPUIDListCount )
		return ( TRUE );

	Index = 0;
	while ( Index < p.SOPUIDListCount )
		{
		if ( UID ( p.SOPUIDList [ Index ] ) ==
			AbsSyntax.AbstractSyntaxName)
			return ( TRUE );
		++Index;
		}
	return ( FALSE );
	}


void
IARQ (AAssociateRQ	&ARQ, BOOL showall)
	{
	char	s[128];

	SystemDebug.printf("A-ASSOCIATE-RQ Packet Dump\n");

	memset(s, 0, 64);
	memcpy(s, ARQ.CallingApTitle, 16);
	OperatorConsole.printf("\tCalling Application Title : \"%s\"\n", s);

	memset(s, 0, 64);
	memcpy(s, ARQ.CalledApTitle, 16);
	OperatorConsole.printf("\tCalled Application Title : \"%s\"\n", s);

	memset(s, 0, 64);
	memcpy(s, ARQ.AppContext.ApplicationContextName.GetBuffer(1),
		ARQ.AppContext.ApplicationContextName.GetSize() );
	OperatorConsole.printf("\tApplication Context : \"%s\", PDU length: %d\n", s, ARQ.UserInfo.GetMax());

	SystemDebug.printf("\tNumber of Proposed Presentation Contexts: %d\n",
		ARQ.PresContexts.GetSize());

	int	Index = 0;
	while (Index < ARQ.PresContexts.GetSize() )
		{
		PresentationContext	PC;

		PC = ARQ.PresContexts.Get(Index);
		PC.TrnSyntax.ClearType = FALSE;

		memset(s, 0, 64);
		memcpy(s,
		PC.AbsSyntax.AbstractSyntaxName.GetBuffer(1),
			PC.AbsSyntax.AbstractSyntaxName.GetSize()%64 );

		OperatorConsole.printf("\tPresentation Context %d \"%s\" %d\n", Index, s, AbstractSyntaxEnabled(*(CheckedPDU_Service*)(&ARQ), PC.AbsSyntax));
		++Index;

		if (showall)
			{
			int IndexTrn = 0;
			while ( IndexTrn < PC.TrnSyntax.GetSize())
				{
				UID uid;

				TransferSyntax TrnSyntax = PC.TrnSyntax.Get(IndexTrn);
				memset(s, 0, 64);
				memcpy(s,
					TrnSyntax.TransferSyntaxName.GetBuffer(1),
					TrnSyntax.TransferSyntaxName.GetSize()%64 );

				uid = TrnSyntax.TransferSyntaxName;

				OperatorConsole.printf("\t\tTransfer Syntax %d \"%s\" %d\n", IndexTrn, s,
					((CheckedPDU_Service*)&ARQ)->CanYouHandleTransferSyntax(TrnSyntax));

				IndexTrn++;
				}
			}
		}
	}

// Help routines to allow dumping of logs after an error occurs
// Test if character is alphanumerical

BOOL	IsAlpha(BYTE	c)
	{
	if((c >= ' ')&&(c <= 125))
		return ( TRUE );
	if(!c)
		return ( TRUE );
	return ( FALSE );
	}

// Test if all of string is alphanumerical

BOOL	IsDataAlpha(UINT	Size, BYTE	*data)
	{
	if(Size > 16)
		Size = 16;
	if(!Size)
		return ( FALSE );
	--Size;
	while(Size)
		{
		if(!IsAlpha(data[Size]))
			return ( FALSE );
		--Size;
		}
	return ( TRUE );
	}

static void TranslateText(char* pSrc, char* pDest, int iSize)
{
	int i;

	/* Translate CR, LF and FF to escape sequences */
	for (i=0; i<iSize; i++)
		switch (*pSrc)
		{
		case '\\':
			*pDest++ = *pSrc;
			*pDest++ = *pSrc++;
			break;
		case '\n':
			*pDest++ = '\\';
			*pDest++ = 'n';
			pSrc++;
			break;
		case '\r':
			*pDest++ = '\\';
			*pDest++ = 'r';
			pSrc++;
			break;
		case '\f':
			*pDest++ = '\\';
			*pDest++ = 'f';
			pSrc++;
			break;
		default:
			*pDest++ = *pSrc++;
			break;
		}
	*pDest = 0;
}

// Print data from a single dicom object
BOOL	PrintUDATA(UINT	Size, void *data, UINT16 TypeCode, char *dest)
	{
	UINT	Index = 0;
	unsigned char	*c;
	UINT16	tint16;
	UINT32	tint32;
	BYTE	*b;
	LE_UINT16	*w;
	char	Str[512];
	LE_UINT32	*l32 = (LE_UINT32*)data;
	LE_UINT16	*l16 = (LE_UINT16*)data;
	LE_INT32	*s32 = (LE_INT32*)data;
	LE_INT16	*s16 = (LE_INT16*)data;

	if(!data)
		{
		if ( TypeCode == 'SQ' )
			{
			sprintf(dest, "Sequence Item");
			return ( TRUE );
			}
	
		sprintf(dest, "(empty)");
		return ( TRUE );
		}

	if ( ! TypeCode || TypeCode == 'UN' )
		{ 			
		switch ( Size )
			{
			case	2:
				tint16 = (*((UINT16 *) data));
				sprintf(dest, "(%d|%x)", (UINT) tint16, (UINT) tint16);
				break;
			case	4:
				tint32 = (*((UINT32 *) data));
				sprintf(dest, "[%d|%x]", (UINT) tint32, (UINT) tint32);
				break;
			default:
				if(IsDataAlpha(Size, (BYTE*) data))
					{
					if(Size > 256) Size = 256;
					TranslateText((char*)data, Str, Size);
					sprintf(dest, "\"%s\"", Str);
					break;
					}
				Index = 0;
				while(Index < Size)
					{
					if(Index >= 16)
						break;
					c = (unsigned char*)data;
					c += Index;
					if (isprint(*c))
						sprintf(dest, "%2.2x(%c)", *c, *c);
					else
						sprintf(dest, "%2.2x", *c);
					++Index;
					}
			}
		}
	else
		{
		switch ( TypeCode )
			{
			case	'AE':
			case	'AS':
			case	'CS':
			case	'DA':
			case	'DS':
			case	'DT':
			case	'IS':
			case	'LO':
			case	'LT':
			case	'PN':
			case	'SH':
			case	'ST':
			case	'TM':
			case	'UI':
				if(Size > 256) Size = 256;
				TranslateText((char*)data, Str, Size);
				sprintf(dest, "\"%s\" ", Str);
				break;

			case	'FL':
			case	'FD':
				break;				

			case	'AT':
				Index = 0;
				while ( Index < Size )
					{
					UINT16	Grp, Elm;
					Grp = (*l16);++l16;
					Elm = (*l16);++l16;
					sprintf(dest, "(%4.4x, %4.4x) ", Grp, Elm);
					Index += 4;
					}
				break;

			case	'OB':
				b = (BYTE*)data;
				while ( Index < Size )
					{
					if ( Index > 10 ) break;
					sprintf(dest, "%2.2x ", *b);
					++b;
					++Index;
					}
				break;

			case	'OW':
				w = (LE_UINT16*)data;				
				while(Index < Size )
					{
					if ( Index > 20 ) break;
					sprintf(dest, "%4.4x ", (UINT16)(*w));
					++w;
					++Index;
					++Index;
					}
				break;
					
			case	'UL':
				while ( Index < Size )
					{
					if ( Index > 40 ) break;
					sprintf(dest, "%ld ", (UINT32)(*l32));
					++l32;
					Index += 4;
					}
				break;

			case	'US':
				while ( Index < Size )
					{
					if ( Index > 20 ) break;			
					sprintf(dest, "%d ", (UINT32)(*l16));
					++l16;
					Index += 2;
					}
				break;

			case	'SL':
				while ( Index < Size )
					{
					if ( Index > 40 ) break;
					sprintf(dest, "%ld ", (UINT32)(*s32));
					++s32;
					Index += 4;
					}
				break;

			case	'SS':
				while ( Index < Size )
					{
					if ( Index > 20 ) break;
					sprintf(dest, "%d ", (UINT32)(*s16));
					++s16;
					Index += 2;
					}
				break;

			}
		}
	return ( TRUE );
	}

// Print a single dicom object
BOOL	DumpVR(VR *vr, FILE *f, int iDepth)
	{
	INT	Index;
	UINT16	TypeCode;
	char	b[256];
	char	s[256];
	char	d[256];

	if (DebugLevel<1 && f==NULL)
		return ( TRUE );

	if (vr->Element==0) return TRUE;

	for (Index=0; Index<iDepth; Index++) b[Index]='>';
        b[iDepth]=0;

	s[0] = '\0';
	TypeCode = 0;
	TypeCode = VRType.RunTimeClass(vr->Group, vr->Element, s);

	PrintUDATA ( vr->Length, vr->Data, TypeCode, d );

	if (f)
		{
		if(TypeCode)
			fprintf(f, "%s%4.4x,%4.4x %6d %c%c %-20.20s   %s\n",
				b, vr->Group, vr->Element, vr->Length,
				TypeCode>>8, TypeCode&0x00ff, s, d);
		else
			fprintf(f, "%s%4.4x,%4.4x %6d UN %-20.20s   %s\n",
				b, vr->Group, vr->Element, vr->Length, "", d);
		}
	else
		{
		if(TypeCode)
			SystemDebug.printf("%s%4.4x,%4.4x %6d %c%c %-20.20s   %s\n",
				b, vr->Group, vr->Element, vr->Length,
				TypeCode>>8, TypeCode&0x00ff, s, d);
		else
			SystemDebug.printf("%s%4.4x,%4.4x %6d UN %-20.20s   %s\n",
				b, vr->Group, vr->Element, vr->Length, "", d);
		}
	return(TRUE);
	}


// Print a composite dicom object + sequences
BOOL
NonDestructiveDumpDICOMObject(
	DICOMObject	*DO, FILE *f = NULL, int iDepth=0)
	{
	DICOMObject	DO2;
	VR		*vr;
	int		Index, i;

	if(DebugLevel<1 && f==NULL)
		return ( TRUE );

	while(vr=DO->Pop())
		{
		DumpVR(vr, f, iDepth);
		if ( vr->SQObjectArray )
			{
			Array < DICOMDataObject * > *ADDO =
				(Array<DICOMDataObject*>*) vr->SQObjectArray;
			
			Index = 0;
			while ( Index < ADDO->GetSize() )
				{
				NonDestructiveDumpDICOMObject(ADDO->Get(Index), f, iDepth+1);

				++Index;
				if (Index == ADDO->GetSize()) break;

				for (i=0; i<iDepth+1; i++)
					if (f)	fprintf(f, "-");
					else	SystemDebug.printf("-");
				if (f)	fprintf(f, "\n");
				else	SystemDebug.printf("\n");
				}
			}
		DO2.Push(vr);
		}
	DO->Reset();
	while(vr=DO2.Pop())
		{
		DO->Push(vr);
		}
	return ( TRUE );
	}


// Check code from dumppacs pasted here for convenience
// List status of mag devices (free space)
BOOL
PrintFreeSpace()
	{
	int		Index;

	OperatorConsole.printf("** MAG DEVICE CHECK\n");
	OperatorConsole.printf("\n");
	Index = 0;
	while ( Index < MAGDevices )
		{
		OperatorConsole.printf ( "Free space on MAG   Device %d := %d MByte\n", Index,
			CheckFreeStoreOnMAGDevice(Index));
		++Index;
		}

	Index = 0;
	while ( Index < MIRRORDevices )
		{
		OperatorConsole.printf ( "Free space on MIRROR Device %d := %d MByte\n", Index,
			CheckFreeStoreOnMIRRORDevice(Index));
		++Index;
		}

	Index = 0;
	while ( Index < CACHEDevices )
		{
		OperatorConsole.printf ( "Free space on CACHE Device %d := %d MByte\n", Index,
			CheckFreeStoreOnCACHEDevice(Index));
		++Index;
		}

	return ( TRUE );
	}

// Print Out A-MAP data struct (used for outgoing C-MOVES)
BOOL
PrintAMap()
	{
	UINT	Index = 0;
	OperatorConsole.printf( "**	AE / IP-PORT Map dump\n");
	OperatorConsole.printf("\n");


	while ( Index < ACRNemaAddressArray.GetSize() )
		{
		ACRNemaAddress	*AAPtr = ACRNemaAddressArray.Get(Index);

		OperatorConsole.printf("%-17s %-30s %-10s %-16s\n",
			AAPtr->Name,
			AAPtr->IP,
			AAPtr->Port,
			AAPtr->Compress);
		++Index;
		}
	return ( TRUE );
	}

// Help routine for listing SQL database configuation; sql field type string
char	*
SQLTypeSymName(
	int	SQLType)
	{
	switch ( SQLType )
		{
		case	SQL_C_CHAR:	return("SQL_C_CHAR");
		case	SQL_C_DATE:	return("SQL_C_DATE");
		default:		return("--bad--");
		}
	return("--bad--");
	}

// Help routine for listing SQL database configuration; dicom object type string
char	*
DICOMTypeSymName(
	int	DICOMType)
	{
	switch ( DICOMType )
		{
		case	DT_STR:			return("DT_STR");
		case	DT_MSTR:		return("DT_MSTR");
		case	DT_DATE:		return("DT_DATE (DA)");	
		case	DT_UINT16:		return("DT_UINT16 (US)");
		case	DT_UINT32:		return("DT_UINT32 (UL)");
		case	DT_UI:			return("DT_UI (UI)");
		case	DT_STARTSEQUENCE:	return("Start sequence");
		case	DT_ENDSEQUENCE:		return("End sequence");
		default:			return ("--bad--");
		}
	return("--bad--");
	}

// Help routine for listing SQL database configuration; list one table
BOOL
DumpDD (
	DBENTRY	*DBE)
	{
	UINT	Index = 0;

	while ( DBE[Index].Group )
		{
		OperatorConsole.printf("0x%4.4x,0x%4.4x  %10s  %4d  %10s  %14s  %10s\n",
			DBE[Index].Group,
			DBE[Index].Element,
			DBE[Index].SQLColumn,
			DBE[Index].SQLLength,
			SQLTypeSymName(DBE[Index].SQLType),
			DICOMTypeSymName(DBE[Index].DICOMType),
			DBE[Index].HL7Tag);
		++Index;
		}
	return ( TRUE );
	}
	
// List full SQL database configuration
BOOL
PrintKFactorFile()
	{
	UINT	Index = 0;
	OperatorConsole.printf( "**	Database Description dump\n");
	OperatorConsole.printf("\n");

	OperatorConsole.printf ("--Patient Database: %s\n", PatientTableName);
	DumpDD(PatientDB);
	OperatorConsole.printf ("--Study Database: %s\n", StudyTableName);
	DumpDD(StudyDB);
	OperatorConsole.printf ("--Series Database: %s\n", SeriesTableName);
	DumpDD(SeriesDB);
	OperatorConsole.printf ("--Image Database: %s\n", ImageTableName);
	DumpDD(ImageDB);
	OperatorConsole.printf ("--WorkList Database: %s\n", WorkListTableName);
	DumpDD(WorkListDB);
	return ( TRUE );
	}


// Delete everything of selected patient, optionally DB only
BOOL
DeletePatient(char *PID, BOOL KeepImages)
	{
	VR		*vr = new VR(0x0010, 0x0020, strlen(PID), (void*)PID, (BOOL) FALSE );
	DICOMDataObject	DDO;

	if (PID==NULL || *PID==0) return FALSE;

	if (!KeepImages)
		OperatorConsole.printf("Deleting patient: %s\n", PID);
	else
		OperatorConsole.printf("Deleting database entries for patient: %s\n", PID);
	DDO.Push(vr);
	RemoveFromPACS(&DDO, KeepImages);

	return ( TRUE );
	}

// Delete study
BOOL
DeleteStudy(char *ID, BOOL KeepImages)
	{
	VR		*vr;
	DICOMDataObject	DDO;

	if (ID==NULL || *ID==0) return FALSE;

	char *p = strchr(ID, ':');

	if (p==0)
		{
		vr = new VR(0x0020, 0x000d, strlen(ID), (void*)ID, (BOOL) FALSE );
		DDO.Push(vr);
		}
	else
		{
		*p=0;
		vr = new VR(0x0020, 0x000d, strlen(p+1), (void*)(p+1), (BOOL) FALSE );
		DDO.Push(vr);
		vr = new VR(0x0010, 0x0020, strlen(ID), (void*)ID, (BOOL) FALSE );
		}
		
	
	if (!KeepImages)
		OperatorConsole.printf("Deleting study: %s\n", ID);
	else
		OperatorConsole.printf("Deleting database entries for study: %s\n", ID);

	RemoveFromPACS(&DDO, KeepImages);

	return ( TRUE );
	}

// Delete studies on date (wildcards or range allowed)
BOOL
DeleteStudies(char *date, BOOL KeepImages)
	{

	VR		*vr = new VR(0x0008, 0x0020, strlen(date), (void*)date, (BOOL) FALSE );
	DICOMDataObject	DDO;

	if (date==NULL || *date==0) return FALSE;

	if (!KeepImages)
		OperatorConsole.printf("Deleting studies on date: %s\n", date);
	else
		OperatorConsole.printf("Deleting database entries for studies on date: %s\n", date);

	DDO.Push(vr);
	RemoveFromPACS(&DDO, KeepImages);

	return ( TRUE );
	}

// Delete series
BOOL
DeleteSeries(char *ID, BOOL KeepImages)
	{
	VR		*vr;
	DICOMDataObject	DDO;

	if (ID==NULL || *ID==0) return FALSE;

	char *p = strchr(ID, ':');

	if (p==0)
		{
		vr = new VR(0x0020, 0x000e, strlen(ID), (void*)ID, (BOOL) FALSE );
		DDO.Push(vr);
		}
	else
		{
		*p=0;
		vr = new VR(0x0020, 0x000e, strlen(p+1), (void*)(p+1), (BOOL) FALSE );
		DDO.Push(vr);
		vr = new VR(0x0010, 0x0020, strlen(ID), (void*)ID, (BOOL) FALSE );
		}
		
	if (!KeepImages)
		OperatorConsole.printf("Deleting series: %s\n", ID);
	else
		OperatorConsole.printf("Deleting database entries for series: %s\n", ID);

	DDO.Push(vr);
	RemoveFromPACS(&DDO, KeepImages);

	return ( TRUE );
	}

// Delete image
BOOL
DeleteImage(char *ID, BOOL KeepImages)
	{
	VR		*vr = new VR(0x0008, 0x0018, strlen(ID), (void*)ID, (BOOL) FALSE );
	DICOMDataObject	DDO;

	if (ID==NULL || *ID==0) return FALSE;

	if (!KeepImages)
		OperatorConsole.printf("Deleting image: %s\n", ID);
	else
		OperatorConsole.printf("Deleting database entry for image: %s\n", ID);

	DDO.Push(vr);
	RemoveFromPACS(&DDO, KeepImages);

	return ( TRUE );
	}

// Compute size of a file
static int 
DFileSize(char *Path)
	{
	FILE *f;
	int res;

	f = fopen(Path, "rb");
	if (f==NULL) return 0;

	fseek(f, 0, SEEK_END);
  	res = ftell(f); /* ask position */
	fclose(f);

	return res;
	}

// Test file exists
static BOOL
DFileExists(char *Path)
	{
	FILE *f;
	int res;

	f = fopen(Path, "rb");
	if (f==NULL) return FALSE;

	fclose(f);
	return TRUE;
	}

// Load DICOM object for GUI given with filename or patientID|sopInstanceUID
DICOMDataObject *LoadForGUI(char *filename)
	{
	char FullFilename[1024], Filename[1024];
	char Device[32];
	char *sop, *pat;
	PDU_Service		PDU;
	DICOMDataObject		DDO;
	VR	*vr;
	
	PDU.AttachRTC(&VRType);

	          sop = strchr(filename, '|');
	if (!sop) sop = strchr(filename, '?');
	if (!sop) sop = strchr(filename, '*');
	if (!sop) sop = strchr(filename, ':');
	if (sop && (sop[1] != '\\'))
		{
		*sop++= 0;
                pat   = filename;
		vr = new VR(0x0008, 0x0018, strlen(sop), (void*)sop, FALSE );

		if(!GetFileName(vr, Filename, Device, TRUE, pat))
			{
			delete vr;
 			return NULL;	// not in database ?
			}

		if (!FindPhysicalDevice(Device, FullFilename, Filename))
			{
			OperatorConsole.printf("***GUI could not find file:%s\n", Filename);
			delete vr;
			return NULL;
			}
	
		strcat(FullFilename, Filename);

		delete vr;	// leak
		return PDU.LoadDICOMDataObject(FullFilename);
		}
	else
		{
		char *p = strrchr(filename, '.');

		// rudimentary gz support
		if (p && stricmp(p, ".gz")==0)
			{ 
			char line[1000];
			char *q = strrchr(filename, PATHSEPCHAR);
			if (!q) q = filename;
			DICOMDataObject *D;
			sprintf(line, "7za e -y \"%s\"", filename);
			system(line);
			*p = 0;
			D = PDU.LoadDICOMDataObject(q+1);
			unlink(q+1);
			*p = '.';
			return D;
			}

		return PDU.LoadDICOMDataObject(filename);
		}
	}

// Delete image from database, where only filename is given
BOOL
DeleteImageFile(char *filename, BOOL KeepImages)
	{
	DICOMDataObject		*pDDO;
	VR			*vrSOPInstanceUID, *vrPat;
	DICOMDataObject		DDO;
	VR			*pVR;

	pDDO = LoadForGUI(filename);
	if(!pDDO)
		{
		OperatorConsole.printf("***[DeleteImageFile] %s -FAILED: Error on Load\n", filename);
		return ( FALSE );
		}

	if (!pDDO->GetVR(0x0010, 0x0020) || 
            !pDDO->GetVR(0x0020, 0x000d) ||
            !pDDO->GetVR(0x0020, 0x000e) ||
            !pDDO->GetVR(0x0008, 0x0018))
		{
		OperatorConsole.printf("***[DeleteImageFile] %s -FAILED: file does not contain correct UIDs\n", filename);
		delete pDDO;
		return ( FALSE );
		}

	pVR = pDDO->GetVR(0x0008, 0x0018);
	vrSOPInstanceUID = new VR(0x0008, 0x0018, pVR->Length, pVR->Data, (BOOL) FALSE );
	DDO.Push(vrSOPInstanceUID);

	pVR = pDDO->GetVR(0x0010, 0x0020);
	vrPat = new VR(0x0010, 0x0020, pVR->Length, pVR->Data, (BOOL) FALSE );
	DDO.Push(vrPat);

	OperatorConsole.printf("Deleting database entry for image: %s\n", filename);
	RemoveFromPACS(&DDO, KeepImages);

	delete pDDO;

	return ( TRUE );
	}

// Ask image UID
BOOL
GetImageFileUID(char *filename, char *UID)
	{
	DICOMDataObject*	pDDO;
	VR			*vrSOPInstanceUID;

	pDDO = LoadForGUI(filename);
	if(!pDDO)
		{
		OperatorConsole.printf("***[GetImageFileUID] %s -FAILED: Error on Load\n", filename);
		return ( FALSE );
		}

	if (!pDDO->GetVR(0x0010, 0x0020) || 
            !pDDO->GetVR(0x0020, 0x000d) ||
            !pDDO->GetVR(0x0020, 0x000e) ||
            !pDDO->GetVR(0x0008, 0x0018))
		{
		OperatorConsole.printf("***[GetImageFileUID] %s -FAILED: file does not contain correct UIDs\n", filename);
		return ( FALSE );
		}

	vrSOPInstanceUID = pDDO->GetVR(0x0008, 0x0018);
	memset(UID, 0, 255);
	memcpy(UID, vrSOPInstanceUID->Data, vrSOPInstanceUID->Length);

	delete pDDO;

	return ( TRUE );
	}

// Change UID of given DDO
static BOOL 
ChangeUIDinDDO(DICOMDataObject *pDDO, int group, int element, char *name)
	{
	char s[255], NewUID[255];
	VR   *vr;
	int  len;

	vr = pDDO->GetVR(group, element);
	if (!vr) return ( FALSE);

	memset(s, 0, 255);
	memcpy(s, vr->Data, vr->Length);

	if (!ChangeUID(s, name, NewUID))
		{
		OperatorConsole.printf("***[ModifyImageFile] FAILED to change %s\n", name);
		return ( FALSE );
		}

	len = strlen(NewUID); if (len&1) len++;
        vr->ReAlloc(len);
	memcpy(vr->Data, NewUID, len);

	return ( TRUE );
	}

// Change any VR of given DDO
static BOOL 
ChangeVRinDDO(DICOMDataObject *pDDO, int group, int element, char *text)
	{
	char s[255];
	VR   *vr;
	int  len;

	vr = pDDO->GetVR(group, element);
	if (!vr) 
		{
		vr = new VR(group, element, 10, TRUE);
		}

	memset(s, 0, 255);
	memcpy(s, vr->Data, vr->Length);

	len = strlen(text); if (len&1) len++;
        vr->ReAlloc(len);
	memcpy(vr->Data, text, len);

//	pDDO->Push(vr);
	pDDO->ReplaceVR(vr);	// also works in sequences

	return ( TRUE );
	}

// Change patient ID of pDDO in memory
BOOL
ModifyPATIDofDDO(DICOMDataObject *pDDO, char *NewPATID)
	{
	int			len;
	PDU_Service		PDU;
	DICOMDataObject		DDO;
	Database		DB;
	char			s[255];
	BOOL			ok;
	VR   			*vr;

	PDU.AttachRTC(&VRType);


	if (NewPATID==NULL || *NewPATID==0)
		{
		OperatorConsole.printf("***[ModifyPATIDofDDO] Empty patient ID not allowed\n");
		return ( FALSE );
		}

	if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
		{
		OperatorConsole.printf("***[ModifyPATIDofDDO] Error Connecting to SQL\n");
		return ( FALSE );
		}

	// Just to make sure the table exists
	// DB.CreateTable ( "UIDMODS", "MODTime int, OldUID varchar(64), MODType varchar(32), NewUID varchar(64)" );

	// check image in memory
	if(!pDDO)
		{
		OperatorConsole.printf("***[ModifyPATIDofDDO] -FAILED: No image passed\n");
		return ( FALSE );
		}

	if (!pDDO->GetVR(0x0010, 0x0020) || 
            !pDDO->GetVR(0x0020, 0x000d) ||
            !pDDO->GetVR(0x0020, 0x000e) ||
            !pDDO->GetVR(0x0008, 0x0018))
		{
		OperatorConsole.printf("***[ModifyPATIDofDDO] -FAILED: image does not contain correct UIDs\n");
		return ( FALSE );
		}

	ok  = ChangeUIDinDDO(pDDO, 0x0020, 0x000d, "StudyUID");
	ok &= ChangeUIDinDDO(pDDO, 0x0020, 0x000e, "SeriesUID");
	ok &= ChangeUIDinDDO(pDDO, 0x0008, 0x0018, "SOPInstanceUID");

	if (!ok)
		{
		OperatorConsole.printf("***[ModifyPATIDofDDO] -FAILED: could not change all UIDs\n");
		return ( FALSE );
		}

	// change patient ID
	vr = pDDO->GetVR(0x0010, 0x0020);
	memset(s, 0, 255);
	memcpy(s, vr->Data, vr->Length);

	len = strlen(NewPATID); if (len&1) len++;
        vr->ReAlloc(len);
	memcpy(vr->Data, NewPATID, len);

	OperatorConsole.printf("Modified patient ID to: %s\n", NewPATID);
	return ( TRUE );
	}

// Merge (SeriesUID or StudyUID {given in type}) of pDDO in memory
BOOL
MergeUIDofDDO(DICOMDataObject *pDDO, char *type)
	{
	int			len;
	PDU_Service		PDU;
	DICOMDataObject		DDO;
	Database		DB;
	char			s[255];
	BOOL			ok;
	VR   			*vr;

	PDU.AttachRTC(&VRType);

	if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
		{
		OperatorConsole.printf("***[MergeUIDofDDO] Error Connecting to SQL\n");
		return ( FALSE );
		}

	// check image in memory
	if(!pDDO)
		{
		OperatorConsole.printf("***[MergeUIDofDDO] -FAILED: No image passed\n");
		return ( FALSE );
		}

	if (!pDDO->GetVR(0x0010, 0x0020) || 
            !pDDO->GetVR(0x0020, 0x000d) ||
            !pDDO->GetVR(0x0020, 0x000e) ||
            !pDDO->GetVR(0x0008, 0x0018))
		{
		OperatorConsole.printf("***[MergeUIDofDDO -FAILED: image does not contain correct UIDs\n");
		return ( FALSE );
		}

	ok  = TRUE;
	if (strcmp(type, "StudyUID")==0) 
		ok &= ChangeUIDinDDO(pDDO, 0x0020, 0x000d, "StudyUID");
	ok  = ChangeUIDinDDO(pDDO, 0x0020, 0x000e, "SeriesUID");
	ok &= ChangeUIDinDDO(pDDO, 0x0008, 0x0018, "SOPInstanceUID");

	if (!ok)
		{
		OperatorConsole.printf("***[MergeUIDofDDO] -FAILED: could not change all UIDs\n");
		return ( FALSE );
		}

	return ( TRUE );
	}

// forward references
BOOL
SaveToDisk(Database	&DB, DICOMDataObject	*DDOPtr, char 	*Filename, BOOL NoKill, unsigned char *calling=NULL, unsigned char *called=NULL, int Syntax=0, BOOL nopreget=FALSE);

void TestCompress(char *filename, char *modes);
void TestForward(char *filename, char *mode, char *server);
void TestSyntax(char *filename, int syntax);
void TestThreadedSave(char *filename);
void ProcessHL7Data(char *data);

// Add image file to server (also copies file!); optional changes patient ID before entering file
BOOL
AddImageFile(char *filename, char *NewPatid)
	{
	DICOMDataObject*	pDDO;
	int			len, i;
	VR			*vrSOPInstanceUID;
	DICOMDataObject		DDO;
	Database		DB;
	char			rFilename[1024];
	BOOL			rc;

	if (NewPatid)
		{
		if (NewPatid[0]=='$' && NewPatid[1]=='c')
			{
			TestCompress(filename, "unasn1n2n3n4j1j2j3j4j5j6k1k2k4k8");
			return TRUE;
			}
		if (NewPatid[0]=='$' && NewPatid[1]=='t')
			{
			TestForward(filename, "un", NewPatid+2);
			TestForward(filename, "as", NewPatid+2);
			TestForward(filename, "n1", NewPatid+2);
			TestForward(filename, "n2", NewPatid+2);
			TestForward(filename, "n3", NewPatid+2);
			TestForward(filename, "n4", NewPatid+2);
			TestForward(filename, "j1", NewPatid+2);
			TestForward(filename, "j2", NewPatid+2);
			TestForward(filename, "j3", NewPatid+2);
			TestForward(filename, "j4", NewPatid+2);
			TestForward(filename, "j5", NewPatid+2);
			TestForward(filename, "j6", NewPatid+2);
			TestForward(filename, "k1", NewPatid+2);
			TestForward(filename, "k2", NewPatid+2);
			TestForward(filename, "k4", NewPatid+2);
			TestForward(filename, "k8", NewPatid+2);
			return TRUE;
			}
		if (NewPatid[0]=='$' && NewPatid[1]=='s')
			{
			TestSyntax(filename, 1);
			TestSyntax(filename, 2);
			TestSyntax(filename, 3);
			TestSyntax(filename, 4);
			TestSyntax(filename, 5);
			TestSyntax(filename, 6);
			TestSyntax(filename, 7);
			TestSyntax(filename, 8);
			TestSyntax(filename, 9);
			TestSyntax(filename, 10);
			return TRUE;
			}
		if (NewPatid[0]=='$' && NewPatid[1]=='r')
			{
			for (i=0; i<atoi(NewPatid+2); i++)
				TestSyntax(filename, 0);
			return TRUE;
			}
		if (NewPatid[0]=='$' && NewPatid[1]=='t')
			{
			for (i=0; i<atoi(NewPatid+2); i++)
				TestThreadedSave(filename);
			return TRUE;
			}
		}

	// rudimentary hl7 support
	if (strlen(filename)>4 && stricmp(filename+strlen(filename)-4, ".hl7")==0)
		{
		int len = DFileSize(filename);
		if (len)
			{
			char *p=(char*) malloc(len+1);
			FILE *f;
			f = fopen(filename, "rb");
			fread(p, 1, len, f);
			p[len]=0;
      	               	fclose(f); 
			ProcessHL7Data(p);
			free(p);
			OperatorConsole.printf("[AddImageFile] loaded HL7 file: %s\n", filename);
			return TRUE;
			}
		else
			{
			OperatorConsole.printf("*** [AddImageFile] could not open HL7 file: %s\n", filename);
			return FALSE;
			}
		}

	if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
		{
		OperatorConsole.printf("***Error Connecting to SQL\n");
		return ( FALSE );
		}

	pDDO = LoadForGUI(filename);
	if(!pDDO)
		{
		OperatorConsole.printf("***[AddImageFile] %s -FAILED: Error on Load\n", filename);
		return ( FALSE );
		}

	if (NewPatid)
		{
		if (!ModifyPATIDofDDO(pDDO, NewPatid))
			{
			OperatorConsole.printf("***[AddImageFile] Error changing patient ID for file: %s\n", filename);
			delete pDDO;
			return FALSE;
			}
		}

	if (!pDDO->GetVR(0x0010, 0x0020) || 
            !pDDO->GetVR(0x0020, 0x000d) ||
            !pDDO->GetVR(0x0020, 0x000e) ||
            !pDDO->GetVR(0x0008, 0x0018))
		{
		OperatorConsole.printf("***[AddImageFile] %s -FAILED: file does not contain correct UIDs\n", filename);
		delete pDDO;
		return ( FALSE );
		}

        // recompress dropped files
	rc = recompress(&pDDO, DroppedFileCompression, "", DroppedFileCompression[0]=='n' || DroppedFileCompression[0]=='N');
        rc = TRUE;	// failed compression leaves original object

	if ((rc == FALSE) || (!SaveToDisk(DB, pDDO, rFilename, TRUE, (unsigned char *)"dropped", (unsigned char *)"dropped")))
		{
		OperatorConsole.printf("***[AddImageFile] Error entering object into server: %s\n", filename);
		if (pDDO)
			delete pDDO;
		return FALSE;
		}

	OperatorConsole.printf("Added file: %s\n", rFilename);
	return ( TRUE );
	}


// Make a new UID for external use (prefix configurable via UIDPrefix in dicom.ini)
extern int UIDPostfix;
static char UIDPrefix[65] = "1.2.826.0.1.3680043.2.135.1066.6";		// default value
static BOOL dgLoadRoot = TRUE;
BOOL
GenUID(char	*oString)
	{ 
	if (dgLoadRoot)
		{
		char szRootSC[64];
  		if (GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig, szRootSC, 64, ConfigFile))
			{
			if (GetPrivateProfileString(szRootSC, "UIDPrefix", "1.2.826.0.1.3680043.2.135.1066", UIDPrefix, 64, ConfigFile))
				strcat(UIDPrefix, ".6");
			}
		dgLoadRoot = FALSE;
		}

#ifdef WIN32
	sprintf(oString, "%s.%u.%d.%u", UIDPrefix, time(NULL), GetTickCount()%1000, (UIDPostfix++)%100);
#else
	sprintf(oString, "%s.%u.%u", UIDPrefix, time(NULL), (UIDPostfix++)%10000);
#endif
	return ( TRUE );
	}

int CallImportConverterN(DICOMDataObject *DDO, int N, char *pszModality, char *pszStationName, char *pszSop, char *patid, char *calling, char *called, char *Storage);

// Change patient ID of image file
BOOL
ModifyPATIDofImageFile(char *filename, char *NewPATID, BOOL DelFile, char *script)
	{
	DICOMDataObject*	pDDO;
	int			sIndex, Index, len;
	DICOMDataObject		DDO;
	Database		DB;
	char			rFilename[1024], s[255];
	char			MirrorFile[1024], Device[255], Physical[1024];
	BOOL			ok;
	VR   			*vr;
	int			devlen, mirrordevlen;

	if (NewPATID==NULL || *NewPATID==0)
		{
		OperatorConsole.printf("***[ModifyPATIDofImageFile] Empty patient ID not allowed\n");
		return ( FALSE );
		}

	// try to locate the mirror file because that one has to change too
	MirrorFile[0] = 0;
	Index = 0;
	while ( Index < MAGDevices )
		{
		sprintf(Device, "MAG%d", Index);
		GetPhysicalDevice(Device, Physical);
		devlen = strlen(Physical);
		if (memicmp(Physical, filename, strlen(Physical))==0)
			{
			sprintf(Device, "MIRROR%d", Index);
			if (GetPhysicalDevice(Device, MirrorFile))
				{
				mirrordevlen = strlen(MirrorFile);
				strcat(MirrorFile, filename + strlen(Physical));
				}
			break;
			}
		++Index;

		if (Index == MAGDevices)
			{
			OperatorConsole.printf("***File to modify is not on a MAG device\n");
			return ( FALSE );
			}
		}

	// remove image file from database
	if (DelFile)
		if (!DeleteImageFile(filename, TRUE))
			{
			OperatorConsole.printf("***Could not remove image file from database\n");
			return ( FALSE );
			}

	if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
		{
		OperatorConsole.printf("***Error Connecting to SQL\n");
		return ( FALSE );
		}

	// Just to make sure the table exists
	// DB.CreateTable ( "UIDMODS", "MODTime int, OldUID varchar(64), MODType varchar(32), NewUID varchar(64)" );

	// load image into memory
	pDDO = LoadForGUI(filename);
	if(!pDDO)
		{
		OperatorConsole.printf("***[ModifyPATIDofImageFile] %s -FAILED: Error on Load\n", filename);
		return ( FALSE );
		}

	// change patient ID of object file in memory
	if (!ModifyPATIDofDDO(pDDO, NewPATID))
		{
		delete pDDO;
		return FALSE;
		}

	if (script)
		CallImportConverterN(pDDO, -1, NULL, NULL, NULL, NULL, NULL, NULL, script);

	// add the image in memory to the server, also makes a copy of the image
	if (!SaveToDisk(DB, pDDO, rFilename, TRUE, (unsigned char *)"modpatid", (unsigned char *)"modpatid"))
		{
		delete pDDO;
		OperatorConsole.printf("***Error entering object into server: %s\n", filename);
		return ( FALSE );
		}
	OperatorConsole.printf("Modified image: %s\n", rFilename);

	if (!DelFile) 
		{
		return ( TRUE );
		}

	// delete the original image since it would show up with original PATID after regen
	if (stricmp(filename, rFilename))
		{
		unlink(filename);
		OperatorConsole.printf("Deleting file: %s\n", filename);
		}

	// delete the mirror image since it would show up with original PATID after regen
	if (strlen(MirrorFile))
		if (stricmp(MirrorFile, rFilename))
			{
			unlink(MirrorFile);
			OperatorConsole.printf("Deleting mirror file: %s\n", MirrorFile);
			}


	// remove the source directory and any required subdirectories (just fails quietly if directory not yet empty)
	strcpy(s, filename);
	for (sIndex = strlen(s)-1; sIndex>=devlen; sIndex--)
		if (s[sIndex]==PATHSEPCHAR)
		{
		s[sIndex]='\0';
		rmdir(s);
		}

	// remove the mirror source directory and any required subdirectories (just fails quietly if directory not yet empty)
	strcpy(s, MirrorFile);
	for (sIndex = strlen(s)-1; sIndex>=mirrordevlen; sIndex--)
		if (s[sIndex]==PATHSEPCHAR)
		{
		s[sIndex]='\0';
		rmdir(s);
		}

	return ( TRUE );
	}

// Merge SeriesUID or StudyUID (passed in type) of image file
BOOL
MergeUIDofImageFile(char *filename, BOOL DelFile, char *type)
	{
	DICOMDataObject*	pDDO;
	int			sIndex, Index, len;
	DICOMDataObject		DDO;
	Database		DB;
	char			rFilename[1024], s[255];
	char			MirrorFile[1024], Device[255], Physical[1024];
	BOOL			ok;
	VR   			*vr;
	int			devlen, mirrordevlen;

	// try to locate the mirror file because that one has to change too
	MirrorFile[0] = 0;
	Index = 0;
	while ( Index < MAGDevices )
		{
		sprintf(Device, "MAG%d", Index);
		GetPhysicalDevice(Device, Physical);
		devlen = strlen(Physical);
		if (memicmp(Physical, filename, strlen(Physical))==0)
			{
			sprintf(Device, "MIRROR%d", Index);
			if (GetPhysicalDevice(Device, MirrorFile))
				{
				mirrordevlen = strlen(MirrorFile);
				strcat(MirrorFile, filename + strlen(Physical));
				}
			break;
			}
		++Index;

		if (Index == MAGDevices)
			{
			OperatorConsole.printf("***File to merge is not on a MAG device\n");
			return ( FALSE );
			}
		}

	// remove image file from database
	if (DelFile)
		if (!DeleteImageFile(filename, TRUE))
			{
			OperatorConsole.printf("***Could not remove image file from database\n");
			return ( FALSE );
			}

	if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
		{
		OperatorConsole.printf("***Error Connecting to SQL\n");
		return ( FALSE );
		}

	// load image into memory
	pDDO = LoadForGUI(filename);
	if(!pDDO)
		{
		OperatorConsole.printf("***[MergeUIDofImageFile] %s -FAILED: Error on Load\n", filename);
		return ( FALSE );
		}

	// change patient ID of object file in memory
	if (!MergeUIDofDDO(pDDO, type))
		{
		delete pDDO;
		return FALSE;
		}

	// add the image in memory to the server, also makes a copy of the image
	if (!SaveToDisk(DB, pDDO, rFilename, TRUE, (unsigned char *)"merging", (unsigned char *)"merging"))
		{
		delete pDDO;
		OperatorConsole.printf("***Error entering object into server: %s\n", filename);
		return ( FALSE );
		}
	OperatorConsole.printf("Merged image: %s\n", rFilename);

	if (!DelFile) 
		return ( TRUE );

	// delete the original image since it would show up with original series UID after regen
	if (stricmp(filename, rFilename))
		{
		unlink(filename);
		OperatorConsole.printf("Deleting file: %s\n", filename);
		}

	// delete the mirror image since it would show up with original series UID after regen
	if (strlen(MirrorFile))
		if (stricmp(MirrorFile, rFilename))
			{
			unlink(MirrorFile);
			OperatorConsole.printf("Deleting mirror file: %s\n", MirrorFile);
			}


	// remove the source directory and any required subdirectories (just fails quietly if directory not yet empty)
	strcpy(s, filename);
	for (sIndex = strlen(s)-1; sIndex>=devlen; sIndex--)
		if (s[sIndex]==PATHSEPCHAR)
		{
		s[sIndex]='\0';
		rmdir(s);
		}

	// remove the mirror source directory and any required subdirectories (just fails quietly if directory not yet empty)
	strcpy(s, MirrorFile);
	for (sIndex = strlen(s)-1; sIndex>=mirrordevlen; sIndex--)
		if (s[sIndex]==PATHSEPCHAR)
		{
		s[sIndex]='\0';
		rmdir(s);
		}

	return ( TRUE );
	}

// Help routine for spawning a process on WIN32 or UNIX

#ifndef UNIX	// Win32...

BOOL
BackgroundExec(
	char	*ProcessBinary,				// name of executable
	char	*Args)					// arguments
	{
	PROCESS_INFORMATION	PINFO;
	STARTUPINFO		SINFO;
	SECURITY_ATTRIBUTES	SA;
	HANDLE			hMap;
	char			CommandLine[1024];

	memset((void*)&SINFO, 0, sizeof(STARTUPINFO));
	SINFO.cb = sizeof(STARTUPINFO);

	SA.nLength = sizeof (SECURITY_ATTRIBUTES);
	SA.lpSecurityDescriptor = NULL;
	SA.bInheritHandle = TRUE;

	sprintf(CommandLine, "%s %s", ProcessBinary, Args);
	if(!CreateProcess(ProcessBinary, CommandLine, NULL, NULL, TRUE,
		DETACHED_PROCESS, NULL, NULL, &SINFO, &PINFO))
		{
		OperatorConsole.printf("***Failed to create process: %d\n", GetLastError());
		}
	else
		{
		CloseHandle(PINFO.hThread);
		CloseHandle(PINFO.hProcess);
		}

	return ( TRUE );
	}

#else		// UNIX...

BOOL
BackgroundExec(
	char	*theExecName,
	char	*theArg)
	{

	// fork and exec a new process... (just replicate 1 thread...)

	pid_t aPID = fork();

	if(aPID == 0)
	{
		// this is the child process...
		// lose controlling tty

		setsid();

		// close all open files, except standard I/O
		// (could close those also, and attach them to /dev/console)
		// like:
		// fd = open("/dev/console", 1);
        //	if(fd != 1)
        //	{
        //	    dup2(fd, 1);
        //	    close(fd);
        //	    fd = 1;
        //	}
        //	dup2(1, 2);

		int anIndex;
		int anOpenFileMax;

		for(anIndex = 3, anOpenFileMax = sysconf(_SC_OPEN_MAX); anIndex < anOpenFileMax; ++anIndex)
			{
			close(anIndex);
			}

		// reset signals

		for(anIndex = 1; anIndex <= SIGUSR2; ++anIndex)
			{
			(void)signal(anIndex, SIG_DFL);	
			}

		// do the exec()

		if(execlp(theExecName, theExecName, theArg, NULL))
			{
			SystemDebug.printf(
				"DriverApp::ServerChildInit(): failed to execlp(%s)\n",
				theExecName);
			//_exit(255);
			}
		}
	return ( TRUE );
	}

#endif

////////////////////////////////////////////////////////////////////////
// copy a file (possibly opening as shared file)

static BOOL
DFileCopy2(char *source, char *target, int smode)
{
	BOOL		err_status = TRUE; 	/* Error status                 */
	char		*copybuffer;        	/* The copy buffer              */
	int		inhandle, outhandle; 	/* Handle of input, output files */
	unsigned 	bufsize;            	/* Size of copy buffer          */
	unsigned	bytes;              	/* Actual transferred bytes     */

	/* Open input file for read-only. parser error if operation fails.  */

	if (!smode)
		inhandle = open( source, O_RDONLY | O_BINARY, S_IREAD);
	else
 		inhandle = sopen( source, O_RDONLY | O_BINARY, SH_DENYNO, S_IREAD);

	if( inhandle == -1) return FALSE;

	/* Ok, open output file. Parser error on permission failure: */

	outhandle = open( target, O_CREAT | O_TRUNC | O_BINARY | O_RDWR, 0666);
	if( outhandle == -1)
 		{ 
		close(inhandle);
		return FALSE;
		}

	/* get a large buffer */
	bufsize = 0xF000;

	copybuffer = (char *)malloc(bufsize);
	if (copybuffer == NULL)
   		{
		close(inhandle);
		close(outhandle);
		return FALSE;
		}

	while( !eof(inhandle) && err_status)
		{
		bytes= (unsigned)read( inhandle, copybuffer, bufsize);

#ifndef WIN32
		if (bytes == 0)
       			break;
#endif

		if (bytes == (unsigned) -1)
			{ 
			err_status= FALSE;
       			break;
			}

		if (bytes != (unsigned)write(outhandle, copybuffer, bytes))
			{
			err_status= FALSE;
       			break;
			}
		}

	free(copybuffer);


	close(outhandle);
	close(inhandle);

	/* Clean up mess in case of error : */
	if(err_status != TRUE) 
		unlink(target);

	return err_status;
	}

// Special RunTime-Classing Storage mechanism.  Used for "Generic"
// Unknown outgoing C-Store requests from the Query / Retrieve Engine
// Defined here because needed in forward code

class	RunTimeClassStorage	:
	public	StandardStorage
	{
		UID		MyUID;

	public:
		// is not used anymore and would not be thread safe (db's may not be shared)
		// Database	DB;
		BOOL	uGetUID(UID &uid) { return ( GetUID(uid) ); };
		inline	BOOL Read ( PDU_Service *PDU, DICOMCommandObject *DCO,
					DICOMDataObject *DDO )
			{ return ( StandardStorage :: Read ( PDU, DCO, DDO ) ); };
		inline	BOOL Write ( PDU_Service *PDU, DICOMDataObject *DDO)
			{ return ( StandardStorage :: Write ( PDU, DDO ) ); };
		BOOL	GetUID(UID	&);
		BOOL	SetUID(UID	&);
		BOOL	SetUID(VR	*);
		BOOL	SetUID(DICOMDataObject	*);
	};

// This routine is used as a filter for export converter.
// It works by combining a given filter string with a query 
// for a given SOP, and returning TRUE if a record is found.
// maxname is used to truncate fieldnames to fit with the DB spec.
// limitations: PatientID, SeriesInstanceUID and StudyInstanceUID 
// cannot be queried. All other DB fields can be queried.

int TestFilter(char *query, char *sop, int maxname, char *patid=NULL)
{	Database aDB;
	char Tables[512], QueryString[512], Dum[256];
        int result, instring=0, inword=0, i, L, Index;
	char *p;
	SQLLEN sdword;

	if (*query)
	 	sprintf(Tables, "%s, %s, %s, %s",
			ImageTableName, SeriesTableName, StudyTableName, PatientTableName);
	else
	 	sprintf(Tables, "%s",
			ImageTableName);

	if(!aDB.Open(DataSource, UserName, Password, DataHost))
		{
		OperatorConsole.printf("***Filter: Unable to open database %s as user %s on %s\n",
					DataSource, UserName, DataHost);
		return 0;
		}

	// truncate the identifiers in the query to 10 characters
        // this is our default maximum field name length for the DB
        L = strlen(query);
	for (i=0; i<L; i++)
	{ if (instring)
	  { if (query[i]=='\'')
	    { instring = 0;
	    }
	    continue;
	  }
	  else if (inword)
	  { if (query[i]==' ')
	    { inword = 0;
	    }
	    else
	    { inword++;
  	      if (inword>maxname) query[i] = ' ';
	    }
	  }
	  else
	  { if (query[i]=='\'')
	    { instring = 1;
	    }
	    else if (isalpha(query[i]))
	    { inword = 1;
	    }
	  }
	}

	if (*query)
		sprintf(QueryString, 	"(%s) and "
                                "DICOMImages.SopInstanc = '%s' and "
				"DICOMStudies.PatientID = DICOMPatients.PatientID and "
				"DICOMImages.SeriesInst = DICOMSeries.SeriesInst and "
				"DICOMSeries.StudyInsta = DICOMStudies.StudyInsta",
				query, sop);
	else
		sprintf(QueryString, "DICOMImages.SopInstanc = '%s'", sop);

        // include patid (indexed) for speed; test for rev5 or higher of the database
	if (patid)
		{
		Index = 1;
        	while (ImageDB[Index].Element)
			{
			if (ImageDB[Index].Group==0x0010 && ImageDB[Index].Element==0x0020)
				{ 
				char newpatid[128];
				strcpy(newpatid, patid);
				DICOM2SQLQuery(newpatid);
				sprintf(QueryString+strlen(QueryString), " AND DICOMImages.ImagePat = %s", newpatid);
				break;
        			}
			Index++;
			}
		}

	if (!aDB.Query(Tables, "DICOMImages.ObjectFile", QueryString, NULL))
		{
		OperatorConsole.printf("***Filter: Unable to query for image record\n");
		aDB.Close();
		return 0;
		}

	aDB.BindField (1, SQL_C_CHAR, Dum, 255, &sdword);
	result = aDB.NextRecord();

	aDB.Close();	

	return result;
	}

/* The following function is called at the end of 'SaveToDisk' or through a queue
   that is filled from SaveToDisk. After the arrival and storage of each DicomObject, 
   several executables may be invoked, to perform further processing.
*/

#ifdef WIN32
#include "shellapi.h"
#endif

// forward references
static int htoin(char *value, int len);
static int isxdig(char ch);
BOOL prefetch_queue(char *operation, char *patientid, char *server, char *studyuid, char *seriesuid, char *compress, char *modality, char *date, char *sop, char *imagetype, char *seriesdesc);
void reset_queue_fails(int N);

static BOOL match(char *source, char *target)
{ if (strlen(target)==1 && target[0]=='*') return TRUE;

  char Source[512];
  strcpy(Source, source);
  while (strlen(Source)>0 && Source[strlen(Source)-1]==' ') Source[strlen(Source)-1] = 0;

  if (strlen(target)> 1 && target[strlen(target)-1]=='*')
  { if (memicmp(Source, target, strlen(target)-1)) return FALSE;
  }
  else
  if (strlen(target)> 1 && target[0]=='*')
  { if (memicmp(Source+strlen(Source)-strlen(target)+1, target+1, strlen(target)-1)) return FALSE;
  }
  else
    if (stricmp(Source, target)) return FALSE;

  return TRUE;
}

// Search a composite dicom object:
// desc=gggg,eeee 	plain element
// desc=*gggg,eeee 	search in all sequences
// desc=/*gggg,eeee 	idem
// desc=/gggg,eeee.i..  search specified in sequence, ith element (recursive)
// desc=/gggg,eeee..    idem, first element
// e.g. /gggg,eeee.0/gggg,eeee/*gggg,eeee: to search specified sequences
//
// desc=(desc1)desc2 search desc1 in DICOM object which sop is specified in (desc1), patient ID taken from 0010,0020
// e.g. %V(/300c,0060.0/0008,1155)/gggg,eeee.0/gggg,eeee
//
// returns length of parsed desc string

int SearchDICOMObject(DICOMObject *DDO, char *desc, char *result)
	{
	DICOMObject	DDO2;
	VR		*vr;
	int		Index, i, g, e, len;
	char		*p;
	BOOL		seq=FALSE, wild=FALSE;

	*result=0;
	i      =0;

        if (desc[i]=='(')
		{
		i++;
		char r[300];
		DICOMObject *pDDO;

		SearchDICOMObject(DDO, "0010,0020", r);
		strcat(r, ":");
		i += SearchDICOMObject(DDO, desc+i, r+strlen(r));
		if (desc[i]==')') i++;
		//else OperatorConsole.printf("***Syntax error in %V(desc)desc statement %s at %d\n", desc, i);
		pDDO = LoadForGUI(r);
		i += SearchDICOMObject(pDDO, desc+i, result);
		delete pDDO;
		return i;
		}

	if (desc[i]=='/') i++, seq=TRUE;
	if (desc[i]=='*') i++, seq=FALSE, wild=TRUE;

	if (!isxdig(desc[i])) return i;  
  	g = htoin(desc+i,   4);
  	e = htoin(desc+i+5, 4);
	i +=9;

	// get plain element
	if (!seq && !wild)
	    	{ 
		if (!DDO) return i;
		vr = DDO->GetVR(g, e);
		if (vr)
			{ 
			strncpy(result, (char*)vr->Data, vr->Length);
  	                result[vr->Length] = 0;
  	                len = vr->Length - 1;
                        while(len>0)
				{ 
				if (result[len] == ' ')
  	                  		result[len] = 0;
                          	else
                            		break;
                          	len--;
                        	}
                      	}

		return i;
		}

	// get specified sequence element (only 0..9 supported for now)
	else if (seq)
	    	{ 
		int j=0;

		if (desc[i]=='.') 
			{
			Index = atoi(desc+i+1);
			i+=2;
			}
		else
			Index = 0;

		vr = NULL;
		if (DDO) vr = DDO->GetVR(g, e);
		if (vr)
			{ 
			if ( vr->SQObjectArray )
				{
				Array < DICOMDataObject * > *ADDO =
					(Array<DICOMDataObject*>*) vr->SQObjectArray;

				j = SearchDICOMObject(ADDO->Get(Index), desc+i, result);
				}
			else
				{
				strncpy(result, (char*)vr->Data, vr->Length);
  		                result[vr->Length] = 0;
  	        	        len = vr->Length - 1;
                        	while(len>0)
					{ 
					if (result[len] == ' ')
  		                  		result[len] = 0;
                	          	else
                        	    		break;
                          		len--;
                        		}
                      		}
			}
		else
			{
			// ignore rest of desc
			char dum[1024];
			j = SearchDICOMObject(DDO, desc+i, dum);
			}

		return i+j;
		}
	
	// wild: search all sequences
	if (!DDO) return i;
	while(vr=DDO->Pop())
		{
		if (*result==0)
			{ 
			if ( vr->SQObjectArray )
				{
				Array < DICOMDataObject * > *ADDO =
					(Array<DICOMDataObject*>*) vr->SQObjectArray;
				
				Index = 0;
				while ( Index < ADDO->GetSize() )
					{
					SearchDICOMObject(ADDO->Get(Index), desc, result);
					if (*result) break;
					++Index;
					}
				}
			else if (vr->Group==g && vr->Element==e)
				{
				strncpy(result, (char*)vr->Data, vr->Length);
  	        	        result[vr->Length] = 0;
  	                	len = vr->Length - 1;
	                        while(len>0)
					{ 
					if (result[len] == ' ')
  	                	  		result[len] = 0;
                          		else
                            			break;
	                          	len--;
        	                	}
				}
			}

		DDO2.Push(vr);
		}

	DDO->Reset();
	while(vr=DDO2.Pop())
		{
		DDO->Push(vr);
		}

	return i;
	}

  
// operates asynchronously on images AFTER it they are stored in the database
// return TRUE if it worthwile to retry (connection or write failed)
// For general scripting use:
// CallExportConverterN(FileName, -1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, script)

BOOL CallExportConverterN(char *pszFileName, int N, char *pszModality, char *pszStationName, char *pszSop, char *patid, char *calling, char *called, ExtendedPDU_Service *PDU, char *ForwardLastUID)
{ char		szRootSC[64];
  char		szEntry[32];
  char		szTemp[66];
  int		i, rc, MaxLength, part, skipping;
  char		szExecName[512], szNext[512];
  char		szExecModality[66];
  char		szExecStationName[66];
  char		szExecFilter[512];
  char		ExportCalledAE[18], ExportCallingAE[18], Called[18], Calling[18];
  BOOL		StripGroup2;

  DICOMDataObject  *DDO = NULL;
  ExtendedPDU_Service PDU2;

  PDU2.AttachRTC(&VRType);

  if (!GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig,
		szRootSC, 64, ConfigFile)) return FALSE;

  /* filter using modality, stationname, calling and called */

  if (pszModality)
  { sprintf(szEntry, "ExportModality%d", N);
    if (!GetPrivateProfileString(szRootSC, szEntry, "*", szExecModality, 64, ConfigFile)) return FALSE;
    if (!match(pszModality, szExecModality)) return FALSE;
  }

  if (pszStationName)
  { sprintf(szEntry, "ExportStationName%d", N);
    if (!GetPrivateProfileString(szRootSC, szEntry, "*", szExecStationName, 64, ConfigFile)) return FALSE;
    if (!match(pszStationName, szExecStationName)) return FALSE;
  }

  if (calling)
  { sprintf(szEntry, "ExportCallingAE%d", N);
    if (!GetPrivateProfileString(szRootSC, szEntry, "*", ExportCallingAE, 18, ConfigFile)) return FALSE;
    if (!match(calling, ExportCallingAE)) return FALSE;
  }

  if (called)
  { sprintf(szEntry, "ExportCalledAE%d", N);
    if (!GetPrivateProfileString(szRootSC, szEntry, "*", ExportCalledAE, 18, ConfigFile)) return FALSE;
    if (!match(called, ExportCalledAE)) return FALSE;
  }

  /* apply database filter */

  if (called && calling)
  { sprintf(szEntry, "ExportFilter%d", N);
    if (!GetPrivateProfileString(szRootSC, szEntry, "*", szExecFilter, 510, ConfigFile)) return FALSE;
    if (szExecFilter[0]!='*') 
    { if (!GetPrivateProfileString(szRootSC, "TruncateFieldNames", "255",	szTemp, 32, ConfigFile)) return FALSE;
      MaxLength = atoi(szTemp);
  
      if (!TestFilter(szExecFilter, pszSop, MaxLength, patid)) return FALSE;
    }
  }

  /* get the export converter name: empty is none */

  if (called && calling)
  { sprintf(szEntry, "ExportConverter%d", N);
    if (!GetPrivateProfileString(szRootSC, szEntry, "",
          szExecName, 512, ConfigFile)) return FALSE;
    if (szExecName[0]==0) return FALSE;
  }
  else
    strcpy(szExecName, ForwardLastUID);

  rc       = 1;	// status for if statement and stop
  part     = 0;
  skipping = 0;

  while(1)
  { szNext[0] = 0;

    if (skipping || rc==4)			// {} block being skipped
    { if      (szExecName[0]=='{') skipping++, rc=0;
      else if (szExecName[0]=='}') skipping--, rc=0;
    }

    // remove {} for all export converters (not only those with a %)
    i = 0;
    if      (szExecName[0] == '{') {i++; while(szExecName[i]==' ')i++; memmove(szExecName, szExecName+i, strlen(szExecName+i)+1); }
    else if (szExecName[0] == '}') {i++; while(szExecName[i]==' ')i++; memmove(szExecName, szExecName+i, strlen(szExecName+i)+1); }
        
    // find ; not in string: splits export commands
    int instring=0;
    int L = strlen(szExecName);
    for (int k=0; k<L; k++) 
    { if (szExecName[k]=='"') instring = !instring;
      if (szExecName[k]==';' && instring==0)
      { szExecName[k] = 0;			// split command
        while (szExecName[k+1]==' ') k++;	// skip spaces after ;
        strcpy(szNext, szExecName+k+1);		// remaining for next
        break;
      }
    }
  
    if (skipping)	// {} block being skipped
    { SystemDebug.printf("Exportconverter%d.%d skipped\n", N, part);
    }

    else if (szExecName[0]==0)	// leftover of { or }
    { 
    }

    else if (rc==4)	// ifxxxx statement causes a skip
    { rc = 0;
      SystemDebug.printf("Exportconverter%d.%d not executed because of previous statement\n", N, part);
    }

    /* for all export converters: first substitute % variables */
  
    else
    { char line[1000];

      if (strchr(szExecName, '%'))
      { int i=0, l, L, len;
        char *p;
        VR   *pVR;
        char vr[256];
   
        /* substitute %x variables and remove possible { and } at begin of statement */
  
        line[0] = 0;
  
        L = strlen(szExecName);
        while (i<L)
        { if      (i==0 && szExecName[i] == '{') {i++; while(szExecName[i]==' ')i++;}
          else if (i==0 && szExecName[i] == '}') {i++; while(szExecName[i]==' ')i++;}
          else if (szExecName[i] == '%')
          { char *ps = line + strlen(line);

            switch(tolower(szExecName[i+1]))
            { case 'f': strcat(line, pszFileName);	// %f=filename
                        break;
              case 'm': if (pszModality)
                          strcat(line, pszModality);	// %m=modality
                        else
		        { char result[256];
                          if (!DDO) DDO = PDU2.LoadDICOMDataObject(pszFileName);
	      	          SearchDICOMObject(DDO, "0008,0060", result);
                          strcat(line, result);
                        }
                        break;
              case 's': if (pszStationName)
                          strcat(line, pszStationName);	// %s=stationname
                        else
		        { char result[256];
                          if (!DDO) DDO = PDU2.LoadDICOMDataObject(pszFileName);
	      	          SearchDICOMObject(DDO, "0008,1100", result);
                          strcat(line, result);
                        }
                        break;
              case 'o': if (pszSop)
                          strcat(line, pszSop);		// %o=object instance
                        else
		        { char result[256];
                          if (!DDO) DDO = PDU2.LoadDICOMDataObject(pszFileName);
	      	          SearchDICOMObject(DDO, "0008,0018", result);
                          strcat(line, result);
                        }
                        break;
              case 'i': if (patid)
                          strcat(line, patid);		// %i=patient ID
                        else
		        { char result[256];
                          if (!DDO) DDO = PDU2.LoadDICOMDataObject(pszFileName);
	      	          SearchDICOMObject(DDO, "0010,0020", result);
                          strcat(line, result);
                        }
                        break;
              case 'u': Calling[0]=0;
		        if (calling) strcpy(Calling, calling);
        	        while (strlen(Calling)>0 && Calling[strlen(Calling)-1]==' ') Calling[strlen(Calling)-1] = 0;
		        strcat(line, Calling);		// %u=SCU
                        break;
              case 'c': Called[0]=0;
		        if (called) strcpy(Called, called);
		        while (strlen(Called)>0 && Called[strlen(Called)-1]==' ') Called[strlen(Called)-1] = 0;
		        strcat(line, Called);		// %c=calledAE = scp
                        break;
              case 'd': time_t TimeOfDay;		// %d=date and time
    		        char TimeString[128], buf[64];
  		        TimeOfDay = time(NULL);
  		        strcpy(TimeString, ctime_r(&TimeOfDay, buf));
  		        TimeString[strlen(TimeString)-1] = '\0';
                        strcat(line, TimeString);
                        break;
              case 'b': p = strrchr(pszFileName, PATHSEPCHAR);
                        if (p)				// %b=file base name
                        { strcat(line, p+1);
                          p = strrchr(line, '.');
                          if (p) *p=0;
                        }
                        break;
              case 'p': strcat(line, pszFileName);	// %p=file path
                        p = strrchr(line, PATHSEPCHAR);
                        if (p) *p=0;
                        break;
              case 'n': strcat(line, "\n");		// %n=newline
                        break;
              case '%': strcat(line, "%");		// %%=%
                        break;
              case 'v': 				// %Vxxxx,yyyy=any vr (group and element must have 4 digits)
		        char result[256];		// %V*xxxx,yyyy=any vr in any sequence
                        if (!DDO) DDO = PDU2.LoadDICOMDataObject(pszFileName);
	      	        i += SearchDICOMObject(DDO, szExecName+i+2, result);
                        strcat(line, result);
                        break;

              // case 'q': not (yet) supported for export converters - probably no need
            }
            i += 2;

  	    // get substring of variable
            if (szExecName[i]=='[')
            { int a=0, b=-1;
              sscanf(szExecName+i, "[%d,%d]", &a, &b);
              for (;i<L;i++) if (szExecName[i-1]==']') break;
              if (a>strlen(ps)) a=strlen(ps);
              if (b>strlen(ps)) b=strlen(ps);
              if (b>=a) {memmove(ps, ps+a, b-a+1); ps[b-a+1]=0;}
            }
          }
          else
          { l = strlen(line);
            line[l]   = szExecName[i++];
            line[l+1] = 0;
          }
        }
  
        // if (DDO) delete DDO;
      }
      else
	strcpy(line, szExecName);

      /* converter: direct forward of received object */

      if (memicmp(line, "forward to", 10)==0 ||
          memicmp(line, "forward compressed", 18)==0
         )
      { char host[64], port[64], compress[64], Level[64], Temp[66];
        char *p;
        RunTimeClassStorage	RTCStorage;
        UID			AppUID ("1.2.840.10008.3.1.1.1");
        UID			iUID, uid;
        VR			*vr;
        int			offset = 11, level;
  
        // fallback if none passed: forces one association per image
        if (!PDU) PDU = &PDU2;
  
        // preprocess the forwarder string
        if (memicmp(line, "forward compressed as ", 22)==0) offset = 28;
  					       // xx to	where xx = 'is', 'un', 'n0'..'n4', or 'j0'..'j6'
  
        if(!GetACRNema(line+offset, (char *)host, (char *)port, (char *)compress))
        { strcpy(host, line+offset);
          p = strchr(host, ':');
          if (p) 
          { *p=0;
            strcpy(port, p+1);
          }
          else 
            strcpy(port, "5678");
        }
  
        /// offer transfer syntax (compression) from forward string
        if (offset==28)
        { compress[0] = line[22];
          compress[1] = line[23];
          compress[2] = 0;
        }
  
        OperatorConsole.printf("ExportConverter%d.%d: forward %s to %s\n", N, part, pszFileName, line+offset);
  
        // load and check the data
        if (!DDO) 
          DDO = PDU2.LoadDICOMDataObject(pszFileName);

        if (!DDO)
        { OperatorConsole.printf("*** ExportConverter%d.%d: Forward failed to load DICOM object %s\n", N, part, pszFileName);
          return FALSE;
        }
  
        // get UID at selected ForwardAssociationLevel into szTemp
        vr = NULL;
        level = 0;
        GetPrivateProfileString(szRootSC, "ForwardAssociationLevel", "STUDY", Level, 64, ConfigFile);
        if      (strncmp(Level, "PATIENT", 7)==0) level=1;
        else if (strncmp(Level, "STUDY",   5)==0) level=2;
        else if (strncmp(Level, "SERIES",  6)==0) level=3;
        else if (strncmp(Level, "IMAGE",   5)==0) level=4;
        else if (strncmp(Level, "SOPCLASS",8)==0) level=5, PDU[part].ClearAbstractSyntaxs();
        else if (strncmp(Level, "GLOBAL",  6)==0) level=6;
        szTemp[0] = 0;
        if (level==1) vr = DDO->GetVR(0x0010, 0x0020);
        if (level==2) vr = DDO->GetVR(0x0020, 0x000d);
        if (level==3) vr = DDO->GetVR(0x0020, 0x000e);
        if (level==4) vr = DDO->GetVR(0x0008, 0x0018);
        if (level==5) vr = DDO->GetVR(0x0008, 0x0016);
        if (vr) memcpy(szTemp, vr->Data, vr->Length);
  
        // get sopclass (to check whether it is accepted at the current connection)
        vr = DDO -> GetVR(0x0008, 0x0016);
        if (!vr)
        { OperatorConsole.printf("*** ExportConverter%d.%d: Forward failed because SopClass is missing in %s\n", N, part, pszFileName);
          delete DDO;
          return FALSE;
        }
  
        // does the existing link accept images of this type - if not then hang up to force a reconnect
        // also hang up when UID at ForwardAssociationLevel changes to start a new association
        if (PDU[part].Link.Connected)
        { SetUID ( iUID, vr );
          if (!PDU[part].IsAbstractSyntaxAccepted(iUID) || strcmp(ForwardLastUID+part*66, szTemp)!=0 )
          { //OperatorConsole.printf("!!! ExportConverter%d.%d: attempt to reconnect %s \n", N, part, szTemp);
            GetPrivateProfileString(szRootSC, "ForwardAssociationRelease", "1", Temp, 64, ConfigFile);
            if (atoi(Temp)) PDU[part].Close();
  	    else            PDU[part].Link.Close();
          }
        }
  
        // for new check of UID at ForwardAssociationLevel
        strcpy(ForwardLastUID+part*66, szTemp);
  
        // (re)connect
        if (!PDU[part].Link.Connected)
        { PDU[part].AttachRTC(&VRType);
          PDU[part].SetRequestedCompressionType(compress);
  
          PDU[part].SetApplicationContext ( AppUID );
          PDU[part].SetLocalAddress ( MYACRNEMA );
          PDU[part].SetRemoteAddress ( (unsigned char *)(line+offset) );
          PDU[part].SetTimeOut(TCPIPTimeOut);
  
          vr = DDO -> GetVR(0x0008, 0x0016);
          SetUID ( iUID, vr );
          PDU[part].AddAbstractSyntax ( iUID );		// adds type of this image to presentation contexts
    
          uid.Set("1.2.840.10008.1.1");		// This one should always accept: verification
          PDU[part].AddAbstractSyntax(uid);		// assures connect will not return FALSE because image is not accepted
    
          if (!PDU[part].Connect((unsigned char *)&host, (unsigned char *)&port))
          { OperatorConsole.printf("*** ExportConverter%d.%d: Forward failed to connect to host %s\n", N, part, line+offset);
            delete DDO;
            return TRUE; // may be good idea to retry later
          }
        }
  
        vr = DDO -> GetVR(0x0008, 0x0016);
        SetUID ( iUID, vr );
        if (!PDU[part].IsAbstractSyntaxAccepted(iUID))
        { OperatorConsole.printf("*** ExportConverter%d.%d: DICOM server %s does not accept type of forwarded image\n", N, part, line+offset);
          PDU[part].Close();
          delete DDO;
          return FALSE;
        }
  
        // recompress data to be forwarded here according to accepted compression mode; strip group 2 unless "as" or "is"
        p = PDU[part].GetAcceptedCompressionType(iUID);
        StripGroup2 = memicmp(p, "as", 2)!=0 && memicmp(p, "is", 2)!=0;
        recompress(&DDO, p, "", StripGroup2);
  
        RTCStorage.SetUID(iUID);
  
        if (!RTCStorage.Write(PDU+part, DDO))
        { OperatorConsole.printf("*** ExportConverter%d.%d: Forward failed to send DICOM image to %s\n", N, part, line+offset);
          PDU[part].Close();
          delete DDO;
          return FALSE; // NOT a good idea to retry later
        }
      
        ImagesForwarded++;
        if (level==4) 
        { GetPrivateProfileString(szRootSC, "ForwardAssociationRelease", "1", Temp, 64, ConfigFile);
          if (atoi(Temp)) PDU[part].Close();
          else		  PDU[part].Link.Close();
        }

        delete DDO;	// write was destructive
        DDO = NULL;
      }
  
      /* converter: write "string" to file - uses text file, use %n for newline */
  
      else if (memicmp(line, "write \"", 7)==0)
      { char string[256];
        char *file;
        FILE *f;
        char *p;
  
        strcpy(string, line+7);
        p = strstr(string, "\" to ");
        if (!p) p = strstr(string, "\" TO ");
        if (p)
        { *p=0;
  
          OperatorConsole.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
  
          file = p+5;
          f = fopen(file, "wt");
          if (f)
          { fputs(string, f);
            fclose(f);
          }
          else
            OperatorConsole.printf("*** Exportconverter%d.%d: Failed to write to file %s\n", N, part, file);
        }
      }
  
      /* converter: append "string" to file - uses text file, use %n for newline */
  
      else if (memicmp(line, "append \"", 8)==0)
      { char string[256];
        char *file;
        FILE *f;
        char *p;
  
        strcpy(string, line+8);
        p = strstr(string, "\" to ");
        if (!p) p = strstr(string, "\" TO ");
        if (p)
        { *p=0;
  
          OperatorConsole.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
  
          file = p+5;
          f = fopen(file, "at");
          if (f)
          { fputs(string, f);
            fclose(f);
          }
          else
            OperatorConsole.printf("*** Exportconverter%d.%d: Failed to append to file %s\n", N, part, file);
        }
      }
  
      /* converter: copy file to file; copy file to directory */
  
      else if (memicmp(line, "copy ", 5)==0)
      { char string[1024];
        char *file;
        struct stat statbuf;
        char *p;
        
	strcpy(string, line+5);
        p = strstr(string, " to ");
        if (!p) p = strstr(string, " TO ");
        if (p)
        { *p=0;
  
          OperatorConsole.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
  
          file = p+4;
  
  	  /* if the destination a directory then append the source filename to it */
  
          stat(file, &statbuf);
          if (statbuf.st_mode & S_IFDIR)
          { p = strrchr(pszFileName, PATHSEPCHAR);
            if (p)
            { if (file[strlen(file)-1]==PATHSEPCHAR) 
                file[strlen(file)-1]=0;
              strcat(file, p);
            }
          }
  
          if (!DFileCopy2(string, file, 0))
            OperatorConsole.printf("*** Exportconverter%d.%d: Failed to copy %s to %s\n", N, part, string, file);
  	else
  	  ImagesCopied++;
        }
      }

      /* converter: ifnotempty "string" (only execute next converter if string not empty) */
    
      else if (memicmp(line, "ifnotempty \"", 12)==0)	
      { char string[1024];
        char *p;
    
        strcpy(string, line+12);
        p = strrchr(string, '"');
        if (p)
        { *p=0;

          SystemDebug.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
    
          int len = strlen(string);
          if (len==0) rc = 4;
        }
      }
    
      /* converter: ifempty "string" (only execute next converter if string empty) */
    
      else if (memicmp(line, "ifempty \"", 9)==0)	
      { char string[1024];
        char *p;
    
        strcpy(string, line+9);
        p = strrchr(string, '"');
        if (p)
        { *p=0;
    
          SystemDebug.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
    
          int len = strlen(string);
          if (len!=0) rc = 4;
        }
      }
  
      /* converter: ifequal "string1","string2" (only execute next converter if string1 equals string2) */
    
      else if (memicmp(line, "ifequal \"", 9)==0)	
      { char string[1024];
        char *p, *string2;
    
        strcpy(string, line+9);
        p = strrchr(string, '"');
        if (p)
        { *p=0;

          p = strstr(string, "\",\"");
          if (!p) p = strstr(string, "\", \"");
	  if (p) 
          { string2 = strchr(p+1, '"')+1;
            *p=0;
            SystemDebug.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
            int cmp = strcmp(string, string2);
            if (cmp!=0) rc = 4;
          }
        }
      }
 
      /* converter: ifnotequal "string1","string2" (only execute next converter if string1 not equals string2) */
    
      else if (memicmp(line, "ifnotequal \"", 12)==0)	
      { char string[1024];
        char *p, *string2;
    
        strcpy(string, line+12);
        p = strrchr(string, '"');
        if (p)
        { *p=0;

          p = strstr(string, "\",\"");
          if (!p) p = strstr(string, "\", \"");
	  if (p) 
          { string2 = strchr(p+1, '"')+1;
            *p=0;
            SystemDebug.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
            int cmp = strcmp(string, string2);
            if (cmp==0) rc = 4;
          }
        }
      }

      /* converter: ifmatch "string1","string2" (only execute next converter if string1 matches string2 - case insensitive) */
    
      else if (memicmp(line, "ifmatch \"", 9)==0)	
      { char string[1024];
        char *p, *string2;
    
        strcpy(string, line+9);
        p = strrchr(string, '"');
        if (p)
        { *p=0;

          p = strstr(string, "\",\"");
          if (!p) p = strstr(string, "\", \"");
	  if (p) 
          { string2 = strchr(p+1, '"')+1;
            *p=0;
            SystemDebug.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
            if (!match(string, string2)) rc = 4;
          }
        }
      }

      /* converter: ifnotmatch "string1","string2" (only execute next converter if string1 not matches string2 - case insensitive) */
    
      else if (memicmp(line, "ifnotmatch \"", 12)==0)	
      { char string[1024];
        char *p, *string2;
    
        strcpy(string, line+12);
        p = strrchr(string, '"');
        if (p)
        { *p=0;

          p = strstr(string, "\",\"");
          if (!p) p = strstr(string, "\", \"");
	  if (p) 
          { string2 = strchr(p+1, '"')+1;
            *p=0;
            SystemDebug.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
            if (match(string, string2)) rc = 4;
          }
        }
      }

      /* converter: ifnumequal "string1","string2" (only execute next converter if string1 num equals string2 - integer) */
    
      else if (memicmp(line, "ifnumequal \"", 12)==0)	
      { char string[1024];
        char *p, *string2;
    
        strcpy(string, line+12);
        p = strrchr(string, '"');
        if (p)
        { *p=0;

          p = strstr(string, "\",\"");
          if (!p) p = strstr(string, "\", \"");
	  if (p) 
          { string2 = strchr(p+1, '"')+1;
            *p=0;
            SystemDebug.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
            if (atoi(string)!=atoi(string2)) rc = 4;
          }
        }
      }

      /* converter: ifnumnotequal "string1","string2" (only execute next converter if string1 num not equals string2 - integer) */
    
      else if (memicmp(line, "ifnumnotequal \"", 15)==0)	
      { char string[1024];
        char *p, *string2;
    
        strcpy(string, line+15);
        p = strrchr(string, '"');
        if (p)
        { *p=0;

          p = strstr(string, "\",\"");
          if (!p) p = strstr(string, "\", \"");
	  if (p) 
          { string2 = strchr(p+1, '"')+1;
            *p=0;
            SystemDebug.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
            if (atoi(string)==atoi(string2)) rc = 4;
          }
        }
      }

      /* converter: ifnumgreater "string1","string2" (only execute next converter if string1 num greater than string2 num - integer) */
    
      else if (memicmp(line, "ifnumgreater \"", 14)==0)	
      { char string[1024];
        char *p, *string2;
    
        strcpy(string, line+14);
        p = strrchr(string, '"');
        if (p)
        { *p=0;

          p = strstr(string, "\",\"");
          if (!p) p = strstr(string, "\", \"");
	  if (p) 
          { string2 = strchr(p+1, '"')+1;
            *p=0;
            SystemDebug.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
            if (atoi(string)<=atoi(string2)) rc = 4;
          }
        }
      }

      /* converter: ifnumless "string1","string2" (only execute next converter if string1 num less than string2 num - integer) */
    
      else if (memicmp(line, "ifnumless \"", 11)==0)	
      { char string[1024];
        char *p, *string2;
    
        strcpy(string, line+11);
        p = strrchr(string, '"');
        if (p)
        { *p=0;

          p = strstr(string, "\",\"");
          if (!p) p = strstr(string, "\", \"");
	  if (p) 
          { string2 = strchr(p+1, '"')+1;
            *p=0;
            SystemDebug.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
            if (atoi(string)>=atoi(string2)) rc = 4;
          }
        }
      }

      /* converter: between "string1","string2" (execute next converter on time in hours: between "9", "17"; defer) */
    
      else if (memicmp(line, "between \"", 9)==0)	
      { char string[1024];
        char *p, *string2;
        int h1, h2, h;
	struct tm tmbuf;
        time_t t = time(NULL);

        localtime_r(&t, &tmbuf);
        h = tmbuf.tm_hour;
	
        strcpy(string, line+9);
        h1 = atoi(string);
        p = strrchr(string, '"');
        if (p)
        { *p=0;

          p = strstr(string, "\",\"");
          if (!p) p = strstr(string, "\", \"");
	  if (p) 
          { string2 = strchr(p+1, '"')+1;
            *p=0;
            h2 = atoi(string2);
            SystemDebug.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
            if (h2>=h1 && (h <h1 || h>=h2)) rc = 4;	// between "9", "17": skip if h<9 or h>=17
            if (h2< h1 && (h <h1 && h>=h2)) rc = 4;	// between "17", "9": skip if h<=17 and h>=9
          }
        }
      }

      /* converter: prefetch */
  
      else if (memicmp(line, "prefetch", 8)==0)
      { if (prefetch_queue("prefetch", patid, "", "", "", "", "", "", "", "", ""))
          OperatorConsole.printf("Exportconverter%d.%d: queued prefetch %s\n", N, part, patid);
      }
  
      /* converter: delayed "[forward|get] [patient|study|series|image] [compressed as xx] 
                             [date yyyymmdd-yyyymmdd] [now -ddd+ddd] [age -ddd+ddd] 
                             [modality mm] [sop xxxx] [imagetype xxxx] [seriesdesc xxxx] [to/from AE]" 
         and: delayed "delete [patient|study|series|image] [date yyyymmdd-yyyymmdd] [now -ddd+ddd] 
			      [age -ddd+ddd] [modality mm] [sop xxxx] [imagetype xxxx] [seriesdesc xxxx]"
         and: delayed "process [patient|study|series|image] [by command]"

         Will process data only from the current patient, but filters 'now, age, modality, sop, imagetype, seriesdesc' 
	 will read from other studies and series
      */
  
      else if (memicmp(line, "forward patient ",  16)==0  ||
               memicmp(line, "forward study ",    14)==0  ||
               memicmp(line, "forward series ",   15)==0  ||
               memicmp(line, "forward image ",    14)==0  ||
               memicmp(line, "get patient ",      12)==0  ||
               memicmp(line, "get study  ",       10)==0  ||
               memicmp(line, "get series ",       11)==0  ||
               memicmp(line, "get image ",        10)==0  ||
               memicmp(line, "delete patient ",   15)==0  ||
               memicmp(line, "delete study  ",    13)==0  ||
               memicmp(line, "delete series ",    14)==0  ||
               memicmp(line, "delete image ",     13)==0  ||
               memicmp(line, "process patient ",  16)==0  ||
               memicmp(line, "process study  ",   14)==0  ||
               memicmp(line, "process series ",   15)==0  ||
               memicmp(line, "process image ",    14)==0)
      { char *level = strchr(line, ' ')+1; char *compress="", *dest="", *date="", *modality="", *sop="", *imagetype="", *seriesdesc="";
        char studyuid[65], seriesuid[65], vr[200], dat[200];
        VR *pVR;
        int len;

        studyuid[0]=0;
        seriesuid[0]=0;

        if (!DDO) 
          DDO = PDU2.LoadDICOMDataObject(pszFileName);

        char *p=level;
        p = strchr(p, ' ');
        while(p)
        { *p=0;
          p++;
  
          if (memicmp(p, "compressed as ", 14)==0) 
          { compress = p+14; 
            p = strchr(p+14, ' '); 
          }
          else if (memicmp(p, "to ", 3)==0) 
          { dest = p+3; 
            p = strchr(p+3, ' '); 
          }
          else if (memicmp(p, "by ", 3)==0) 
          { dest = p+3; 
            p = strchr(p+3, ' '); 
          }
          else if (memicmp(p, "from ", 5)==0) 
          { dest = p+5; 
            p = strchr(p+5, ' '); 
          }
          else if (memicmp(p, "date ", 5)==0) 	// date yyyymmdd or yyyymmdd-yyyymmdd
          { date = p+5; 
            p = strchr(p+5, ' '); 
            level = "selection";		// causes that no UID is passed to query
          }
          else if (memicmp(p, "now ", 4)==0) 	// now -365+365: within one year from now (+/- compulsory)
	  { struct tm tmbuf1, tmbuf2;
	    char *q;
            int from=0, to=0;
	    from=atoi(p+4);
            q = strchr(p+5, '+'); if (!q) q = strchr(p+5, '-'); if (q) to=atoi(q);
		
	    time_t t1 = time(NULL)+24*3600*from;
	    time_t t2 = time(NULL)+24*3600*to;
            localtime_r(&t1, &tmbuf1);
            localtime_r(&t2, &tmbuf2);

	    sprintf(dat, "%04d%02d%02d-%04d%02d%02d", tmbuf1.tm_year+1900, tmbuf1.tm_mon, tmbuf1.tm_mday, tmbuf2.tm_year+1900, tmbuf2.tm_mon, tmbuf2.tm_mday);
	    date = dat;

            p = strchr(p+4, ' '); 
            level = "selection";		// causes that no UID is passed to query
          }
          else if (memicmp(p, "age ", 4)==0) 	// age -365+365: within one year from series date (+/- compulsory)
	  { struct tm tmbuf1, tmbuf2;
	    char *q;
            int from=0, to=0;
	    from=atoi(p+4);
            q = strchr(p+5, '+'); if (!q) q = strchr(p+5, '-'); if (q) to=atoi(q);

            SearchDICOMObject(DDO, "0008,0020", dat);
            memset(&tmbuf1, 0, sizeof(tmbuf1));
	    sscanf(dat, "%04d%02d%02d", &tmbuf1.tm_year, &tmbuf1.tm_mon, &tmbuf1.tm_mday);
            tmbuf1.tm_year -= 1900;

            time_t t = mktime(&tmbuf1);
	    time_t t1 = t+24*3600*from;
	    time_t t2 = t+24*3600*to;
            localtime_r(&t1, &tmbuf1);
            localtime_r(&t2, &tmbuf2);

	    sprintf(dat, "%04d%02d%02d-%04d%02d%02d", tmbuf1.tm_year+1900, tmbuf1.tm_mon, tmbuf1.tm_mday, tmbuf2.tm_year+1900, tmbuf2.tm_mon, tmbuf2.tm_mday);
	    date = dat;

            p = strchr(p+4, ' '); 
            level = "selection";		// causes that no UID is passed to query
          }
          else if (memicmp(p, "modality ", 9)==0) 
          { modality = p+9; 
            p = strchr(p+9, ' '); 
            level = "selection";		// causes that no UID is passed to query
          }
          else if (memicmp(p, "sop ", 4)==0) 
          { sop = p+4; 
            p = strchr(p+4, ' '); 
            level = "selection";		// causes that no UID is passed to query except the given SOP
          }
          else if (memicmp(p, "imagetype ", 10)==0) 
          { imagetype = p+10; 
            p = strchr(p+10, ' '); 
            level = "selection";		// causes that no UID is passed to query
          }
          else if (memicmp(p, "seriesdesc ", 11)==0) 
          { seriesdesc = p+11; 
            p = strchr(p+11, ' '); 
            level = "selection";		// causes that no UID is passed to query
          }
          else if (memicmp(p, "series ", 7)==0) 
          { char *a=p+7;
            p = strchr(p+7, ' ');
	    *p=0;
	    strcpy(seriesuid, a);
            level = "selection";		// causes that no UID is passed to query
          }
          else if (memicmp(p, "study ", 6)==0) 
          { char *a=p+6;
            p = strchr(p+6, ' ');
            *p=0;
	    strcpy(studyuid, a);
            level = "selection";		// causes that no UID is passed to query
          }
        }

        if (DDO)
        { pVR = DDO->GetVR(0x0020, 0x000d);
          if (pVR && (memicmp(level, "stu", 3)==0 || memicmp(level, "ser", 3)==0) || memicmp(level, "ima", 3)==0)
          { strncpy(vr, (char*)pVR->Data, pVR->Length);
            vr[pVR->Length] = 0;
            len = pVR->Length - 1;
            while(len>0)
            { if (vr[len] == ' ')
                vr[len] = 0;
              else
                break;
              len--;
            }
            strcpy(studyuid, vr);
          }

          pVR = DDO->GetVR(0x0020, 0x000e);
          if (pVR && (memicmp(level, "ser", 3)==0 || memicmp(level, "ima", 3)==0))
          { strncpy(vr, (char*)pVR->Data, pVR->Length);
            vr[pVR->Length] = 0;
            len = pVR->Length - 1;
            while(len>0)
            { if (vr[len] == ' ')
                vr[len] = 0;
              else
                break;
              len--;
            }
            strcpy(seriesuid, vr);
          }

          pVR = DDO->GetVR(0x0008, 0x0018);
          if (pVR && memicmp(level, "ima", 3)==0)
          { strncpy(vr, (char*)pVR->Data, pVR->Length);
            vr[pVR->Length] = 0;
            len = pVR->Length - 1;
            while(len>0)
            { if (vr[len] == ' ')
                vr[len] = 0;
              else
                break;
              len--;
            }
            sop = vr;
          }
        }

	line[11]=0;
	if (sop) level = "single object";
	if (prefetch_queue(line, patid, dest, studyuid, seriesuid, compress, modality, date, sop, imagetype, seriesdesc))
          OperatorConsole.printf("Exportconverter%d.%d: queued %s - (%s %s %s of %s) to %s\n", N, part, line, level, modality, date, patid, dest);
      }
  
      /* converter: preretrieve */
  
      else if (memicmp(line, "preretrieve ", 12)==0)
      { if (prefetch_queue("preretrieve", patid, line+12, "", "", "", "", "", "", "", ""))
          OperatorConsole.printf("Exportconverter%d.%d: queued preretrieve of patient %s from %s\n", N, part, patid, line+12);
      }
  
      /* converter: nop with % item */

      else if (memicmp(line, "nop", 3)==0)
      { OperatorConsole.printf("Exportconverter%d.%d: %s\n", N, part, line);
      }

      else if (memicmp(line, "stop", 4)==0)
      { SystemDebug.printf("Exportconverter%d.%d: stop\n", N, part);
        rc = 3;	
      }

      else if (memicmp(line, "silentstop", 10)==0)
      { rc = 3;	
      }

      else if (memicmp(line, "defer", 5)==0)
      { SystemDebug.printf("Exportconverter%d.%d: defer\n", N, part);
        reset_queue_fails(N);
        if (DDO) delete DDO;
        return TRUE; // retry later
      }

        /* converter: prefetch */

      else if (memicmp(line, "prefetch", 8)==0)
      { if (prefetch_queue("prefetch", patid, "", "", "", "", "", "", "", "", ""))
          OperatorConsole.printf("Exportconverter%d.%d: queued prefetch %s\n", N, part, patid);
      }

      /* default: execute the command */

      else if (strchr(szExecName, '%'))
      {
        OperatorConsole.printf("Exportconverter%d.%d executes: %s\n", N, part, line);
  
  #ifdef WIN32
        WinExec(line, SW_MINIMIZE);     
  #else
  //      char *s=strchr(line, ' ');
  //      *s=0;
  //      BackgroundExec(line, s+1);
        system(line);
  #endif
        ImagesExported++;
      }

      /* for backwards compatibility: execute executable with single parameter */
  
      else
#ifdef WIN32
      { int r = spawnlpe(_P_WAIT, szExecName,
                    szExecName,		/* argv[0] */
                    pszFileName,		/* argv[1] */
                    NULL,			/* Allways NULL */
                    NULL);		/* Environment */
        if (r == -1)
          OperatorConsole.printf("***Exportconverter%d: Spawning '%s' failed (argv=%s)\n", N, szExecName, pszFileName);
        ImagesExported++;
      }
#else
  //    BackgroundExec(szExecName, pszFileName);
      { char line[1000];
        sprintf(line, "%s %s", szExecName, pszFileName);
        system(line);
        ImagesExported++;
      }   
#endif
    }

    // process next command
    if (szNext[0]==0) break;
    strcpy(szExecName, szNext);
    part++;

    if (rc==3) break;
  }

  if (DDO) delete DDO;
  return FALSE;
}

void CallExportConverters(char *pszFileName, char *pszModality, char *pszStationName, char *pszSop, char *patid, char *calling, char *called)
{ char		szRootSC[64];
  char		szEntry[32];
  char		szTemp[32];
  int		i, iNbConverters;

  if (!GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig,
		szRootSC, 64, ConfigFile)) return;
  if (!GetPrivateProfileString(szRootSC, "ExportConverters", "0",
		szTemp, 128, ConfigFile)) return;
  iNbConverters = atoi(szTemp);
  if (iNbConverters>MAXExportConverters) iNbConverters=MAXExportConverters;

  /* Loop over all converters */
  for(i=0; i<iNbConverters; i++)
    CallExportConverterN(pszFileName, i, pszModality, pszStationName, pszSop, patid, calling, called, NULL, NULL);
}

// forward reference
void SetStringVR(VR **vr, int g, int e, char *String);
static void NewTempFile(char *name, char *ext);

// external reference from nkiqrsop.cxx
void SaveDICOMDataObject(char *Filename, DICOMDataObject* pDDO);

static VR *VariableVRs[3] = {NULL, NULL, NULL}; // variables x, y, z

// operates synchronously on image BEFORE it is stored in the database
// For general scripting use:
// CallImportConverterN(DDO, -1, NULL, NULL, NULL, NULL, NULL, NULL, script);

int CallImportConverterN(DICOMDataObject *DDO, int N, char *pszModality, char *pszStationName, char *pszSop, char *patid, char *calling, char *called, char *Storage)
{ char		szRootSC[64];
  char		szEntry[32];
  char		szTemp[66];
  int		i, MaxLength;
  char		szExecName[512], szNext[512];
  char		szExecModality[66];
  char		szExecStationName[66];
  char		szExecFilter[512];
  char		ImportCalledAE[18], ImportCallingAE[18], Called[18], Calling[18];
  int           rc, part, skipping;

  if (!GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig,
		szRootSC, 64, ConfigFile)) return FALSE;

  /* filter using modality, stationname, calling and called */

  if (pszModality)
  { sprintf(szEntry, "ImportModality%d", N);
    if (!GetPrivateProfileString(szRootSC, szEntry, "*", szExecModality, 64, ConfigFile)) return FALSE;
    if (!match(pszModality, szExecModality)) return FALSE;
  }

  if (pszStationName)
  { sprintf(szEntry, "ImportStationName%d", N);
    if (!GetPrivateProfileString(szRootSC, szEntry, "*", szExecStationName, 64, ConfigFile)) return FALSE;
    if (!match(pszStationName, szExecStationName)) return FALSE;
  }

  if (calling)
  { sprintf(szEntry, "ImportCallingAE%d", N);
    if (!GetPrivateProfileString(szRootSC, szEntry, "*", ImportCallingAE, 18, ConfigFile)) return FALSE;
    if (!match(calling, ImportCallingAE)) return FALSE;
  }

  if (called)
  { sprintf(szEntry, "ImportCalledAE%d", N);
    if (!GetPrivateProfileString(szRootSC, szEntry, "*", ImportCalledAE, 18, ConfigFile)) return FALSE;
    if (!match(called, ImportCalledAE)) return FALSE;
  }

  /* get the import converter name */

  if (called && calling)
  { sprintf(szEntry, "ImportConverter%d", N);
    if (!GetPrivateProfileString(szRootSC, szEntry, "silentstop",
          szExecName, 512, ConfigFile)) return FALSE;
  }
  else
  { strcpy(szExecName, Storage);
  }

  /* only import converters using %x syntax are allowed */

  rc       = 1;	// status for if statement, destroy and stop
  part     = 0;
  skipping = 0;

  while(1)
  { char line[1000], vr[256];
    int i=0, l, L, len;
    unsigned int g, e;
    char *p;
    VR *pVR, *newVR;
    DICOMDataObject DDO2;
    Array < DICOMDataObject * > ADDO;
    char level;

    szNext[0] = 0;

    if (skipping || rc==4)			// {} block being skipped
    { if      (szExecName[0]=='{') skipping++, rc=0;
      else if (szExecName[0]=='}') skipping--, rc=0;
    }
        
    // find ; not in string: splits import commands
    int instring=0;
    L = strlen(szExecName);
    for (int k=0; k<L; k++) 
    { if (szExecName[k]=='"') instring = !instring;
      if (szExecName[k]==';' && instring==0)
      { szExecName[k] = 0;			// split command
        while (szExecName[k+1]==' ') k++;	// skip spaces after ;
        strcpy(szNext, szExecName+k+1);		// remaining for next
        break;
      }
    }

    /* substitute %x variables and remove possible { and } at begin of statement */

    line[0] = 0;

    L = strlen(szExecName);
    while (i<L)
    { if      (i==0 && szExecName[i] == '{') {i++; while(szExecName[i]==' ')i++;}
      else if (i==0 && szExecName[i] == '}') {i++; while(szExecName[i]==' ')i++;}
      else if (szExecName[i] == '%')
      { char *ps = line + strlen(line);

        switch(tolower(szExecName[i+1]))
        { case 'm': if (pszModality)
                      strcat(line, pszModality);   // %m=modality
                    else
		    { char result[256];
	      	      SearchDICOMObject(DDO, "0008,0060", result);
                      strcat(line, result);
                    }
                    break;
          case 's': if (pszStationName)
                      strcat(line, pszStationName);// %s=stationname
                    else
		    { char result[256];
	      	      SearchDICOMObject(DDO, "0008,1110", result);
                      strcat(line, result);
                    }
                    break;
          case 'o': if (pszSop)
                      strcat(line, pszSop);	  // %o=object instance
                    else
		    { char result[256];
	      	      SearchDICOMObject(DDO, "0008,0018", result);
                      strcat(line, result);
                    }
                    break;
          case 'i': if (patid)
                      strcat(line, patid);	  // %i=patient ID
                    else
		    { char result[256];
	      	      SearchDICOMObject(DDO, "0010,0020", result);
                      strcat(line, result);
                    }
                    break;
          case 'u': Calling[0]=0;
		    if (calling) strcpy(Calling, calling);
        	    while (strlen(Calling)>0 && Calling[strlen(Calling)-1]==' ') Calling[strlen(Calling)-1] = 0;
		    strcat(line, Calling);	// %u=SCU
                    break;
          case 'c': Called[0]=0;
		    if (called) strcpy(Called, called);
        	    while (strlen(Called)>0 && Called[strlen(Called)-1]==' ') Called[strlen(Called)-1] = 0;
		    strcat(line, Called);	// %c=calledAE = scp
                    break;
          case 'd': time_t TimeOfDay;		// %d=date and time
		    char TimeString[128], buf[64];
		    TimeOfDay = time(NULL);
		    strcpy(TimeString, ctime_r(&TimeOfDay, buf));
		    TimeString[strlen(TimeString)-1] = '\0';
                    strcat(line, TimeString);
                    break;
          case 'n': strcat(line, "\n");		// %n=newline
                    break;
          case '%': strcat(line, "%");		// %%=%
                    break;
          case 'x': 				// %x, %y, %z are general purpose variables
          case 'y': 
          case 'z': 
                    pVR = VariableVRs[tolower(szExecName[i+1])-'x'];
                    if (!pVR) pVR = new VR;
                    VariableVRs[tolower(szExecName[i+1])-'x'] = pVR;
	            strncpy(vr, (char*)pVR->Data, pVR->Length);
	            vr[pVR->Length] = 0;
	            len = pVR->Length - 1;
                    while(len>0)
                    { if (vr[len] == ' ')
	                vr[len] = 0;
                      else
                        break;
                      len--;
                    }
                    strcat(line, vr);
                    break;
          case 'v': 				// %Vxxxx,yyyy=any vr (group and element must have 4 digits)
		    char result[256];		// %V*xxxx,yyyy=any vr in any sequence
	      	    i += SearchDICOMObject(DDO, szExecName+i+2, result);
                    strcat(line, result);
                    break;

	  case 'q': level = szExecName[i+2];	// %QXxxxx,yyyy=from db (group and element must have 4 digits)
                    g = htoin(szExecName+i+3, 4);
	            e = htoin(szExecName+i+8, 4);

		    switch(level)
                    { case 'p':
                      case 'P': 		// %QPxxxx,yyyy=query from patient db on patid
		        SetStringVR(&pVR, 0x0008, 0x0052, "PATIENT"); 
	                DDO2.Push(pVR); 
		        pVR = DDO->GetVR(0x0010, 0x0020);
			if (pVR && pVR->Length)
			{ newVR = new VR(pVR->Group, pVR->Element, pVR->Length, (BOOL) TRUE);
			  memcpy(newVR->Data, pVR->Data, pVR->Length);
	                  DDO2.Push(newVR);
                          SetStringVR(&pVR, g, e, ""); 
	                  DDO2.Push(pVR);
                          QueryOnPatient (&DDO2, &ADDO);
                        }
			break;
		      case 's':
                      case 'S': 		// %QSxxxx,yyyy=from study db on patid and study uid
		        SetStringVR(&pVR, 0x0008, 0x0052, "STUDY"); 
	                DDO2.Push(pVR); 
		        pVR = DDO->GetVR(0x0010, 0x0020);
			if (pVR && pVR->Length)
			{ newVR = new VR(pVR->Group, pVR->Element, pVR->Length, (BOOL) TRUE);
  			  memcpy(newVR->Data, pVR->Data, pVR->Length);
	                  DDO2.Push(newVR);
		          pVR = DDO->GetVR(0x0020, 0x000d);
			  if (pVR && pVR->Length)
			  { newVR = new VR(pVR->Group, pVR->Element, pVR->Length, (BOOL) TRUE);
			    memcpy(newVR->Data, pVR->Data, pVR->Length);
	                    DDO2.Push(newVR);
                            SetStringVR(&pVR, g, e, ""); 
	                    DDO2.Push(pVR);
                            QueryOnStudy (&DDO2, &ADDO);
                          }
                        }
			break;
		      case 'e':
                      case 'E': 		// %QExxxx,yyyy=from series db on patid, study and series uid
		        SetStringVR(&pVR, 0x0008, 0x0052, "SERIES"); 
	                DDO2.Push(pVR); 
		        pVR = DDO->GetVR(0x0010, 0x0020);
			if (pVR && pVR->Length)
			{ newVR = new VR(pVR->Group, pVR->Element, pVR->Length, (BOOL) TRUE);
			  memcpy(newVR->Data, pVR->Data, pVR->Length);
	                  DDO2.Push(newVR);
		          pVR = DDO->GetVR(0x0020, 0x000d);
			  if (pVR && pVR->Length)
			  { newVR = new VR(pVR->Group, pVR->Element, pVR->Length, (BOOL) TRUE);
			    memcpy(newVR->Data, pVR->Data, pVR->Length);
	                    DDO2.Push(newVR);
		            pVR = DDO->GetVR(0x0020, 0x000e);
			    if (pVR && pVR->Length)
                            { newVR = new VR(pVR->Group, pVR->Element, pVR->Length, (BOOL) TRUE);
			      memcpy(newVR->Data, pVR->Data, pVR->Length);
	                      DDO2.Push(newVR);
                              SetStringVR(&pVR, g, e, ""); 
	                      DDO2.Push(pVR);
                              QueryOnSeries (&DDO2, &ADDO);
                            }
                          }
                        }
			break;
                      case 'w':
                      case 'W': 		// %QWxxxx,yyyy=from worklist db on accession number
		        SetStringVR(&pVR, 0x0008, 0x0052, "MODALITYWORKLIST"); 
	                DDO2.Push(pVR); 
		        pVR = DDO->GetVR(0x0008, 0x0050);
                        if (pVR && pVR->Length)
			{ newVR = new VR(pVR->Group, pVR->Element, pVR->Length, (BOOL) TRUE);
  			  memcpy(newVR->Data, pVR->Data, pVR->Length);
	                  DDO2.Push(newVR);
                          SetStringVR(&pVR, g, e, ""); 
	                  DDO2.Push(pVR);
                          QueryOnModalityWorkList (&DDO2, &ADDO);
                        }
			break;
                      default:
                        break;
                    }
                    
                    if (ADDO.GetSize()>0) pVR = ADDO.Get(0)->GetVR(g, e);
		    else  		  pVR = NULL;

                    if (pVR) 
	            { strncpy(vr, (char*)pVR->Data, pVR->Length);
	              vr[pVR->Length] = 0;
	              len = pVR->Length - 1;
                      while(len>0)
                      { if (vr[len] == ' ')
	                  vr[len] = 0;
                        else
                          break;
                        len--;
                      }
                      strcat(line, vr);
	            }

		    for (int j=0; j<ADDO.GetSize(); j++)
		    { while(pVR=ADDO.Get(j)->Pop())
			delete pVR;
                    }

                    i+=10;
                    break;
        }
        i += 2;

	// get substring of variable
        if (szExecName[i]=='[')
        { int a=0, b=-1;
          sscanf(szExecName+i, "[%d,%d]", &a, &b);
          for (;i<L;i++) if (szExecName[i-1]==']') break;
          if (a>strlen(ps)) a=strlen(ps);
          if (b>strlen(ps)) b=strlen(ps);
          if (b>=a) {memmove(ps, ps+a, b-a+1); ps[b-a+1]=0;}
        }
      }
      else
      { l = strlen(line);
        line[l]   = szExecName[i++];
        line[l+1] = 0;
      }
    }

    if (skipping)	// {} block being skipped
    { SystemDebug.printf("Importconverter%d.%d skipped\n", N, part);
    }

    else if (rc==4)	// ifxxxx statement causes a skip
    { rc = 0;
      SystemDebug.printf("Importconverter%d.%d not executed because of previous statement\n", N, part);
    }

    /* converter: write "string" to file - uses text file, use %n for newline */

    else if (memicmp(line, "write \"", 7)==0)
    { char string[256];
      char *file;
      FILE *f;

      strcpy(string, line+7);
      p = strstr(string, "\" to ");
      if (!p) p = strstr(string, "\" TO ");
      if (p)
      { *p=0;

        OperatorConsole.printf("Importconverter%d.%d executes: %s\n", N, part, line);

        file = p+5;
        f = fopen(file, "wt");
        if (f)
        { fputs(string, f);
          fclose(f);
        }
        else
          OperatorConsole.printf("*** Importconverter%d.%d: Failed to write to file %s\n", N, part, file);
      }
    }

    /* converter: append "string" to file - uses text file, use %n for newline */

    else if (memicmp(line, "append \"", 8)==0)
    { char string[256];
      char *file;
      FILE *f;

      strcpy(string, line+8);
      p = strstr(string, "\" to ");
      if (!p) p = strstr(string, "\" TO ");
      if (p)
      { *p=0;

        OperatorConsole.printf("Importconverter%d.%d executes: %s\n", N, part, line);

        file = p+5;
        f = fopen(file, "at");
        if (f)
        { fputs(string, f);
          fclose(f);
        }
        else
          OperatorConsole.printf("*** Importconverter%d.%d: Failed to append to file %s\n", N, part, file);
      }
    }

    /* converter: set xxxx,yyyy to "items like %Vxxxx,yyyy or %QP0010,0030" */
    /* converter: set xxxx,yyyy if "items like %Vxxxx,yyyy or %QP0010,0030" */
    /* converter: set x to "items like %Vxxxx,yyyy or %QP0010,0030" */
    /* converter: set y if "items like %Vxxxx,yyyy or %QP0010,0030" */

    else if (memicmp(line, "set ", 4)==0)	
    { char string[1024];
      char *file;
      int offset=13;

      if (strchr("xyzXYZ", line[4]))
      { g=e=0;
        offset = 5;
      }
      else
      { g = htoin(line+4, 4);
        e = htoin(line+9, 4);
      }

      if (memicmp(line+offset, " to \"", 4)==0 ||
          memicmp(line+offset, " if \"", 4)==0)
      { strcpy(string, line+offset+5);
        p = strrchr(string, '"');
        if (p)
        { *p=0;
          int len = strlen(string);
          VR *vr;
          
  	  if (len&1)
          { len++;
            strcat(string, " ");
          }
  
          if (memicmp(line+offset, " to \"", 4)==0 || len>0)	// if does not set when len==0
     	  { if (g==0 && e==0)
            { vr = VariableVRs[tolower(line[4])-'x'];
              if (!vr) vr = new VR;
              VariableVRs[tolower(line[4])-'x'] = vr;
            }
            else
              vr = DDO->GetVR(g, e);

    	    if(!vr) 
  	    { vr = new VR(g, e, len, (BOOL)TRUE);
  	      DDO->Push(vr);
            }
            else
    	      vr->ReAlloc(len);
  
  	    memcpy(vr->Data, string, len);
            OperatorConsole.printf("Importconverter%d.%d executes: %s\n", N, part, line);
          }
          else
            SystemDebug.printf("Importconverter%d.%d executes void: %s\n", N, part, line);
        }
      }
    }

    /* converter: setifempty xxxx,yyyy to "items like %Vxxxx,yyyy or %QP0010,0030" */
    /* converter: setifempty xxxx,yyyy if "items like %Vxxxx,yyyy or %QP0010,0030" */
    /* converter: setifempty x to "items like %Vxxxx,yyyy or %QP0010,0030" */
    /* converter: setifempty y if "items like %Vxxxx,yyyy or %QP0010,0030" */

    else if (memicmp(line, "setifempty ", 11)==0)	
    { char string[1024];
      char *file;
      int offset=20;

      if (strchr("xyzXYZ", line[11]))
      { g=e=0;
        offset = 12;
      }
      else
      { g = htoin(line+11, 4);
        e = htoin(line+16, 4);
      }

      if (memicmp(line+offset, " to \"", 4)==0 ||
          memicmp(line+offset, " if \"", 4)==0)
      { strcpy(string, line+offset+5);
        p = strrchr(string, '"');
        if (p)
        { VR *vr;
          *p=0;
          int len = strlen(string);
          
  	  if (len&1)
          { len++;
            strcat(string, " ");
          }
  
          if (memicmp(line+offset, " if \"", 4)==0 || len>0)	// if does not set when len==0
          { if (g==0 && e==0)
            { vr = VariableVRs[tolower(line[11])-'x'];
              if (!vr) vr = new VR;
              VariableVRs[tolower(line[11])-'x'] = vr;
            }
            else
              vr = DDO->GetVR(g, e);

  	    if(!vr) 
  	    { vr = new VR(g, e, len, (BOOL)TRUE);
              memcpy(vr->Data, string, len);
  	      DDO->Push(vr);
              OperatorConsole.printf("Importconverter%d.%d executes: %s\n", N, part, line);
            }
            else if (vr->Length==0)
            { vr->ReAlloc(len);
              memcpy(vr->Data, string, len);
              OperatorConsole.printf("Importconverter%d.%d executes: %s\n", N, part, line);
            }
            else
              SystemDebug.printf("Importconverter%d.%d executes void: %s\n", N, part, line);
          }
          else
            SystemDebug.printf("Importconverter%d.%d executes void: %s\n", N, part, line);
        }
      }
    }

    /* converter: delete xxxx,yyyy */

    else if (memicmp(line, "delete ", 7)==0)
    { g = htoin(line+7, 4);
      e = htoin(line+12, 4);

      pVR = DDO->GetVR(g, e);
      if (pVR) 
      { DDO->DeleteVR(pVR);
        OperatorConsole.printf("Importconverter%d.%d executes: %s\n", N, part, line);
      }
      else
        SystemDebug.printf("Importconverter%d.%d executes void: %s\n", N, part, line);
    }

    /* converter: save to filename (filename can be generated using all % tricks) */

    else if (memicmp(line, "save to ", 8)==0)
    { char *file = line+8;

      OperatorConsole.printf("Importconverter%d.%d executes: %s\n", N, part, line);

      DICOMDataObject *pDDO = MakeCopy(DDO);
      SaveDICOMDataObject(file, pDDO);
      delete pDDO;
    }

    /* converter: process with (command line can be generated using all % tricks, tempfile appended) */

    else if (memicmp(line, "process with ", 13)==0)
    { char cline[512];
      char tempfile[512];
      DICOMDataObject *DO2;

      PDU_Service PDU;
      PDU.AttachRTC(&VRType);

      NewTempFile(tempfile, ".dcm");
      strcpy(cline, line+13);
      if (cline[strlen(cline)-1]!=' ') strcat(cline, " ");
      strcat(cline, tempfile);

      OperatorConsole.printf("Importconverter%d.%d executes: process with %s\n", N, part, cline);

      SaveDICOMDataObject(tempfile, DDO);
      DDO->Reset();

      system(cline);

      DO2 = PDU.LoadDICOMDataObject(tempfile);
      while ( pVR = DO2->Pop() )
      { DDO->Push(pVR);
      }
      delete DO2;
    }

    /* converter: destroy (will prevent the image from being stored in the database) */

    else if (memicmp(line, "destroy", 7)==0 && calling && called)
    { OperatorConsole.printf("Importconverter%d.%d: destroyed received image\n", N, part);
      rc = 2;	
    }

    /* converter: storage (sets preferred device for storing image, e.g, STORAGE MAG1) */
    /* disabled for general scripting */

    else if (memicmp(line, "storage ", 8)==0 && called && calling)
    { OperatorConsole.printf("Importconverter%d.%d: sets preferred storage to %s\n", N, part, line+8);
      strcpy(Storage, line+8);
    }

    /* converter: compression (sets preferred compression for storing image, e.g, COMPRESSION un) */

    else if (memicmp(line, "compression ", 12)==0)
    { OperatorConsole.printf("Importconverter%d.%d: compression to %s\n", N, part, line+12);
      recompress(&DDO, line+12, "", line[12]=='n' || line[12]=='N');
    }

    /* converter: prefetch */

    else if (memicmp(line, "prefetch", 8)==0)
    { if (prefetch_queue("prefetch", patid, "", "", "", "", "", "", "", "", ""))
        OperatorConsole.printf("Importconverter%d.%d: queued prefetch %s\n", N, part, patid);
    }

    /* converter: preretrieve */

    else if (memicmp(line, "preretrieve ", 12)==0)
    { if (prefetch_queue("preretrieve", patid, line+12, "", "", "", "", "", "", "", ""))
        OperatorConsole.printf("Importconverter%d.%d: queued preretrieve of patient %s from %s\n", N, part, patid, line+12);
    }

    /* converter: stop and silentstop (will prevent further converters from running for this image) */

    else if (memicmp(line, "stop", 4)==0)
    { SystemDebug.printf("Importconverter%d.%d: stop\n", N, part);
      rc = 3;	
    }

    else if (memicmp(line, "silentstop", 10)==0)
    { rc = 3;	
    }

    /* converter: if "string" (only execute next converter if string not empty) */

    else if (memicmp(line, "ifnotempty \"", 12)==0)	
    { char string[1024];
      char *p;

      strcpy(string, line+12);
      p = strrchr(string, '"');
      if (p)
      { *p=0;

        SystemDebug.printf("Importconverter%d.%d executes: %s\n", N, part, line);

        int len = strlen(string);
        if (len==0) rc = 4;
      }
    }

    /* converter: ifempty "string" (only execute next converter if string empty) */

    else if (memicmp(line, "ifempty \"", 9)==0)	
    { char string[1024];
      char *p;

      strcpy(string, line+9);
      p = strrchr(string, '"');
      if (p)
      { *p=0;

        SystemDebug.printf("Importconverter%d.%d executes: %s\n", N, part, line);

        int len = strlen(string);
        if (len!=0) rc = 4;
      }
    }

    /* converter: ifequal "string1","string2" (only execute next converter if string1 equals string2) */
  
    else if (memicmp(line, "ifequal \"", 9)==0)	
    { char string[1024];
      char *p, *string2;
  
      strcpy(string, line+9);
      p = strrchr(string, '"');
      if (p)
      { *p=0;

        p = strstr(string, "\",\"");
        if (!p) p = strstr(string, "\", \"");
        if (p) 
        { string2 = strchr(p+1, '"')+1;
          *p=0;
          SystemDebug.printf("Importconverter%d.%d executes: %s\n", N, part, line);
          int cmp = strcmp(string, string2);
          if (cmp!=0) rc = 4;
        }
      }
    }

    /* converter: ifnotequal "string1","string2" (only execute next converter if string1 not equals string2) */
  
    else if (memicmp(line, "ifnotequal \"", 12)==0)	
    { char string[1024];
      char *p, *string2;
  
      strcpy(string, line+12);
      p = strrchr(string, '"');
      if (p)
      { *p=0;

        p = strstr(string, "\",\"");
        if (!p) p = strstr(string, "\", \"");
        if (p) 
        { string2 = strchr(p+1, '"')+1;
          *p=0;
          SystemDebug.printf("Importconverter%d.%d executes: %s\n", N, part, line);
          int cmp = strcmp(string, string2);
          if (cmp==0) rc = 4;
        }
      }
    }

    /* converter: ifmatch "string1","string2" (only execute next converter if string1 matches string2 - case insensitive) */
  
    else if (memicmp(line, "ifmatch \"", 9)==0)	
    { char string[1024];
      char *p, *string2;
  
      strcpy(string, line+9);
      p = strrchr(string, '"');
      if (p)
      { *p=0;

        p = strstr(string, "\",\"");
        if (!p) p = strstr(string, "\", \"");
        if (p) 
        { string2 = strchr(p+1, '"')+1;
          *p=0;
          SystemDebug.printf("Importconverter%d.%d executes: %s\n", N, part, line);
          if (!match(string, string2)) rc = 4;
        }
      }
    }

    /* converter: ifnotmatch "string1","string2" (only execute next converter if string1 not matches string2 - case insensitive) */
  
    else if (memicmp(line, "ifnotmatch \"", 12)==0)	
    { char string[1024];
      char *p, *string2;
  
      strcpy(string, line+12);
      p = strrchr(string, '"');
      if (p)
      { *p=0;

        p = strstr(string, "\",\"");
        if (!p) p = strstr(string, "\", \"");
        if (p) 
        { string2 = strchr(p+1, '"')+1;
          *p=0;
          SystemDebug.printf("Importconverter%d.%d executes: %s\n", N, part, line);
          if (match(string, string2)) rc = 4;
        }
      }
    }

    /* converter: ifnumequal "string1","string2" (only execute next converter if string1 num equals string2 - integer) */
  
    else if (memicmp(line, "ifnumequal \"", 12)==0)	
    { char string[1024];
      char *p, *string2;
  
      strcpy(string, line+12);
      p = strrchr(string, '"');
      if (p)
      { *p=0;

        p = strstr(string, "\",\"");
        if (!p) p = strstr(string, "\", \"");
        if (p) 
        { string2 = strchr(p+1, '"')+1;
          *p=0;
          SystemDebug.printf("Importconverter%d.%d executes: %s\n", N, part, line);
          if (atoi(string)!=atoi(string2)) rc = 4;
        }
      }
    }

    /* converter: ifnumnotequal "string1","string2" (only execute next converter if string1 num not equals string2 - integer) */
  
    else if (memicmp(line, "ifnumnotequal \"", 15)==0)	
    { char string[1024];
      char *p, *string2;
  
      strcpy(string, line+15);
      p = strrchr(string, '"');
      if (p)
      { *p=0;

        p = strstr(string, "\",\"");
        if (!p) p = strstr(string, "\", \"");
        if (p) 
        { string2 = strchr(p+1, '"')+1;
          *p=0;
          SystemDebug.printf("Importconverter%d.%d executes: %s\n", N, part, line);
          if (atoi(string)==atoi(string2)) rc = 4;
        }
      }
    }

    /* converter: ifnumgreater "string1","string2" (only execute next converter if string1 num greater than string2 num - integer) */
  
    else if (memicmp(line, "ifnumgreater \"", 14)==0)	
    { char string[1024];
      char *p, *string2;
  
      strcpy(string, line+14);
      p = strrchr(string, '"');
      if (p)
      { *p=0;

        p = strstr(string, "\",\"");
        if (!p) p = strstr(string, "\", \"");
        if (p) 
        { string2 = strchr(p+1, '"')+1;
          *p=0;
          SystemDebug.printf("Importconverter%d.%d executes: %s\n", N, part, line);
          if (atoi(string)<=atoi(string2)) rc = 4;
        }
      }
    }

    /* converter: ifnumless "string1","string2" (only execute next converter if string1 num less than string2 num - integer) */
  
    else if (memicmp(line, "ifnumless \"", 11)==0)	
    { char string[1024];
      char *p, *string2;
  
      strcpy(string, line+11);
      p = strrchr(string, '"');
      if (p)
      { *p=0;

        p = strstr(string, "\",\"");
        if (!p) p = strstr(string, "\", \"");
        if (p) 
        { string2 = strchr(p+1, '"')+1;
          *p=0;
          SystemDebug.printf("Importconverter%d.%d executes: %s\n", N, part, line);
          if (atoi(string)>=atoi(string2)) rc = 4;
        }
      }
    }

    /* converter: between "string1","string2" (execute next converter on time in hours: between "9", "17"; defer) */
  
    else if (memicmp(line, "between \"", 9)==0)	
    { char string[1024];
      char *p, *string2;
      int h1, h2, h;
      struct tm tmbuf;
      time_t t = time(NULL);

      localtime_r(&t, &tmbuf);
      h = tmbuf.tm_hour;

      strcpy(string, line+9);
      h1 = atoi(string);
      p = strrchr(string, '"');
      if (p)
      { *p=0;

        p = strstr(string, "\",\"");
        if (!p) p = strstr(string, "\", \"");
        if (p) 
        { string2 = strchr(p+1, '"')+1;
          *p=0;
          h2 = atoi(string2);
          SystemDebug.printf("Importconverter%d.%d executes: %s\n", N, part, line);
          if (h2>=h1 && (h <h1 || h>=h2)) rc = 4;	// between "9", "17": skip if h<9 or h>=17
          if (h2< h1 && (h <h1 && h>=h2)) rc = 4;	// between "17", "9": skip if h<=17 and h>=9
        }
      }
    }

    /* converter: nop (no operation) */

    else if (memicmp(line, "nop", 3)==0)
    { OperatorConsole.printf("Importconverter%d.%d: %s\n", N, part, line);
    }

    // process next command
    if (szNext[0]==0) break;
    strcpy(szExecName, szNext);
    part++;

    if (rc==3 || rc==2) break;
    //  stop     destroy
  }

  return rc;
}

BOOL CallImportConverters(DICOMDataObject *DDO, char *pszModality, char *pszStationName, char *pszSop, char *patid, char *calling, char *called, char *Storage)
{ char		szRootSC[64];
  char		szEntry[32];
  char		szTemp[32];
  int		i, iNbConverters;
  int		rc;

  if (!GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig,
		szRootSC, 64, ConfigFile)) return FALSE;
  if (!GetPrivateProfileString(szRootSC, "ImportConverters", "99",
		szTemp, 128, ConfigFile)) return FALSE;
  iNbConverters = atoi(szTemp);

  /* Loop over all converters */
  rc = 0;
  for(i=0; i<iNbConverters; i++)
  { //if (rc==4)	// ifxxxx statement causes a skip
    //{ rc = 0;
    //  OperatorConsole.printf("Importconverter%d not executed because of previous statement\n", i);
    //}
    //else
      rc = CallImportConverterN(DDO, i, pszModality, pszStationName, pszSop, patid, calling, called, Storage);

    if (rc==3 || rc==2) break;
    //  stop     destroy
  }

  // the status of the last one is returned: FALSE means destroy command (status 2) was executed
  return !(rc==2);
}

//////////////////////////////////////////////////////////////////////////////////
/* This code is a simple thread safe queue with its own processing thread       */
//////////////////////////////////////////////////////////////////////////////////

#ifndef WIN32
#define EnterCriticalSection(a) pthread_mutex_lock(a)
#define LeaveCriticalSection(a) pthread_mutex_unlock(a)
#define CRITICAL_SECTION pthread_mutex_t
#define InitializeCriticalSection(a) pthread_mutex_init(a, NULL);
#define mkdir(a) mkdir(a, 0777)
#endif

struct conquest_queue
{ int    top;
  int    bottom;
  int    num;
  int    entrysize;
  char   *data;
#ifdef WIN32
  HANDLE threadhandle;
#else
  pthread_t threadhandle;
#endif
  CRITICAL_SECTION critical;
  int    delay;
  BOOL   (*process)   (char *, ExtendedPDU_Service	*PDU, char *);
  BOOL   (*mayprocess)(char *, ExtendedPDU_Service	*PDU, char *);
  int    fails;			// counts failures
  time_t lastfailtime;		// set on failure, cleared on success
  int    maxfails;		// if set and exceeded stop retrying
  char	 failfile[512];		// file to store failed entries in

  ExtendedPDU_Service *PDU;	// to allow keeping association open
  time_t opentime;		// set on success
  int    pdu_refreshdelay;	// clear accepted syntaxes after .. s (use long time)
  int    pdu_closedelay;	// close unused pdu after .. s (use short time)
  int    pdu_release;		// if set, release pdu else just drop link
  char   *ForwardLastUID;	// remember last uid of link (hang up?)
} q1;

extern int NumIndexing;		// from odbci.cpp: is -1 on startup, >0 during DBF indexing

static BOOL WINAPI processthread(struct conquest_queue *q)
{ char *data = (char *)malloc(q->entrysize);

  while (1)
  { while (1)
    { EnterCriticalSection(&q->critical);

      if (q->top!=q->bottom) 
      { memcpy(data, q->data + q->bottom * q->entrysize, q->entrysize);

        // it is possible to delay processing of queue through this function

        if (q->mayprocess)
        { if (!q->mayprocess(data, q->PDU, q->ForwardLastUID)) 
          { LeaveCriticalSection(&q->critical);
            Sleep(q->delay);
            continue;
          }
        }

        q->bottom = (q->bottom+1)%q->num;
        LeaveCriticalSection(&q->critical);

	// only try to process if last failure was more than FailHoldOff seconds ago

        if (time(NULL)-q->lastfailtime > FailHoldOff)
        { if (q->process(data, q->PDU, q->ForwardLastUID)) 
  	  { q->lastfailtime = time(NULL);		// failed
            q->fails++;

	    // store failures
            if (q->failfile[0])
            { FILE *f = fopen(q->failfile, "ab");
	      if (f)
              { fwrite(data, q->entrysize, 1, f);
                fclose(f);
                OperatorConsole.printf("*** Queue: holding processing of file %s\n", data);
              }
            }
          }
          else
          { q->lastfailtime= 0;				// remember that last process was a success
            q->opentime    = time(NULL);		// PDU must be open by now
	    q->fails       = 0;
          }
        }
        else
        { // store failures (actually these were not even tried)
          if (q->failfile[0])
          { FILE *f = fopen(q->failfile, "ab");
            if (f)
            { fwrite(data, q->entrysize, 1, f);
              fclose(f);
              OperatorConsole.printf("*** Queue: holding processing of file %s\n", data);
            }
          }
        }

        break;		// this break avoids delays when the queue is not empty
      }

      LeaveCriticalSection(&q->critical);

      Sleep(q->delay);

      // Retry logic: RetryDelay s after last failure retry to process the data
      // This condition would also be at startup: therefore wait until DB completely indexed

      if (time(NULL)-q->lastfailtime>RetryDelay && q->failfile[0] && NumIndexing==0)
      { int rec=0, done=0, count=0;
        FILE *f = fopen(q->failfile, "r+b");
        
        if (f)
        { while(1)
          { fseek(f, rec * q->entrysize, SEEK_SET);

            if (fread(data, 1, q->entrysize, f)!=q->entrysize) 	// if eof -> done
            { done=1;
              break;
            }

            if (data[0])					// filled entry ?
            { OperatorConsole.printf("Queue: retrying processing of file %s\n", data);

              if (q->process(data, q->PDU, q->ForwardLastUID))
              { q->lastfailtime = time(NULL);			// failed again
		q->fails++;

		if (q->maxfails && (q->fails>q->maxfails))
		{ OperatorConsole.printf("*** Queue: %d retries exceeds set maximum of %d - giving up\n", q->fails, q->maxfails);
                  q->lastfailtime = 0;
		  q->fails        = 0;
                  done = 1;
		}
                break;						// retry later
              }
              else
	      { q->lastfailtime = 0;
                q->opentime     = time(NULL);			// PDU must be open by now
		q->fails        = 0;

                data[0]=0;					// mark data as processed
                fseek(f, rec * q->entrysize, SEEK_SET);
                fwrite(data, 1, 1, f);

                count++;					// every 10 files make time to process..
                if(count>10) break;				// ..queue entries that are freshly created
              }
            }
            rec++;						// next entry in file
          }

          fclose(f);
          if (done) unlink(q->failfile);			// delete file when not needed anymore
        }
      }

      for(int i=0; i<MAXExportConverters; i++)
      { if (q->PDU && (time(NULL)-q->opentime > q->pdu_closedelay)) // close unused association after 5 seconds
        { if (q->PDU[i].Link.Connected)
          //q->PDU[i].Close(); just hangup without AReleaseRQ: otherwise server sometimes fails to reconnect

          if (q->pdu_release) q->PDU[i].Close();
	  else                q->PDU[i].Link.Close();
        }

        if (q->PDU && (time(NULL)-q->opentime > q->pdu_refreshdelay)) // clean collected sopclasses after one hour of inactivity
        { if (!q->PDU[i].Link.Connected)
            q->PDU[i].ClearAbstractSyntaxs();
          q->opentime = time(NULL);
        }
      }
    }
  }

  free(data);
  return TRUE;
}

struct conquest_queue *new_queue(int num, int size, int delay, BOOL (*process)(char *, ExtendedPDU_Service *PDU, char *), ExtendedPDU_Service *PDU, int maxfails)
{ struct conquest_queue *result;
   unsigned long ThreadID;

  result               = new conquest_queue;
  result->top          = 0;
  result->bottom       = 0;
  result->num          = num;
  result->entrysize    = size;
  result->delay        = delay;
  result->process      = process;
  result->mayprocess   = NULL;
  result->data         = (char *)malloc(num * size);
  result->fails        = 0;
  result->maxfails     = maxfails;
  result->lastfailtime = 0;
  result->failfile[0]  = 0;
  result->PDU	       = PDU;
  result->opentime     = 0;
  result->pdu_refreshdelay = 3600;
  result->pdu_closedelay   = 5;
  result->pdu_release      = 0;
  result->ForwardLastUID   = NULL;

  InitializeCriticalSection(&result->critical);

  /* Note: since the queue is thread safe it is possible to start more than one thread to service it */

#ifdef WIN32
  result->threadhandle = 
    CreateThread(NULL, 0x000ff000, (LPTHREAD_START_ROUTINE) processthread, result, 0, &ThreadID);
#else
  pthread_create(&result->threadhandle, NULL, (void*(*)(void*))processthread, (void *)result);
  pthread_detach(result->threadhandle);
#endif

  return result;
};

void into_queue(struct conquest_queue *q, char *in)
{ while (1)
  { EnterCriticalSection(&q->critical);

    if ((q->top+1)%q->num != q->bottom)
    { memcpy(q->data + q->top * q->entrysize, in, q->entrysize);
      q->top = (q->top+1)%q->num;
      LeaveCriticalSection(&q->critical);
      return;
    }

    LeaveCriticalSection(&q->critical);

    Sleep(q->delay);
  }
}

// enters entry into queue if first numunique characters are not there.
// note: will discard entry when no space in queue
// intended for prefetching stuff: use a large queue

BOOL into_queue_unique(struct conquest_queue *q, char *in, int numunique)
{ int i;

  EnterCriticalSection(&q->critical);

  // no space!
  if ((q->top+1)%q->num == q->bottom) 
  { LeaveCriticalSection(&q->critical);
    return FALSE;
  }

  // search for identical items
  for (i=q->bottom; i!=(q->top+1)%q->num; i = (i+1)%q->num)
  { if (memcmp(q->data + i * q->entrysize, in, numunique)==0)
    { LeaveCriticalSection(&q->critical);
      return FALSE;
    }
  }
  
  // enter new item
  memcpy(q->data + q->top * q->entrysize, in, q->entrysize);
  q->top = (q->top+1)%q->num;
  LeaveCriticalSection(&q->critical);
  return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// This code starts a thread for each defined export converter
//////////////////////////////////////////////////////////////////////////////////

static struct conquest_queue **eqN = NULL;
static ExtendedPDU_Service ForwardPDU[MAXExportConverters][MAXExportConverters];	// max 20*20 with remaining association

BOOL exportprocessN(char *data, ExtendedPDU_Service *PDU, char *t)
{ return CallExportConverterN(data, data[1024+18+18+66], data+1024, data+1024+18, data+1024+18+18, data+1024+18+18+66+4, data+1024+18+18+66+4+66, data+1024+18+18+66+4+66+18, PDU, t);
}

void reset_queue_fails(int N)
{ eqN[N]->fails = 0;
}

void export_queueN(struct conquest_queue **q, char *pszFileName, int N, char *pszModality, char *pszStationName, char *pszSop, char *patid, char *calling, char *called)
{ char data[1536];

  if (!*q) 
  { char szRootSC[64], szTemp[32];

    *q = new_queue(QueueSize, 1536, 100, exportprocessN, N>=MAXExportConverters ? NULL : ForwardPDU[N], 0);
    sprintf((*q)->failfile, "ExportFailures%s_%d", Port, N);
    (*q)->ForwardLastUID = (char *) malloc(MAXExportConverters * 66);
    memset((*q)->ForwardLastUID, 0, MAXExportConverters * 66);

    GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig, szRootSC, 64, ConfigFile);

    GetPrivateProfileString(szRootSC, "ForwardAssociationRefreshDelay", "3600", szTemp, 128, ConfigFile);
    (*q)->pdu_refreshdelay = atoi(szTemp);

    GetPrivateProfileString(szRootSC, "ForwardAssociationCloseDelay", "5", szTemp, 128, ConfigFile);
    (*q)->pdu_closedelay   = atoi(szTemp);

    GetPrivateProfileString(szRootSC, "ForwardAssociationRelease", "1",	szTemp, 128, ConfigFile);
    (*q)->pdu_release   = atoi(szTemp);

    GetPrivateProfileString(szRootSC, "MaximumExportRetries", "0", szTemp, 128, ConfigFile);
    (*q)->maxfails   = atoi(szTemp);
  }

  if (pszFileName==NULL) return;

  memset(data, 0, 1536);

  strcpy(data,           pszFileName);
  strcpy(data+1024,       pszModality);
  strcpy(data+1024+18,    pszStationName);
  strcpy(data+1024+18+18, pszSop);
         data[1024+18+18+66] = N;
  strcpy(data+1024+18+18+66+4, patid);

  if(calling) strcpy(data+1024+18+18+66+4+66, calling);
  if(called)  strcpy(data+1024+18+18+66+4+66+18, called);

  into_queue(*q, data);
};

// This routine queues all export converter requests. Use instead of CallExportConverters

void QueueExportConverters(char *pszFileName, char *pszModality, char *pszStationName, char *pszSop, char *patid, char *calling, char *called)
{ char		szRootSC[64];
  char		szEntry[32];
  char		szTemp[32];
  int		i, iNbConverters;

  if (!GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig,
		szRootSC, 64, ConfigFile)) return;
  if (!GetPrivateProfileString(szRootSC, "ExportConverters", "0",
		szTemp, 128, ConfigFile)) return;
  iNbConverters = atoi(szTemp);
  if (!iNbConverters) return;
  if (iNbConverters>MAXExportConverters) iNbConverters=MAXExportConverters;

  if (!eqN) 
  { eqN = (struct conquest_queue **)malloc(iNbConverters * sizeof(struct conquest_queue *));
    memset(eqN, 0, iNbConverters * sizeof(struct conquest_queue *));
  };

  /* Loop over all converters */
  for(i=0; i<iNbConverters; i++)
  { export_queueN(&eqN[i], pszFileName, i, pszModality, pszStationName, pszSop, patid, calling, called);
  }
}

//////////////////////////////////////////////////////////////////////////////////
// This code is a queue for filecopy requests; it is used to speed mirror copying
//////////////////////////////////////////////////////////////////////////////////

// return TRUE if it worthwile to retry (copy failed)

BOOL copyprocess(char *data, ExtendedPDU_Service *PDU, char *dum)
{ if (DFileCopy2(data, data+1024, 0))
  { OperatorConsole.printf("Mirror copy: %s\n", data+1024);
    return FALSE;
  }
  else
  { OperatorConsole.printf("***Writing mirror copy failed: %s\n", data+1024);
    return TRUE;
  }
}

void mirrorcopy_queue(char *pszFileName, char *pszDestination)
{ char data[2048];
  static struct conquest_queue *q = NULL;

  if (!q) 
  { q = new_queue(QueueSize, 2048, 100, copyprocess, NULL, 0);
    sprintf(q->failfile, "CopyFailures%s", Port);
  }
  if (pszFileName==NULL) return;

  strcpy(data,           pszFileName);
  strcpy(data+1024,      pszDestination);

  into_queue(q, data);
};

//////////////////////////////////////////////////////////////////////////////////
// This code is a queue for prefetch and preforward requests
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// read all files of a patient (to populate cache) use windows call for windows version
BOOL TestFile(char *FullFilename, char *status);

static int DFileRead(char *source, int MaxRead)
{
	BOOL		err_status = TRUE; 	/* Error status                 */
	char		*copybuffer;        	/* The copy buffer              */
	unsigned 	bufsize;            	/* Size of copy buffer          */
	unsigned long	bytes, totbytes;      	/* Actual transferred bytes     */
#ifdef WIN32
        HANDLE          handle = CreateFile(source, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if( handle == NULL) return FALSE;
#else
	/* Open input file for read-only. parser error if operation fails.  */
	int inhandle = sopen( source, O_RDONLY | O_BINARY, SH_DENYNO, S_IREAD);
	if( inhandle == -1) return FALSE;
#endif

	/* get a large buffer */
	bufsize = 0x4000;
	copybuffer = (char *)malloc(bufsize);
	if (copybuffer == NULL)
   		{
#ifdef WIN32
	        CloseHandle(handle);
#else
		close(inhandle);
#endif

		return -1;
		}

	totbytes=0;

	while( /*!eof(inhandle) && */err_status)
		{
#ifdef WIN32
                ReadFile(handle, copybuffer, bufsize, &bytes, NULL);
#else
		bytes= (unsigned)read( inhandle, copybuffer, bufsize);
#endif

//#ifndef WIN32
		if (bytes == 0)
       			break;
//#endif

		if (bytes == (unsigned) -1)
			{ 
			err_status= FALSE;
       			break;
			}

		totbytes += (bytes+512)>>10;
		if (MaxRead && totbytes>(MaxRead>>10)) break;
		}

	free(copybuffer);
#ifdef WIN32
        CloseHandle(handle);
#else
	close(inhandle);
#endif
	return totbytes;
}

char LastPrefetch[65] = "";
BOOL MakeSafeString (char *in, char *string );

BOOL
PrefetchPatientData (char *PatientID, int MaxRead)
	{
	Database aDB;
	char Tables[512], DeviceName[512], ObjectFile[1024], FileNameFrom[1024];
	char QueryString[512], PatientIDValue[512], PhysicalFrom[1024];
	SQLLEN SQLResultLength;
	BOOL Status=TRUE;
	char error[256];

	if (strcmp(PatientID, LastPrefetch)==0) return TRUE;

	OperatorConsole.printf("Prefetching patient %s\n", PatientID);

	sprintf(Tables, "%s", ImageTableName);

	if(!aDB.Open(DataSource, UserName, Password, DataHost))
		{
		OperatorConsole.printf("***Unable to open database %s as user %s on %s\n",
					DataSource, UserName, DataHost);
		return FALSE;
		}
	
	MakeSafeString(PatientID, PatientIDValue);
	sprintf(QueryString, 	"DICOMImages.ImagePat = %s", PatientIDValue);

	if (!aDB.Query(Tables, "DICOMImages.DeviceName, DICOMImages.ObjectFile", QueryString, NULL))
		{
		OperatorConsole.printf("***Unable to query for image records for prefetch\n");
		aDB.Close();
		return FALSE;
		}

	aDB.BindField (1, SQL_C_CHAR, DeviceName, 255, &SQLResultLength);
	aDB.BindField (2, SQL_C_CHAR, ObjectFile, 255, &SQLResultLength);

	while (aDB.NextRecord())
		{ 	
		if (!FindPhysicalDevice(DeviceName, PhysicalFrom, ObjectFile)) continue;
		strcpy(FileNameFrom, PhysicalFrom);
		strcat(FileNameFrom, ObjectFile);

		SystemDebug.printf("Prefetching file: %s\n", FileNameFrom);

		if (MaxRead<0)
			{
			if (!TestFile(FileNameFrom, error))
				{
				OperatorConsole.printf("***Prefetch read failed for file: %s (%s)\n", FileNameFrom, error);
				Status = FALSE;
				}
			}
		else
			{
			if (DFileRead(FileNameFrom, MaxRead)<0)
				{
				OperatorConsole.printf("***Prefetch read failed for file: %s\n", FileNameFrom);
				Status = FALSE;
				}
			}
		}

	aDB.Close();	
	strcpy(LastPrefetch, PatientID);

	return Status;
	}

// move patient from this to other server: returns TRUE is meaningful to retry
static BOOL DcmMove(char *patid, char* pszSourceAE, char* pszDestinationAE, char *studyuid, char *seriesuid, char *compress, char *modality, char *date, char *sop, char *imagetype, char *seriesdesc, int id)
{	PDU_Service		PDU;
	DICOMCommandObject	DCO;
	DICOMCommandObject	DCOR;
	DICOMDataObject		DDO;
	DICOMDataObject		DDOR;
	UID			uid;
	VR			*vr;
	LE_UINT16		command, datasettype, messageid, priority;
	BYTE			SOP[64];
	BYTE			AppTitle[64], host[64], port[64], compr[64];
	BOOL			rc = FALSE;

	if (strcmp(pszSourceAE, "(local)")==0)
  	  pszSourceAE = (char *)MYACRNEMA;

	PDU.ClearAbstractSyntaxs();
	PDU.SetLocalAddress((unsigned char*)MYACRNEMA);
	PDU.SetRemoteAddress((unsigned char*)pszSourceAE);

	uid.Set("1.2.840.10008.3.1.1.1");	// Application context (DICOM App)
	PDU.SetApplicationContext(uid);
	uid.Set("1.2.840.10008.5.1.4.1.2.1.2");	// patientrootmove
	PDU.AddAbstractSyntax(uid);

	if (strcmp(pszSourceAE, (char *)MYACRNEMA)==0)
		{
		if(!PDU.Connect((BYTE *)"127.0.0.1", Port)) 
			{
			OperatorConsole.printf("***forward xxxx to: local server not working\n");
			return FALSE;	// self not working: no retry
			}
		messageid = id;
		}
	else
		{
		if(!GetACRNema(pszSourceAE, (char *)host, (char *)port, (char *)compr)) 
			{
			OperatorConsole.printf("***preretrieve: AE not found: %s\n", pszSourceAE);
			return FALSE; 	// wrong address no retry
			}
		if(!PDU.Connect(host, port)) 
			{
			OperatorConsole.printf("***preretrieve: could not connect to AE: %s\n", pszSourceAE);
			return TRUE;	// remote not working: retry
			}
		messageid = id;	// special to indicate move to itself - no import/exportconverters
		}

	sprintf((char*) SOP, "1.2.840.10008.5.1.4.1.2.1.2");
	vr = new VR (0x0000, 0x0002, strlen((char*)SOP), (void*) SOP, FALSE);
	DCO.Push(vr);
	command = EN0021;
	vr = new VR (0x0000, 0x0100, 0x0002, &command, FALSE);
	DCO.Push(vr);
	priority = 0;
	vr = new VR (0x0000, 0x0700, 0x0002, &priority, FALSE);
	DCO.Push(vr);
	datasettype = EN0102;	
	vr = new VR (0x0000, 0x0800, 0x0002, &datasettype, FALSE);
	DCO.Push(vr);

	vr = new VR (0x0000, 0x0110, 0x0002, &messageid, FALSE);
	DCO.Push(vr);

	memset((char*)AppTitle, ' ', 20);
	memcpy((void*)AppTitle, pszDestinationAE, strlen(pszDestinationAE));
	vr = new VR(0x0000, 0x0600, 16, (void*)&AppTitle[0], FALSE);
	DCO.Push(vr);

	if      (*sop)       SetStringVR(&vr, 0x0008, 0x0052, "IMAGE"); 
	else if (*modality)  SetStringVR(&vr, 0x0008, 0x0052, "SERIES"); 
	else if (*date)      SetStringVR(&vr, 0x0008, 0x0052, "SERIES"); 
	else if (*seriesuid) SetStringVR(&vr, 0x0008, 0x0052, "SERIES"); 
	else if (*studyuid)  SetStringVR(&vr, 0x0008, 0x0052, "STUDY"); 
	else                 SetStringVR(&vr, 0x0008, 0x0052, "PATIENT"); 
	DDO.Push(vr);

	if (*patid)
		{
		SetStringVR(&vr, 0x0010, 0x0020, patid); 
		DDO.Push(vr);
		}
	if (*studyuid)
		{
                vr = new VR(0x0020, 0x000d, strlen(studyuid), (void*)studyuid, FALSE );
		DDO.Push(vr);
		}
	if (*seriesuid)
		{
                vr = new VR(0x0020, 0x000e, strlen(seriesuid), (void*)seriesuid, FALSE );
		DDO.Push(vr);
		}
	if (*sop)
		{
                vr = new VR(0x0008, 0x0018, strlen(sop), (void*)sop, FALSE );
		DDO.Push(vr);
		}
	if (*modality)
		{
		SetStringVR(&vr, 0x0008, 0x0060, modality); 
		DDO.Push(vr);
		}
	if (*date)
		{
		SetStringVR(&vr, 0x0008, 0x0020, date); 
		DDO.Push(vr);
		}
	if (*imagetype)
		{
                vr = new VR(0x0008, 0x0008, strlen(imagetype), (void*)imagetype, FALSE );
		DDO.Push(vr);
		}
	if (*seriesdesc)
		{
                vr = new VR(0x0008, 0x103e, strlen(seriesdesc), (void*)imagetype, FALSE );
		DDO.Push(vr);
		}
	if (*compress)
		{
		SetStringVR(&vr, 0x9999, 0x0700, compress); 
		DCO.Push(vr);
		}

	uid.Set(SOP);
	PDU.Write(&DCO, uid);
	PDU.Write(&DDO, uid);
	
	while(TRUE)
		{
		if(!PDU.Read(&DCOR))
			{
			OperatorConsole.printf("***preretrieve/forward xxx to: association lost\n");
			return TRUE;	// associate lost: may retry
			}

		if(DCOR.GetUINT16(0x0000, 0x0100)!=0x8021)
			{
			OperatorConsole.printf("***preretrieve/forward xxx to: invalid C-move response\n");
			rc = FALSE;
			goto EXIT; 	// not a C-MOVE-RSP ? no retry
			}
		
		if(DCOR.GetUINT16(0x0000, 0x0800)!=0x0101)
			{	
			PDU.Read(&DDOR);// ignore the data set
			DDOR.Reset();
			}

		if(DCOR.GetUINT16(0x0000, 0x0900)==0x0000)
			{
			rc = FALSE;
			goto EXIT;	// completed OK, no retry
			}
		else if (DCOR.GetUINT16(0x0000, 0x0900)==0xff00)
			{		// continuing no problem
			DCOR.Reset();
			continue;
			}
		else
			{
			OperatorConsole.printf("***preretrieve/forward xxx to: remote DICOM error\n");
			rc = FALSE;	// remote DICOM error: NO retry FOR NOW
			goto EXIT;
			}
		DCOR.Reset();
		}
EXIT:
	PDU.Close();
	return rc;
}

// delete patient data from this server: always returns FALSE

static BOOL DcmDelete(char *patid, char *studyuid, char *seriesuid, char *modality, char *date, char *sop, char *imagetype)
{	DICOMDataObject		DDO;
	VR			*vr;

	if (*patid)
		{
		SetStringVR(&vr, 0x0010, 0x0020, patid); 
		DDO.Push(vr);
		}
	if (*studyuid)
		{
                vr = new VR(0x0020, 0x000d, strlen(studyuid), (void*)studyuid, FALSE );
		DDO.Push(vr);
		}
	if (*seriesuid)
		{
                vr = new VR(0x0020, 0x000e, strlen(seriesuid), (void*)seriesuid, FALSE );
		DDO.Push(vr);
		}
	if (*sop)
		{
                vr = new VR(0x0008, 0x0018, strlen(sop), (void*)sop, FALSE );
		DDO.Push(vr);
		}
	if (*modality)
		{
		SetStringVR(&vr, 0x0008, 0x0060, modality); 
		DDO.Push(vr);
		}
	if (*date)
		{
		SetStringVR(&vr, 0x0008, 0x0020, date); 
		DDO.Push(vr);
		}
	if (*imagetype)
		{
                vr = new VR(0x0008, 0x0008, strlen(imagetype), (void*)imagetype, FALSE );
		DDO.Push(vr);
		}

	RemoveFromPACS(&DDO, FALSE);
	return FALSE;
}

// runs in the thread
BOOL prefetchprocess(char *data, ExtendedPDU_Service *PDU, char *dum)
{ if (memicmp(data+65, "prefetch",     8)==0) PrefetchPatientData(data, 0);
  if (memicmp(data+65, "forward ",     8)==0) return DcmMove  (data, (char *)MYACRNEMA, data+82, data+100, data+165, data+230, data+256, data+276, data+300, data+366, data+500, 5);
  if (memicmp(data+65, "preretrieve", 11)==0) return DcmMove  (data, data+82, (char *)MYACRNEMA, data+100, data+165, data+230, data+256, data+276, data+300, data+366, data+500, 0xfbad);
  if (memicmp(data+65, "get ",         4)==0) return DcmMove  (data, data+82, (char *)MYACRNEMA, data+100, data+165, data+230, data+256, data+276, data+300, data+366, data+500, 0xfbad);
  if (memicmp(data+65, "delete ",      7)==0) return DcmDelete(data, data+100, data+165, data+256, data+276, data+300, data+366);
  if (memicmp(data+65, "process ",     8)==0) 
#ifdef WIN32
     WinExec(data+230, SW_MINIMIZE);     
#else
     system(data+230);
#endif

  return FALSE;
}

// blocks the thread so as not to prefetch too often
BOOL mayprefetchprocess(char *data, ExtendedPDU_Service *PDU, char *dum)
{ if ((*(unsigned int*)(data+596)) + ForwardCollectDelay > time(NULL)) return FALSE; // hold until e.g. 10 minutes past
  return TRUE;
}

// enter prefetch or preforward request into queue
BOOL prefetch_queue(char *operation, char *patientid, char *server, char *studyuid, char *seriesuid, char *compress, char *modality, char *date, char *sop, char *imagetype, char *seriesdesc)
{ char data[600];
  static struct conquest_queue *q = NULL;

  if (!q) 
  { char szRootSC[64], szTemp[32];

    q = new_queue(QueueSize, 600, 100, prefetchprocess, NULL, 0);
    q->mayprocess = mayprefetchprocess;
    sprintf(q->failfile, "DelayedFetchForwardFailures%s", Port);

    GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig, szRootSC, 64, ConfigFile);
    GetPrivateProfileString(szRootSC, "MaximumDelayedFetchForwardRetries", "0", szTemp, 128, ConfigFile);
    q->maxfails   = atoi(szTemp);
  }

  memset(data, 0,  600);
  strncpy(data,     patientid, 64);
  strncpy(data+65,  operation, 16);
  *(unsigned int*)(data+596) = time(NULL);

  if (memicmp(data+65, "process ",     8)==0) 
  { strncpy(data+100, studyuid,  64);
    strncpy(data+165, seriesuid, 64);
    strncpy(data+230, server,   360);
    return into_queue_unique(q, data, 230);
  }
  else
  { strncpy(data+82,  server,    17);
    strncpy(data+100, studyuid,  64);
    strncpy(data+165, seriesuid, 64);
    strncpy(data+230, compress,  25);
    strncpy(data+256, modality,  19);
    strncpy(data+276, date,      23);
    strncpy(data+300, sop,       64);
    strncpy(data+366, imagetype, 128);
    strncpy(data+500, seriesdesc,64);
    return into_queue_unique(q, data, 596);
  }
};

//////////////////////////////////////////////////////////////////////////////////
// This queue is intended to prefetch (in disk cache) patient data 
// likely to be requested soon in the time that the server is inactive
// Enter P+patientID or S+studyUID to be read using into_queue or enter 
// empty item to abort current prefetch
//////////////////////////////////////////////////////////////////////////////////

static BOOL WINAPI prefetcherthread(struct conquest_queue *q)
{ char data[66], curitem[65];
  Database aDB;
  char DeviceName[512], ObjectFile[1024], FileNameFrom[1024];
  char QueryString[512], QueryValue[512], PhysicalFrom[1024];
  SQLLEN SQLResultLength;
  BOOL Status=TRUE;
  BOOL qact = FALSE;
  int  count, kb;
  time_t TimeOfDay;

  curitem[0]=0;

  while (1)
  { while (1)
    { EnterCriticalSection(&q->critical);

      if (q->top!=q->bottom) 
      { memcpy(data, q->data + q->bottom * q->entrysize, q->entrysize);
        q->bottom = (q->bottom+1)%q->num;
        LeaveCriticalSection(&q->critical);

	if (data[0]==0 && qact)		// abort current prefetch
	{ int skipped = 0;
          while (aDB.NextRecord()) skipped++;
          OperatorConsole.printf("Aborted prefetch after reading %d images (%d MB) in %d s, %d images skipped\n", count, (kb+512)>>10, time(NULL) - TimeOfDay, skipped);
          qact = FALSE;
	  curitem[0]=0;
          aDB.Close();
	  break;
	};

	if (data[0]==0) break;		// ignore abort when not active
        if (strcmp(curitem, data+1)==0) break;	// already prefetched

        if (!qact)
	  if(!aDB.Open(DataSource, UserName, Password, DataHost))
	  { OperatorConsole.printf("***Unable to open database for prefetch\n");
	    break;
          };

	MakeSafeString(data+1, QueryValue);
        TimeOfDay = time(NULL);
        kb = 0;
        count = 0;

        if (data[0]=='P')
  	{ OperatorConsole.printf("Prefetching patient %s\n", data+1);
          sprintf(QueryString, 	"DICOMImages.ImagePat = %s", QueryValue);
	  if (!aDB.Query("DICOMImages", "DICOMImages.DeviceName, DICOMImages.ObjectFile", QueryString, NULL))
	  { OperatorConsole.printf("***Unable to query for image records for prefetch\n");
	    break;
          };
        }
        else
        { OperatorConsole.printf("Prefetching study %s\n", data+1);
          sprintf(QueryString, 	"DICOMSeries.StudyInsta = %s and DICOMSeries.SeriesInst = DICOMImages.SeriesInst", QueryValue);
	  if (!aDB.Query("DICOMSeries, DICOMImages", "DICOMImages.DeviceName, DICOMImages.ObjectFile", QueryString, NULL))
	  { OperatorConsole.printf("***Unable to query on study for image records for prefetch\n");
	    break;
          };
        };

	aDB.BindField (1, SQL_C_CHAR, DeviceName, 255, &SQLResultLength);
	aDB.BindField (2, SQL_C_CHAR, ObjectFile, 255, &SQLResultLength);
	qact = TRUE;
	strcpy(curitem, data+1);

	// do not break: delay a bit
      }
      else if (qact)
      { LeaveCriticalSection(&q->critical);

        if (aDB.NextRecord())
	{ if (!FindPhysicalDevice(DeviceName, PhysicalFrom, ObjectFile)) break;
	  strcpy(FileNameFrom, PhysicalFrom);
	  strcat(FileNameFrom, ObjectFile);

	  SystemDebug.printf("Prefetching file: %s\n", FileNameFrom);
  	  kb += DFileRead(FileNameFrom, 0);
          count++;
          break;
	}
	else
        { qact = FALSE;
          aDB.Close();
          OperatorConsole.printf("Prefetch done: read %d images (%d MB) in %d s\n", count, (kb+512)>>10, time(NULL) - TimeOfDay);
        }
      }
      else
        LeaveCriticalSection(&q->critical);

      Sleep(q->delay);
    }
  }

  DeleteCriticalSection(&q->critical);

  return TRUE;
}

struct conquest_queue *prefetcherqueue;

struct conquest_queue *new_prefetcherqueue(void)
{ struct conquest_queue *result;
  unsigned long ThreadID;
  int num=1000, size = 66;

  result               = new conquest_queue;
  result->top          = 0;
  result->bottom       = 0;
  result->num          = num;
  result->entrysize    = size;
  result->delay        = 10;
  result->process      = NULL;
  result->mayprocess   = NULL;
  result->data         = (char *)malloc(num * size);
  result->fails        = 0;
  result->lastfailtime = 0;
  result->failfile[0]  = 0;
  result->PDU	       = NULL;
  result->opentime     = 0;
  result->pdu_refreshdelay = 3600;
  result->pdu_closedelay   = 5;
  result->pdu_release      = 0;
  result->ForwardLastUID   = NULL;

  InitializeCriticalSection(&result->critical);

#ifdef WIN32
  result->threadhandle = 
    CreateThread(NULL, 0x000ff000, (LPTHREAD_START_ROUTINE) prefetcherthread, result, 0, &ThreadID);
#else
  pthread_create(&result->threadhandle, NULL, (void*(*)(void*))prefetcherthread, (void *)result);
  pthread_detach(result->threadhandle);
#endif

  return result;
};

// Control prefetcher from queries or moves

static int prefetchermode=0;

BOOL prefetcher(struct DICOMDataObject *DDO, BOOL move)
{ VR *pVR = NULL;
  char id[66];
  int len;

  if (!prefetchermode) return FALSE;

  if (!prefetcherqueue) 
    prefetcherqueue = new_prefetcherqueue();

  if (!DDO) return FALSE;

  memset(id, 0, 66);

  if (move)
  { pVR = DDO->GetVR(0x0008, 0x0018);
    if (pVR && pVR->Length) return FALSE;	// do not abort on single sop move

    into_queue(prefetcherqueue, id);	   	// abort on all other moves
    return FALSE;
  }

  pVR = DDO->GetVR(0x0010, 0x0020);		// get patientid
  if (!pVR)
    id[0] = 0;
  else
  { id[0] = 'P';
    strncpy(id+1, (char*)pVR->Data, pVR->Length);
    id[pVR->Length+1] = 0;
    len = pVR->Length - 1;
    while(len>0)
    { if (id[len+1] == ' ')
        id[len+1] = 0;
      else
        break;
      len--;
    }
  }

  // no patient id or wildcard provided: try study
  if (id[0]==0 || id[1]==0 || strchr(id, '*')!=NULL)
  { pVR = DDO->GetVR(0x0020, 0x000d);
    if (!(pVR && pVR->Length)) return FALSE;	// no study: do not affect prefetch

    id[0]='S';					// prefetch on study (for e.g. kpacs)
    strncpy(id+1, (char*)pVR->Data, pVR->Length);
    id[pVR->Length+1] = 0;
  }

  // any other query will start prefetch of patient or study
  into_queue(prefetcherqueue, id);
  return TRUE;
};

//////////////////////////////////////////////////////////////////////////////////
// This code is for worklist processing
//////////////////////////////////////////////////////////////////////////////////

// will replace all items in object with worklist fields (if filled) for AccessionNumber
// returns TRUE if replacement was succesfull: AccessionNumber was found or no entries to replace
// Note: since PatientId and StudyInstanceUID may be changed by this operation, this operation
// cannot be done after the original images were entered into the server. I will need to consider
// generating new SOP and Series UID's in this case

BOOL ApplyWorklist(DICOMDataObject *DDOPtr, Database *DB)
	{
	DBENTRY	*DBE;
	int Index, I;
	char Fields[1024];
	char QueryString[256];
	VR *vr;
	char *Data[100]; 
	SQLLEN SQLResultLength[100];

	if (!WorkListDB[0].Element)				// no worklist DB
		return FALSE;

	DBE = WorkListDB;
	Fields[0]=0;

	vr = DDOPtr->GetVR(DBE[0].Group, DBE[0].Element);	// query string for AccessionNumber
	if (vr)
		{
		strcpy(QueryString, DBE[0].SQLColumn);
//		strcat(QueryString, DBE[0].SQLColumn);
		strcat(QueryString, " = '");
		QueryString[strlen(QueryString) + vr->Length]=0;
		memcpy(QueryString+strlen(QueryString), vr->Data, vr->Length);
		if (QueryString[strlen(QueryString)-1]==' ') QueryString[strlen(QueryString)-1]=0;
		strcat(QueryString, "'");

//		strcat(QueryString, " and PatientID = '");	// patient ID cannnot be used: may be the same in image as in worklist !
//		vr = DDOPtr->GetVR(0x0010, 0x0020);
//		memcpy(QueryString+strlen(QueryString), vr->Data, vr->Length);
//		if (QueryString[strlen(QueryString)-1]==' ') QueryString[strlen(QueryString)-1]=0;
//		strcat(QueryString, "'");
		}
	else
		return FALSE;

	if (vr->Length==0)
		return FALSE;					// accessionnumber not filled in

	Index = 1; 						// do not read AccessionNumber from db
	I = 0;
	while ( TRUE )
		{
		if(!DBE[Index].Element) break;			// end of fields
		vr = DDOPtr->GetVR(DBE[Index].Group, DBE[Index].Element);
		if (vr && DBE[Index].DICOMType!=DT_STARTSEQUENCE && DBE[Index].DICOMType!=DT_ENDSEQUENCE)
			{					// field is real and present in image
			strcat(Fields, DBE[Index].SQLColumn);
			strcat(Fields, ",");
			++I;
			}
		++Index;
		}
	Fields[strlen(Fields)-1]=0;				// remove trailing ,
        
	if (I==0) return TRUE;					// nothing to do

								// no db access
	if (!DB->Query(WorkListTableName, Fields, QueryString, NULL))
		return FALSE;

	Index = 1; 						// do not read AccessionNumber from db
	I = 0;
	while ( TRUE )
		{
		if(!DBE[Index].Element) break;			// end of fields
		vr = DDOPtr->GetVR(DBE[Index].Group, DBE[Index].Element);
		if (vr && DBE[Index].DICOMType!=DT_STARTSEQUENCE && DBE[Index].DICOMType!=DT_ENDSEQUENCE)
			{					// field is real and present in image
			Data[I] = (char *)malloc(256);		// data area for db read
			DB->BindField (I+1, SQL_C_CHAR, Data[I], 255, SQLResultLength+I);
			Data[I][0]=0;
			++I;
			}
		++Index;
		}

	if (!DB->NextRecord())					// no record found ?
		return FALSE;

	Index = 1; 						// do not read AccessionNumber from db
	I = 0;
	while ( TRUE )
		{
		if(!DBE[Index].Element) break;
		vr = DDOPtr->GetVR(DBE[Index].Group, DBE[Index].Element);
		if (vr && DBE[Index].DICOMType!=DT_STARTSEQUENCE && DBE[Index].DICOMType!=DT_ENDSEQUENCE)
			{					// field is real and present in image

#if 0								// mvh 20051208
			SQLResultLength[I] = strlen(Data[I]);
#endif

			if (SQLResultLength[I])			// only replace if db field contains data
				{
				if (SQLResultLength[I] & 1)	// make length even
					{
					if (DBE[Index].DICOMType==DT_UI)
						Data[I][SQLResultLength[I]++] = 0;	// uid
					else
						Data[I][SQLResultLength[I]++] = ' ';	// text
					}
				Data[I][SQLResultLength[I]] = 0;
					
				vr->ReAlloc(SQLResultLength[I]);
				memcpy(vr->Data, (void*)Data[I], SQLResultLength[I]);
				}

			free(Data[I]);
			++I;
			}
		++Index;
		}

	return TRUE;						// success
	}


// Save a dicom composite data object to disk and update database; returns filename
// Warning: Filename must point to an array of 1024 characters that is filled

BOOL    NewDeleteSopFromDB(char *pat, char *study, char *series, char *sop, Database &aDB);

BOOL
SaveToDisk(Database	&DB, DICOMDataObject	*DDOPtr, char 	*Filename, BOOL NoKill, unsigned char *calling, unsigned char *called, int Syntax, BOOL nopreget)
	{
	FILE		*fp;
	DICOMDataObject	DDO;
	PDU_Service	PDU;
	Debug		AutoRoute;
	char		s[1024];
	char		rRoot[1024], rFilename[1024], MirrorFilename[1024];
	char		Device[256], MirrorDevice[256], Device2[256];
	char		SOPInstanceUID[256];
	int		len, UseChapter10, sIndex, devlen, has_dcm_extension=0;
	VR*		pVR;
	char		szModality[66];
	char		szStationName[66];
	char		szSop[66], szSeries[66], szStudy[66];
	char		patid[66];
	BOOL		bForcedImplicit = FALSE;
	char		*p;
	char		Storage[64];

	PDU.AttachRTC(&VRType);

	if (WorkListMode==1) 			// optionally update information from worklist db
		ApplyWorklist(DDOPtr, &DB);
	else if (WorkListMode==2) 		// compulsory update information from worklist db
		{
		if (!ApplyWorklist(DDOPtr, &DB)) 
			{
			OperatorConsole.printf("***[WorkListMode 2] Worklist entry (AccessionNumber) not found; object not saved\n");
			return FALSE;
			}
		}

	FixImage(DDOPtr);	// Make sure the required DICOM UID's are there + fix patient ID if enabled

	/* PDU.SaveDICOMDataObject destroys the contents of DDOPtr. We need to know
	   the modality in order to decide whether ExportConverters or ImportConverters 
	   should be called. Get the modality here...
	*/
	pVR = DDOPtr->GetVR(0x0008, 0x0060);
	if (!pVR)
	  szModality[0] = 0;
	else
	{ strncpy(szModality, (char*)pVR->Data, pVR->Length);
	  szModality[pVR->Length] = 0;
	  len = pVR->Length - 1;
	  while(len>0)
          { if (szModality[len] == ' ')
	      szModality[len] = 0;
            else
              break;
            len--;
          }
	}

	/* same for the StationName */
	pVR = DDOPtr->GetVR(0x0008, 0x1010);
	if (!pVR)
	  szStationName[0] = 0;
	else
	{ strncpy(szStationName, (char*)pVR->Data, pVR->Length);
	  szStationName[pVR->Length] = 0;
	  len = pVR->Length - 1;
	  while(len>0)
          { if (szStationName[len] == ' ')
	      szStationName[len] = 0;
            else
              break;
            len--;
          }
	}

	/* same for the Sop */
	pVR = DDOPtr->GetVR(0x0008, 0x0018);
	if (!pVR)
	  szSop[0] = 0;
	else
	{ strncpy(szSop, (char*)pVR->Data, pVR->Length);
	  szSop[pVR->Length] = 0;
	  len = pVR->Length - 1;
	  while(len>0)
          { if (szSop[len] == ' ')
	      szSop[len] = 0;
            else
              break;
            len--;
          }
	}

	/* same for the Series */
	pVR = DDOPtr->GetVR(0x0020, 0x000e);
	if (!pVR)
	  szSeries[0] = 0;
	else
	{ strncpy(szSeries, (char*)pVR->Data, pVR->Length);
	  szSeries[pVR->Length] = 0;
	  len = pVR->Length - 1;
	  while(len>0)
          { if (szSeries[len] == ' ')
	      szSeries[len] = 0;
            else
              break;
            len--;
          }
	}

	/* same for the Study */
	pVR = DDOPtr->GetVR(0x0020, 0x000d);
	if (!pVR)
	  szStudy[0] = 0;
	else
	{ strncpy(szStudy, (char*)pVR->Data, pVR->Length);
	  szStudy[pVR->Length] = 0;
	  len = pVR->Length - 1;
	  while(len>0)
          { if (szStudy[len] == ' ')
	      szStudy[len] = 0;
            else
              break;
            len--;
          }
	}

	/* same for the patid */
	pVR = DDOPtr->GetVR(0x0010, 0x0020);
	if (!pVR)
	  patid[0] = 0;
	else
	{ strncpy(patid, (char*)pVR->Data, pVR->Length);
	  patid[pVR->Length] = 0;
	  len = pVR->Length - 1;
	  while(len>0)
          { if (patid[len] == ' ')
	      patid[len] = 0;
            else
              break;
            len--;
          }
	}

	Storage[0]=0;
	if (!nopreget)
		if (!CallImportConverters(DDOPtr, szModality, szStationName, szSop, patid, (char *)calling, (char *)called, Storage))
			{
			// the import converter destroyed the image: silently cancel store
			Filename[0]=0;
			delete DDOPtr;
			return TRUE;
			}

	// make a filename for the new object; note: rRoot (patient directory part of rFilename) is not used any longer
	*rRoot=1;
	if (!GenerateFileName(DDOPtr, Device, rRoot, rFilename, NoKill, Syntax, (char *)called, (char *)calling, &DB))
        	{  
		//OperatorConsole.printf("***Failed to create filename for DICOM object (is it a slice ?)\n");
#ifdef FAILSAFE_STORAGE
		strcpy(rFilename, "failsafe_storage\\");
		GenUID(rFilename + strlen(rFilename));
		strcat(rFilename, ".v2");
		OperatorConsole.printf("***There was a DB problem, failsafe storage created following filename: %s\n", rFilename);
#else
           	return FALSE; //!!!!!!!!! added mvh+ljz 19980409
#endif
        	}

	// newly written image (not rewritten image): use selection preferred storage if enough space
	if (*rRoot)
		{
		if (Storage[0])
			{
			int PreferredDevice = atoi(Storage+3);
			if(CheckFreeStoreOnMAGDevice(PreferredDevice) > 30)
				sprintf(Device, "MAG%d", PreferredDevice);
			}
		}

	// If MAG is full, may write to MIRROR, but database will say MAG
	if (memicmp(Device, "MIRROR", 6)==0)
		sprintf(Device2, "MAG%d", atoi(Device+6));
	else
		strcpy(Device2, Device);	// fix 20040606 (was sprintf(Device2, "MAG%d", atoi(Device+3));)

	// do not rewrite image not on MAG but it is not an error (warning is generated earlier)
	if (memicmp(Device2, "MAG", 3)!=0)
		{
		Filename[0]=0;
		delete DDOPtr;	// 20040402
		return TRUE;
		}

	// When written replace virtual MAG device, i.e. MAG0.1 (for VirtualServerFor1) with real one: MAG0
	p = strchr(Device2, '.');
	if (p && p[1]>='0' && p[1]<='9') *p = 0;

	// try to enter object into database (object is not saved when this fails)
	if(!SaveToDataBase(DB, DDOPtr, rFilename, Device2, *rRoot))
		{
		OperatorConsole.printf("***Error saving to SQL: %s\n", rFilename);
#ifndef FAILSAFE_STORAGE
		return FALSE;
#endif
		}

	// Make the patient directory (and any required subdirectories), that might not be there
	GetPhysicalDevice(Device, Filename);
	devlen = strlen(Filename);
	strcat(Filename, rFilename);

	for (sIndex = devlen; sIndex<=strlen(Filename); sIndex++)
		if (Filename[sIndex]==PATHSEPCHAR)
			{
			strcpy(s, Filename);
			s[sIndex]='\0';
			mkdir(s);
			}

	// Find a mirror device if it exists (will not do this if MIRROR is being written because MAG if full)
	if (memicmp(Device, "MAG", 3)==0)
		{
		sprintf(MirrorDevice, "MIRROR%d", atoi(Device+3));
		if (!GetPhysicalDevice(MirrorDevice, MirrorFilename))
			MirrorDevice[0]=0;
		}
	else
		MirrorDevice[0]=0;

	// Make the mirror patient directory (and any required subdirectories), that might not be there
	if (MirrorDevice[0])
		{
		devlen = strlen(MirrorFilename);
		strcat(MirrorFilename, rFilename);

		for (sIndex = devlen; sIndex<strlen(MirrorFilename); sIndex++)
			if (MirrorFilename[sIndex]==PATHSEPCHAR)
				{
				strcpy(s, MirrorFilename);
				s[sIndex]='\0';
				mkdir(s);
				}
		}

	// UIDs must be zero padded (not with a space)
	VR	*vrSOPInstanceUID = DDOPtr->GetVR(0x0008, 0x0018);
	if(vrSOPInstanceUID)
		{
		memset((void*)SOPInstanceUID, 0, 128);
		memcpy((void*)SOPInstanceUID, vrSOPInstanceUID->Data, vrSOPInstanceUID->Length);
		// Fix a bad encoding engine on a remote side..
		if(vrSOPInstanceUID->Length)
			if(SOPInstanceUID[vrSOPInstanceUID->Length-1]==' ')
				SOPInstanceUID[vrSOPInstanceUID->Length-1] = '\0';
		}
	else
		SOPInstanceUID[0] = '\0';

	// check the filename to determine format; this avoids format change on rewrite
	len = strlen(rFilename);
	if (len>4)
		has_dcm_extension = (stricmp(rFilename+len-4, ".dcm")==0);


	/* Type of save depends on file name extension is .dcm (UseChapter10) 
	   and on the transfer syntax (not ImplicitLittleEndian, not
	   ExplicitLittleEndian and not ExplicitBigEndian)
	*/
	pVR = DDOPtr->GetVR(0x0002, 0x0010);	// TransferSyntaxUID
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
				OperatorConsole.printf("SaveToDisk: cannot rewrite jpeg/rle image in v2 format: %s\n", Filename);
				Filename[0]=0;
				delete DDOPtr;
				return TRUE;
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

	// is it nki compressed and attempt to store as .dcm --> decompress ?
	if (DDOPtr->GetVR(0x7fdf, 0x0010)!=NULL && DDOPtr->GetVR(0x7fe0, 0x0010)==NULL && has_dcm_extension)
		{
		OperatorConsole.printf("SaveToDisk: *** warning: cannot write NKI compressed in dcm format, decompressing: %s\n", Filename);
		DecompressNKI(DDOPtr);
		}
		
	int t = time(NULL);
	if (!UseChapter10)
		PDU.SaveDICOMDataObject(Filename, ACRNEMA_VR_DUMP, DDOPtr);
	else if (bForcedImplicit)
	 	PDU.SaveDICOMDataObject(Filename, DICOM_CHAPTER_10_IMPLICIT, DDOPtr);
	else
	// OR (tested OK) chapter 10 format (does not compress) code in filepdu.cxx (dicom.lib)
	 	PDU.SaveDICOMDataObject(Filename, DICOM_CHAPTER_10_EXPLICIT, DDOPtr);
	SaveTime += time(NULL)-t;

	// 20080817: check file was actually written, if not report error and delete database entry
	if (!DFileExists(Filename))
		{
		OperatorConsole.printf("***Error writing file: %s\n", Filename);
		NewDeleteSopFromDB(patid, szStudy, szSeries, szSop, DB);
		return FALSE;
		}

	if (MirrorDevice[0])
		{
#if DONTQUEUEMIRRORCOPY
                if (DFileCopy2(Filename, MirrorFilename, 0))
			OperatorConsole.printf("Mirror copy: %s\n", MirrorFilename);
		else
			OperatorConsole.printf("***Writing mirror copy failed: %s\n", MirrorFilename);
#else
		mirrorcopy_queue(Filename, MirrorFilename);
#endif
		}

	// notify the autorouter that a dicom object has landed
	if ( Edition != E_PERSONAL )
		{
		if(SOPInstanceUID[0]!='\0')
			AutoRoute.printf("%s", SOPInstanceUID);
		}

	// notify additional executables that a dicom object has landed

	if (!nopreget)
#if DONTQUEUEEXPORTCONVERTERS
		CallExportConverters(Filename, szModality, szStationName, szSop, patid, (char *)calling, (char *)called, NULL, NULL);
#else
        	QueueExportConverters(Filename, szModality, szStationName, szSop, patid, (char *)calling, (char *)called);
#endif

	delete DDOPtr;

	return ( TRUE );
	}

// some help for the simple user
BOOL
PrintOptions ()
	{
	fprintf(stderr, "\n");
	fprintf(stderr, "DGATE: UCDMC/NKI DICOM server thread and PACS utility application %s\n", VERSION);
	fprintf(stderr, "\n");
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "(1) DGATE <-!#|-v|-u#|-^#>    Report as in dicom.ini|stdout|UDP|File(#=port)\n");
	fprintf(stderr, "          [-p#|-b]            Set listen port|run in non-theaded debug mode\n");
	fprintf(stderr, "          [-i|-r|-arDEVICE]   Init|Init/regenerate DB|Regen single device\n");
	fprintf(stderr, "          [-d|-m|-k]          List (-d) devices (-m) AE map (-k) DICOM.SQL\n");
	fprintf(stderr, "          [-t|-o]             Test console|Test database\n");
	fprintf(stderr, "          [-sOpts|-es d u p]  Create ODBC data source, database (WIN32 only)\n");
	fprintf(stderr, "          [-nd|-nc#|-jd|-jc#] NKI de-/compress#|JPEG de-/compress# FILE\n");
	fprintf(stderr, "          [-as#,N|-amFROM,TO] Select#KB to archive of MAGN|move device data\n");
	fprintf(stderr, "          [-au|-aeFROM,TO]    Undo select for archiving|rename device\n");
	fprintf(stderr, "          [-av|-atDEVICE]     Verify mirror disk|Test read files for DEVICE\n");
	fprintf(stderr, "          [-abJUKEBOX1.2,N]   Make cacheset to burn JUKEBOX1,CD2 from MAGN\n");
	fprintf(stderr, "          [-acJUKEBOX1.2]     Verify JUKEBOX1,CD2 against cacheset\n");
	fprintf(stderr, "          [-adJUKEBOX1.2]     Verify and delete cacheset for JUKEBOX1, CD2\n");
	fprintf(stderr, "          [-f<p|t|s|i>ID]     Delete DB for Patient, sTudy, Series, Image\n");
	fprintf(stderr, "          [-f<e|d|z>file]     Enter/Delete DB of file, Zap server file\n");
	fprintf(stderr, "          [-faFILE<,ID>]      Add file to server<optionally change PATID>\n");
	fprintf(stderr, "          [-zID]              Delete (zap) patient\n");
	fprintf(stderr, "          [-frDEVICE,DIR]     Regen single directory DIR on DEVICE\n");
	fprintf(stderr, "          [-f<c|k>PATID,file] Change/Kopy PATID of file (irreversible/once)\n");
	fprintf(stderr, "          [-f?file|-fu|-c#]   get UID of file|Make new UID|UID helper(0..99)\n");
	fprintf(stderr, "          [-ff#]              Delete old patients until #MB free\n");
	fprintf(stderr, "          [-gSERVER,DATE]     grab images from SERVER of date not on here\n");
	fprintf(stderr, "                              Otherwise: run as threaded server, port=1111\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "(2) DGATE FileMapping         Run server child; shared memory has socket#\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "(3) DGATE --command:arguments Send command to running server in other process\n");
	fprintf(stderr, "                              (works directly - use with care)\n");
	fprintf(stderr, "Delete options:\n");
	fprintf(stderr, "    --deleteimagefile:file                  Delete given image file from server\n" );
	fprintf(stderr, "    --deletepatient:patid                   Delete given patient from server\n" );
	fprintf(stderr, "    --deletestudy:patid:studyuid            Delete given study from server\n" );
	fprintf(stderr, "    --deletestudies:date(range)             Delete studies from server on date\n" );
	fprintf(stderr, "    --deleteseries:patid:seriesuid          Delete given series from server\n" );
	fprintf(stderr, "    --deleteimagefromdb:file                Delete given file from db only\n" );
	fprintf(stderr, "    --deletesopfromdb:pat,study,series,sop  Delete specified image from db only\n" );
	fprintf(stderr, "\n");
	fprintf(stderr, "DICOM move options:\n");
	fprintf(stderr, "    --movepatient:source,dest,patid         Move patient, source e.g. (local)\n" );
	fprintf(stderr, "    --movestudy:source,dest,patid:studyuid  Move study, patid: optional\n" );
	fprintf(stderr, "    --movestudies:source,dest,date(range)   Move studies on date\n" );
	fprintf(stderr, "    --moveseries:source,dest,patid:seruid   Move series patid: optional\n" );
	fprintf(stderr, "\n");
	fprintf(stderr, "Dbase without ODBC options:\n");
	fprintf(stderr, "    --packdbf:                Pack dbase databases and recreate memory index\n" );
	fprintf(stderr, "    --indexdbf:               Re-create memory index for dbase database\n" );
	fprintf(stderr, "\n");
	fprintf(stderr, "Modification of dicom objects:\n");
	fprintf(stderr, "    --modifypatid:file,patid  Change patid of given file\n" );
	fprintf(stderr, "    --anonymize:file,patid    Anonymize given file\n" );
	fprintf(stderr, "    --mergestudy:uid,uid,..   Start merging studies with given studyuids\n" );
	fprintf(stderr, "    --mergestudyfile:file     Use to process all files to merge\n" );
	fprintf(stderr, "    --mergeseries:uid,uid,..  Start merging series with given seriesuids\n" );
	fprintf(stderr, "    --mergeseriesfile:file    Use to process all files to merge\n" );
	fprintf(stderr, "\n");
	fprintf(stderr, "Maintenance options:\n");
	fprintf(stderr, "    --initializetables:       Clear and create database\n" );
	fprintf(stderr, "    --initializetables:1      Clear and create database without indices\n" );
	fprintf(stderr, "    --initializetables:2      Clear and create worklist database\n" );
	fprintf(stderr, "    --regen:                  Re-generate entire database\n" );
	fprintf(stderr, "    --regendevice:device      Re-generate database for single device\n" );
	fprintf(stderr, "    --regendir:device,dir     Re-generate database for single directory\n" );
	fprintf(stderr, "    --regenfile:file          Re-enter given file in database\n" );
	fprintf(stderr, "    --addimagefile:file,patid Copy file into server, optionally with new patid\n" );
	fprintf(stderr, "    --testcompress:file       Test: enter file in server with many compressions\n" );
	fprintf(stderr, "    --debuglog_on:file/port   Start debug logging\n" );
	fprintf(stderr, "    --log_on:file/port/pipe   Start normal logging\n" );
	fprintf(stderr, "    --debuglevel:#            Set debug logging level\n" );
	fprintf(stderr, "    --makespace:#             Delete old patients to make #MB space\n" );
	fprintf(stderr, "    --get_freestore:dev,fmt   Report free #Mb on device\n" );
	fprintf(stderr, "    --get_param:name,fmt      Read any parameter from DICOM.INI\n" );
	fprintf(stderr, "    --get_ini_param:name,fmt  Read any parameter from DICOM.INI\n" );
	fprintf(stderr, "    --get_ini_num:index,fmt   List any entry from DICOM.INI\n" );
	fprintf(stderr, "    --get_ini:fmt             List all entries from DICOM.INI\n" );
	fprintf(stderr, "    --put_param:name,value    Write any parameter to DICOM.INI\n" );
	fprintf(stderr, "    --delete_param:name       Delete any parameter from DICOM.INI\n" );
	fprintf(stderr, "    --read_ini:               Re-read all parameters from DICOM.INI\n" );
	fprintf(stderr, "    --get_amap:index,fmt      List any entry from ACRNEMA.MAP\n" );
	fprintf(stderr, "    --get_amaps:fmt           List all entries from ACRNEMA.MAP\n" );
	fprintf(stderr, "    --put_amap:i,AE,ip,p#,cmp Write entry in memory for ACRNEMA.MAP\n" );
	fprintf(stderr, "    --delete_amap:index       Delete entry in memory for ACRNEMA.MAP\n" );
	fprintf(stderr, "    --write_amap:             Write ACRNEMA.MAP from memory to disk\n" );
	fprintf(stderr, "    --read_amap:              Re-read ACRNEMA.MAP from disk to memory\n" );
	fprintf(stderr, "    --get_sop:index,fmt       List any accepted service class UID\n" );
	fprintf(stderr, "    --put_sop:index,UID,name  Write/add accepted service class UID\n" );
	fprintf(stderr, "    --delete_sop:index        Delete accepted service class UID\n" );
	fprintf(stderr, "    --get_transfer:index,fmt  List any accepted transfer syntax\n" );
	fprintf(stderr, "    --put_transfer:in,UID,nam Write/add accepted transfer syntax\n" );
	fprintf(stderr, "    --delete_transfer:index   Delete accepted transfer syntax\n" );
	fprintf(stderr, "    --get_application:idx,fmt List any accepted application UID\n" );
	fprintf(stderr, "    --put_application:i,U,n   Write/add accepted application UID\n" );
	fprintf(stderr, "    --delete_application:inde Delete accepted application UID\n" );
	fprintf(stderr, "    --get_localae:index,fmt   List any accepted local AE title\n" );
	fprintf(stderr, "    --put_localae:in,AE,name  Write/add accepted local AE title\n" );
	fprintf(stderr, "    --delete_localae:index    Delete accepted local AE title\n" );
	fprintf(stderr, "    --get_remoteae:index,fmt  List any accepted remote AE title\n" );
	fprintf(stderr, "    --put_remoteae:in,AE,name Write/add accepted remote AE title\n" );
	fprintf(stderr, "    --delete_remoteae:index   Delete accepted remote AE title\n" );
	fprintf(stderr, "    --get_dic:index,fmt       List any dicom dictionary item\n" );
	fprintf(stderr, "    --get_sqldef:level,in,fmt List any database field definition\n" );
        fprintf(stderr, "    --dump_header:filein,fileout        Create header dump of file\n" );
        fprintf(stderr, "    --forward:file,mode,server          Send file with compr. mode to server\n");
        fprintf(stderr, "    --convert_to_gif:file,size,out,l/w  Downsize and convert to mono GIF\n");
        fprintf(stderr, "    --convert_to_bmp:file,size,out,l/w  Downsize and convert to color BMP\n");
        fprintf(stderr, "    --convert_to_dicom:file,size,comp   Downsize/compress DICOM for cgi\n");
	fprintf(stderr, "    --query:table|fields|where|fmt|file Arbitrary query output to file\n" );
	fprintf(stderr, "    --query2:tab|fld|whe|fmt|max|file   Same but limit output rows to max\n" );
	fprintf(stderr, "    --patientfinder:srv|str|fmt|file    List patients on server\n" );
	fprintf(stderr, "    --studyfinder:srv|str|fmt|file      List studies on server\n" );
	fprintf(stderr, "    --seriesfinder:srv|str|fmt|file     List series on server\n" );
	fprintf(stderr, "    --serieslister:srv|pat|stu|fmt|file List series in a study\n" );
	fprintf(stderr, "    --imagelister:srv|pat|ser|fmt|file  List files in a series\n" );
	fprintf(stderr, "    --extract:PatientID = 'id'          Extract all dbase tables to X..\n" );
	fprintf(stderr, "    --extract:                          Extract patient dbase table to XA..\n" );
	fprintf(stderr, "    --addrecord:table|flds|values       Append record, values must be in ''\n" );
	fprintf(stderr, "    --deleterecord:table,where          Delete record from table\n" );
	fprintf(stderr, "    --grabimagesfromserver:AE,date      Update this server from other\n" );
	fprintf(stderr, "    --display_status:file               Display server status\n" );
	fprintf(stderr, "    --loadhl7:file                      Load HL7 data into worklist\n" );
	fprintf(stderr, "    --clonedb:AE                        Clone db from server for testing\n" );
	fprintf(stderr, "    --prefetch:patientid                prefetch all images for improved speed\n" );
	fprintf(stderr, "    --checklargestmalloc:               Estimates DICOM object size limit\n" );
	fprintf(stderr, "    --quit:                             Stop the server\n" );
	fprintf(stderr, "\n");
	fprintf(stderr, "Archival options:\n");
	fprintf(stderr, "    --renamedevice:from,to              Rename device in database\n" );
	fprintf(stderr, "    --verifymirrordisk:device           Verify mirror disk for selected device\n" );
	fprintf(stderr, "    --testimages:device                 Test read all images on device\n" );
	fprintf(stderr, "    --movedatatodevice:to,from          Move data from one device to another\n" );
	fprintf(stderr, "    --selectlruforarchival:kb,device    Step 1 for archival: to device.Archival\n" );
	fprintf(stderr, "    --preparebunchforburning:to,from    Step 2 for archival: moves to cache\n" );
	fprintf(stderr, "    --deletebunchafterburning:deviceto  Step 3 for archival: deletes from cache\n" );
	fprintf(stderr, "    --comparebunchafterburning:deviceto Part step 3 - compare jukebox to cache\n" );
	fprintf(stderr, "    --restoremagflags:                  Undo archival sofar\n" );

	return ( TRUE );
	}

// external references
BOOL 	SelectLRUForArchival(char *Device, int KB);
BOOL	PrepareBunchForBurning(char *DeviceFrom, char *DeviceTo);
BOOL	MoveDataToDevice(char *DeviceFrom, char *DeviceTo);
BOOL	RestoreMAGFlags();
BOOL	CompareBunchAfterBurning(char *DeviceTo);
BOOL	DeleteBunchAfterBurning(char *DeviceTo);
BOOL	VerifyMirrorDisk(char *DeviceFrom);
BOOL	RenameDevice(char *DeviceFrom, char *DeviceTo);
BOOL	TestImages(char *DeviceFrom);
BOOL	PanicKillOff(int MAGThreshHold);

// forward references
BOOL 	GrabImagesFromServer(BYTE *calledae, char *studydate, char *destination);
static BOOL SendServerCommand(char *NKIcommand1, char *NKIcommand2, int con, char *buf=NULL, BOOL html=TRUE);

#if defined(__BORLANDC__) || defined(__WATCOMC__)
typedef BOOL (__stdcall *_SQLConfigDataSource)(HWND       hwndParent,
                                               WORD       fRequest,
                                               LPCSTR     lpszDriver,
                                               LPCSTR     lpszAttributes);
_SQLConfigDataSource __SQLConfigDataSource;
#define SQLConfigDataSource __SQLConfigDataSource
#define  ODBC_ADD_DSN     1               // Add data source
#endif

// Main routine for parsing the command line; return FALSE when not running
// as server or a socket # when running as server thread
int
ParseArgs (int	argc, char	*argv[])
	{
	UINT		Valid;
	int		valid_argc, mode=0;
	int		Socketfd = 0;
	BOOL		bUIDPostfixSet = FALSE;

	Valid = 2;

	ConfigMicroPACS();

	UserLog.On(UserLogFile);
	UserLog.AddTimeStamps(TRUE);
	TroubleLog.On(TroubleLogFile);
	TroubleLog.AddTimeStamps(TRUE);

	valid_argc = 1;

	while ( valid_argc < argc )
		{
#ifndef UNIX	// Win32...
		if ((argv[valid_argc][0]=='-')||(argv[valid_argc][1]=='/'))
#else		// UNIX..
		if(argv[valid_argc][0]=='-')
#endif
			{
			switch ( argv[valid_argc][1] )
				{
				case	'v':	// verbose mode
				case	'V':
					SystemDebug.On();
					OperatorConsole.On();
					break;

				case	'b':	// debug mode
				case	'B':
					SystemDebug.On();
					OperatorConsole.On();
					NoThread=TRUE;
					break;

				case	'c':	// set UID counter
				case	'C':
					UIDPostfix = atoi(argv[valid_argc]+2);
					bUIDPostfixSet = TRUE;
					break;

				case	'u':
				case	'U':	// be verbose to specified pipe/udp
					if(argv[valid_argc][2] == PATHSEPCHAR)
						{
						OperatorConsole.OnMsgPipe(argv[valid_argc]+2);
						SystemDebug.OnMsgPipe(argv[valid_argc]+2);
						}
					else
						{
						OperatorConsole.OnUDP(OCPipeName, argv[valid_argc]+2);
						SystemDebug.OnUDP(OCPipeName, argv[valid_argc]+2);
						}
					break;


				case	'!':	// be verbose to pipe/udp specified in DICOM.INI (no debug)
					if(OCPipeName[0] == PATHSEPCHAR)
						{
						OperatorConsole.OnMsgPipe(OCPipeName);
						}
					else
						{
                                                if (argv[valid_argc][2]>='0' && argv[valid_argc][2]<='9')
						{
							OperatorConsole.OnUDP(OCPipeName, argv[valid_argc]+2);
#if 0
							SystemDebug.OnUDP(OCPipeName, argv[valid_argc]+2);
#endif
						}
						else
							OperatorConsole.OnUDP(OCPipeName, "1111");
						}
					break;

				case	'^':	// be verbose to passed file with timestamps (no debug)
					OperatorConsole.On(argv[valid_argc]+2);
					OperatorConsole.AddTimeStamps(1);
					break;

				case	'#':	// be verbose to passed file with timestamps (with debug)
					OperatorConsole.On(argv[valid_argc]+2);
					OperatorConsole.AddTimeStamps(1);
					SystemDebug.On(argv[valid_argc]+2);
					SystemDebug.AddTimeStamps(1);
					break;

				case	'p':	// override dicom.ini port#
				case	'P':
					strcpy((char*)Port, argv[valid_argc]+2);
					++Valid;
					break;
					// fall through was not intended !

				case	'r':	// init and regenerate database
				case	'R':
					RunServer = FALSE;
					NeedPack = 2;
					SystemDebug.printf("Regen Database\n");

					if(!LoadKFactorFile((char*)KFACTORFILE))
						{
						OperatorConsole.printf("***Could not load kfactor file: %s\n", KFACTORFILE);
						exit(1);
						}

					OperatorConsole.printf("Step 1: Re-intialize SQL Tables\n");

					mode = atoi(argv[valid_argc]+2);
					InitializeTables (mode);

					OperatorConsole.printf("Step 2: Load / Add DICOM Object files\n");

					Regen();

					OperatorConsole.printf("Regeneration Complete\n");

					return ( FALSE );

				case	'd':	// List magnetic device status
				case	'D':
					RunServer = FALSE;
					NeedPack = FALSE;

					SystemDebug.Off();
					PrintFreeSpace();
					return ( FALSE );

				case	'm':	// list map of outgoing AE's
				case	'M':
					RunServer = FALSE;
					NeedPack = FALSE;

					if(!InitACRNemaAddressArray())
						{
						OperatorConsole.printf("***Error loading acr-nema map file:%s\n",ACRNEMAMAP);
						exit(1);
						}

					PrintAMap();
					return ( FALSE );


				case	'k':	// list K factor file (DICOM.SQL)
				case	'K':
					RunServer = FALSE;
					NeedPack = FALSE;

					if(!LoadKFactorFile((char*)KFACTORFILE))
						{
						OperatorConsole.printf("***Could not load kfactor file: %s\n", KFACTORFILE);
						exit (1);
						}

					PrintKFactorFile();
					return ( FALSE );


				case	't':	// test (TCP/IP) console connnection
				case	'T':
					{ 
					char lt[] = "This is a very long text output for testing -- ";
					RunServer = FALSE;
					NeedPack = FALSE;
					OperatorConsole.printf("This output is generated by the dicom server application\n");
					OperatorConsole.printf("If you can read this, the console communication is OK\n");
                          		SystemDebug.printf("This is systemdebug output; can you read this ?\n");
                          		SystemDebug.printf("%s%s%s%s%s%s\n", lt, lt, lt, lt, lt, lt);
					OperatorConsole.printf(" ---------- Succesful end of test -----------\n");
					return ( FALSE );
					}

				case	'o':	// test ODBC database connnection
				case	'O':
					{
					Database aDB;
					int	i;
					RunServer = FALSE;
					NeedPack = FALSE;
					for (i=1; i<=10; i++)
						{
						OperatorConsole.printf("Attempting to open database; test #%d of 10\n", i);
						if(!aDB.Open(DataSource, UserName, Password, DataHost))
							{
							OperatorConsole.printf("***Unable to open database %s as user %s on %s\n",
							DataSource, UserName, DataHost);
							exit (1);
							}
	
						OperatorConsole.printf("Creating test table\n");
						aDB.CreateTable ( "xtesttable", "Name varchar(254), AccessTime  int" );

						OperatorConsole.printf("Adding a record\n");
						aDB.AddRecord("xtesttable", "Name", "'Testvalue'");

						OperatorConsole.printf("Dropping test table\n");
						aDB.DeleteTable ( "xtesttable" );

						OperatorConsole.printf("Closing database\n");
						aDB.Close();
						}

					OperatorConsole.printf(" ---------- Succesful end of test -----------\n");
					return ( FALSE );
					}

				case	'a':	// Archival code
				case	'A':	// Archival code
					{
					RunServer = FALSE;
					NeedPack = 5;
					OperatorConsole.printf(" ---------- Start archival operation  -----------\n");

					// Select a number of KB to archive from device MAGn (default MAG0)
					if (argv[valid_argc][2] == 's' || argv[valid_argc][2] == 'S')
						{
						char MAG[256];
						char *p;
						p = strchr(argv[valid_argc]+3,',');
						if (p) 
							sprintf(MAG, "MAG%d", atoi(p+1));
						else 
							strcpy(MAG, "MAG?");
						if (!SelectLRUForArchival(MAG, atoi(argv[valid_argc]+3)))
							exit(1);
						}

					// Create cache set for burning for JUKEBOXn.n from MAGn (default MAG0)
					else if (argv[valid_argc][2] == 'b' || argv[valid_argc][2] == 'B')
						{
						char MAG[256], Device[256];
						char *p;
						strcpy(Device, argv[valid_argc]+3);
						p = strchr(Device,',');
						if (p) 
							{
							sprintf(MAG, "MAG%d", atoi(p+1));
							*p = 0;
							}
						else 
							strcpy(MAG, "MAG?");
						if (!PrepareBunchForBurning(MAG, Device))
							exit(1);
						}

					// Move (all) data from device1 to device2
					else if (argv[valid_argc][2] == 'm' || argv[valid_argc][2] == 'M')
						{
						char Device1[256];
						char *p;
						strcpy(Device1, argv[valid_argc]+3);
						p = strchr(Device1,',');
						if (p) 
							{
							*p = 0;
							if (!MoveDataToDevice(Device1, p+1)) exit(1);
							}
						else
							exit(1);
						}

					// Undo any archiving that was not completed
					else if (argv[valid_argc][2] == 'u' || argv[valid_argc][2] == 'U')
						{
						if (!RestoreMAGFlags())
							exit(1);
						}

					// Verify CD in jukebox against its cache set
					else if (argv[valid_argc][2] == 'c' || argv[valid_argc][2] == 'C')
						{
						if (!CompareBunchAfterBurning(argv[valid_argc]+3))
							exit(1);
						}

					// Verify MAG device against its mirror device
					else if (argv[valid_argc][2] == 'v' || argv[valid_argc][2] == 'V')
						{
						if (!VerifyMirrorDisk(argv[valid_argc]+3))
							exit(1);
						}

					// Test read slices on device
					else if (argv[valid_argc][2] == 't' || argv[valid_argc][2] == 'T')
						{
						if (!TestImages(argv[valid_argc]+3))
							exit(1);
						}

					// Delete cache set for burned CD
					else if (argv[valid_argc][2] == 'd' || argv[valid_argc][2] == 'D')
						{
						if (!DeleteBunchAfterBurning(argv[valid_argc]+3))
							exit(1);
						}

					// Regen a single device (for database maintenance)
					else if (argv[valid_argc][2] == 'r' || argv[valid_argc][2] == 'R')
						{
						if(!LoadKFactorFile((char*)KFACTORFILE))
							{
							OperatorConsole.printf("***Could not load kfactor file: %s\n", KFACTORFILE);
							exit(1);
							}
						OperatorConsole.printf("Regen single device: %s\n", argv[valid_argc]+3);
						if (!Regen(argv[valid_argc]+3, FALSE)) exit(1);
						OperatorConsole.printf("Regeneration Complete\n");
						}
						
					// rEname device name
					else if (argv[valid_argc][2] == 'e' || argv[valid_argc][2] == 'E')
						{
						char DeviceFrom[256];
						char *p;
						strcpy(DeviceFrom, argv[valid_argc]+3);
						p = strchr(DeviceFrom,',');
						if (p)
							{ 
							*p = 0;
							if (!RenameDevice(DeviceFrom, p+1)) exit(1);
							}
						else
							{
							OperatorConsole.printf("option requires two device names\n");
							exit(1);
							}
						}

					else
						{
						OperatorConsole.printf("Unknown archiving option\n");
						exit(1);
						}

					OperatorConsole.printf(" ---------- End of archival operation -----------\n");
					return ( FALSE );
					}


#ifdef WIN32
				case	's':
				case	'S':	// Create ODBC data source
					{
					char Options[1512], Driver[512];
					int i, len;
#if defined(__BORLANDC__) || defined(__WATCOMC__)
					HINSTANCE hODBCInst = LoadLibrary("odbccp32.dll");
					__SQLConfigDataSource= (_SQLConfigDataSource)GetProcAddress(hODBCInst, "SQLConfigDataSource");
#endif
					strcpy(Driver, "Microsoft dBase Driver (*.dbf)");

					strcpy(Options, 
						"DSN=ConQuestPacs;"
                   				"Description=ConQuest DICOM server data source;"
						"Fil=dBase III;"
						"DriverID=21;"
						"Deleted=1;"
                   				"DefaultDir=C:\\quirt");

					for (i=2; i<strlen(argv[valid_argc]); i++)
						{
						if (argv[valid_argc][i] == ';')
							{
							memcpy(Driver, argv[valid_argc]+2, i-2);
							Driver[i-2]=0;
							strcpy(Options, argv[valid_argc]+i+1);
							Options[strlen(Options)+1] = 0;
							break;
							}
						}

					OperatorConsole.printf("Creating data source\n");
					OperatorConsole.printf("Driver = %s\n", Driver);
					OperatorConsole.printf("Options = %s\n", Options);

					len = strlen(Options);
					for (i=0; i<len; i++)
						if (Options[i] == ';') Options[i] = 0;

					RunServer = FALSE;
					NeedPack = FALSE;
					if ( !SQLConfigDataSource(NULL, ODBC_ADD_SYS_DSN, Driver, Options) )
					// if ( !SQLConfigDataSource(NULL, ODBC_ADD_DSN, Driver, Options) )
						{ char		szMsg[256] = "(No details)";
						  char		szTmp[280];
						  DWORD		dwErrorCode;
						  OperatorConsole.printf("***Datasource configuration FAILED\n");
#if !defined(__BORLANDC__) && !defined(__WATCOMC__) && 0
						  SQLInstallerError(1, &dwErrorCode, szMsg, sizeof(szMsg) - 1, NULL);
						  sprintf(szTmp, "***%s (ec=%d)\n", szMsg, dwErrorCode);
						  OperatorConsole.printf(szTmp);
#endif
						  exit(1);
						}
					else
						OperatorConsole.printf("Datasource configuration succesful\n");
					OperatorConsole.printf(" ----------------------------------\n");
					return ( FALSE );
					}
#endif


				case	'e':	// create database
				case	'E':
					{
					Database DB;
					DebugLevel=4;
					if (strcmp(argv[valid_argc+2], "root")==0 || strcmp(argv[valid_argc+2], "sa")==0)
						{
						if (DB.Open ( DataSource, argv[valid_argc+2], argv[valid_argc]+2, DataHost))
							if (DB.CreateDatabase(argv[valid_argc+1], argv[valid_argc+2], argv[valid_argc]+2))
								OperatorConsole.printf("succesfully created database\n");
						}
					else
						{
						if (DB.Open ( DataSource, "sa", argv[valid_argc]+2, DataHost))
							if (DB.CreateDatabase(argv[valid_argc+1], argv[valid_argc+2], argv[valid_argc+3]))
								OperatorConsole.printf("succesfully created database and login\n");
						}
					OperatorConsole.printf(" ----------------------------------\n");
					exit(0);
					}

				case	'i':	// initialize database
				case	'I':
					RunServer = FALSE;
					NeedPack = FALSE;

					OperatorConsole.printf("Initializing Tables\n");

					if(!LoadKFactorFile((char*)KFACTORFILE))
						{
						OperatorConsole.printf("***Could not load kfactor file: %s\n", KFACTORFILE);
						exit(1);
						}

					mode = atoi(argv[valid_argc]+2);
					InitializeTables (mode);

					return ( FALSE );

				case	'z':	// delete (zap) patient
				case	'Z':
					RunServer = FALSE;
					NeedPack = 5;

					if(!LoadKFactorFile((char*)KFACTORFILE))
						{
						OperatorConsole.printf("***Could not load kfactor file: %s\n", KFACTORFILE);
						exit (1);
						}

					if (!DeletePatient(argv[valid_argc]+2, FALSE))
						exit(1);

					return ( FALSE );

				case	'g':	// grab data from server
				case	'G':
					{
					char *p;

					RunServer = FALSE;
					NeedPack = 5;

					if(!InitACRNemaAddressArray())
						{
						OperatorConsole.printf("***Error loading acr-nema map file:%s\n",ACRNEMAMAP);
						exit(1);
						}

					if(!LoadKFactorFile((char*)KFACTORFILE))
						{
						OperatorConsole.printf("***Could not load kfactor file: %s\n", KFACTORFILE);
						exit (1);
						}

					p = strchr(argv[valid_argc]+2,',');
					if (p)
						{
						*p=0;
						if (!GrabImagesFromServer((unsigned char *)argv[valid_argc]+2, p+1, (char *)MYACRNEMA))
							exit(1);
						}
					else
						{
						if (!GrabImagesFromServer((unsigned char *)argv[valid_argc]+2, "*", (char *)MYACRNEMA))
							exit(1);
						}

					return ( FALSE );
					}

				case	'f':	// Database fix options
				case	'F':
					RunServer = FALSE;
					NeedPack = 5;

					if(!LoadKFactorFile((char*)KFACTORFILE))
						{
						OperatorConsole.printf("***Could not load kfactor file: %s\n", KFACTORFILE);
						exit (1);
						}

					
					if (argv[valid_argc][2] == 'p' || argv[valid_argc][2] == 'P')
						{
						if (!DeletePatient(argv[valid_argc]+3, TRUE))
							exit(1);
						}

					if (argv[valid_argc][2] == 'f' || argv[valid_argc][2] == 'F')
						{
						if (LargestFreeMAG()>atoi(argv[valid_argc]+3)) exit(0);

						if (!PanicKillOff(atoi(argv[valid_argc]+3)))
							exit(1);
						}

					else if (argv[valid_argc][2] == 't' || argv[valid_argc][2] == 'T')
						{
						if (!DeleteStudy(argv[valid_argc]+3, TRUE))
							exit(1);
						}

					else if (argv[valid_argc][2] == 's' || argv[valid_argc][2] == 'S')
						{
						if (!DeleteSeries(argv[valid_argc]+3, TRUE))
							exit(1);
						}

					else if (argv[valid_argc][2] == 'i' || argv[valid_argc][2] == 'I')
						{
						if (!DeleteImage(argv[valid_argc]+3, TRUE))
							exit(1);
						}

					else if (argv[valid_argc][2] == 'r' || argv[valid_argc][2] == 'R')
						{
						char par[256];
						char *p;
						strcpy(par, argv[valid_argc]+3);
						p = strchr(par,',');
						if (p)
							{
							*p=0;
							if (!Regen(par, FALSE, p+1))
								exit(1);
							}
						else
							if (!Regen("MAG0", FALSE, par))
								exit(1);
						}

					else if (argv[valid_argc][2] == 'e' || argv[valid_argc][2] == 'E')
						{
						if (!RegenFile(argv[valid_argc]+3))
							exit(1);
						}

					else if (argv[valid_argc][2] == 'd' || argv[valid_argc][2] == 'D')
						{
						if (!DeleteImageFile(argv[valid_argc]+3, TRUE))
							exit(1);
						}

					else if (argv[valid_argc][2] == 'z' || argv[valid_argc][2] == 'Z')
						{
						if (!DeleteImageFile(argv[valid_argc]+3, FALSE))
							exit(1);
						}

					else if (argv[valid_argc][2] == 'a' || argv[valid_argc][2] == 'A')
						{
						char par[1024];
						char *p;

						strcpy(par, argv[valid_argc]+3);
						p = strchr(par,',');
						if (bUIDPostfixSet)
							gl_iFileCounter = UIDPostfix;
						else
							gl_iFileCounter = ComputeCRC(par, strlen(par));
						if (p)
							{
							*p=0;
							if (!AddImageFile(par, p+1))
								exit(1);
							}
						else
							{
							if (!AddImageFile(par, NULL))
								exit(1);
							}
						}

					else if (argv[valid_argc][2] == '?')
						{
						char UID[256];
						if (!GetImageFileUID(argv[valid_argc]+3, UID))
							exit(1);
						printf("%s\n", UID);
						}

					else if (argv[valid_argc][2] == 'u' || argv[valid_argc][2] == 'U')
						{
						char UID[256];
						if (!GenUID(UID))
							exit(1);
						printf("%s\n", UID);
						}

					else if (argv[valid_argc][2] == 'c' || argv[valid_argc][2] == 'C')
						{
						char par[256];
						char *p;
						strcpy(par, argv[valid_argc]+3);
						p = strchr(par,',');
						if (p)
							{
							*p=0;
							if (!ModifyPATIDofImageFile(p+1, par, TRUE, NULL))
								exit(1);
							}
						}
					else if (argv[valid_argc][2] == 'k' || argv[valid_argc][2] == 'K')
						{
						char par[256];
						char *p;
						strcpy(par, argv[valid_argc]+3);
						p = strchr(par,',');
						if (p)
							{
							*p=0;
							if (!ModifyPATIDofImageFile(p+1, par, FALSE, NULL))
								exit(1);
							}
						}
					return ( FALSE );



				case	'n':	// NKI compression options
				case	'N':
					RunServer = FALSE;
					NeedPack = FALSE;

					if (argv[valid_argc][2] == 'd' || argv[valid_argc][2] == 'D')
						{
						int dum;
						if (!DecompressImageFile(argv[valid_argc]+3, &dum))
							exit(1);
						}

					if (argv[valid_argc][2] == 'c' || argv[valid_argc][2] == 'C')
						{
						int dum;
						if (!CompressNKIImageFile(argv[valid_argc]+4, argv[valid_argc][3]-'0', &dum))
							exit(1);
						}

					return ( FALSE );


				case	'j':	// JPEG compression options
				case	'J':
					RunServer = FALSE;
					NeedPack = FALSE;

					if (argv[valid_argc][2] == 'd' || argv[valid_argc][2] == 'D')
						{
						int dum;
						if (!DecompressImageFile(argv[valid_argc]+3, &dum))
							exit(1);
						}

					if (argv[valid_argc][2] == 'c' || argv[valid_argc][2] == 'C')
						{
						int dum;
						if (!CompressJPEGImageFile(argv[valid_argc]+4, argv[valid_argc][3], &dum))
							exit(1);
						}

					if (argv[valid_argc][2] == '*')		// test recompressFile
						{
						char mode[4];
						mode[0] = argv[valid_argc][3];
						mode[1] = argv[valid_argc][4];
						mode[2] = 0;
						if (!recompressFile(argv[valid_argc]+5, mode))
							exit(1);
						}

					if (argv[valid_argc][2] == '-')		// test recompress
						{
						PDU_Service	PDU;
						VR *pVR;

						PDU.AttachRTC(&VRType);

						DICOMDataObject *pDDO;
						pDDO = PDU.LoadDICOMDataObject(argv[valid_argc]+5);
						char mode[4];
						mode[0] = argv[valid_argc][3];
						mode[1] = argv[valid_argc][4];
						mode[2] = 0;
						if (!recompress(&pDDO, mode, "", mode[0]=='n' || mode[0]=='N'))
							exit(1);
// mvh 20040602: why is this deleted at all?
//					        pVR = pDDO->GetVR(0x0002, 0x0010);
//        					if (pVR) pDDO->DeleteVR(pVR);
						PDU.SaveDICOMDataObject(argv[valid_argc]+5, ACRNEMA_VR_DUMP, pDDO);
						delete pDDO;
						}

					return ( FALSE );


				case	'-':	// Server command
					SendServerCommand("Server command sent using DGATE -- option", argv[valid_argc]+2, 0, NULL, FALSE);
					exit(1);

				default:   	// provide some help
					RunServer = FALSE;
					NeedPack = FALSE;
					PrintOptions();
					return (FALSE);
				}
			}
		else
			{
			FILE	*logFile;

// This is a simple way to get a socket#; but why make it simple if it can be done complex ?
//			Socketfd = (void*)atoi(argv[valid_argc]);


// Open some channel (UDP/pipe) for statistics when running as server child (from .ini file)
			if(OCPipeName[0] == PATHSEPCHAR)
				{
				OperatorConsole.OnMsgPipe(OCPipeName);
				}
			else
				{
				OperatorConsole.OnUDP(OCPipeName, "1111");
				}

// WIN32: get the socket# through shared memory
#ifndef UNIX
			HANDLE		hMap;
			IPCBlock	*IPCBlockPtrInstance;
			hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS,
				FALSE,
				argv[valid_argc]);
			if(!hMap)
				{
				OperatorConsole.printf("***Child: Unable to OpenFileMapping : %d (%s)\n",
					GetLastError(), argv[valid_argc]);
				exit(0);
				}

			IPCBlockPtrInstance = (IPCBlock*)MapViewOfFile(hMap,
				FILE_MAP_ALL_ACCESS, 0,0, sizeof(IPCBlock));
			if(!IPCBlockPtrInstance)
				{
				OperatorConsole.printf("***Child: Unable to MapViewOfFile : %d\n",
					GetLastError());
				}
			Socketfd = IPCBlockPtrInstance->Socketfd;
			UnmapViewOfFile(IPCBlockPtrInstance);
			CloseHandle(hMap);

// UNIX: get the socket# from a small file or pipe
#else
#if 0
			IPCBlock aIPCBlock;
			ifstream aStream(argv[valid_argc], ios::in | ios::nocreate);
			if(aStream.good())
				{
				aStream.read((unsigned char *)&aIPCBlock, sizeof(aIPCBlock));
				}
			if(!aStream.good() || aStream.gcount() != sizeof(aIPCBlock) ||
				aIPCBlock.Magic != IPCBlockMagic)
				{
				// magic # assures safety --
				// we don't try unlink("/"), for example
				OperatorConsole.printf("***Child: Unable to read file %s\n",
					argv[valid_argc]);
				}
			else
				{
					unlink(argv[valid_argc]);
				}
			Socketfd = aIPCBlock.Socketfd;
#endif
#endif
			}

		++valid_argc;
		}

        /* check access for logging and saving images */

	if (1)
		{
	        FILE *f;
	        char Filename[1024];
		int i;

	        f = fopen(TroubleLogFile, "at");
        	if (f) fclose(f);
	        if (!f) OperatorConsole.printf("*** Not enough rights to write to logfiles\n");

		memset(Filename, 0, 1024);
		GetPhysicalDevice("MAG0", Filename);
		strcat(Filename, "printer_files");
		mkdir(Filename);
		i = strlen(Filename);
		Filename[i]   = PATHSEPCHAR;
		Filename[i+1] = 0;
		strcat(Filename, "accesstest.log");
	        f = fopen(Filename, "at");
        	if (f) fclose(f);
	        if (!f) OperatorConsole.printf("*** Not enough rights to write in MAG0\n");
		}

// prepare to run as a child server thread
	if(!LoadKFactorFile((char*)KFACTORFILE))
		{
		OperatorConsole.printf("***Error loading kfactor file:%s\n", KFACTORFILE);
		return ( FALSE );
		}

	if(!InitACRNemaAddressArray())
		{
		OperatorConsole.printf("***Error loading acr-nema map file:%s\n",ACRNEMAMAP);
		return ( FALSE );
		}

	if(!atoi((char*)Port))
		return ( Socketfd );
	if(Valid > 1)				// always TRUE !!!!!
		return ( Socketfd );

	return ( Socketfd );
	}

/*
BOOL
SetString(VR	*vr, char	*s, int	Max)
	{
	memset((void*)s, 0, Max);
	if(vr)
		if(vr->Data)
			{
			memcpy((void*)s, vr->Data, vr->Length%Max);
			return ( TRUE );
			}
	return ( FALSE );
	}
*/

class	DriverApp
	{
	public:
		virtual	BOOL	ServerChild ( int ) = 0;
	public:
		BOOL		ServerChildThread ( int );
		BOOL		Server ( BYTE * );
		int		ChildSocketfd;
		volatile int	Lock;
		DriverApp () { Lock = 0; };
	};

// Query / Retrieve Engine

class	MyPatientRootQuery	:
	public	PatientRootQuery
	{
	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		int	RecordsFound;
	};

class	MyStudyRootQuery	:
	public	StudyRootQuery
	{
	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		int	RecordsFound;
	};

class	MyPatientStudyOnlyQuery	:
	public	PatientStudyOnlyQuery
	{
	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		int	RecordsFound;
	};

	
class	MyModalityWorkListQuery	:
	public	ModalityWorkListQuery
	{
	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		int	RecordsFound;
	};


class	MyPatientRootRetrieve	:
	public	PatientRootRetrieve
	{
		RunTimeClassStorage	RTCStorage;

	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		BOOL	RetrieveOn ( DICOMDataObject *, DICOMDataObject **,
							StandardStorage **);
		BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE * );
	};
	
class	MyPatientRootRetrieveNKI	:
	public	PatientRootRetrieveNKI
	{
		RunTimeClassStorage	RTCStorage;

	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		BOOL	RetrieveOn ( DICOMDataObject *, DICOMDataObject **,
							StandardStorage **,
							DICOMCommandObject	   *,
						        Array < DICOMDataObject *> *,
							void 		*);
		BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE *, BYTE * );
	};

class	MyPatientRootRetrieveGeneric	:
	public	PatientRootRetrieveGeneric
	{
		RunTimeClassStorage	RTCStorage;

	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		BOOL	RetrieveOn ( DICOMDataObject *, DICOMDataObject **,
							StandardStorage **,
							DICOMCommandObject	   *,
						        Array < DICOMDataObject *> *,
							void 		*);
		BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE *, BYTE * );
	};

class	MyStudyRootRetrieve	:
	public	StudyRootRetrieve
	{
		RunTimeClassStorage	RTCStorage;

	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		BOOL	RetrieveOn ( DICOMDataObject *, DICOMDataObject **,
							StandardStorage **);
		BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE * );
	};

class	MyStudyRootRetrieveNKI	:
	public	StudyRootRetrieveNKI
	{
		RunTimeClassStorage	RTCStorage;

	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		BOOL	RetrieveOn ( DICOMDataObject *, DICOMDataObject **,
							StandardStorage **,
							DICOMCommandObject	   *,
						        Array < DICOMDataObject *> *,
							void 		*);
		BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE *, BYTE * );
	};

class	MyStudyRootRetrieveGeneric	:
	public	StudyRootRetrieveGeneric
	{
		RunTimeClassStorage	RTCStorage;

	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		BOOL	RetrieveOn ( DICOMDataObject *, DICOMDataObject **,
							StandardStorage **,
							DICOMCommandObject	   *,
						        Array < DICOMDataObject *> *,
							void 		*);
		BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE *, BYTE * );
	};

class	MyPatientStudyOnlyRetrieve	:
	public	PatientStudyOnlyRetrieve
	{
		RunTimeClassStorage	RTCStorage;

	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		BOOL	RetrieveOn ( DICOMDataObject *, DICOMDataObject **,
							StandardStorage **);
		BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE * );
	};
	
class	MyPatientStudyOnlyRetrieveNKI	:
	public	PatientStudyOnlyRetrieveNKI
	{
		RunTimeClassStorage	RTCStorage;

	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		BOOL	RetrieveOn ( DICOMDataObject *, DICOMDataObject **,
							StandardStorage **,
							DICOMCommandObject	   *,
						        Array < DICOMDataObject *> *,
							void 		*);
		BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE *, BYTE * );
	};

class	MyPatientStudyOnlyRetrieveGeneric	:
	public	PatientStudyOnlyRetrieveGeneric
	{
		RunTimeClassStorage	RTCStorage;

	public:
		BOOL	SearchOn (DICOMDataObject *, Array < DICOMDataObject *> *);
		BOOL	CallBack (DICOMCommandObject *, DICOMDataObject *);
		BOOL	RetrieveOn ( DICOMDataObject *, DICOMDataObject **,
							StandardStorage **,
							DICOMCommandObject	   *,
						        Array < DICOMDataObject *> *,
							void 		*);
		BOOL	QualifyOn ( BYTE *, BYTE *, BYTE *, BYTE *, BYTE * );
	};

// This class handles storage; used to allow error messages to be sent back to the caller

class	MyUnknownStorage	:	public UnknownStorage
	{
	public:
		Database *DB;		// must be set by caller (needs one per thread)
		BOOL	 nopreget;	// must be set by caller (derived from messageid)

				// called for each incoming DDO
		UINT16 CheckObject(DICOMDataObject *DDO, PDU_Service *PDU) 
			{
			char Filename[1024];
			char Called[20], Calling[20];
			char *Compression;

			if (!DB) return 0;	// no db set: processing done by caller (failsafe_storage)

#ifdef	DEBUG_MODE
		//OperatorConsole.printf("C-Store (RTC class)\n");
#endif
			ImagesReceived++;

			// If the called AE looks like SERVER~j2, then the last 2 characters override IncomingCompression
			strcpy(Called, (char *)(((AAssociateAC *)PDU)->CalledApTitle));
			while (Called[strlen(Called)-1]==' ') Called[strlen(Called)-1] = 0;

			strcpy(Calling, (char *)(((AAssociateAC *)PDU)->CallingApTitle));
			while (Calling[strlen(Calling)-1]==' ') Calling[strlen(Calling)-1] = 0;

                        Compression = strchr(Called, '~');
			if (!Compression) Compression = IncomingCompression; else Compression++;
					
			// Recompress (e.g., leave as is or decompress) incoming files
			recompress(&DDO, Compression, "", Compression[0]=='n' || Compression[0]=='N');

			// Virtual compression: save in database, not on disk
			if (Compression[0]=='v' || Compression[0]=='V')
				{
				char		rRoot[1024], rFilename[1024];
				char		Device[256];

				FixImage(DDO);	// Make sure the required DICOM UID's are there + fix patient ID if enabled
		
				*rRoot=1;
				if (!GenerateFileName(DDO, Device, rRoot, rFilename, TRUE, 0, Called, Calling, DB))
					{
					OperatorConsole.printf("**** Virtual storage filename problem - %s\n", rFilename);
					}
				if(!SaveToDataBase(*DB, DDO, rFilename, Device, *rRoot))
					{
					OperatorConsole.printf("**** Virtual storage db problem - %s\n", rFilename);
					}

				OperatorConsole.printf("Virtual file: %s\n", rFilename);
				delete DDO;

				ImagesSaved++;
				return  0;
				}

			// NOTE: NOT THREAD SAFE - IF ONE THREAD HANDLES READS AND WRITES THIS OPERATION CAN FAIL DUE TO DB SHARING:
			if(!SaveToDisk(*DB, DDO, Filename, FALSE, ((AAssociateAC *)PDU)->CallingApTitle, ((AAssociateAC *)PDU)->CalledApTitle, 0, nopreget))
				{
				delete DDO;
				return StorageFailedErrorCode;	// Processing failed
				}

			else
				{
#ifdef	DEBUG_MODE
				if(strlen(Filename))
					OperatorConsole.printf("Written file: %s\n", Filename);
#endif
				ImagesSaved++;
				return 0;
				}
			}
	};

class	StorageApp	:
	public	DriverApp
	{
		// MyUnknownStorage		SOPUnknownStorage;
		//Verification				SOPVerification;
		//MyPatientRootQuery			SOPPatientRootQuery;
		//MyPatientRootRetrieve			SOPPatientRootRetrieve;
		//MyPatientRootRetrieveNKI		SOPPatientRootRetrieveNKI;
		//MyPatientRootRetrieveGeneric		SOPPatientRootRetrieveGeneric;

		//MyStudyRootQuery			SOPStudyRootQuery;
		//MyStudyRootRetrieve			SOPStudyRootRetrieve;
		//MyStudyRootRetrieveNKI			SOPStudyRootRetrieveNKI;
		//MyStudyRootRetrieveGeneric		SOPStudyRootRetrieveGeneric;

		//MyPatientStudyOnlyQuery			SOPPatientStudyOnlyQuery;
		//MyPatientStudyOnlyRetrieve		SOPPatientStudyOnlyRetrieve;
		//MyPatientStudyOnlyRetrieveNKI		SOPPatientStudyOnlyRetrieveNKI;
		//MyPatientStudyOnlyRetrieveGeneric	SOPPatientStudyOnlyRetrieveGeneric;

		//MyModalityWorkListQuery			SOPModalityWorkListQuery;
	public:
		BOOL	ServerChild ( int );
		void FailSafeStorage(CheckedPDU_Service *PDU);
		BOOL PrinterSupport( CheckedPDU_Service *PDU, DICOMCommandObject *DCO, DICOMDataObject *PrintData[]);
		BOOL StorageCommitmentSupport( CheckedPDU_Service *PDU, DICOMCommandObject *DCO, DICOMDataObject **CommitData);
	};

// from dbsql.cpp
BOOL
UpdateOrAddToTable(
	Database		&DB,
	DBENTRY			*DCMGateDB,
	char			*TableName,
	DICOMDataObject		*DDOPtr,
	char			*ObjectFile,
	char			*DeviceName,
	char			*Patid,
	char			*Modality,
	BOOL			*Added,
	BOOL			JustAdd,
	BOOL			CheckDuplicates);

void KodakFixer(DICOMDataObject	*DDOPtr, BOOL tokodak)
{ 	VR *vr = DDOPtr->GetVR(0x0010, 0x0020);
  	char patid[11];
  	int Length;

 	if (!vr) return;
	if (!vr->Data) return;
	if (!vr->Length) return;
	if (vr->Length>8) return;

	memset(patid, 0, 10);
	memcpy(patid, (char *)(vr->Data), vr->Length);
	patid[vr->Length]=0;
	if (patid[vr->Length-1]==' ') patid[vr->Length-1] = 0;

	if (!tokodak && strlen(patid)==8 && patid[0] == '0' && atoi(patid)>1000000 )
		{
		vr->ReAlloc(8);
		patid[8] = ' ';
		memcpy(vr->Data, patid + 1, 8);

		SystemDebug.printf("Fixed patient ID from kodak, now %s\n", patid+1);
		}

	if (tokodak && strlen(patid)==7 && patid[0] != '0' && atoi(patid)>1000000 )
		{
		vr->ReAlloc(8);
		((char *)(vr->Data))[0]='0';
		memcpy(((char *)(vr->Data))+1, patid, 7);

		//sprintf((char *)(vr->Data), "0%s", patid);

		SystemDebug.printf("Fixed patient ID to kodak, now 0%s\n", patid);
		}
	}

// virtual query: pass query (extended) through to other server and populate db with results (no images)

int VirtualQuery(DICOMDataObject *DDO, char *Level, int N, Array < DICOMDataObject  *> *pADDO)
	{
	char AE[48];
	strcpy(AE, VirtualServerFor[N]);
	if (*AE==0) return FALSE;

        char *param = strchr(AE, ',');
        if (param) *param++ = 0;

	unsigned char 	ip[64], port[64], compress[64], SOP[66];
	char		rRoot[1024], rFilename[1024], ID[66];
	VR		*vr;
	UID		uid;
	DICOMCommandObject	DCO, DCOR;
	DICOMDataObject		DDOR;
	LE_UINT16	command, datasettype, messageid, priority;
	BOOL		quit;
	Database 	DB;
	char		Device[256];
	DICOMDataObject	*DDOPtr, *DDOPtr2;
	DBENTRY		*DBE;
	int		Index, level, count=0;
		
	level=0;
	if      (strncmp(Level, "PATIENT", 7)==0) level=1;
	else if (strncmp(Level, "STUDY",   5)==0) level=2;
	else if (strncmp(Level, "SERIES",  6)==0) level=3;
	else if (strncmp(Level, "IMAGE",   5)==0) level=4;

	PDU_Service	PDU;
	PDU.AttachRTC(&VRType);

	if(!GetACRNema(AE, (char *)ip, (char *)port, (char *)compress))
		return 0;

	// 20070103
	if (param)
		if (stricmp(param, "FixKodak")==0)		// disallow wildcard date queries
			{
			BOOL flag;
			vr = DDO->GetVR(0x0010, 0x0030);	// birthdate
			flag =         (vr && vr->Length>0 && *(char *)vr->Data == '*');
			vr = DDO->GetVR(0x0008, 0x0020);	// study date
			flag = flag || (vr && vr->Length>0 && *(char *)vr->Data == '*');
			vr = DDO->GetVR(0x0008, 0x0021);	// series date
			flag = flag || (vr && vr->Length>0 && *(char *)vr->Data == '*');
			vr = DDO->GetVR(0x0008, 0x0022);	// acq date
			flag = flag || (vr && vr->Length>0 && *(char *)vr->Data == '*');
			vr = DDO->GetVR(0x0008, 0x0023);	// img date
			flag = flag || (vr && vr->Length>0 && *(char *)vr->Data == '*');

			if (flag)
				{
				OperatorConsole.printf("Virtual query - blocked wildcard date query to %s\n", AE);
				return 0;
				}
			}
	

	// Start setting up connection for C-FIND
	PDU.ClearAbstractSyntaxs();
	PDU.SetLocalAddress(MYACRNEMA);
	PDU.SetRemoteAddress((unsigned char *)AE);

	uid.Set("1.2.840.10008.3.1.1.1");	// Application context (DICOM App)
	PDU.SetApplicationContext(uid);

	if (level==1) uid.Set("1.2.840.10008.5.1.4.1.2.1.1"); // PatientRootQuery
	else          uid.Set("1.2.840.10008.5.1.4.1.2.2.1"); // StudyRootQuery
	PDU.AddAbstractSyntax(uid);

	PDU.SetTimeOut(TCPIPTimeOut);

	// Make the association for the FIND on port/ip
	if(!PDU.Connect(ip, port))
		{
		OperatorConsole.printf("*** Virtual query - failed to connect for C-FIND to %s\n", AE);
		return ( 0 );
		}


	// Start a Patient/StudyRootQuery

	if (level==1) strcpy((char*) SOP, "1.2.840.10008.5.1.4.1.2.1.1"); // PatientRootQuery
	else          strcpy((char*) SOP, "1.2.840.10008.5.1.4.1.2.2.1"); // StudyRootQuery
	vr = new VR (0x0000, 0x0002, strlen((char*)SOP), (void*) SOP, FALSE);
	DCO.Push(vr);
	command = EN0020;
	vr = new VR (0x0000, 0x0100, 0x0002, &command, FALSE);
	DCO.Push(vr);
	priority = 0;	// MEDIUM
	vr = new VR (0x0000, 0x0700, 0x0002, &priority, FALSE);
	DCO.Push(vr);
	datasettype = EN0102;	
	vr = new VR (0x0000, 0x0800, 0x0002, &datasettype, FALSE);
	DCO.Push(vr);
	messageid = EN0003;
	vr = new VR (0x0000, 0x0110, 0x0002, &messageid, FALSE);
	DCO.Push(vr);

	DDOPtr  = MakeCopy(DDO);
	DDOPtr2 = MakeCopy(DDO);
	
	// Use passed data object and Level for query

	SetStringVR(&vr, 0x0008, 0x0052, Level);
	DDOPtr->ReplaceVR(vr);

	// Here fixkodak fiximage should operate adding 0 to e.g., 9901234 patient ID
	if (param)
		if (stricmp(param, "FixKodak")==0)
			KodakFixer(DDOPtr, TRUE);

///////// virtual queries not via database (experimental - complicated cleanup of pADDO required)
	if (pADDO && 1)
		{
		MyPatientRootQuery mq;
		MyStudyRootQuery sq;
		int s=pADDO->GetSize();
	
		// forward query
		if (level==1) mq.Write(&PDU, DDOPtr, pADDO);
		else          sq.Write(&PDU, DDOPtr, pADDO);
	
		PDU.Close();

		// anything recieved ?
		if (pADDO->GetSize() != s)
			{
			DBENTRY *dum;
			int el[1000], gr[1000], count, i, j, count2;
			BOOL us[1000];
			DICOMDataObject DO2;

			if (DebugLevel>=3)
				{
				SystemDebug.printf("Virtual query: first response from %s (total %d records)\n", AE, pADDO->GetSize() - s);
				NonDestructiveDumpDICOMObject(pADDO->Get(s));
				}

			// build list of valid response vr's: those in query AND in our database at correct level
			// Our database model is used because the virtual server mimics ours with data from elsewhere

			count=0;
			while (vr = DDOPtr2->Pop())
				{
				if (level==0)
					{
					if (VerifyIsInDBE(vr, WorkListDB, dum))
						{
						us[count  ] = FALSE;
						el[count  ] = vr->Element;
						gr[count++] = vr->Group;
						delete vr;
						continue;
						}
					}
				if (level>=1)
					{
					if (VerifyIsInDBE(vr, PatientDB, dum))
						{
						us[count  ] = FALSE;
						el[count  ] = vr->Element;
						gr[count++] = vr->Group;
						delete vr;
						continue;
						}
					}
				if (level>=2)
					{
					if (VerifyIsInDBE(vr, StudyDB, dum))
						{
						us[count  ] = FALSE;
						el[count  ] = vr->Element;
						gr[count++] = vr->Group;
						delete vr;
						continue;
						}
					}
				if (level>=3)
					{
					if (VerifyIsInDBE(vr, SeriesDB, dum))
						{
						us[count  ] = FALSE;
						el[count  ] = vr->Element;
						gr[count++] = vr->Group;
						delete vr;
						continue;
						}
					}
				if (level>=4)
					{
					if (VerifyIsInDBE(vr, ImageDB, dum))
						{
						us[count  ] = FALSE;
						el[count  ] = vr->Element;
						gr[count++] = vr->Group;
						delete vr;
						continue;
						}
					}
				delete vr;
				}

			count2=0;	// counts valid fields during first record

			OperatorConsole.printf("Virtualquery: processing response from server: %s\n", AE);
			for (i=s; i<pADDO->GetSize(); i++)
				{
				if (i==s+1 && count2==count)		// after one record: all fields were valid 
					break;

				if (i==s+1 && count2!=count) 
					OperatorConsole.printf("Virtualquery: cleaning response from server: %s %d %d\n", AE, count2, count);

				while(vr=pADDO->Get(i)->Pop())
					{
					for (j=0; j<count; j++)
						{
						if (0x0052==vr->Element && 0x0008==vr->Group)
							{
							delete vr;
							break;	// auto generated - no need to copy
							}

						if (0x0000==vr->Element)
							{
							delete vr;
							break;	// auto generated - no need to copy
							}

						if (el[j]==vr->Element && gr[j]==vr->Group)
							{		// copy valid fields
							DO2.Push(vr);
							us[j] = TRUE;
							count2++;
							break;
							}

						if (j==count-1)
							{
							count2=-10000;	// superfluous fields: cleanup always required
							delete vr;
							}
						}

					}

				for (j=0; j<count; j++)			// insert empties for missing fields
					{
					if (!us[j])
						{
						vr = new VR(gr[j], el[j], 0, FALSE);
						DO2.Push(vr);
						}
					else
						us[j]=FALSE;
					}

				pADDO->Get(i)->Reset();
				while(vr=DO2.Pop())
					pADDO->Get(i)->Push(vr);

				DO2.Reset();
				}

			// Here fixkodak fiximage should operate stripping 0 to e.g., 09901234 patient ID
			for (i=s; i<pADDO->GetSize(); i++)
				if (param)
					if (stricmp(param, "FixKodak")==0)
						KodakFixer(pADDO->Get(i), FALSE);

			}
		else
			while (vr = DDOPtr2->Pop())
				delete vr;

		delete DDOPtr2;
		delete DDOPtr;

		return pADDO->GetSize() - s;
		}
//////////////////////////////////

	Index = 0; DBE = PatientDB;
	if (level==1) while ( TRUE )	// patient query
		{
		if(!DBE[Index].Element) break;
		if (DDOPtr->GetVR(DBE[Index].Group, DBE[Index].Element)) {++Index; continue; }
		SetStringVR(&vr, DBE[Index].Group, DBE[Index].Element, "");
		DDOPtr->Push(vr);
		++Index;
		}
	Index = 0; DBE = StudyDB;
	if (level>=2) while ( TRUE )	// study, series and image query
		{
		if(!DBE[Index].Element) break;
		if (DDOPtr->GetVR(DBE[Index].Group, DBE[Index].Element)) {++Index; continue; }
		SetStringVR(&vr, DBE[Index].Group, DBE[Index].Element, "");
		DDOPtr->Push(vr);
		++Index;
		}
	Index = 0; DBE = SeriesDB;
	if (level>=3) while ( TRUE )	// series and image query
		{
		if(!DBE[Index].Element) break;
		if (DDOPtr->GetVR(DBE[Index].Group, DBE[Index].Element)) {++Index; continue; }
		SetStringVR(&vr, DBE[Index].Group, DBE[Index].Element, "");
		DDOPtr->Push(vr);
		++Index;
		}
	Index = 0; DBE = ImageDB;
	if (level>=4) while ( TRUE )	// image query
		{
		if(!DBE[Index].Element) break;
		if (DDOPtr->GetVR(DBE[Index].Group, DBE[Index].Element)) {++Index; continue; }
		SetStringVR(&vr, DBE[Index].Group, DBE[Index].Element, "");
		DDOPtr->Push(vr);
		++Index;
		}

	// Here fixkodak fiximage should operate adding 0 to e.g., 9901234 patient ID
	if (param)
		if (stricmp(param, "FixKodak")==0)
			KodakFixer(DDOPtr, TRUE);
	
	// Write query command object and data object
	PDU.Write(&DCO, uid);
	PDU.Write(DDOPtr, uid);

	DB.Open ( DataSource, UserName, Password, DataHost );

	// Get response

	quit = 0;
	while(!quit)
		{
		if(!PDU.Read(&DCOR))
			{
			OperatorConsole.printf("*** Virtual query - association lost in C-FIND to %s\n", AE);
			break;
			}
		// is this a C-Find-RSP ?
		if(DCOR.GetUINT16(0x0000, 0x0100)!=0x8020)
			{
			OperatorConsole.printf("*** Virtual query - wrong response for C-FIND to %s\n", AE);
			break;
			}
		// is there a data set?
		if(DCOR.GetUINT16(0x0000, 0x0800)==0x0101)
			{
			OperatorConsole.printf("Virtual query from %s - finished\n", AE);
			break;
			}
		// no success
		if(DCOR.GetUINT16(0x0000, 0x0900)==0x0000)
			{
			OperatorConsole.printf("*** Virtual query - C-FIND failed to %s\n", AE);
			break;
			}
		// read response data
		if(!PDU.Read(&DDOR))
			{
			OperatorConsole.printf("*** Virtual query - C-FIND failed to %s\n", AE);
			break;
			}

		while(vr = DCOR.Pop())
			delete vr;

		// enter new results in database
		memset((void*)ID, 0, 65);
		vr = DDOR.GetVR(0x0010, 0x0020);
		if(vr) strncpy(ID, (char*) vr->Data, vr->Length);
		if (vr->Length)
		        if (ID[vr->Length-1]==' ') ID[vr->Length-1]=0;

		if (level>=4)			// full db add
			{
			FixImage(&DDOR);	// Make sure the required DICOM UID's are there + fix patient ID if enabled
		
			*rRoot=1;
			*rFilename=0;
			if (!GenerateFileName(&DDOR, Device, rRoot, rFilename, TRUE, 0, (char *)MYACRNEMA, (char *)AE, &DB))
				OperatorConsole.printf("**** Virtual query (%s) filename problem - %s\n", AE, rFilename);

			if (*rRoot) 		// keeps set if image is not on server
				{
				if (strchr(Device, '.')==NULL)
					sprintf(Device+strlen(Device), ".%d", N);	// set virtual device # (keep lowest)
				if(!SaveToDataBase(DB, &DDOR, rFilename, Device, TRUE))
					OperatorConsole.printf("**** Virtual query db problem - %s\n", rFilename);
				else
					{
					OperatorConsole.printf("Virtual query (%s) added - %s\n", AE, rFilename);
					count++;
					}
				}
			}
		else				// partial db add
			{
			BOOL AddedImage, AddedStudy, AddedSeries, AddedPatient, AddedWorkList;

			if (level>=3)
				UpdateOrAddToTable(DB, SeriesDB, SeriesTableName, &DDOR, "",  NULL,  ID, NULL, &AddedSeries, FALSE, FALSE);
			if (level>=2)
				UpdateOrAddToTable(DB, StudyDB,  StudyTableName,  &DDOR, "",  NULL,  ID, NULL, &AddedStudy, FALSE, TRUE);
			if (level>=1)
				UpdateOrAddToTable(DB, PatientDB,PatientTableName,&DDOR, "",  NULL,  ID, NULL, &AddedPatient, FALSE, FALSE);
			if (level==0)
				UpdateOrAddToTable(DB, WorkListDB, WorkListTableName,&DDOR, "",  NULL,  ID, NULL, &AddedWorkList, FALSE, FALSE);

			OperatorConsole.printf("Virtual query (%s) added db at level - %s\n", AE, Level);
			count++;
			}

		while(vr = DDOR.Pop())
			delete vr;
		}

	while (vr = DDOPtr2->Pop())
		delete vr;
	while (vr = DDOPtr->Pop())
		delete vr;
	delete DDOPtr2;
	delete DDOPtr;

	PDU.Close();	
	return count;
	}

// sort functions used by RemoveQueryDuplicates

static int SortPatient(const void* pElem1, const void* pElem2)
	{
	VR *vr1, *vr2;
	DICOMDataObject *ddo1= *(DICOMDataObject **) pElem1;
	DICOMDataObject *ddo2= *(DICOMDataObject **) pElem2;

	vr1 = ddo1->GetVR(0x0010, 0x0020);
	vr2 = ddo2->GetVR(0x0010, 0x0020);

	if (vr1==NULL || vr2==NULL)
		return 0;
	if (vr1->Length < vr2->Length)
		return -1;
	if (vr1->Length > vr2->Length)
		return  1;
	return memicmp((char *)vr1->Data, (char *)vr2->Data, vr1->Length);
	}

static int SortStudy(const void* pElem1, const void* pElem2)
	{
	VR *vr1, *vr2;
	DICOMDataObject *ddo1= *(DICOMDataObject **) pElem1;
	DICOMDataObject *ddo2= *(DICOMDataObject **) pElem2;

	vr1 = ddo1->GetVR(0x0020, 0x000d);
	vr2 = ddo2->GetVR(0x0020, 0x000d);

	if (vr1==NULL || vr2==NULL)
		return 0;
	if (vr1->Length < vr2->Length)
		return -1;
	if (vr1->Length > vr2->Length)
		return  1;
	return memicmp((char *)vr1->Data, (char *)vr2->Data, vr1->Length);
	}

static int SortSeries(const void* pElem1, const void* pElem2)
	{
	VR *vr1, *vr2;
	DICOMDataObject *ddo1= *(DICOMDataObject **) pElem1;
	DICOMDataObject *ddo2= *(DICOMDataObject **) pElem2;

	vr1 = ddo1->GetVR(0x0020, 0x000e);
	vr2 = ddo2->GetVR(0x0020, 0x000e);

	if (vr1==NULL || vr2==NULL)
		return 0;
	if (vr1->Length < vr2->Length)
		return -1;
	if (vr1->Length > vr2->Length)
		return  1;
	return memicmp((char *)vr1->Data, (char *)vr2->Data, vr1->Length);
	}

static int SortImages(const void* pElem1, const void* pElem2)
	{
	VR *vr1, *vr2;
	DICOMDataObject *ddo1= *(DICOMDataObject **) pElem1;
	DICOMDataObject *ddo2= *(DICOMDataObject **) pElem2;

	vr1 = ddo1->GetVR(0x0008, 0x0018);
	vr2 = ddo2->GetVR(0x0008, 0x0018);

	if (vr1==NULL || vr2==NULL)
		return 0;
	if (vr1->Length < vr2->Length)
		return -1;
	if (vr1->Length > vr2->Length)
		return  1;
	return memicmp((char *)vr1->Data, (char *)vr2->Data, vr1->Length);
	}

static int SortAccession(const void* pElem1, const void* pElem2)
	{
	VR *vr1, *vr2;
	DICOMDataObject *ddo1= *(DICOMDataObject **) pElem1;
	DICOMDataObject *ddo2= *(DICOMDataObject **) pElem2;

	vr1 = ddo1->GetVR(0x0008, 0x0050);
	vr2 = ddo2->GetVR(0x0008, 0x0050);

	if (vr1==NULL || vr2==NULL)
		return 0;
	if (vr1->Length < vr2->Length)
		return -1;
	if (vr1->Length > vr2->Length)
		return  1;
	return memicmp((char *)vr1->Data, (char *)vr2->Data, vr1->Length);
	}

static int SortImageNumber(const void* pElem1, const void* pElem2)
	{
	VR *vr1, *vr2;
	DICOMDataObject *ddo1= *(DICOMDataObject **) pElem1;
	DICOMDataObject *ddo2= *(DICOMDataObject **) pElem2;
	char t1[68], t2[68];

	vr1 = ddo1->GetVR(0x0020, 0x0013);
	vr2 = ddo2->GetVR(0x0020, 0x0013);

	if (vr1==NULL || vr2==NULL)
		return 0;

	strncpy(t1, (char *)vr1->Data, vr1->Length); t1[vr1->Length]=0;
	strncpy(t2, (char *)vr2->Data, vr2->Length); t2[vr2->Length]=0;

	if (atoi(t1) < atoi(t2))
		return -1;
	else if (atoi(t1) > atoi(t2))
		return  1;
	else
		return  0;
	}

// Remove duplicate entries from a query response - on selected Level

void RemoveQueryDuplicates(char *Level, Array < DICOMDataObject * > *ADDO)
	{
	DICOMDataObject **addo = (DICOMDataObject **)malloc(ADDO->GetSize() * sizeof(DICOMDataObject *));
	int i, j, g, e;
	VR *vr1, *vr2;

	for (i=0; i<ADDO->GetSize(); i++) addo[i] = ADDO->Get(i);

	// sort on primary key
	     if (strncmp(Level, "PATIENT", 7)==0) qsort(addo, ADDO->GetSize(), sizeof(DICOMDataObject *), SortPatient),  g=0x0010, e=0x0020;
	else if (strncmp(Level, "STUDY",   5)==0) qsort(addo, ADDO->GetSize(), sizeof(DICOMDataObject *), SortStudy),    g=0x0020, e=0x000d;
	else if (strncmp(Level, "SERIES",  6)==0) qsort(addo, ADDO->GetSize(), sizeof(DICOMDataObject *), SortSeries),   g=0x0020, e=0x000e;
	else if (strncmp(Level, "IMAGE",   5)==0) qsort(addo, ADDO->GetSize(), sizeof(DICOMDataObject *), SortImages),   g=0x0008, e=0x0018;
	else if (strncmp(Level, "WORKLIST",8)==0) qsort(addo, ADDO->GetSize(), sizeof(DICOMDataObject *), SortAccession),g=0x0008, e=0x0050;
        else 
		{
		free(addo);
		return;
		}

        j = ADDO->GetSize();
	while(ADDO->GetSize()) ADDO->RemoveAt(0);

	// remove duplicates of primary key
	for (i=0; i<j; i++) 
		{
		if (i>0)
			{
			vr1 = addo[i-1]->GetVR(g, e);
			vr2 = addo[i  ]->GetVR(g, e);
			if (vr1 && vr2 && vr1->Length==vr2->Length)
				if (memicmp((char *)vr1->Data, (char *)vr2->Data, vr1->Length)==0)
					{
					ADDO->Get(ADDO->GetSize()-1) = addo[i];
					delete addo[i-1];
					continue;
					}
			}
		ADDO->Add(addo[i]);
		}
	SystemDebug.printf("Virtualquery: removed %d duplicates out of %d entries\n", j-ADDO->GetSize(), j);
	free(addo);
	}

// Patient, study or series finder (server functional); search string starts with = means exact

BOOL	PatientStudyFinder(char *server, char *str, char *fmt, FILE *f, char *level)
	{	
	DICOMDataObject DDO;
	VR *vr;
	char tmp[256];
	char outp[7][256];
	BOOL local=stricmp(server, (char *)MYACRNEMA)==0;
	Array < DICOMDataObject * > ADDO;
	int i, j;

	// search ID
	SetStringVR(&vr, 0x0008, 0x0052, level); 
	DDO.Push(vr); 
	SetStringVR(&vr, 0x0010, 0x0010, ""); 
	DDO.Push(vr);
	if (str[0] == '=')
		sprintf(tmp, "%s", str+1);
	else
		sprintf(tmp, "*%s*", str);
	SetStringVR(&vr, 0x0010, 0x0020, tmp); 
	DDO.Push(vr);
	if (level[0]=='S')
		{
		SetStringVR(&vr, 0x0008, 0x0020, ""); 
		DDO.Push(vr);
		SetStringVR(&vr, 0x0020, 0x000d, ""); 
		DDO.Push(vr);
		if (level[1]=='E')
			{
			SetStringVR(&vr, 0x0008, 0x0031, ""); 
			DDO.Push(vr);
			SetStringVR(&vr, 0x0020, 0x000e, ""); 
			DDO.Push(vr);
			SetStringVR(&vr, 0x0008, 0x0060, ""); 
			DDO.Push(vr);
			}
		else
			{
			SetStringVR(&vr, 0x0008, 0x0061, ""); 
			DDO.Push(vr);
			}
		}

	if (strcmp(server, "local")==0)
		{
		if (level[0]=='P') QueryOnPatient (&DDO, &ADDO);
		if (level[0]=='S' && level[1]=='T') QueryOnStudy   (&DDO, &ADDO);
		if (level[0]=='S' && level[1]=='E') QueryOnSeries  (&DDO, &ADDO);
		}
	else
		{
		strncpy(VirtualServerFor[10], server, 16);
		VirtualServerFor[10][16]=0;
		VirtualQuery(&DDO, level, 10, &ADDO);
		}

	DDO.Reset();

	// search name
	SetStringVR(&vr, 0x0008, 0x0052, level); 
	DDO.Push(vr); 
	if (str[0] == '=')
		sprintf(tmp, "%s", str+1);
	else
		sprintf(tmp, "*%s*", str);
	SetStringVR(&vr, 0x0010, 0x0010, tmp); 
	DDO.Push(vr);
	SetStringVR(&vr, 0x0010, 0x0020, ""); 
	DDO.Push(vr);
	if (level[0]=='S')
		{
		SetStringVR(&vr, 0x0008, 0x0020, ""); 
		DDO.Push(vr);
		SetStringVR(&vr, 0x0020, 0x000d, ""); 
		DDO.Push(vr);
		if (level[1]=='E')
			{
			SetStringVR(&vr, 0x0008, 0x0031, ""); 
			DDO.Push(vr);
			SetStringVR(&vr, 0x0020, 0x000e, ""); 
			DDO.Push(vr);
			SetStringVR(&vr, 0x0008, 0x0060, ""); 
			DDO.Push(vr);
			}
		else
			{
			SetStringVR(&vr, 0x0008, 0x0061, ""); 
			DDO.Push(vr);
			}
		}
	if (strcmp(server, "local")==0)
		{
		if (level[0]=='P') QueryOnPatient (&DDO, &ADDO);
		if (level[0]=='S' && level[1]=='T') QueryOnStudy   (&DDO, &ADDO);
		if (level[0]=='S' && level[1]=='E') QueryOnSeries  (&DDO, &ADDO);
		}
	else
		{
		strncpy(VirtualServerFor[10], server, 16);
		VirtualServerFor[10][16]=0;
		VirtualQuery(&DDO, level, 10, &ADDO);
		}

	DDO.Reset();

	// search study date (=passed as is), only yyyy, yyyymm, yyyymmdd or data range queries
	if (level[0]=='S' && str[0]!='=')
		{
		SetStringVR(&vr, 0x0008, 0x0052, level); 
		DDO.Push(vr); 
		if (str[0] == '=')
			sprintf(tmp, "%s", str+1);
		else
			{
			if (strlen(str)==4 && atoi(str)>1900 && atoi(str)<2200)
				sprintf(tmp, "%s0101-%s1231", str, str);
			else if (strlen(str)==6 && atoi(str)>190000 && atoi(str)<220000)
				sprintf(tmp, "%s01-%s31", str, str);
			else if (strlen(str)==8 && atoi(str)>19000000 && atoi(str)<22000000)
				sprintf(tmp, "%s", str);
			else if (strlen(str)==9 && atoi(str)>19000000 && atoi(str)<22000000 && str[8]=='-')
				sprintf(tmp, "%s", str);
			else if (strlen(str)==9 && atoi(str+1)>19000000 && atoi(str+1)<22000000 && str[0]=='-')
				sprintf(tmp, "%s", str);
			else if (strlen(str)==17 && atoi(str)>19000000 && atoi(str)<22000000 && str[8]=='-' && atoi(str+9)>19000000 && atoi(str+9)<22000000)
				sprintf(tmp, "%s", str);
			else
				sprintf(tmp, "invalid");
			}
                

		SetStringVR(&vr, 0x0008, 0x0020, tmp); 
		DDO.Push(vr);
		SetStringVR(&vr, 0x0010, 0x0010, ""); 
		DDO.Push(vr);
		SetStringVR(&vr, 0x0010, 0x0020, ""); 
		DDO.Push(vr);
		SetStringVR(&vr, 0x0020, 0x000d, ""); 
		DDO.Push(vr);
		if (level[1]=='E')
			{
			SetStringVR(&vr, 0x0008, 0x0031, ""); 
			DDO.Push(vr);
			SetStringVR(&vr, 0x0020, 0x000e, ""); 
			DDO.Push(vr);
			SetStringVR(&vr, 0x0008, 0x0060, ""); 
			DDO.Push(vr);
			}
		else
			{
			SetStringVR(&vr, 0x0008, 0x0061, ""); 
			DDO.Push(vr);
			}


		if (strcmp(tmp, "invalid")!=0)
			{
			if (strcmp(server, "local")==0)
				{
				if (level[1]=='T') QueryOnStudy   (&DDO, &ADDO);
				if (level[1]=='E') QueryOnSeries  (&DDO, &ADDO);
				}
			else
				{
				strncpy(VirtualServerFor[10], server, 16);
				VirtualServerFor[10][16]=0;
				VirtualQuery(&DDO, level, 10, &ADDO);
				}
			}

		DDO.Reset();
		}

	// search study UID (as is) for series level (exact only)
	if (level[0]=='S' && level[1]=='E' && str[0] == '=' && strlen(str)>15)
		{
		SetStringVR(&vr, 0x0008, 0x0052, level); 
		DDO.Push(vr); 
		SetStringVR(&vr, 0x0008, 0x0020, ""); 
		DDO.Push(vr);
		SetStringVR(&vr, 0x0010, 0x0010, ""); 
		DDO.Push(vr);
		SetStringVR(&vr, 0x0010, 0x0020, ""); 
		DDO.Push(vr);
		if (str[0] == '=')
			sprintf(tmp, "%s", str+1);
		else
			sprintf(tmp, "%s", str);
		SetStringVR(&vr, 0x0020, 0x000d, tmp); 
		DDO.Push(vr);
		SetStringVR(&vr, 0x0008, 0x0031, ""); 
		DDO.Push(vr);
		SetStringVR(&vr, 0x0020, 0x000e, ""); 
		DDO.Push(vr);
		SetStringVR(&vr, 0x0008, 0x0060, ""); 
		DDO.Push(vr);


		if (strcmp(server, "local")==0)
			{
			QueryOnSeries  (&DDO, &ADDO);
			}
		else
			{
			strncpy(VirtualServerFor[10], server, 16);
			VirtualServerFor[10][16]=0;
			VirtualQuery(&DDO, level, 10, &ADDO);
			}

		DDO.Reset();
		}

	RemoveQueryDuplicates(level, &ADDO);

	for (i=0; i<ADDO.GetSize(); i++)
		{
		j = 0;
		while(vr=ADDO.Get(i)->Pop())
			{
			if (0x0052==vr->Element && 0x0008==vr->Group)
				{
				delete vr;
				continue;
				}

			if (0x0000==vr->Element)
				{
				delete vr;
				continue;
				}

			if (j<7)
				{
				memcpy(outp[j], vr->Data, vr->Length);
				if (outp[j][vr->Length-1]==' ') outp[j++][vr->Length-1]=0;
				else                            outp[j++][vr->Length]=0;
				}
			delete vr;
			}

		if (level[0]=='P')
  			fprintf(f, fmt, outp[0], outp[1], 
					outp[0], outp[1], 
					outp[0], outp[1], 
					outp[0], outp[1], 
					outp[0], outp[1]);
		else if (level[1]=='T')
  			fprintf(f, fmt, outp[0], outp[1], outp[2], outp[3], outp[4], 
					outp[0], outp[1], outp[2], outp[3], outp[4], 
					outp[0], outp[1], outp[2], outp[3], outp[4], 
					outp[0], outp[1], outp[2], outp[3], outp[4], 
					outp[0], outp[1], outp[2], outp[3], outp[4]);
		else if (level[1]=='E')
  			fprintf(f, fmt, outp[0], outp[1], outp[2], outp[3], outp[4], outp[5], outp[6],
					outp[0], outp[1], outp[2], outp[3], outp[4], outp[5], outp[6],
					outp[0], outp[1], outp[2], outp[3], outp[4], outp[5], outp[6],
					outp[0], outp[1], outp[2], outp[3], outp[4], outp[5], outp[6],
					outp[0], outp[1], outp[2], outp[3], outp[4], outp[5], outp[6]);

		}

	return TRUE;
	}

// Image lister: get filenames, url's or pat:uid identifier of all objects in one series
BOOL	ImageFileLister(char *server, char *pat, char *series, char *fmt, FILE *f)
	{	
	DICOMDataObject DDO;
	VR *vr;
	char Device[256], Filename[512], FullFilename[512], Sop[66];
	BOOL local=stricmp(server, (char *)MYACRNEMA)==0;
	Array < DICOMDataObject * > ADDO;
	int i, j;
	DICOMDataObject **addo;

	// search ID
	SetStringVR(&vr, 0x0008, 0x0052, "IMAGE"); 
	DDO.Push(vr); 
	if (pat) 
		{
		SetStringVR(&vr, 0x0010, 0x0020, pat); 
		DDO.Push(vr);
		}
	if (series) 
		{
		SetStringVR(&vr, 0x0020, 0x000e, series); 
		DDO.Push(vr);
		}
	SetStringVR(&vr, 0x9999, 0x0800, ""); 	// filename
	DDO.Push(vr);
	SetStringVR(&vr, 0x9999, 0x0801, ""); 	// device
	DDO.Push(vr);
	SetStringVR(&vr, 0x0020, 0x0013, ""); 	// slice number
	DDO.Push(vr);
	SetStringVR(&vr, 0x0008, 0x0018, ""); 	// uid
	DDO.Push(vr);

	if (strcmp(server, "local")==0)
		{
		QueryOnImage (&DDO, &ADDO);
		}
	else
		{
		strncpy(VirtualServerFor[10], server, 16);
		VirtualServerFor[10][16]=0;
		VirtualQuery(&DDO, "IMAGE", 10, &ADDO);
		}

	// sort on image number
	addo = (DICOMDataObject **)malloc(ADDO.GetSize() * sizeof(DICOMDataObject *));
	for (i=0; i<ADDO.GetSize(); i++) addo[i] = ADDO.Get(i);
	qsort(addo, ADDO.GetSize(), sizeof(DICOMDataObject *), SortImageNumber);
        j = ADDO.GetSize(); while(ADDO.GetSize()) ADDO.RemoveAt(0);
	for (i=0; i<j; i++) ADDO.Add(addo[i]);
	free(addo);

	// locate files
	for (i=0; i<ADDO.GetSize(); i++)
		{
		Filename[0] = 0; Device[0] = 0;

		vr = ADDO.Get(i)->GetVR(0x9999, 0x0800);
		if (vr) 
			{ 
			memcpy(Filename, vr->Data, vr->Length);
  		  	if (Filename[vr->Length-1]==' ') Filename[vr->Length-1] = 0;
			else Filename[vr->Length] = 0;
			}
		vr = ADDO.Get(i)->GetVR(0x9999, 0x0801);
			if (vr) 
			{
			memcpy(Device, vr->Data, vr->Length);
  			if (Device[vr->Length-1]==' ') Device[vr->Length-1] = 0;
			else Device[vr->Length] = 0;
			}
		vr = ADDO.Get(i)->GetVR(0x0008, 0x0018);
			if (vr) 
			{
			memcpy(Sop, vr->Data, vr->Length);
  			if (Sop[vr->Length-1]==' ') Sop[vr->Length-1] = 0;
			else Sop[vr->Length] = 0;
			}

		if (FindPhysicalDevice(Device, FullFilename, Filename))
			{
			if (i==ADDO.GetSize()-1 && fmt[strlen(fmt)-1]=='*') fmt[strlen(fmt)-1]=0;
			strcat(FullFilename, Filename);
			if      (fmt[0]=='#') 
				fprintf(f, fmt+1, Device, Filename, i, Device, Filename, i, Device, Filename, i );
			else if (fmt[0]=='@') 
				{
				for (j=0; j<strlen(Filename); j++) if (Filename[j]=='\\') Filename[j]='/';
				fprintf(f, fmt+1, Device, Filename, i, Device, Filename, i, Device, Filename, i );
				}
			else if (fmt[0]==':') 
				{
                                char out[100]; int outlen=0;
				for (j=0; j<strlen(pat); j++)
					{
					if (pat[j]==' ') { out[outlen++] = '%'; out[outlen++] = '2'; out[outlen++] = '0'; }
					else 		   out[outlen++] = pat[j];
					}
				out[outlen++]=0;
				fprintf(f, fmt+1, out, Sop, i, out, Sop, i, out, Sop, i);
				}
			else
				fprintf(f, fmt, FullFilename, i, FullFilename, i, FullFilename, i);
			}

		while(vr=ADDO.Get(i)->Pop())
			delete vr;
		}

	return TRUE;
	}

// Series lister: get all series of one (or all) study
BOOL	SeriesUIDLister(char *server, char *pat, char *study, char *fmt, FILE *f)
	{	
	DICOMDataObject DDO;
	VR *vr;
	char UID[256];
	BOOL local=stricmp(server, (char *)MYACRNEMA)==0;
	Array < DICOMDataObject * > ADDO;
	int i;

	// search ID
	SetStringVR(&vr, 0x0008, 0x0052, "SERIES"); 
	DDO.Push(vr); 
	if (pat) 
		{
		SetStringVR(&vr, 0x0010, 0x0020, pat); 
		DDO.Push(vr);
		}
	if (study) 
		{
		SetStringVR(&vr, 0x0020, 0x000d, study); 
		DDO.Push(vr);
		}
	SetStringVR(&vr, 0x0020, 0x000e, ""); 
	DDO.Push(vr);

	if (strcmp(server, "local")==0)
		{
		QueryOnSeries (&DDO, &ADDO);
		}
	else
		{
		strncpy(VirtualServerFor[10], server, 16);
		VirtualServerFor[10][16]=0;
		VirtualQuery(&DDO, "SERIES", 10, &ADDO);
		}

	for (i=0; i<ADDO.GetSize(); i++)
		{
		UID[0] = 0;

		vr = ADDO.Get(i)->GetVR(0x0020, 0x000e);
		if (vr) 
			{ 
			memcpy(UID, vr->Data, vr->Length);
  		  	if (UID[vr->Length-1]==' ') UID[vr->Length-1] = 0;
			else UID[vr->Length] = 0;

			fprintf(f, fmt, UID, i, UID, i, UID, i);
			}

		while(vr=ADDO.Get(i)->Pop())
			delete vr;
		}

	return TRUE;
	}

// Implementation: Query
BOOL	MyPatientRootQuery	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	char	Level [ 10 ];
	int	sources=0, count1, count2;
	BOOL	st;

	SetString(DDOPtr->GetVR(0x0008, 0x0052), Level, 10);
	strcpy(QueryRetrieveLevel, Level);
	OperatorConsole.printf("(PatientRootQuery) search level: %s\n", Level);
	if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		sources += VirtualQuery(DDOPtr, Level, i, ADDO)!=0;
	count1 = ADDO->GetSize();

        prefetcher(DDOPtr, FALSE);

	if(!strncmp(Level, "PATIENT", 7))
		st = ( QueryOnPatient (DDOPtr, ADDO));
	else if(!strncmp(Level, "STUDY", 5))
		st = ( QueryOnStudy (DDOPtr, ADDO));
	else if(!strncmp(Level, "SERIES", 6))
		st = ( QueryOnSeries (DDOPtr, ADDO));
	else if(!strncmp(Level, "IMAGE", 5))
		st = ( QueryOnImage (DDOPtr, ADDO));
	else
		st = FALSE;

	count2 = ADDO->GetSize()-count1;
	sources += count2!=0;

	if (sources>1)	// more than one source; data can be duplicated
		RemoveQueryDuplicates(Level, ADDO);

	RecordsFound = ADDO->GetSize();

	return st;
	}

BOOL	MyStudyRootQuery	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	char	Level [ 10 ];
	int	sources=0, count1, count2;
	BOOL	st;

	SetString(DDOPtr->GetVR(0x0008, 0x0052), Level, 10);
	strcpy(QueryRetrieveLevel, Level);
	OperatorConsole.printf("(StudyRootQuery) search level: %s\n", Level);
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		sources += VirtualQuery(DDOPtr, Level, i, ADDO)!=0;
	count1 = ADDO->GetSize();

        prefetcher(DDOPtr, FALSE);

	if(!strncmp(Level, "STUDY", 5))
		st = ( QueryOnStudy (DDOPtr, ADDO));
	else if(!strncmp(Level, "SERIES", 6))
		st = ( QueryOnSeries (DDOPtr, ADDO));
	else if(!strncmp(Level, "IMAGE", 5))
		st = ( QueryOnImage (DDOPtr, ADDO));
	else
		st = FALSE;

	count2 = ADDO->GetSize()-count1;
	sources += count2!=0;
	if (sources>1)
		RemoveQueryDuplicates(Level, ADDO);

	if (DebugLevel>=3 && ADDO->GetSize()!=0)
		{
		SystemDebug.printf("First record of cleaned response:\n");
		NonDestructiveDumpDICOMObject(ADDO->Get(0));
		}

	RecordsFound = ADDO->GetSize();
	return st;
	}

BOOL	MyPatientStudyOnlyQuery	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	char	Level [ 10 ];
	int	sources=0, count1, count2;
	BOOL	st;

	SetString(DDOPtr->GetVR(0x0008, 0x0052), Level, 10);
	strcpy(QueryRetrieveLevel, Level);
	OperatorConsole.printf("(PatientStudyOnlyQuery) search level: %s\n", Level);
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		sources += VirtualQuery(DDOPtr, Level, i, ADDO)!=0;
	count1 = ADDO->GetSize();

        prefetcher(DDOPtr, FALSE);

	if(!strncmp(Level, "PATIENT", 7))
		st = ( QueryOnPatient (DDOPtr, ADDO));
	else if(!strncmp(Level, "STUDY", 5))
		st = ( QueryOnStudy (DDOPtr, ADDO));
	else
		st = FALSE;

	count2 = ADDO->GetSize()-count1;
	sources += count2!=0;
	if (sources>1)
		RemoveQueryDuplicates(Level, ADDO);

	if (DebugLevel>=3 && ADDO->GetSize()!=0)
		{
		SystemDebug.printf("First record of cleaned response:\n");
		NonDestructiveDumpDICOMObject(ADDO->Get(0));
		}

	RecordsFound = ADDO->GetSize();
	return st;
	}

BOOL	MyModalityWorkListQuery	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	char	Level [ 10 ];
	int	sources=0, count1, count2;
	BOOL	st;

	SetString(DDOPtr->GetVR(0x0008, 0x0052), Level, 10);
	strcpy(QueryRetrieveLevel, Level);

	OperatorConsole.printf("(ModalityWorkListQuery) search level: %s\n", Level);
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

//	for (int i=0; i<10; i++)
//		sources += VirtualQuery(DDOPtr, Level, i, ADDO)!=0;
	count1 = ADDO->GetSize();

//        prefetcher(DDOPtr, FALSE);

	st = ( QueryOnModalityWorkList (DDOPtr, ADDO));

	count2 = ADDO->GetSize()-count1;
	sources += count2!=0;
	if (sources>1)
		RemoveQueryDuplicates(Level, ADDO);

	if (DebugLevel>=3 && ADDO->GetSize()!=0)
		{
		SystemDebug.printf("First record of cleaned response:\n");
		NonDestructiveDumpDICOMObject(ADDO->Get(0));
		}

	RecordsFound = ADDO->GetSize();
	return st;
	}

BOOL	MyPatientRootQuery	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyStudyRootQuery	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyPatientStudyOnlyQuery	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyModalityWorkListQuery	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyPatientRootRetrieve	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	VR		*vr;
//	char	Level [ 10 ];

        prefetcher(DDOPtr, TRUE);

	SystemDebug.printf("MyPatientRootRetrieve :: SearchOn\n");
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		VirtualQuery(DDOPtr, "IMAGE", i, NULL);

	if(!DDOPtr->GetVR(0x0008, 0x0018))
		{
		vr = new VR ( 0x0008, 0x0018, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0008, 0x0016))
		{
		vr = new VR ( 0x0008, 0x0016, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000d))
		{
		vr = new VR ( 0x0020, 0x000d, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000e))
		{
		vr = new VR ( 0x0020, 0x000e, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0800))
		{
		vr = new VR ( 0x9999, 0x0800, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0801))
		{
		vr = new VR ( 0x9999, 0x0801, 0, FALSE );
		DDOPtr->Push(vr);
		}
	QueryOnImage(DDOPtr, ADDO);
	if(!ADDO->GetSize())
		return(FALSE);
        OperatorConsole.printf("Number of images to send: %d\n", ADDO->GetSize());
	return(TRUE);
	}

BOOL	MyPatientRootRetrieveNKI	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	VR		*vr;
//	char	Level [ 10 ];

        prefetcher(DDOPtr, TRUE);

	SystemDebug.printf("MyPatientRootRetrieveNKI :: SearchOn\n");
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		VirtualQuery(DDOPtr, "IMAGE", i, NULL);

	if(!DDOPtr->GetVR(0x0008, 0x0018))
		{
		vr = new VR ( 0x0008, 0x0018, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0008, 0x0016))
		{
		vr = new VR ( 0x0008, 0x0016, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000d))
		{
		vr = new VR ( 0x0020, 0x000d, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000e))
		{
		vr = new VR ( 0x0020, 0x000e, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0800))
		{
		vr = new VR ( 0x9999, 0x0800, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0801))
		{
		vr = new VR ( 0x9999, 0x0801, 0, FALSE );
		DDOPtr->Push(vr);
		}
	QueryOnImage(DDOPtr, ADDO);
	if(!ADDO->GetSize())
		return(FALSE);
        OperatorConsole.printf("Number of images to send: %d\n", ADDO->GetSize());
	return(TRUE);
	}

BOOL	MyPatientRootRetrieveGeneric	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	VR		*vr;
//	char	Level [ 10 ];

        prefetcher(DDOPtr, TRUE);

	SystemDebug.printf("MyPatientRootRetrieveGeneric :: SearchOn\n");
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		VirtualQuery(DDOPtr, "IMAGE", i, NULL);

	if(!DDOPtr->GetVR(0x0008, 0x0018))
		{
		vr = new VR ( 0x0008, 0x0018, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0008, 0x0016))
		{
		vr = new VR ( 0x0008, 0x0016, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000d))
		{
		vr = new VR ( 0x0020, 0x000d, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000e))
		{
		vr = new VR ( 0x0020, 0x000e, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0800))
		{
		vr = new VR ( 0x9999, 0x0800, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0801))
		{
		vr = new VR ( 0x9999, 0x0801, 0, FALSE );
		DDOPtr->Push(vr);
		}
	QueryOnImage(DDOPtr, ADDO);
	if(!ADDO->GetSize())
		return(FALSE);
        OperatorConsole.printf("Number of images to send: %d\n", ADDO->GetSize());
	return(TRUE);
	}

BOOL	MyStudyRootRetrieve	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	VR		*vr;
//	char	Level [ 10 ];

        prefetcher(DDOPtr, TRUE);

	SystemDebug.printf("MyStudyRootRetrieve :: SearchOn\n");
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		VirtualQuery(DDOPtr, "IMAGE", i, NULL);

	if(!DDOPtr->GetVR(0x0008, 0x0018))
		{
		vr = new VR ( 0x0008, 0x0018, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0008, 0x0016))
		{
		vr = new VR ( 0x0008, 0x0016, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000d))
		{
		vr = new VR ( 0x0020, 0x000d, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000e))
		{
		vr = new VR ( 0x0020, 0x000e, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0800))
		{
		vr = new VR ( 0x9999, 0x0800, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0801))
		{
		vr = new VR ( 0x9999, 0x0801, 0, FALSE );
		DDOPtr->Push(vr);
		}
	QueryOnImage(DDOPtr, ADDO);
	if(!ADDO->GetSize())
		return(FALSE);
	OperatorConsole.printf("Number of Images to send: %d\n", ADDO->GetSize());
	return(TRUE);
	}

BOOL	MyStudyRootRetrieveNKI	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	VR		*vr;
//	char	Level [ 10 ];

        prefetcher(DDOPtr, TRUE);

	SystemDebug.printf("MyStudyRootRetrieveNKI :: SearchOn\n");
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		VirtualQuery(DDOPtr, "IMAGE", i, NULL);

	if(!DDOPtr->GetVR(0x0008, 0x0018))
		{
		vr = new VR ( 0x0008, 0x0018, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0008, 0x0016))
		{
		vr = new VR ( 0x0008, 0x0016, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000d))
		{
		vr = new VR ( 0x0020, 0x000d, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000e))
		{
		vr = new VR ( 0x0020, 0x000e, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0800))
		{
		vr = new VR ( 0x9999, 0x0800, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0801))
		{
		vr = new VR ( 0x9999, 0x0801, 0, FALSE );
		DDOPtr->Push(vr);
		}
	QueryOnImage(DDOPtr, ADDO);
	if(!ADDO->GetSize())
		return(FALSE);
	OperatorConsole.printf("Number of Images to send: %d\n", ADDO->GetSize());
	return(TRUE);
	}

BOOL	MyStudyRootRetrieveGeneric	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	VR		*vr;
//	char	Level [ 10 ];

        prefetcher(DDOPtr, TRUE);

	SystemDebug.printf("MyStudyRootRetrieveGeneric :: SearchOn\n");
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		VirtualQuery(DDOPtr, "IMAGE", i, NULL);

	if(!DDOPtr->GetVR(0x0008, 0x0018))
		{
		vr = new VR ( 0x0008, 0x0018, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0008, 0x0016))
		{
		vr = new VR ( 0x0008, 0x0016, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000d))
		{
		vr = new VR ( 0x0020, 0x000d, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000e))
		{
		vr = new VR ( 0x0020, 0x000e, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0800))
		{
		vr = new VR ( 0x9999, 0x0800, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0801))
		{
		vr = new VR ( 0x9999, 0x0801, 0, FALSE );
		DDOPtr->Push(vr);
		}
	QueryOnImage(DDOPtr, ADDO);
	if(!ADDO->GetSize())
		return(FALSE);
	OperatorConsole.printf("Number of Images to send: %d\n", ADDO->GetSize());
	return(TRUE);
	}

BOOL	MyPatientStudyOnlyRetrieve	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	VR		*vr;
//	char	Level [ 10 ];

        prefetcher(DDOPtr, TRUE);

	SystemDebug.printf("MyPatientStudyOnlyRetrieve :: SearchOn\n");
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		VirtualQuery(DDOPtr, "IMAGE", i, NULL);

	if(!DDOPtr->GetVR(0x0008, 0x0018))
		{
		vr = new VR ( 0x0008, 0x0018, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0008, 0x0016))
		{
		vr = new VR ( 0x0008, 0x0016, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000d))
		{
		vr = new VR ( 0x0020, 0x000d, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000e))
		{
		vr = new VR ( 0x0020, 0x000e, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0800))
		{
		vr = new VR ( 0x9999, 0x0800, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0801))
		{
		vr = new VR ( 0x9999, 0x0801, 0, FALSE );
		DDOPtr->Push(vr);
		}
	QueryOnImage(DDOPtr, ADDO);
	if(!ADDO->GetSize())
		return(FALSE);
	OperatorConsole.printf("Number of Images to send: %d\n", ADDO->GetSize());
	return(TRUE);
	}

BOOL	MyPatientStudyOnlyRetrieveNKI	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	VR		*vr;
//	char	Level [ 10 ];

        prefetcher(DDOPtr, TRUE);

	SystemDebug.printf("MyPatientStudyOnlyRetrieveNKI :: SearchOn\n");
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		VirtualQuery(DDOPtr, "IMAGE", i, NULL);

	if(!DDOPtr->GetVR(0x0008, 0x0018))
		{
		vr = new VR ( 0x0008, 0x0018, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0008, 0x0016))
		{
		vr = new VR ( 0x0008, 0x0016, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000d))
		{
		vr = new VR ( 0x0020, 0x000d, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000e))
		{
		vr = new VR ( 0x0020, 0x000e, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0800))
		{
		vr = new VR ( 0x9999, 0x0800, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0801))
		{
		vr = new VR ( 0x9999, 0x0801, 0, FALSE );
		DDOPtr->Push(vr);
		}
	QueryOnImage(DDOPtr, ADDO);
	if(!ADDO->GetSize())
		return(FALSE);
	OperatorConsole.printf("Number of Images to send: %d\n", ADDO->GetSize());
	return(TRUE);
	}


BOOL	MyPatientStudyOnlyRetrieveGeneric	::	SearchOn (
	DICOMDataObject	*DDOPtr,
	Array < DICOMDataObject * >	*ADDO)
	{
	VR		*vr;
//	char	Level [ 10 ];

        prefetcher(DDOPtr, TRUE);

	SystemDebug.printf("MyPatientStudyOnlyRetrieveGeneric :: SearchOn\n");
        if (DebugLevel>=2) NonDestructiveDumpDICOMObject(DDOPtr);

	for (int i=0; i<10; i++)
		VirtualQuery(DDOPtr, "IMAGE", i, NULL);

	if(!DDOPtr->GetVR(0x0008, 0x0018))
		{
		vr = new VR ( 0x0008, 0x0018, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0008, 0x0016))
		{
		vr = new VR ( 0x0008, 0x0016, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000d))
		{
		vr = new VR ( 0x0020, 0x000d, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x0020, 0x000e))
		{
		vr = new VR ( 0x0020, 0x000e, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0800))
		{
		vr = new VR ( 0x9999, 0x0800, 0, FALSE );
		DDOPtr->Push(vr);
		}
	if(!DDOPtr->GetVR(0x9999, 0x0801))
		{
		vr = new VR ( 0x9999, 0x0801, 0, FALSE );
		DDOPtr->Push(vr);
		}
	QueryOnImage(DDOPtr, ADDO);
	if(!ADDO->GetSize())
		return(FALSE);
	OperatorConsole.printf("Number of Images to send: %d\n", ADDO->GetSize());
	return(TRUE);
	}

BOOL	MyPatientRootRetrieve	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyPatientRootRetrieveNKI	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyPatientRootRetrieveGeneric	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyStudyRootRetrieve	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyStudyRootRetrieveNKI	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyStudyRootRetrieveGeneric	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyPatientStudyOnlyRetrieve	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyPatientStudyOnlyRetrieveNKI	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

BOOL	MyPatientStudyOnlyRetrieveGeneric	::	CallBack (
	DICOMCommandObject	*DCOPtr,
	DICOMDataObject		*DDOPtr)
	{
	return ( TRUE );
	}

/* Read ahead implementation */

struct ReadAheadThreadData
	{
	char				*SOPs;
	char				*Studies;
	char				*Series;
	char				*ObjectFiles;
	char				*Devices;
	char				*pats;
	int				NumToRead;
	DICOMDataObject			**DDOOutPtr;
	Database			*DB;
	int				iVrSizeLimit;
#ifdef WIN32
	HANDLE 				Handle;
#else
  	pthread_t 			Handle;
#endif
	DICOMCommandObject	   	*pDCO;
	char				*Filenames;
	volatile BOOL			Busy;
	volatile BOOL			ForceEnd;
	volatile BOOL			TimeOut;
	char 				*FullFilenames;
	int 				filesmissing;
	int 				numreadahead;
	};

#ifdef WIN32
#define NULLHANDLE NULL
#else
#define NULLHANDLE 0
#endif

// Move any missing slices from the AE to here

BOOL VirtualServer(struct ReadAheadThreadData *ratd, int N)
	{
	char AE[48];
	strcpy(AE, VirtualServerFor[N]);
	if (*AE==0) return FALSE;

        char *param = strchr(AE, ',');
        if (param) *param++ = 0;

  	unsigned char 		ip[64], port[64], compress[64];
	VR			*vr;
	char			SOP[66];
	DICOMCommandObject	DCO, DCOR2;
	DICOMDataObject		DDO, DDOR2;
	LE_UINT16		command, datasettype, messageid=3, priority, tempid;
	UID			uid;
	int			quit, i, ims=0;
	PDU_Service		PDU2;
	char			*pats, *series, *studies, *sops, *p;

	OperatorConsole.printf("Virtual server - collecting %d missing objects from %s\n", ratd->filesmissing, AE);

	PDU2.AttachRTC(&VRType);
	if(!GetACRNema(AE, (char *)ip, (char *)port, (char *)compress))
		return FALSE;

	PDU2.ClearAbstractSyntaxs();
	PDU2.SetLocalAddress(MYACRNEMA);
	PDU2.SetRemoteAddress((unsigned char *)AE);

	uid.Set("1.2.840.10008.3.1.1.1");	// Application context (DICOM App)
	PDU2.SetApplicationContext(uid);

	uid.Set("1.2.840.10008.5.1.4.1.2.2.2"); // StudyRootRetrieve (C-Move SOP)
	PDU2.AddAbstractSyntax(uid);

        PDU2.SetTimeOut(TCPIPTimeOut);

	// Make association for MOVE on port/ip
	if(!PDU2.Connect(ip, port))
		{
		PDU2.Close();
		OperatorConsole.printf("*** Virtual server - failed to connect %s (%s %s) for C-MOVE\n", AE, ip, port);
		return(FALSE);
		}

	// collect move requests together if possible (same patients) in pats and sops; else set pats to NULL
	pats=NULL; studies=NULL; series=NULL; sops = (char *)malloc(66*ratd->NumToRead); sops[0]=0;
	for (i=0; i<ratd->NumToRead; i++)
		{
		if (*(ratd->FullFilenames+i*1024)) continue;	// present

		if (!pats)    pats    = strdup(ratd->pats   +i*66);
		if (!studies) studies = strdup(ratd->Studies+i*66);
		if (!series)  series  = strdup(ratd->Series +i*66);

		else if (strcmp(pats,    ratd->pats   +i*66) || 
                         strcmp(studies, ratd->Studies+i*66) ||
                         strcmp(series,  ratd->Series +i*66))
			{
			free(pats);    pats    = NULL;
			free(studies); studies = NULL;
			free(series);  series  = NULL;
			break;
			}
		strcat(sops, ratd->SOPs+i*66);
		strcat(sops, "\\");
		}
	sops[strlen(sops)-1]=0;
	if (!pats) free(sops);

	// perform one or more c-move requests
	for (i=0; i<ratd->NumToRead; i++)
		{
		if (*(ratd->FullFilenames+i*1024)) continue;	// present

		p = strchr(ratd->Devices+i*32, '.');
		if (p && atoi(p+1)!=N) continue;		// not on this server

		DCO.Reset();
		DDO.Reset();

		// move missing file to this server
		strcpy((char*) SOP, "1.2.840.10008.5.1.4.1.2.2.2"); // StudyRootMove (C-Move SOP)
		vr = new VR (0x0000, 0x0002, strlen((char*)SOP), (void*) SOP, FALSE);
		DCO.Push(vr);
		command = EN0021;
		vr = new VR (0x0000, 0x0100, 0x0002, &command, FALSE);
		DCO.Push(vr);
		priority = 0;	// MEDIUM
		vr = new VR (0x0000, 0x0700, 0x0002, &priority, FALSE);
		DCO.Push(vr);
		datasettype = EN0102;	
		vr = new VR (0x0000, 0x0800, 0x0002, &datasettype, FALSE);
		DCO.Push(vr);	
		messageid++;
#if NATIVE_ENDIAN == LITTLE_ENDIAN //Little Endian
		tempid = messageid;
#else //Big Endian like Apple power pc
		tempid = SwitchEndian((UINT16) messageid);
#endif //Big Endian
		vr = new VR (0x0000, 0x0110, 0x0002, &tempid, FALSE);
		DCO.Push(vr);
		vr = new VR(0x0000, 0x0600, 16, (void*)MYACRNEMA, FALSE);
		DCO.Push(vr);

		// Data object. Query level is image; ask for patient ID and SopInstanceUID 
		SetStringVR(&vr, 0x0008, 0x0052, "IMAGE"); 
		DDO.Push(vr); 

		if (pats)	// one request
			{
			SetStringVR(&vr, 0x0010, 0x0020, pats); 
			DDO.Push(vr); 
			SetStringVR(&vr, 0x0008, 0x0018, sops);
			DDO.Push(vr); 
			SetStringVR(&vr, 0x0020, 0x000d, studies);
			DDO.Push(vr); 
			SetStringVR(&vr, 0x0020, 0x000e, series);
			DDO.Push(vr); 
			}
		else		// many requests
			{
			SetStringVR(&vr, 0x0010, 0x0020, ratd->pats+i*66); 
			DDO.Push(vr); 
			SetStringVR(&vr, 0x0008, 0x0018, ratd->SOPs+i*66);
			DDO.Push(vr); 
			SetStringVR(&vr, 0x0020, 0x000d, ratd->Studies+i*66);
			DDO.Push(vr); 
			SetStringVR(&vr, 0x0020, 0x000e, ratd->Series+i*66);
			DDO.Push(vr); 
			}

		// Here fixkodak fiximage should operate adding 0 to e.g., 9901234 patient ID
		if (param)
			if (stricmp(param, "FixKodak")==0)
				KodakFixer(&DDO, TRUE);

		// Write command object and data object
		PDU2.Write(&DCO, uid);
		PDU2.Write(&DDO, uid);

		// Get response
		quit = 0;
		while(!quit)
			{
			if(!PDU2.Read(&DCOR2))
				{
				OperatorConsole.printf("*** Virtual server - C-MOVE association lost to %s\n", AE);
				quit=1;
				}
			if(DCOR2.GetUINT16(0x0000, 0x0100)!=0x8021)
				{
				OperatorConsole.printf("*** Virtual server - C-MOVE got a wrong response from %s\n", AE);
				quit=1;
				}
			// ignore the data set
			if(DCOR2.GetUINT16(0x0000, 0x0800)!=0x0101)
				{
				PDU2.Read(&DDOR2);
				DDOR2.Reset();
				}
			if(DCOR2.GetUINT16(0x0000, 0x0900)==0x0000)	// success ?
				{
				if (pats)
					OperatorConsole.printf("Virtual server - Grabbed %d images for %s from %s\n", ims, pats, AE);
				else
					OperatorConsole.printf("Virtual server - Grabbed %s from %s\n", ratd->SOPs+i*66, AE);
				quit=1;
				}
			else if (DCOR2.GetUINT16(0x0000, 0x0900)==0xff00)	// continuing no problem
				{
				ims++;
				}
			else
				{
				UINT16	RemoteError = DCOR2.GetUINT16(0x0000, 0x0900);
				OperatorConsole.printf("*** Virtual server - remote C-MOVE DICOM Error code %x (%d) from %s\n", RemoteError, RemoteError, AE);
				quit=1;
				}
			DCOR2.Reset();
			}

		if(pats)			// one request
			{
			free(pats);
			free(sops);
			free(series);
			free(studies);
			break;
			}
		}
		PDU2.Close();

		ratd->filesmissing -= ims;	// how many are left ?

		return (ratd->filesmissing==0);	// all missing images retrieved? -> OK
	}

static BOOL WINAPI ReadAheadThread(struct ReadAheadThreadData *ratd)
	{
	DICOMDataObject	*DDOMask;
	char		FullFilename[1024];
	char		Filename[1024];
	char		Device[256];
	PDU_Service	PDU, PDU2;
	int		len, UseChapter10=0, i, j;
	DICOMDataObject	*DDOOut;
//	char		*pats, *sops;

	ratd->filesmissing=0;

	PDU.AttachRTC(&VRType);

	// Check if all files are there and generate their full filenames

	ratd->FullFilenames = (char *)malloc(ratd->NumToRead * 1024);

	for (i=0; i<ratd->NumToRead; i++)
		{
		strcpy(Filename, ratd->ObjectFiles + i * 256);
		strcpy(Device,   ratd->Devices     + i * 32);

		if (!FindPhysicalDevice(Device, ratd->FullFilenames+i*1024, Filename))
			{
			*(ratd->FullFilenames+i*1024)=0;
			ratd->filesmissing++;
			}
		else
			strcat(ratd->FullFilenames+i*1024, Filename);
		}

	// virtual server: grab images in db but not on disk from other server; send them and then delete again

	if (ratd->filesmissing>0)
		{
		for (int i=0; i<10; i++)
			if (VirtualServer(ratd, i)) break;
		}

	for (i=0; i<ratd->NumToRead; i++)
		{
		if (*(ratd->FullFilenames+i*1024)==0)			// retry; may have been collected by virtual server
			{
			strcpy(Filename, ratd->ObjectFiles + i * 256);
			strcpy(Device,   ratd->Devices     + i * 32);

			if (!FindPhysicalDevice(Device, FullFilename, Filename))
				{
				OperatorConsole.printf("***Could not find file:%s\n", Filename);
				free(ratd->FullFilenames);

				// mvh 20060401: fix hang condition when a read ahead failed
				ratd->Busy = FALSE;
				ratd->TimeOut = TRUE;

				return FALSE;
				}
			strcat(FullFilename, Filename);
			}
		else
			strcpy(FullFilename, ratd->FullFilenames + i*1024);

		len = strlen(FullFilename);
		if (len>4)
			UseChapter10 = (stricmp(FullFilename+len-4, ".dcm")==0);

		if (ratd->ForceEnd) goto TimeOut;

		int t = time(NULL);
		if (!UseChapter10)
		  	DDOOut = LoadImplicitLittleEndianFile(FullFilename, ratd->iVrSizeLimit);
		else	
			DDOOut = PDU.LoadDICOMDataObject(FullFilename);
		LoadTime += time(NULL)-t;

		if (!DDOOut) 
			// mvh 20060401: fix hang condition when a read ahead failed
			{
			OperatorConsole.printf("***Could not read file:%s\n", Filename);
			ratd->Busy = FALSE;
			ratd->TimeOut = TRUE;
			return FALSE;
			}

		if (!CacheVirtualData)
			{
			if (*(ratd->FullFilenames+i*1024)==0)		// if virtual server retrieved data
				unlink(FullFilename);			// delete data if not caching
			}

		if (ratd->ForceEnd)
			{
			delete DDOOut;
			goto TimeOut;
			}

		t = time(NULL);
		ProcessDDO(&DDOOut, ratd->pDCO);
		ProcessTime += time(NULL)-t;

		strcpy(ratd->Filenames + i*1024, FullFilename);

		ratd->DDOOutPtr[i] = DDOOut;

		// Make the thread wait when it is further than numreadahead slices ahead - self timeout is disabled
		j = 0;
		while (i>=ratd->numreadahead && ratd->DDOOutPtr[i-ratd->numreadahead]) 
			{
			if (ratd->ForceEnd) goto TimeOut;
			Sleep(1);
			//printf(".");
			//if (j++ > 5000) goto TimeOut;
			}
		}

	ratd->Busy = FALSE;
	free(ratd->FullFilenames);
	return TRUE;

	// Happens when the client hangs up the TCP/IP connection (signalled by ForceEnd)
TimeOut:
	ratd->TimeOut = TRUE;
	ratd->Busy    = FALSE;
	free(ratd->FullFilenames);
	return TRUE;
	}

BOOL
RetrieveOn (
	DICOMDataObject	*DDOMask,
	DICOMDataObject	**DDOOutPtr,
	StandardStorage	**SStorage,
	RunTimeClassStorage		&RTCStorage,
	DICOMCommandObject	   *pDCO  = NULL,
        Array < DICOMDataObject *> *pADDO = NULL,
	void			   *ExtraBytes = NULL)
	{
	VR		*vr;
	char		FullFilename[1024];
	char		Filename[1024];
	char		Device[256];
	PDU_Service	PDU;
	DICOMDataObject	*pDDO;
	int		len, UseChapter10=0, i, j;
	VR*		pVR;
	unsigned int	iVrSizeLimit;
	unsigned long	ThreadID;
	struct 	ReadAheadThreadData *ratd;

	PDU.AttachRTC(&VRType);

	ratd = (struct 	ReadAheadThreadData *)ExtraBytes;

	// assume read will fail unless proven otherwise
	*DDOOutPtr = NULL;

	// This line will disable the read ahead thread
	if (!EnableReadAheadThread || NoThread) pADDO = NULL;

	// mvh 20060401: fix hang condition when a read ahead failed
	if (ratd->TimeOut) return FALSE;

	// start read ahead thread when first slice of more than (one:normal, zero:virtual server) is requested
	if (pADDO && pADDO->Get(0)==DDOMask && pADDO->GetSize() > 0)
		{
		VR		*vr;

		iVrSizeLimit = 0xffffffff;
		if (pDCO)
			{
			pVR = pDCO->GetVR(0x9999, 0x0100);
		    	if (pVR)
	    			iVrSizeLimit = (unsigned int)(*((unsigned int*)pVR->Data));
		  	}
		
		// wait for thread to end from possible previous move on same association
		while (ratd->Busy) 
			{
			Sleep(10);
			printf(".");
			}

		// free memory of thread if not already done so
		if (ratd->Devices)
			{
			if (ratd->pDCO) delete ratd->pDCO; ratd->pDCO = NULL;
			for (i=0; i<ratd->NumToRead; i++) 
				if (ratd->DDOOutPtr[i])
					delete ratd->DDOOutPtr[i];
			free(ratd->DDOOutPtr);  ratd->DDOOutPtr   = NULL;
			free(ratd->Filenames);  ratd->Filenames   = NULL;
			free(ratd->ObjectFiles);ratd->ObjectFiles = NULL;
			free(ratd->Devices);    ratd->Devices     = NULL;
			free(ratd->SOPs);       ratd->SOPs        = NULL;
			free(ratd->Series);     ratd->Series      = NULL;
			free(ratd->Studies);    ratd->Studies     = NULL;
			free(ratd->pats);       ratd->pats        = NULL;
			CloseHandle(ratd->Handle); ratd->Handle   = NULLHANDLE;
			}

		// initialize thread data
		ratd->Busy    = TRUE;
		ratd->TimeOut = FALSE;
		if (pDCO)
		  ratd->pDCO  = (DICOMCommandObject *)MakeCopy((DICOMDataObject *)pDCO);

		ratd->numreadahead = EnableReadAheadThread<2 ? 5 : EnableReadAheadThread;
		ratd->NumToRead = pADDO->GetSize();
		ratd->SOPs   = (char *) malloc(ratd->NumToRead * 66);
		ratd->Series = (char *) malloc(ratd->NumToRead * 66);
		ratd->Studies= (char *) malloc(ratd->NumToRead * 66);
		ratd->pats   = (char *) malloc(ratd->NumToRead * 66);
		ratd->ObjectFiles = (char *) malloc(ratd->NumToRead * 256);
		ratd->Devices     = (char *) malloc(ratd->NumToRead * 32);
		memset(ratd->SOPs,    0, ratd->NumToRead * 66);
		memset(ratd->Series,  0, ratd->NumToRead * 66);
		memset(ratd->Studies, 0, ratd->NumToRead * 66);
		memset(ratd->pats,    0, ratd->NumToRead * 66);
		memset(ratd->ObjectFiles, 0, ratd->NumToRead * 256);
		memset(ratd->Devices,     0, ratd->NumToRead * 32);

		for (i=0; i<ratd->NumToRead; i++) 
			{
			vr = pADDO->Get(i)->GetVR(0x9999, 0x0800);
			if (vr) 
				{ 
				memcpy(ratd->ObjectFiles+i*256, vr->Data, vr->Length);
  			  	if ((ratd->ObjectFiles+i*256)[vr->Length-1]==' ') (ratd->ObjectFiles+i*256)[vr->Length-1] = 0;
				}

			vr = pADDO->Get(i)->GetVR(0x9999, 0x0801);
			if (vr) 
				{
				memcpy(ratd->Devices+i*32, vr->Data, vr->Length);
  			  	if ((ratd->Devices+i*32)[vr->Length-1]==' ') (ratd->Devices+i*32)[vr->Length-1] = 0;
				}

			vr = pADDO->Get(i)->GetVR(0x0008, 0x0018);
			if (vr) memcpy(ratd->SOPs+i*66, vr->Data, vr->Length);

			vr = pADDO->Get(i)->GetVR(0x0020, 0x000d);
			if (vr) memcpy(ratd->Studies+i*66, vr->Data, vr->Length);

			vr = pADDO->Get(i)->GetVR(0x0020, 0x000e);
			if (vr) memcpy(ratd->Series+i*66, vr->Data, vr->Length);

			vr = pADDO->Get(i)->GetVR(0x0010, 0x0020);
			if (vr) 
                        	{ 
				memcpy(ratd->pats+i*66, vr->Data, vr->Length);
  			  	if ((ratd->pats+i*66)[vr->Length-1]==' ') (ratd->pats+i*66)[vr->Length-1] = 0;
				}
			}

		ratd->iVrSizeLimit  = iVrSizeLimit;

		ratd->DDOOutPtr  = (DICOMDataObject **) malloc(ratd->NumToRead * sizeof(DICOMDataObject *));
		memset(ratd->DDOOutPtr, 0, ratd->NumToRead * sizeof(DICOMDataObject *));

		ratd->Filenames  = (char *) malloc(ratd->NumToRead * 1024);
		memset(ratd->Filenames, 0, ratd->NumToRead * 1024);

		// start the thread
#ifdef WIN32
		ratd->Handle     = CreateThread(NULL, 0x000ff000, 
                                    (LPTHREAD_START_ROUTINE) ReadAheadThread,
			            ratd, 0, &ThreadID);
#else
		pthread_create(&ratd->Handle, NULL, (void*(*)(void*))ReadAheadThread, (void *)ratd);
		pthread_detach(ratd->Handle);
#endif
		}


	// Get an item from the read ahead thread if it is running
	if (pADDO && ratd->Handle!=NULLHANDLE && !ratd->TimeOut)
		{
		j = 0;
		while (!ratd->TimeOut)
			{
			for (i=0; i<ratd->NumToRead; i++)
				{
				if (pADDO->Get(i)==DDOMask && ratd->DDOOutPtr[i])
					{
					*DDOOutPtr = ratd->DDOOutPtr[i];
					strcpy(Filename, ratd->Filenames + i*1024);
					ratd->DDOOutPtr[i] = NULL;

					// if it is the last item wait for the thread to end and free the memory
					if (i==ratd->NumToRead-1)
						{
						while (1)
							{
							ratd->ForceEnd = TRUE;
#ifdef WIN32
							if (WaitForSingleObject(ratd->Handle, 10) != WAIT_TIMEOUT) break;
							if (j++ > 200) break;
#else
							while(ratd->Busy) Sleep(10);
							if (j++ > 200) break;
#endif
							}
						if (ratd->pDCO) delete ratd->pDCO; ratd->pDCO = NULL;
						free(ratd->DDOOutPtr); ratd->DDOOutPtr = NULL;
						free(ratd->Filenames); ratd->Filenames = NULL;
						free(ratd->SOPs);      ratd->SOPs      = NULL;
						free(ratd->Series);     ratd->Series      = NULL;
						free(ratd->Studies);    ratd->Studies     = NULL;
						free(ratd->pats);       ratd->pats         = NULL;
						free(ratd->ObjectFiles); ratd->ObjectFiles = NULL;
						free(ratd->Devices);     ratd->Devices     = NULL;
                                                CloseHandle(ratd->Handle); ratd->Handle = NULLHANDLE;
						}

					goto further;
					}
				}
			Sleep(1);		// 20021215: fixes bug reported by Aaron Cann - 100% load
			}

			// mvh 20060401: fix hang condition when a read ahead failed
			if (ratd->TimeOut)
				{
				if (ratd->Devices)
					{
					if (ratd->pDCO) delete ratd->pDCO; ratd->pDCO = NULL;
					for (i=0; i<ratd->NumToRead; i++) 
						if (ratd->DDOOutPtr[i])
							delete ratd->DDOOutPtr[i];
					free(ratd->DDOOutPtr);  ratd->DDOOutPtr   = NULL;
					free(ratd->Filenames);  ratd->Filenames   = NULL;
					free(ratd->ObjectFiles);ratd->ObjectFiles = NULL;
					free(ratd->Devices);    ratd->Devices     = NULL;
					free(ratd->SOPs);       ratd->SOPs        = NULL;
					free(ratd->Series);     ratd->Series      = NULL;
					free(ratd->Studies);    ratd->Studies     = NULL;
					free(ratd->pats);       ratd->pats        = NULL;
					CloseHandle(ratd->Handle); ratd->Handle   = NULLHANDLE;
					}
				return FALSE;
				}


further:
		if(*DDOOutPtr)
			{
			OperatorConsole.printf("Sending file : %s\n", Filename);	// note subtle difference in message!
			RTCStorage.SetUID(DDOMask->GetVR(0x0008, 0x00016));
			(*SStorage) = &RTCStorage;
			ImagesSent++;

			SystemDebug.printf("Image Loaded from Read Ahead Thread, returning TRUE\n");
			return ( TRUE );
			}		
		}


	// use normal read code used when only one slice is requested

	char pat[66];				/* get the patient ID */
        pat[0]=0;
	vr = DDOMask->GetVR(0x0010, 0x0020);
	if (vr) 
        	{ 
		memcpy(pat, vr->Data, vr->Length);
                pat[vr->Length]=0;
  		if (pat[vr->Length-1]==' ') pat[vr->Length-1] = 0;
		}

	char series[66];			/* get the series UID */
        series[0]=0;
	vr = DDOMask->GetVR(0x0020, 0x000e);
	if (vr) 
        	{ 
		memcpy(series, vr->Data, vr->Length);
                series[vr->Length]=0;
  		if (series[vr->Length-1]==' ') series[vr->Length-1] = 0;
		}

	char study[66];			/* get the study UID */
        study[0]=0;
	vr = DDOMask->GetVR(0x0020, 0x000d);
	if (vr) 
        	{ 
		memcpy(study, vr->Data, vr->Length);
                study[vr->Length]=0;
  		if (study[vr->Length-1]==' ') study[vr->Length-1] = 0;
		}

	vr = DDOMask->GetVR(0x0008, 0x0018);	/* The SOPInstanceUID */
	if(!vr)
		return ( FALSE );	// it aint going to happen


	// New code uses cached database, but this connection might have hung up
	// New code: cache a database for as long as the RTCStorage exists

	// if (!RTCStorage.DB.Connected)
	//	RTCStorage.DB.Open ( DataSource, UserName, Password, DataHost );
	// if(!GetFileName(vr, Filename, Device, /* RTCStorage.DB, */ TRUE))
	//	return ( FALSE );	// not in database ?

	// Therefore use slower old code here: re-opens a database for each slice read
	if(!GetFileName(vr, Filename, Device, TRUE, pat, study, series))
		return ( FALSE );	// not in database ?

	if (!FindPhysicalDevice(Device, FullFilename, Filename))
		{
		OperatorConsole.printf("***Could not find file:%s\n", Filename);
		return FALSE;
		}
	strcat(FullFilename, Filename);

	// check the filename to determine format

	len = strlen(FullFilename);
	if (len>4)
		UseChapter10 = (stricmp(FullFilename+len-4, ".dcm")==0);

	int t = time(NULL);
	if (!UseChapter10)
	{
	  /* Following load-routine works only for 'implicit' (i.e. no type-codes)
	     LittleEndian files.
	     It is able to skip large VRs; only NKI DicomNodes know how to add extra
	     (private) VRs to the DCO, in order to accomplish this.
	  */
	  iVrSizeLimit = 0xffffffff;
	  if (pDCO)
	  { /* Check whether there is a iVrSizeLimitLength */
	    pVR = pDCO->GetVR(0x9999, 0x0100);
	    if (pVR)
	    iVrSizeLimit = (unsigned int)(*((unsigned int*)pVR->Data));
	  }
	  (*DDOOutPtr) = LoadImplicitLittleEndianFile(FullFilename, iVrSizeLimit);
	  /* Decompression and (in the future) other processing takes place in
	     ProcessDDO (c.f. NKIqrsop.cxx)
	  */
	}
	else	
	  // OR: code in filepdu.cxx (dicom.lib); does not decompress but supports all formats
  	{ (*DDOOutPtr) = PDU.LoadDICOMDataObject(FullFilename);
	  // OR: Code in loadddo.c, this directory; includes decompression, does not support sequences
  	  //	(*DDOOutPtr) = LoadDICOMDataObject(FullFilename);

        }
	LoadTime += time(NULL)-t;

	t= time(NULL);
	ProcessDDO(DDOOutPtr, pDCO);
	ProcessTime += time(NULL)-t;

	if(*DDOOutPtr)
		{
		OperatorConsole.printf("Sending file: %s\n", FullFilename);
		ImagesSent++;
		}
	else
		{
		OperatorConsole.printf("***Could not read file: %s\n", FullFilename);
		return ( FALSE );
		}

	RTCStorage.SetUID(DDOMask->GetVR(0x0008, 0x00016));
	(*SStorage) = &RTCStorage;

	SystemDebug.printf("Image Loaded, returning TRUE\n");
	return ( TRUE );
	}

BOOL	MyPatientRootRetrieve	::	RetrieveOn (
	DICOMDataObject	*DDOMask,
	DICOMDataObject	**DDOOutPtr,
	StandardStorage	**SStorage)
	{
	SystemDebug.printf("MyPatientRootRetrieve :: RetrieveOn\n");
	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage));
	}

BOOL	MyPatientRootRetrieveNKI	::	RetrieveOn (
	DICOMDataObject	*DDOMask,
	DICOMDataObject	**DDOOutPtr,
	StandardStorage	**SStorage,
	DICOMCommandObject	   *pDCO,
        Array < DICOMDataObject *> *pADDO,
	void		*ExtraBytes)
	{
	SystemDebug.printf("MyPatientRootRetrieveNKI :: RetrieveOn\n");
//	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, GetDCO(), GetADDO(), ExtraBytes));
	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, pDCO, pADDO, ExtraBytes));
	}

BOOL	MyPatientRootRetrieveGeneric	::	RetrieveOn (
	DICOMDataObject	*DDOMask,
	DICOMDataObject	**DDOOutPtr,
	StandardStorage	**SStorage,
	DICOMCommandObject	   *pDCO,
        Array < DICOMDataObject *> *pADDO,
	void		*ExtraBytes)
	{
	SystemDebug.printf("MyPatientRootRetrieveGeneric :: RetrieveOn\n");
//	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, NULL, GetADDO(), ExtraBytes));
	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, NULL, pADDO, ExtraBytes));
	}

BOOL	MyStudyRootRetrieve	::	RetrieveOn (
	DICOMDataObject	*DDOMask,
	DICOMDataObject	**DDOOutPtr,
	StandardStorage	**SStorage)
	{
	SystemDebug.printf("MyStudyRootRetrieve :: RetrieveOn\n");
	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage));
	}

BOOL	MyStudyRootRetrieveNKI	::	RetrieveOn (
	DICOMDataObject	*DDOMask,
	DICOMDataObject	**DDOOutPtr,
	StandardStorage	**SStorage,
	DICOMCommandObject	   *pDCO,
        Array < DICOMDataObject *> *pADDO,
	void		*ExtraBytes)
	{
	SystemDebug.printf("MyStudyRootRetrieveNKI :: RetrieveOn\n");
//	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, GetDCO(), GetADDO(), ExtraBytes));
	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, pDCO, pADDO, ExtraBytes));
	}

BOOL	MyStudyRootRetrieveGeneric	::	RetrieveOn (
	DICOMDataObject	*DDOMask,
	DICOMDataObject	**DDOOutPtr,
	StandardStorage	**SStorage,
	DICOMCommandObject	   *pDCO,
        Array < DICOMDataObject *> *pADDO,
	void		*ExtraBytes)
	{
	SystemDebug.printf("MyStudyRootRetrieveGeneric :: RetrieveOn\n");
//	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, NULL, GetADDO(), ExtraBytes));
	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, NULL, pADDO, ExtraBytes));
	}

BOOL	MyPatientStudyOnlyRetrieve	::	RetrieveOn (
	DICOMDataObject	*DDOMask,
	DICOMDataObject	**DDOOutPtr,
	StandardStorage	**SStorage)
	{
	SystemDebug.printf("MyPatientStudyOnlyRetrieve :: RetrieveOn\n");
	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage));
	}

BOOL	MyPatientStudyOnlyRetrieveNKI	::	RetrieveOn (
	DICOMDataObject	*DDOMask,
	DICOMDataObject	**DDOOutPtr,
	StandardStorage	**SStorage,
	DICOMCommandObject	   *pDCO,
        Array < DICOMDataObject *> *pADDO,
	void		*ExtraBytes)
	{
	SystemDebug.printf("MyPatientStudyOnlyRetrieveNKI :: RetrieveOn\n");
//	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, GetDCO(), GetADDO(), ExtraBytes));
	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, pDCO, pADDO, ExtraBytes));
	}

BOOL	MyPatientStudyOnlyRetrieveGeneric	::	RetrieveOn (
	DICOMDataObject	*DDOMask,
	DICOMDataObject	**DDOOutPtr,
	StandardStorage	**SStorage,
	DICOMCommandObject	   *pDCO,
        Array < DICOMDataObject *> *pADDO,
	void		*ExtraBytes)
	{
	SystemDebug.printf("MyPatientStudyOnlyRetrieveGeneric :: RetrieveOn\n");
//	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, NULL, GetADDO(), ExtraBytes));
	return(::RetrieveOn(DDOMask, DDOOutPtr,SStorage,RTCStorage, NULL, pADDO, ExtraBytes));
	}

BOOL
QualifyOn(
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort,
	BYTE	*Compress)
	{
	strcpy((char*)MyACRNema, (char*)MYACRNEMA);
	if(!GetACRNema((char*)ACRNema, (char*)RemoteIP, (char*)RemotePort, (char*)Compress))
		{
		// This is a non-standard "feature".  Perhaps the ACR-Nema
		// is reversable via DNS.  You should return FALSE here
		// if you don't want this.
		strcpy((char*)RemoteIP, (char*) ACRNema);
		strcpy((char*)RemotePort, "104");
		SystemDebug.printf("(QualifyOn) (anonymous) IP:%s, PORT:%s\n", RemoteIP,RemotePort);
		return ( TRUE );
		}

	/* GetACRNema is able to discern upper and lower case, so that when two different
           stations have the same AE, we can still map them to different IP addresses...
	   However, when associating to a 'lower case AE station', it may very well be that
	   that station wants an upper case 'Called AE title'.
	*/

	char szRootSC[64], Temp[64];
  	if (GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig, szRootSC, 64, ConfigFile))
		if (GetPrivateProfileString(szRootSC, "SendUpperCaseAE", "0", Temp, 64, ConfigFile))
			if (atoi(Temp)) strupr((char*)ACRNema);

	SystemDebug.printf("(QualifyOn) (mapped) IP:%s, PORT:%s\n", RemoteIP,RemotePort);
	return ( TRUE );
	}

BOOL	MyPatientRootRetrieve	::	QualifyOn (
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort)
	{
	BYTE dummy[64];
	return(::QualifyOn(ACRNema, MyACRNema, RemoteIP, RemotePort, dummy));
	}

BOOL	MyPatientRootRetrieveNKI	::	QualifyOn (
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort,
	BYTE	*Compress)
	{
	return(::QualifyOn(ACRNema, MyACRNema, RemoteIP, RemotePort, Compress));
	}

BOOL	MyPatientRootRetrieveGeneric	::	QualifyOn (
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort,
	BYTE	*Compress)
	{
	return(::QualifyOn(ACRNema, MyACRNema, RemoteIP, RemotePort, Compress));
	}

BOOL	MyStudyRootRetrieve	::	QualifyOn (
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort)
	{
	BYTE dummy[64];
	return(::QualifyOn(ACRNema, MyACRNema, RemoteIP, RemotePort, dummy));
	}

BOOL	MyStudyRootRetrieveNKI	::	QualifyOn (
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort,
	BYTE	*Compress)
	{
	return(::QualifyOn(ACRNema, MyACRNema, RemoteIP, RemotePort, Compress));
	}

BOOL	MyStudyRootRetrieveGeneric	::	QualifyOn (
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort,
	BYTE	*Compress)
	{
	return(::QualifyOn(ACRNema, MyACRNema, RemoteIP, RemotePort, Compress));
	}

BOOL	MyPatientStudyOnlyRetrieve	::	QualifyOn (
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort)
	{
	BYTE dummy[64];
	return(::QualifyOn(ACRNema, MyACRNema, RemoteIP, RemotePort, dummy));
	}

BOOL	MyPatientStudyOnlyRetrieveNKI	::	QualifyOn (
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort,
	BYTE	*Compress)
	{
	return(::QualifyOn(ACRNema, MyACRNema, RemoteIP, RemotePort, Compress));
	}

BOOL	MyPatientStudyOnlyRetrieveGeneric	::	QualifyOn (
	BYTE	*ACRNema,
	BYTE	*MyACRNema,
	BYTE	*RemoteIP,
	BYTE	*RemotePort,
	BYTE	*Compress)
	{
	return(::QualifyOn(ACRNema, MyACRNema, RemoteIP, RemotePort, Compress));
	}

BOOL	RunTimeClassStorage :: GetUID ( UID	&uid )
	{
	uid = MyUID;
	return ( TRUE );
	}

BOOL	RunTimeClassStorage :: SetUID ( UID	&uid )
	{
	MyUID = uid;
	return ( TRUE );
	}

BOOL	RunTimeClassStorage :: SetUID ( VR	*vr )
	{
	char	s [ 66 ];

	memset((void*)s, 0, 66);
	if(vr)
		{
		memcpy((void*)s, vr->Data, vr->Length>64 ? 64 : vr->Length);
		MyUID.Set(s);
		return(TRUE);
		}
	return ( FALSE );
	}

BOOL	RunTimeClassStorage :: SetUID ( DICOMDataObject *DDOPtr)
	{
	return(SetUID (DDOPtr->GetVR(0x0008, 0x0016)));
	}

#ifndef UNIX
BOOL	WINAPI DriverHelper ( DriverApp	* );
#else
ThreadRoutineType	DriverHelper ( ThreadRoutineArgType	);
#endif

BOOL	DriverApp	::	Server ( BYTE	*port )
	{
	Socket		MSocket;
#ifndef UNIX
	DWORD		ThreadID;
	HANDLE		ThreadHandle;
#else
	pthread_t 	ThreadID;

	// NOTE: this Database is not used BUT, if you don't create/open the
	// Database in the main thread, you won't be able to in child threads...

	Database aDB;
	if(!aDB.Open(DataSource, UserName, Password, DataHost))
		{
		OperatorConsole.printf("***Unable to open database %s as user %s on %s\n",
			DataSource, UserName, DataHost);
		return FALSE;
		}
#endif
	MSocket.SetTimeOut(TCPIPTimeOut);

	if (!MSocket.Listen ((char*) port ))
		{
		OperatorConsole.printf("***Failed to Listen () - bind error\n");
		return ( FALSE );	// failed on the bind
		}

	while (MSocket.Accept() )
		{

		while ( Lock )
			Sleep(50);
		Lock = 1;

		ChildSocketfd= MSocket.Socketfd;
		if (NoThread)
			DriverHelper(this);
		else
			{
#ifndef UNIX
			if(!(ThreadHandle = CreateThread(NULL, 0x0000f000, (LPTHREAD_START_ROUTINE) DriverHelper,
					this, 0, &ThreadID)))
#else	// UNIX
			pthread_create(&ThreadID, NULL, DriverHelper, (void *)this);
			pthread_detach(ThreadID);
			if (!ThreadID)
#endif
				{
				OperatorConsole.printf("***Failed to create thread for child\n");
				closesocket ( MSocket.Socketfd );
				}
			else 
				CloseHandle(ThreadHandle);
			}

		MSocket.Socketfd = 0;
		MSocket.Connected = FALSE;

#ifdef DO_LEAK_DETECTION
		if (kbhit())
		{	/* Only in debug-mode */
			Sleep(10000);	// Hope that other threads are finished by now
			/* Cleanup */
			CloseACRNemaAddressArray();
			free(PatientDB);
			free(StudyDB);
			free(SeriesDB);
			free(ImageDB);
			FreeDeviceTables();
			break;
		}
#endif
		}
#ifdef	DEBUG_MODE
		OperatorConsole.printf("***Error in Accept() function call\n");
		SystemDebug.printf("***Attemping to re-bind socket\n");
#endif
	return ( FALSE );
	}

BOOL	DriverApp	::	ServerChildThread ( int	Socketfd )
	{
	//ExitThread ( (int) ServerChild ( Socketfd ) );
	ServerChild( Socketfd );
	return ( FALSE );
	}

// Technically this routine is not needed.  However, Microsoft Visual C++
// has some odd quarks to it.  This work around maintains MT safe.

#ifndef UNIX
BOOL	WINAPI DriverHelper (DriverApp	*App)
	{
#else
ThreadRoutineType DriverHelper(void *theApp)
	{
	DriverApp *App = (DriverApp *)theApp;
#endif
	int	Socketfd;

	// pick up data from passed-in object

	Socketfd = App->ChildSocketfd;

	// release lock on data

	App->Lock = 0;

	App->ServerChildThread ( Socketfd );
	// we never technically get here
	// LJ: in M$VC-debug, we actually do get here, and a crash follows (at least when
	// ODBC is used)
//	ExitThread(0x00);
	return ( THREAD_RETURN_FALSE );
	}

// Log some user information

void LogUser(char* pszOperation, AAssociateRQ* pARQ, DICOMCommandObject* pDCO)
	{
	UserLog.printf("\"%s\",\"%s\"\n", pszOperation, pARQ->CallingApTitle);
//	NonDestructiveDumpDICOMObject(pDCO); object does not exist anymore
	}

// Helps when looking at crash dump

void WipeStack(void)
	{ 
	char	Dummy[512];
  	memset(Dummy, '?', sizeof(Dummy));
	}

#ifdef __BORLANDC__
extern "C" int         _RTLENTRY _EXPFUNC _heapchk    (void);
#endif

// Storage that does not require a database

void StorageApp	:: FailSafeStorage(CheckedPDU_Service *PDU)
{ DICOMDataObject *DDO;
  DICOMCommandObject DCO;
  char Filename[1024];
  char Physical[1024];
  int i;

  // defined here to make thread safe: multiple instances of SOPUnknownStorage.DB required
  MyUnknownStorage SOPUnknownStorage;

  DDO = new DICOMDataObject;

  while(TRUE)
  { DCO.Reset();
    if (!PDU->Read(&DCO))
	break;

    SOPUnknownStorage.DB = NULL;	// no processing
    SOPUnknownStorage.nopreget = FALSE;
    if(!SOPUnknownStorage.Read(PDU, &DCO, DDO)) break;

    GetPhysicalDevice("MAG0", Physical);
    sprintf(Filename, "%sfailsafe_storage", Physical);
    mkdir(Filename);
    i = strlen(Filename);
    Filename[i]   = PATHSEPCHAR;
    Filename[i+1] = 0;
    GenUID(Filename + strlen(Filename));
    strcat(Filename, ".dcm");
    PDU->SaveDICOMDataObject(Filename, DICOM_CHAPTER_10_IMPLICIT, DDO);

    OperatorConsole.printf("***Failsafe storage saved incoming file as: %s\n", Filename);
  }

  delete DDO;
}

void TestCompress(char *filename, char *modes)
{ DICOMDataObject 	*pDDO;
  int			len, i, syntax;
  PDU_Service		PDU;
  VR			*vrSOPInstanceUID;
  Database		DB;
  char			rFilename[1024];
  char 			mode[3];
  char			SOP[66];
  VR			*vr;

  PDU.AttachRTC(&VRType);

  if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
	{
	OperatorConsole.printf("***Error Connecting to SQL\n");
	return;
	}

  pDDO = LoadForGUI(filename);
  if(!pDDO)
	{
	OperatorConsole.printf("***[TestCompress] %s -FAILED: Error on Load\n", filename);
	return;
	}

  for(syntax=3; syntax<=4; syntax++)
  for(i=0; i<strlen(modes); i+=2)
  { memcpy(mode, modes+i, 2);
    mode[2]=0;
    Sleep(1);

    pDDO = LoadForGUI(filename);
    strcpy(rFilename, "org");

    GenUID(SOP); vr = new VR(0x0008, 0x0018, strlen(SOP), (void*)SOP, (BOOL) FALSE ); pDDO->ReplaceVR(vr);
    vr = new VR(0x0020, 0x0013, strlen(rFilename), (void*)rFilename, (BOOL) FALSE ); pDDO->ReplaceVR(vr);
    SaveToDisk(DB, pDDO, rFilename, TRUE, (unsigned char *)"testcompress", (unsigned char *)"testcompress", syntax);
    pDDO = PDU.LoadDICOMDataObject(rFilename);
    OperatorConsole.printf("Added file: %s (filename syntax %d)\n", rFilename, syntax);

    // recompress dropped file
    recompress(&pDDO, mode, "", mode[0]=='n' || mode[0]=='N'); strcpy(rFilename, mode);
    GenUID(SOP); vr = new VR(0x0008, 0x0018, strlen(SOP), (void*)SOP, (BOOL) FALSE ); pDDO->ReplaceVR(vr);
    vr = new VR(0x0020, 0x0013, strlen(rFilename), (void*)rFilename, (BOOL) FALSE ); pDDO->ReplaceVR(vr);
    SaveToDisk(DB, pDDO, rFilename, TRUE, (unsigned char *)"testcompress", (unsigned char *)"testcompress", syntax);
    pDDO = PDU.LoadDICOMDataObject(rFilename);
    OperatorConsole.printf("Added file: %s (compression %s, syntax %d)\n", rFilename, mode, syntax);

    // uncompress dropped file
    recompress(&pDDO, "un");   strcpy(rFilename, mode); strcat(rFilename, "_un");
    GenUID(SOP); vr = new VR(0x0008, 0x0018, strlen(SOP), (void*)SOP, (BOOL) FALSE ); pDDO->ReplaceVR(vr);
    vr = new VR(0x0020, 0x0013, strlen(rFilename), (void*)rFilename, (BOOL) FALSE ); pDDO->ReplaceVR(vr);
    SaveToDisk(DB, pDDO, rFilename, TRUE, (unsigned char *)"testcompress", (unsigned char *)"testcompress", syntax);
    pDDO = PDU.LoadDICOMDataObject(rFilename);
    OperatorConsole.printf("Added file: %s (compression %s_un, syntax %d)\n", rFilename, mode, syntax);

    // recompress dropped file again
    recompress(&pDDO, mode, "", mode[0]=='n' || mode[0]=='N'); strcpy(rFilename, mode); strcat(rFilename, "_un_"); strcat(rFilename, mode);
    GenUID(SOP); vr = new VR(0x0008, 0x0018, strlen(SOP), (void*)SOP, (BOOL) FALSE ); pDDO->ReplaceVR(vr);
    vr = new VR(0x0020, 0x0013, strlen(rFilename), (void*)rFilename, (BOOL) FALSE ); pDDO->ReplaceVR(vr);
    SaveToDisk(DB, pDDO, rFilename, TRUE, (unsigned char *)"testcompress", (unsigned char *)"testcompress", syntax);
    pDDO = PDU.LoadDICOMDataObject(rFilename);
    OperatorConsole.printf("Added file: %s (compression %s_un_%s, syntax %d)\n", rFilename, mode, mode, syntax);
  }

  delete pDDO;
}

void TestForward(char *filename, char *mode, char *server)
{ char host[64], port[64], compress[64];
  char *p;
  RunTimeClassStorage	RTCStorage;
  ExtendedPDU_Service	PDU;
  DICOMDataObject*	pDDO;
  UID			AppUID ("1.2.840.10008.3.1.1.1");
  UID			iUID;
  VR			*vr;
  char			SOP[66], temp[20];
  BOOL			StripGroup2;

  PDU.AttachRTC(&VRType);

  if(!GetACRNema(server, (char *)host, (char *)port, (char *)compress))
  { strcpy(host, server);
    p = strchr(host, ':');
    if (p) 
    { *p=0;
      strcpy(port, p+1);
    }
    else 
      strcpy(port, "5678");
  }

  PDU.SetRequestedCompressionType("");	// default

  pDDO = LoadForGUI(filename);
  if (!pDDO)
  { OperatorConsole.printf("*** Test forward: failed to load DICOM object %s", filename);
    return;
  }

  // offer transfer syntax from forward string
  if (mode)
    PDU.SetRequestedCompressionType(mode);
  else
  // offer transfer syntax from acrnema.map
    PDU.SetRequestedCompressionType(compress);

  PDU.SetApplicationContext ( AppUID );
  PDU.SetLocalAddress ( MYACRNEMA );
  PDU.SetRemoteAddress ( (unsigned char *)server );

  vr = pDDO -> GetVR(0x0008, 0x0016);
  if (!vr)
  { OperatorConsole.printf("*** Test forward: Forward failed because SopClass is missing in %s\n", filename);
    delete pDDO;
    return;
  }
  SetUID ( iUID, vr );
  PDU.AddAbstractSyntax ( iUID );
  PDU.SetTimeOut(TCPIPTimeOut);

  if (!PDU.Connect((unsigned char *)&host, (unsigned char *)&port))
  { OperatorConsole.printf("*** Test forward: Forward failed to connect to host %s\n", server);
    delete pDDO;
    return;
  }

  if (!PDU.IsAbstractSyntaxAccepted(iUID))
  { OperatorConsole.printf("*** Test forward: DICOM server %s does not accept type of forwarded images\n", server);
    PDU.Close();
    delete pDDO;
    return;
  }

  OperatorConsole.printf("Test forward: requested %s, accepted %s, sending to %s = %s:%s\n", mode, PDU.GetAcceptedCompressionType(iUID), server, host, port);

  GenUID(SOP); vr = new VR(0x0008, 0x0018, strlen(SOP), (void*)SOP, (BOOL) FALSE ); pDDO->ReplaceVR(vr);
  sprintf(temp, "%s %s", mode, PDU.GetAcceptedCompressionType(iUID)); vr = new VR(0x0020, 0x0013, strlen(temp), (void*)temp, (BOOL) FALSE ); pDDO->ReplaceVR(vr);

  // recompress data to be forwarded here according to accepted compression mode
  p = PDU.GetAcceptedCompressionType(iUID);
  StripGroup2 = memicmp(p, "as", 2)!=0 && memicmp(p, "is", 2)!=0;
  recompress(&pDDO, p, "", StripGroup2);

  RTCStorage.SetUID(iUID);

  if (!RTCStorage.Write(&PDU, pDDO))
  { OperatorConsole.printf("*** Test forward: Forward failed to send DICOM image to %s\n", server);
    PDU.Close();
    delete pDDO;
    return;
  }

  delete pDDO;
  PDU.Close();
}

void TestSyntax(char *filename, int syntax)
{ DICOMDataObject*	pDDO;
  VR			*vr;
  char			SOP[66], temp[20];
  Database		DB;
  char			rFilename[1024];

  if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
	{
	OperatorConsole.printf("***Error Connecting to SQL\n");
	return;
	}

  pDDO = LoadForGUI(filename);
  if (!pDDO)
  { OperatorConsole.printf("*** Test syntax: failed to load DICOM object %s", filename);
    return;
  }

  GenUID(SOP); vr = new VR(0x0008, 0x0018, strlen(SOP), (void*)SOP, (BOOL) FALSE ); pDDO->ReplaceVR(vr);
  sprintf(temp, "syntax %d", syntax); vr = new VR(0x0020, 0x0013, strlen(temp), (void*)temp, (BOOL) FALSE ); pDDO->ReplaceVR(vr);

  SaveToDisk(DB, pDDO, rFilename, TRUE, (unsigned char *)"testsyntax", (unsigned char *)"testsyntax", syntax);
}

static BOOL WINAPI testsavethread(char *filename)
{ TestSyntax(filename, 0);
  return TRUE;
}

void TestThreadedSave(char *filename)
{ 
#ifdef WIN32
  unsigned long ThreadID;
  CreateThread(NULL, 0x000ff000, (LPTHREAD_START_ROUTINE) testsavethread, filename, 0, &ThreadID);
#else
  pthread_t ThreadID;
  pthread_create(&ThreadID, NULL, (void*(*)(void*))testsavethread, (void *)filename);
  pthread_detach(ThreadID);
#endif
};

///////////////////////////// elementary printer server support //////////////////////////

// basic film session: is created, deleted, set and action

class CFSRQ : public NCreateRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.1"); return TRUE; }
	};

class CFSRSP : public NCreateRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.1"); return TRUE; }
	};

class DFSRQ : public NDeleteRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.1"); return TRUE; }
	};

class DFSRSP : public NDeleteRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.1"); return TRUE; }
	};

class SFSRQ : public NSetRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.1"); return TRUE; }
	};

class SFSRSP : public NSetRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.1"); return TRUE; }
	};

class AFSRQ : public NActionRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.1"); return TRUE; }
	};

class AFSRSP : public NActionRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.1"); return TRUE; }
	};

// basic film box: is created, set, action(printed) and deleted

class CFBRQ : public NCreateRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.2"); return TRUE; }
	};

class CFBRSP : public NCreateRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.2"); return TRUE; }
	};

class DFBRQ : public NDeleteRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.2"); return TRUE; }
	};

class DFBRSP : public NDeleteRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.2"); return TRUE; }
	};

class AFBRQ : public NActionRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.2"); return TRUE; }
	};

class AFBRSP : public NActionRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.2"); return TRUE; }
	};

class SFBRQ : public NSetRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.2"); return TRUE; }
	};

class SFBRSP : public NSetRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.2"); return TRUE; }
	};

// Basic GrayScale ImageBox should only be set

class SBGIBRQ : public NSetRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.4"); return TRUE; }
	};

class SBGIBRRSP : public NSetRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.4"); return TRUE; }
	};

// Basic Color ImageBox should only be set

class SBCIBRQ : public NSetRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.4.1"); return TRUE; }
	};

class SBCIBRRSP : public NSetRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.4.1"); return TRUE; }
	};

// Printer

class GPRQ : public NGetRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.16"); return TRUE; }
	};

class GPRSP : public NGetRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.16"); return TRUE; }
	};

class EPRQ : public NEventReportRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.16"); return TRUE; }
	};

class EPRSP : public NEventReportRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.16"); return TRUE; }
	};

/* OPTIONAL sop classes

// Basic Annotation Box should only be set

class SBABRQ : public NSetRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.15"); return TRUE; }
	} SetBasicAnnotationBoxRequest;

class SBABRRSP : public NSetRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.15"); return TRUE; }
	} SetBasicAnnotationBoxResponse;

// Print job 

class GPJRQ : public NGetRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.14"); return TRUE; }
	} GetPrintJobRequest;

class GPJRSP : public NGetRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.14"); return TRUE; }
	} GetPrintJobResponse;

class EPJRQ : public NEventReportRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.14"); return TRUE; }
	} EventReportPrintJobRequest;

class EPJRSP : public NEventReportRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.14"); return TRUE; }
	} EventReportPrintJobResponse;

// Presentation LUT: is created and deleted

class CPLRQ : public NCreateRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.23"); return TRUE; }
	} CreatePresentationLUTRequest;

class CPLRSP : public NCreateRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.23"); return TRUE; }
	} CreatePresentationLUTResponse;

class DPLRQ : public NDeleteRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.23"); return TRUE; }
	} DeletePresentationLUTRequest;

class DPLRSP : public NDeleteRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.23"); return TRUE; }
	} DeletePresentationLUTResponse;

// Printer configuration: get only

class GPCRQ : public NGetRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.16.376"); return TRUE; }
	} GetPrinterConfigurationRequest;

class GPCRSP : public NGetRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.5.1.1.16.376"); return TRUE; }
	} GetPrinterConfigurationResponse;

*/

static int pcount=0;

BOOL StorageApp ::	PrinterSupport( CheckedPDU_Service *PDU, DICOMCommandObject *DCO, DICOMDataObject *PrintData[])
	{
	DICOMDataObject		*DDO;
	char			uid[65], Filename[1024];
	UID			Uid;
	VR			*vr, *vrs;

	UID			aBasicGrayscalePrintManagementMetaSOPClass("1.2.840.10008.5.1.1.9");
	UID			aBasicColorPrintManagementMetaSOPClass("1.2.840.10008.5.1.1.18");

	UID			aBasicFilmSessionSOPClassUID("1.2.840.10008.5.1.1.1");
	UID			aBasicFilmBoxSOPClassUID("1.2.840.10008.5.1.1.2");
	UID			aBasicGrayscaleImageBoxSOPClassUID("1.2.840.10008.5.1.1.4");
	UID			aBasicColorImageBoxSOPClassUID("1.2.840.10008.5.1.1.4.1");
	UID			aBasicPrinterSOPClassUID("1.2.840.10008.5.1.1.16");
	UID			aAppuid("1.2.840.10008.3.1.1.1");
	UID			aPrinter("1.2.840.10008.5.1.1.17");
	int			i;
	char			szPrintSOP[65];


CFSRQ	CreateBasicFilmSessionRequest;
CFSRSP	CreateBasicFilmSessionResponse;
DFSRQ	DeleteBasicFilmSessionRequest;
DFSRSP	DeleteBasicFilmSessionResponse;
SFSRQ	SetBasicFilmSessionRequest;
SFSRSP	SetBasicFilmSessionResponse;
AFSRQ	ActionBasicFilmSessionRequest;
AFSRSP	ActionBasicFilmSessionResponse;
CFBRQ	CreateBasicFilmBoxRequest;
CFBRSP	CreateBasicFilmBoxResponse;
DFBRQ	DeleteBasicFilmBoxRequest;
DFBRSP	DeleteBasicFilmBoxResponse;
AFBRQ	ActionBasicFilmBoxRequest;
AFBRSP	ActionBasicFilmBoxResponse;
SFBRQ	SetBasicFilmBoxRequest;
SFBRSP	SetBasicFilmBoxResponse;
SBGIBRQ	SetBasicGrayScaleImageBoxRequest;
SBGIBRRSP SetBasicGrayScaleImageBoxResponse;
SBCIBRQ	SetBasicColorImageBoxRequest;
SBCIBRRSP SetBasicColorImageBoxResponse;
GPRQ 	GetPrinterRequest;
GPRSP	GetPrinterResponse;
EPRQ	EventReportPrinterRequest;
EPRSP	EventReportPrinterResponse;

	DDO = new DICOMDataObject;
	
	vr = DCO->GetVR(0x0000, 0x0002);
	if (!vr)
		vr = DCO->GetVR(0x0000, 0x0003);

	if (vr && vr->Length >= strlen("1.2.840.10008.5.1.1.1 "))
		{
		strncpy(szPrintSOP, (char*)vr->Data, vr->Length);
		szPrintSOP[vr->Length] = 0;

		if (strcmp(szPrintSOP, "1.2.840.10008.5.1.1.16") == 0)
			{
			if (GetPrinterRequest.Read(DCO, PDU, DDO))
				{
				OperatorConsole.printf("getting Printer\n");

				// open up PDU for class UIDs used by printer objects
				PDU->AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
				aBasicFilmSessionSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
				aBasicFilmBoxSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
				aBasicGrayscaleImageBoxSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
				aBasicPrinterSOPClassUID);
				
				PDU->AddAbstractSyntaxAlias(aBasicColorPrintManagementMetaSOPClass,
				aBasicFilmSessionSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicColorPrintManagementMetaSOPClass,
				aBasicFilmBoxSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicColorPrintManagementMetaSOPClass,
				aBasicColorImageBoxSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicColorPrintManagementMetaSOPClass,
				aBasicPrinterSOPClassUID);

				PrintData[0] = new DICOMDataObject;
				VR *aVR;
				aVR = new VR(0x2110, 0x0010, strlen("NORMAL"), (void *)"NORMAL", FALSE);
				PrintData[0]->Push(aVR);
				aVR = new VR(0x2110, 0x0020, strlen("NORMAL"), (void *)"NORMAL", FALSE);
				PrintData[0]->Push(aVR);
				aVR = new VR(0x2110, 0x0030, strlen("Conquest dicom printer"), (void *)"Conquest dicom printer", FALSE);
				PrintData[0]->Push(aVR);

				delete DDO;
				return GetPrinterResponse.Write(PDU, DCO, &aPrinter, 0, PrintData[0]);
				}

			if (EventReportPrinterRequest.Read(DCO, PDU, DDO))
				{
				OperatorConsole.printf("N-event Printer\n");
				delete DDO;
				return EventReportPrinterResponse.Write(PDU, DCO, &aPrinter, 0, PrintData[0]);
				}
			}

		else if (strcmp(szPrintSOP, "1.2.840.10008.5.1.1.1") == 0)
			{
			if (CreateBasicFilmSessionRequest.Read(DCO, PDU, DDO))
				{
				PrintData[1] = DDO;

				// open up PDU for class UIDs used by printer objects
				PDU->AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
				aBasicFilmSessionSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
				aBasicFilmBoxSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
				aBasicGrayscaleImageBoxSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
				aBasicPrinterSOPClassUID);
				
				PDU->AddAbstractSyntaxAlias(aBasicColorPrintManagementMetaSOPClass,
				aBasicFilmSessionSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicColorPrintManagementMetaSOPClass,
				aBasicFilmBoxSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicColorPrintManagementMetaSOPClass,
				aBasicColorImageBoxSOPClassUID);
				PDU->AddAbstractSyntaxAlias(aBasicColorPrintManagementMetaSOPClass,
				aBasicPrinterSOPClassUID);

				GenUID(uid); 
				Uid.Set(uid);

				OperatorConsole.printf("Creating Basic Film Session\n");
				CreateBasicFilmSession++;
				BOOL r = CreateBasicFilmSessionResponse.Write(PDU, DCO, &Uid, 0, DDO);
				delete DDO;
				return r;
				}
			if (DeleteBasicFilmSessionRequest.Read(DCO, PDU))
				{
				OperatorConsole.printf("Deleting Basic Film Session\n");
				//delete PrintData[1];
				//PrintData[1] = NULL;
				DeleteBasicFilmSession++;
				delete DDO;
				return DeleteBasicFilmSessionResponse.Write(PDU, DCO, &Uid, 0);
				}

			if (ActionBasicFilmSessionRequest.Read(DCO, PDU, DDO))
				{
				OperatorConsole.printf("Printing Basic Film Session\n");
				ActionBasicFilmSession++;
				delete DDO;
				return ActionBasicFilmSessionResponse.Write(PDU, DCO, NULL, 0, 0, NULL);
				}

			if (SetBasicFilmSessionRequest.Read(DCO, PDU, DDO))
				{
				PrintData[1] = DDO;
				OperatorConsole.printf("Set Basic Film Session\n");
				SetBasicFilmSession++;
				delete DDO;
				return SetBasicFilmSessionResponse.Write(PDU, DCO, NULL, 0, NULL);
				}
			}


		else if (strcmp(szPrintSOP, "1.2.840.10008.5.1.1.2") == 0)
			{
			if (CreateBasicFilmBoxRequest.Read(DCO, PDU, DDO))
				{
				int n=1, rows=1, cols=1;
				char text[256];

				PrintData[2] = DDO;
				GenUID(uid); 
				Uid.Set(uid);

				VR *avr = DDO->GetVR(0x2010, 0x0010);
				if (avr)
					{
					char *p1, *p2;
					memset(text, 0, 256);
					memcpy(text, avr->Data, avr->Length);
					p1 = strchr(text, '\\');
					p2 = strchr(text, ',');
					if (p1 && p2)
						{
						rows = atoi(p1+1);
						cols = atoi(p2+1);
						n =  rows * cols;
						}
					}

				text[0]=0;
				avr = DDO->GetVR(0x2010, 0x0040);
				if (avr)
					{
					memset(text, 0, 256);
					memcpy(text, avr->Data, avr->Length);
					}

				OperatorConsole.printf("Creating Basic Film Box with %d Image boxes - %s - Film# %d\n", n, text, pcount);

				Array < DICOMDataObject * > *SQE = new Array < DICOMDataObject * >;
				vrs = new VR(0x2010, 0x0510, 0, NULL, FALSE);
				vrs->SQObjectArray = (void*) SQE;

				for (i=0; i<n; i++)
					{
					char *uid2 = (char *)malloc(65);
					strcpy(uid2, uid);
					if (uid2[50]=='.') uid2[50]=0;	// make space for pcount, row/col info
                                        else               uid2[51]=0;
					sprintf(uid2 + strlen(uid2), ".%d.%d.%d.%d", pcount%1000, rows, cols, i);
					DICOMDataObject *D = new DICOMDataObject;
					VR *aVR;
					aVR = new VR(0x0008, 0x1150, strlen("1.2.840.10008.5.1.1.4 "), (void *)"1.2.840.10008.5.1.1.4", FALSE);
					D->Push(aVR);
					aVR = new VR(0x0008, 0x1155, (strlen(uid2)+1)&0xfffe, (void *)uid2, TRUE); //leak
					D->Push(aVR);
					SQE->Add(D);
					}

				pcount++;
				DDO->Push(vrs);

				CreateBasicFilmBox++;
				BOOL r = CreateBasicFilmBoxResponse.Write(PDU, DCO, &Uid, 0, DDO);
				delete DDO;
				return r;
				}

			if (ActionBasicFilmBoxRequest.Read(DCO, PDU, DDO))
				{
				OperatorConsole.printf("Printing Basic Film Box\n");
				ActionBasicFilmBox++;
				delete DDO;
				return ActionBasicFilmBoxResponse.Write(PDU, DCO, NULL, 0, 0, NULL);
				}

			if (SetBasicFilmBoxRequest.Read(DCO, PDU, DDO))
				{
				PrintData[2] = DDO;
				OperatorConsole.printf("Set Basic Film Box\n");
				SetBasicFilmBox++;
				delete DDO;
				return SetBasicFilmBoxResponse.Write(PDU, DCO, NULL, 0, NULL);
				}

			if (DeleteBasicFilmBoxRequest.Read(DCO, PDU))
				{
				OperatorConsole.printf("Deleting Basic Film Box\n");
				//delete PrintData[2];
				//PrintData[2] = NULL;
				DeleteBasicFilmBox++;
				delete DDO;
				return DeleteBasicFilmBoxResponse.Write(PDU, DCO, &Uid, 0);
				}
			}

		else if (strcmp(szPrintSOP, "1.2.840.10008.5.1.1.4") == 0)
			{
			if (SetBasicGrayScaleImageBoxRequest.Read(DCO, PDU, DDO))
				{
				vr = DCO->GetVR(0x0000, 0x1001);
				if (vr)
					{
					memset(Filename, 0, 1024);
					GetPhysicalDevice("MAG0", Filename);
					strcat(Filename, "printer_files");
					mkdir(Filename);
					i = strlen(Filename);
					Filename[i]   = PATHSEPCHAR;
					Filename[i+1] = 0;
					memcpy(Filename + strlen(Filename), vr->Data, vr->Length);
					strcat(Filename, ".dcm");
					}

				//OperatorConsole.printf("Setting Basic GrayScale ImageBox\n");

				vr = DDO->GetVR(0x2020, 0x0110);
				if(vr && vr->SQObjectArray)
					{
					Array < DICOMDataObject	* > *aSQArray;
					aSQArray = (Array < DICOMDataObject * > *) vr->SQObjectArray;
					if ( aSQArray->GetSize() )
						{
					 	PDU->SaveDICOMDataObject(Filename, DICOM_CHAPTER_10_EXPLICIT, aSQArray->Get(0));
						OperatorConsole.printf("Print file: %s\n", Filename);
						}
					}

				SetBasicGrayScaleImageBox++;
				delete DDO;
				return SetBasicGrayScaleImageBoxResponse.Write(PDU, DCO, NULL, 0, NULL);
				}
			}

		else if (strcmp(szPrintSOP, "1.2.840.10008.5.1.1.4.1") == 0)
			{
			if (SetBasicColorImageBoxRequest.Read(DCO, PDU, DDO))
				{
				vr = DCO->GetVR(0x0000, 0x1001);
				if (vr)
					{
					memset(Filename, 0, 1024);
					GetPhysicalDevice("MAG0", Filename);
					strcat(Filename, "printer_files");
					mkdir(Filename);
					i = strlen(Filename);
					Filename[i]   = PATHSEPCHAR;
					Filename[i+1] = 0;
					memcpy(Filename + strlen(Filename), vr->Data, vr->Length);
					strcat(Filename, ".dcm");
					}

				//OperatorConsole.printf("Setting Basic Color ImageBox\n");

				vr = DDO->GetVR(0x2020, 0x0111);
				if(vr && vr->SQObjectArray)
					{
					Array < DICOMDataObject	* > *aSQArray;
					aSQArray = (Array < DICOMDataObject * > *) vr->SQObjectArray;
					if ( aSQArray->GetSize() )
						{
					 	PDU->SaveDICOMDataObject(Filename, DICOM_CHAPTER_10_EXPLICIT, aSQArray->Get(0));
						OperatorConsole.printf("Print file: %s\n", Filename);
						}
					}

				SetBasicColorImageBox++;
				delete DDO;
				return SetBasicColorImageBoxResponse.Write(PDU, DCO, NULL, 0, NULL);
				}
			}
		}
	delete DDO;
	return FALSE;
	}

///////////////////////////// elementary storage commitment support as SCP //////////////////////////
//////////// storage commitment as SCU would be needed for a safe Move/Delete operation /////////////

/* first is a sample transaction 

SEND ASSOCIATE-RQ (
PRESENTATION-CONTEXT-ITEMS
("Storage Commitment Push Model SOP Class", "Implicit VR Little Endian")
)

RECEIVE ASSOCIATE-AC (
PRESENTATION-CONTEXT-ITEMS
("Storage Commitment Push Model SOP Class", 0, "Implicit VR Little Endian")
)


SEND N-ACTION-RQ "Commitment Push" (
(0x00000003, "Storage Commitment Push Model SOP Class")
(0x00001001, "1.2.840.10008.1.20.1.1") # Well-known Instance UID
(0x00001008, 1) # Request Storage Commitment

(0x00081195, UI, "NEW:TransactionUID") # Transaction UID
(0x00081199, SQ,
>(0x00081150, UI, "1.2.840.10008.5.1.4.1.1.7") # Referenced SOP Class UID
>(0x00081155, UI, "NEW:ImageInstanceUID1") # Referenced SOP Instance UID
,
>(0x00081150, UI, "1.2.840.10008.5.1.4.1.1.7") # Referenced SOP Class UID
>(0x00081155, UI, "NEW:ImageInstanceUID2") # Referenced SOP Instance UID
,
>(0x00081150, UI, "1.2.840.10008.5.1.4.1.1.7") # Referenced SOP Class UID
>(0x00081155, UI, "NEW:ImageInstanceUID3") # Referenced SOP Instance UID
,
>(0x00081150, UI, "1.2.840.10008.5.1.4.1.1.7") # Referenced SOP Class UID
>(0x00081155, UI, "NEW:ImageInstanceUID4") # Referenced SOP Instance UID
) # Referenced SOP Sequence
(0x00081111, SQ,
>(0x00081150, UI, "1.2.840.10008.3.1.2.3.3") # Referenced SOP Class UID
>(0x00081155, UI, "NEW:MppsUID") # Referenced SOP Instance UID
) # Referenced Study Component Sequence
)

RECEIVE N-ACTION-RSP (
(0x00000002, "Storage Commitment Push Model SOP Class")
(0x00001000, "1.2.840.10008.1.20.1.1") # Well-known Instance UID
(0x00000900, 0x0000)
)


SEND RELEASE-RQ
RECEIVE RELEASE-RP


RECEIVE ASSOCIATE-RQ (
PRESENTATION-CONTEXT-ITEMS
("Storage Commitment Push Model SOP Class", "Implicit VR Little Endian")
)

SEND ASSOCIATE-AC (
PRESENTATION-CONTEXT-ITEMS
("Storage Commitment Push Model SOP Class", 0, "Implicit VR Little Endian")
)


RECEIVE N-EVENT-REPORT-RQ "Commitment Push" (
(0x00000002, "Storage Commitment Push Model SOP Class")
(0x00001000, "1.2.840.10008.1.20.1.1") # Well-known Instance UID
(0x00001002, 1) # Storage Commitment Request Successful

(0x00081195, UI, "TransactionUID") # Transaction UID
(0x00080054, AE, "ARCHIVE") # Retrieve AE Title
(0x00081199, SQ,
>(0x00081150, UI, "1.2.840.10008.5.1.4.1.1.7") # Referenced SOP Class UID
>(0x00081155, UI, "ImageInstanceUID1") # Referenced SOP Instance UID
,
>(0x00081150, UI, "1.2.840.10008.5.1.4.1.1.7") # Referenced SOP Class UID
>(0x00081155, UI, "ImageInstanceUID2") # Referenced SOP Instance UID
,
>(0x00081150, UI, "1.2.840.10008.5.1.4.1.1.7") # Referenced SOP Class UID
>(0x00081155, UI, "ImageInstanceUID3") # Referenced SOP Instance UID
,
>(0x00081150, UI, "1.2.840.10008.5.1.4.1.1.7") # Referenced SOP Class UID
>(0x00081155, UI, "ImageInstanceUID4") # Referenced SOP Instance UID
) # Referenced SOP Sequence
)

SEND N-EVENT-REPORT-RSP (
(0x00000002, "Storage Commitment Push Model SOP Class")
(0x00001000, "1.2.840.10008.1.20.1.1") # Well-known Instance UID
(0x00000900, 0x0000)
)


RECEIVE RELEASE-RQ
SEND RELEASE-RP
*/

class ASCPRQ : public NActionRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.1.20.1"); return TRUE; }
	};

class ASCPRSP : public NActionRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.1.20.1"); return TRUE; }
	};

class ESCPRQ : public NEventReportRQ
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.1.20.1"); return TRUE; }
	};

class ESCPRSP : public NEventReportRSP
	{
	public:	BOOL	GetUID ( UID &theUID ) { theUID.Set("1.2.840.10008.1.20.1"); return TRUE; }
	};

BOOL StorageApp ::	StorageCommitmentSupport( CheckedPDU_Service *PDU, DICOMCommandObject *DCO, DICOMDataObject **CommitData)
	{
	VR *vr;
	DICOMCommandObject DCOR;
	char Sop[65], Calling[20];
	BYTE ip[64], port[64], compress[64];
	int nfail=0;
	BOOL status;

	UID	StorageCommitmentPushModelClassUID("1.2.840.10008.1.20.1");
	UID	StorageCommitmentPushModelInstanceUID("1.2.840.10008.1.20.1.1");

	ASCPRQ	ActionStorageCommitmentPushRequest;
	ASCPRSP	ActionStorageCommitmentPushResponse;

	ESCPRQ	EventStorageCommitmentPushRequest;
	ESCPRSP	EventStorageCommitmentPushResponse;

	// association was closed after action: send response on new association

	if (*CommitData)
		{
		PDU_Service PDU2;
		UID uid;

		strcpy(Calling, (char *)(((AAssociateAC *)PDU)->CallingApTitle));
		while (Calling[strlen(Calling)-1]==' ') Calling[strlen(Calling)-1] = 0;

		PDU2.AttachRTC(&VRType);
		if(!GetACRNema(Calling, (char *)ip, (char *)port, (char *)compress))
			{
			OperatorConsole.printf("*** Storage commitment - host not found: %s\n", Calling);
			return FALSE;
			}

		PDU2.ClearAbstractSyntaxs();
		PDU2.SetLocalAddress(MYACRNEMA);
		PDU2.SetRemoteAddress((unsigned char *)Calling);

		uid.Set("1.2.840.10008.3.1.1.1");	// Application context (DICOM App)
		PDU2.SetApplicationContext(uid);

		uid.Set("1.2.840.10008.1.20.1");
		PDU2.AddAbstractSyntax(uid);

		PDU2.SetTimeOut(TCPIPTimeOut);

		// Make new association for EVENT on port/ip
		if(!PDU2.Connect(ip, port))
			{
			OperatorConsole.printf("*** Storage commitment - failed to reconnect to %s (%s %s) for N-EVENT\n", Calling, ip, port);
			return FALSE;
			}

		status = EventStorageCommitmentPushRequest.Write(&PDU2, *CommitData, &StorageCommitmentPushModelInstanceUID, nfail==0 ? 1 : 2);
			if (status)
			{
			status = PDU2.Read(&DCOR);
			if (status)
				{
				EventStorageCommitmentPushResponse.Read(&DCOR, &PDU2, NULL);
				PDU2.Close();
				return FALSE;
				}
			}

		PDU2.Close();
		return FALSE;
		}

	// is this a storage commitment action request ?

	vr = DCO->GetVR(0x0000, 0x0003);
	if (!vr) return FALSE;
	strncpy(Sop, (char*)vr->Data, vr->Length);
	Sop[vr->Length] = 0;
	if (strcmp(Sop, "1.2.840.10008.1.20.1")!=0) return FALSE;

	vr = DCO->GetVR(0x0000, 0x1001);
	if (!vr) return FALSE;
	strncpy(Sop, (char*)vr->Data, vr->Length);
	Sop[vr->Length] = 0;
	if (strcmp(Sop, "1.2.840.10008.1.20.1.1")!=0) return FALSE;

	vr = DCO->GetVR(0x0000, 0x1008);
	if (!vr) return FALSE;
	if ((int)(*((unsigned short*)vr->Data))!=1) return FALSE;

	// read and acknowledge the action

	*CommitData = new DICOMDataObject;
	if (ActionStorageCommitmentPushRequest.Read(DCO, PDU, *CommitData))
		{
		OperatorConsole.printf("Storage commitment request recieved\n");
 		if (!ActionStorageCommitmentPushResponse.Write(PDU, DCO, &StorageCommitmentPushModelInstanceUID, 1, 0, NULL)) 
			return FALSE;
	
		// delete retired study component sequence if there

		vr = (*CommitData)->GetVR(0x0008, 0x1111);
      		if (vr) (*CommitData)->DeleteVR(vr);

		// add AE title

		SetStringVR(&vr, 0x0008, 0x0054, (char*)MYACRNEMA); 
		(*CommitData)->Push(vr);

		// todo:
		// check presence of each SOPInstance 0008,1155 in sequence 0008,1199 
		// move failures from sequence 0008,1199 to sequence 0008,1198
		// count failures in nfail

		// write an event (on same association) with results

		Sleep(200);

		status = EventStorageCommitmentPushRequest.Write(PDU, *CommitData, &StorageCommitmentPushModelInstanceUID, nfail==0 ? 1 : 2);

		if (status)
			{
			status = PDU->Read(&DCOR);
			if (status)
				{
				EventStorageCommitmentPushResponse.Read(&DCOR, PDU, NULL);
				delete *CommitData;
				*CommitData = NULL;
				return TRUE;
				}
			}

		// in this case, the link was closed: create a new link and try to send response again
		return FALSE;
		}

	return FALSE;
	}

void ConfigDgate(void);
BOOL ToGif(DICOMDataObject* pDDO, char *filename, int size, int append, int level, int window);
BOOL ToBMP(DICOMDataObject* pDDO, char *filename, int size, int append, int level, int window);
int MaybeDownsize(DICOMDataObject* pDDO, DICOMCommandObject* pDCO, int size);

void CloneDB(char *AE)
{ VR *vr1;
  VR *vr;
  DICOMDataObject	DDO;
  Array < DICOMDataObject * > ADDO;
  char patid[66];
  int len;

  strncpy(VirtualServerFor[10], AE, 16);
  VirtualServerFor[10][16]=0;

  SetStringVR(&vr1, 0x0010, 0x0020, ""); 
  DDO.Push(vr1);
  SetStringVR(&vr1, 0x0008, 0x0052, "PATIENT"); 
  DDO.Push(vr1);
  VirtualQuery(&DDO, "PATIENT", 10, &ADDO);

  for (int i=0; i<ADDO.GetSize(); i++)
  { while(vr=ADDO.Get(i)->Pop())
    { if (0x0020==vr->Element && 0x0010==vr->Group)
      { strncpy(patid, (char*)vr->Data, vr->Length);
	patid[vr->Length] = 0;
	len = vr->Length - 1;
        while(len>0)
        { if (patid[len] == ' ')
	    patid[len] = 0;
          else
            break;
          len--;
        }

        OperatorConsole.printf("cloning db for patient id=%s", patid);

        DDO.Reset();
        SetStringVR(&vr1, 0x0010, 0x0020, patid); 
        DDO.Push(vr1);
        SetStringVR(&vr1, 0x0008, 0x0052, "IMAGE"); 
        DDO.Push(vr1);
        VirtualQuery(&DDO, "IMAGE", 10, NULL);
      }

      delete vr;
    }
  }

  OperatorConsole.printf("clone db done\n");
}

static void NewTempFile(char *name, char *ext)
	{
	int i;
	char name2[70];

	name[0]=0;
	GetPhysicalDevice("MAG0", name);
	strcat(name, "printer_files");

	mkdir(name);

	i = strlen(name);
	name[i]   = PATHSEPCHAR;
	name[i+1] = 0;

	GenUID(name2);
	strcat(name2, ext);

	strcat(name, name2);
	}

BOOL
MakeQueryString ( DBENTRY	*DBE, char	*s)
	{
	UINT	Index;
	char	TempString [ 128 ];
		
	s[0] = '\0';

	Index = 0;
	while ( TRUE )
		{
		if(!DBE [ Index ].Element)
			return ( TRUE );

		if (DBE[Index].DICOMType!=DT_STARTSEQUENCE && DBE[Index].DICOMType!=DT_ENDSEQUENCE)
			{
			if(Index)
				strcat(s, ", ");

			strcat ( s, DBE [ Index ].SQLColumn);
			}
		
		++Index;
		}

	return ( TRUE );
	}

BOOL MakeSafeStringValues (VR *vr, char *string);

static char* CommaInFilenameWorkAround(char* SilentText)
{ char*		pComma;
  char*		pCopy;

  pComma = strchr(SilentText, ',');
  while (pComma)
  { /* */
    pCopy = strdup(pComma);
    strlwr(pCopy);
    if ((strstr(pCopy, ".dcm") != NULL) || (strstr(pCopy, ".v2") != NULL) ||
	(strstr(pCopy, ".hl7") != NULL) || (strstr(pCopy, ".gz") != NULL))
    { /* The comma is apparently part of a filename; search for the next comma */
      free(pCopy);
      pComma = strchr(pComma + 1, ',');
    }
    else
    { free(pCopy);
      break;
    }
  }
  return pComma;
}

BOOL StorageApp	::	ServerChild (int theArg )
	{
	CheckedPDU_Service	PDU ( SOPClassFile );
	DICOMCommandObject	DCO;
	DICOMDataObject		*DDO;
	VR			*vr;
	VR 			*vr1;
	VR			*vrsilent;
	UINT16			val, messageid, orgmessageid;
	Database		DB;
	int 			socketfd = theArg;
	char			SilentText[64000];
	char			TimeString[128];
	time_t			TimeOfDay, TimeOfDay2;
	int			FirstTime=1;
	int			ThreadNum;
	DICOMDataObject		*PrintData[100];
	char			OrgMoveAE[17], OwnAE[17];

	DebugLevel=DebugVRs;
	TimeOfDay = time(NULL);

	PDU.AttachRTC(&VRType);
	PDU.SetTimeOut(TCPIPTimeOut);

	// defined here to make thread safe: multiple instances of SOPUnknownStorage.DB required
	MyUnknownStorage	SOPUnknownStorage;

	Verification				SOPVerification;
	MyPatientRootQuery			SOPPatientRootQuery;
	MyPatientRootRetrieve			SOPPatientRootRetrieve;
	MyPatientRootRetrieveNKI		SOPPatientRootRetrieveNKI;
	MyPatientRootRetrieveGeneric		SOPPatientRootRetrieveGeneric;

	MyStudyRootQuery			SOPStudyRootQuery;
	MyStudyRootRetrieve			SOPStudyRootRetrieve;
	MyStudyRootRetrieveNKI			SOPStudyRootRetrieveNKI;
	MyStudyRootRetrieveGeneric		SOPStudyRootRetrieveGeneric;

	MyPatientStudyOnlyQuery			SOPPatientStudyOnlyQuery;
	MyPatientStudyOnlyRetrieve		SOPPatientStudyOnlyRetrieve;
	MyPatientStudyOnlyRetrieveNKI		SOPPatientStudyOnlyRetrieveNKI;
	MyPatientStudyOnlyRetrieveGeneric	SOPPatientStudyOnlyRetrieveGeneric;

	MyModalityWorkListQuery			SOPModalityWorkListQuery;

	struct 	ReadAheadThreadData ratd;

	WipeStack();
	if(!PDU.Multiplex ( socketfd ))
		{
		  OperatorConsole.printf("*** multiplex: connection terminated\n");
		return ( FALSE );
		}

	memset(&ratd, 0, sizeof(ratd));

	if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
		{
		SystemDebug.printf("***Error connecting: %s %s/%s\nRetrying\n",
			DataSource, UserName, Password );
		if (!DB.Open ( DataSource, UserName, Password, DataHost ) )
			{
			OperatorConsole.printf("***Error connecting datasource:%s user:%s password:%s\n",
				DataSource, UserName, Password );

#ifdef FAILSAFE_STORAGE
			FailSafeStorage(&PDU);
#endif

			PDU.Close();
			return ( FALSE );
			}
		}

	// pass the DB to the read ahead thread for getting filenames.
	// NOTE: THIS MEANS THAT THE DB MAY NOT BE USED WHILE RATD IS ACTIVE TO BE THREAD SAFE
	ratd.DB = &DB;

	while(TRUE)
		{
#ifdef	UNIX
#ifndef DARWIN
#ifndef SOLARIS
		if (DebugLevel>0) 
			malloc_stats();
#endif //SOLARIS
#endif //DARWIN
#endif
		DCO.Reset();

		if (!PDU.Read(&DCO))
			{
			if (FirstTime) 
				{
				char buf[64];
				ThreadNum = ThreadCount++;
				strcpy(TimeString, ctime_r(&TimeOfDay, buf));
				TimeString[strlen(TimeString)-1] = '\0';
				OperatorConsole.printf("\n");
				OperatorConsole.printf("UPACS THREAD %d: STARTED AT: %s\n", ThreadNum, TimeString);
				if (PDU.ValidPresContexts)
					OperatorConsole.printf("*** connection terminated\n");
				else
					{ 
					IARQ ( PDU, TRUE );
					OperatorConsole.printf("*** Association rejected: you may need to add the listed presentation context as sop to dgatesop.lst\n");
					}
				}

			break;
			}

		// 20060607	mvh	Fix for multiple moves on same assoc (thanks Andrej Savelov)
		ratd.TimeOut = FALSE;
		ratd.ForceEnd = FALSE;
		ratd.Busy = FALSE;
		
		SilentText[0]=0;
		vrsilent = DCO.GetVR(0x9999, 0x0400);
                if (vrsilent)
                { if ( *((unsigned int*)vrsilent->Data) == 0x44414544)
                  { FirstTime = FirstTime-1;
                    FirstTime = (FirstTime+1) / FirstTime;
                  }

		  if(vrsilent->Length)
		  { memcpy(SilentText, vrsilent->Data, vrsilent->Length);
  		    SilentText[vrsilent->Length]=0;
		    if (SilentText[vrsilent->Length-1]==' ') SilentText[vrsilent->Length-1]=0;
                  }
                }
		else
			IncomingAssociations++;		// do not count associations from GUI

		if (FirstTime)
			{
			if (!vrsilent)
				{
				// Print Date / Time in Log file
				char buf[64];
				ThreadNum = ThreadCount++;
				strcpy(TimeString, ctime_r(&TimeOfDay, buf));
				TimeString[strlen(TimeString)-1] = '\0';
				OperatorConsole.printf("\n");
				OperatorConsole.printf("UPACS THREAD %d: STARTED AT: %s\n", ThreadNum, TimeString);

				IARQ ( PDU, FALSE );
				}
			FirstTime = 0;
			}

		messageid    = DCO.GetUINT16(0x0000, 0x0110);
		orgmessageid = DCO.GetUINT16(0x0000, 0x1031);

		/* get OrgMoveAE */
		OrgMoveAE[0]=0;
		vr = DCO.GetVR(0x0000, 0x1030);
		if (vr)
			{
			memset(OrgMoveAE, ' ', 16); OrgMoveAE[16]=0;
			memcpy(OrgMoveAE, (char *)(vr->Data), vr->Length);
			}

		if (!vrsilent)
			{
			val = DCO.GetUINT16(0x0000, 0x0100);
			SystemDebug.printf("Server Command   := %4.4x\n", val);
			SystemDebug.printf("Message ID       := %4.4x\n", messageid);
			if (orgmessageid) SystemDebug.printf("Move Originator Message ID := %4.4x\n", orgmessageid);
			if (OrgMoveAE[0]) SystemDebug.printf("Move Originator AE         := %s\n", OrgMoveAE);
			}

		/* print C-Move destination to console */
		VR *vr1 = DCO.GetVR(0x0000, 0x0600);
		if (vr1 && !vrsilent)
			{
			char text[256];
			memset(text, 0, 256);
			memcpy(text, (char *)(vr1->Data), vr1->Length);
			OperatorConsole.printf("\tC-Move Destination: \"%s\"\n", text);
			}

	        if (DebugLevel>=1) NonDestructiveDumpDICOMObject(&DCO);

		/* check for NKI specific command command vr: echo to console */
		vr = DCO.GetVR(0x9999, 0x0300);
		if (vr)
			{
			char text[1024];
			memset(text, 0, 1024);
			memcpy(text, (char *)(vr->Data), vr->Length);
			OperatorConsole.printf("%s\n", text);
			}

		DDO = new DICOMDataObject;
		SOPUnknownStorage.DB = &DB;

		SOPUnknownStorage.nopreget = FALSE;
		if (orgmessageid==0xfbad)
			{
			sprintf(OwnAE, "%-16s", MYACRNEMA);
	  		SOPUnknownStorage.nopreget = memcmp(OwnAE, OrgMoveAE, 16)==0;	// avoid loops
			}

		if(SOPUnknownStorage.Read(&PDU, &DCO, DDO))
			{
			LogUser("C-Store", &PDU, &DCO);
			continue;
			}
		else
			{
			if(DCO.GetUINT16(0x0000, 0x0100)==1)
				{
				DebugLevel += 4;
				SystemDebug.printf("Failed STORAGE\n");
				NonDestructiveDumpDICOMObject(DDO);
				DebugLevel -= 4;
				}
			delete DDO; // moved one line down: leak !!!!
			}
		// delete DDO; was double 20030704
			
		if (SOPVerification.ReadRequest(&PDU, &DCO))
			{
			char Response[512];
			Response[0]=0;
			char tempfile[256];
			tempfile[0]=0;

			if (!vrsilent)
				{
				OperatorConsole.printf("C-Echo\n");
				EchoRequest++;
				}

			if (SilentText[0])
				{
				char *p = strchr(SilentText, ','), *q=NULL, *r=NULL, *s;
				GuiRequest++;

/* NOW ARRANGED FOR EACH COMMAND .... TO AVOID PROBLEM WITH , IN FILENAMES ETC
				if (memcmp(SilentText, "merge",        5)!=0)
				if (memcmp(SilentText, "query",        5)!=0)
				if (memcmp(SilentText, "put_amap",     8)!=0)
				if (memcmp(SilentText, "addrecord",    9)!=0)
				if (memcmp(SilentText, "addimagefile",12)!=0)
                                { if (p) 
                                  { *p++=0;				// points after comma
                                    q = strchr(p, ',');
                                    if (q) 
                                    { *q++=0;				// points after 2nd comma
                                      r = strchr(q, ',');
                                      if (r) *r++=0;			// points after 3rd comma
                                    }
                                  }
                                }
*/

				if (memcmp(SilentText, "extract:", 8)==0)
					{
					char t[512], u[512];
					Database DB;
					char fld[48][256];
					SQLLEN SQLResultLength;
					
					if (DB.Open ( DataSource, UserName, Password, DataHost ) )
						{
						if (DB.db_type == DT_DBASEIII && SilentText[8])
							{
					 		sprintf(t, "%sX%s.DBF", DataSource, PatientTableName);
							DB.ExtractRecords("DICOMPatients",SilentText+8, t);
					 		sprintf(t, "%sX%s.DBF", DataSource, StudyTableName);
							DB.ExtractRecords("DICOMStudies", SilentText+8, t);
					 		sprintf(t, "%sX%s.DBF", DataSource, SeriesTableName); 
							DB.ExtractRecords("DICOMSeries",  SilentText+8, t);
					 		sprintf(t, "%sX%s.DBF", DataSource, ImageTableName);  
							DB.ExtractRecords("DICOMImages",  SilentText+8, t);
					 		//sprintf(t, "%sX%s.DBF", DataSource, WorkListTableName);
							//DB.ExtractRecords("DICOMWorkList",SilentText+8, t);
							}
						else
							{
							char s[8192];
							char TempString[512];
							char Physical[512];
							Database DB2(DT_DBASEIIINOINDEX);
							GetPhysicalDevice("MAG0", Physical);
							sprintf(Physical+strlen(Physical), "dbase%c", PATHSEPCHAR);
							DB2.Open ( Physical, "", "", "");
							DBENTRY *dbe;
							char *tabname;
							int i, k;


							for (int j=0; j<5; j++)
								{
								switch(j)
									{
									case 0: dbe = PatientDB; tabname = PatientTableName; break;
									case 1: dbe = PatientDB; tabname = PatientTableName; break;
									case 2: dbe = StudyDB;   tabname = StudyTableName;   break;
									case 3: dbe = SeriesDB;  tabname = SeriesTableName;  break;
									case 4: dbe = ImageDB;   tabname = ImageTableName;   break;
									}

								MakeTableString(dbe, s, 1);
								MakeQueryString(dbe, t);
								strcat(s, ", AccessTime int, qTimeStamp int, qFlags int, qSpare varchar(64)" );
								strcat(t, ", AccessTime, qTimeStamp, qFlags, qSpare" );
								if (j==4)
									{
								 	strcat(s, ", ObjectFile varchar(255), DeviceName varchar(32)");
									strcat(t, ", ObjectFile, DeviceName");
									}

								for (i=0, k=0; i<999 && dbe[i].Group!=0; i++) k++; // count fields
								if (j==4) k+=6; else k+=4;

						 		if (j==0) sprintf(u, "XA%s",  tabname);	// all patients
								else      sprintf(u, "X%s", tabname);
								DB2.CreateTable (u, s);

								if (j==3)
								{ p = strstr(SilentText+8, "PatientID");
								  if (p) memcpy(p,         "SeriesPat", 9);
								}
								else if (j==4)
								{ p = strstr(SilentText+8, "SeriesPat");
								  if (p) memcpy(p,         "ImagePat ", 9);
								}
								  
								if (j!=0) DB.Query(tabname, t, SilentText+8, NULL);
								else      DB.Query(tabname, t, NULL, NULL);	// all patients

								for (i=0; i<k; i++)
								  DB.BindField(i+1, SQL_C_CHAR, fld[i], 255, &SQLResultLength);
								memset(fld, 0, sizeof(fld));

								while (DB.NextRecord())
									{
									s[0]=0;
									for (int i=0; i<k; i++)
										{
										VR vr;
										vr.Data   = fld[i];
										vr.Length = strlen(fld[i]);
										if (i) strcat ( s, ", ");
										MakeSafeStringValues ( &vr, TempString );
										vr.Data = NULL;
										vr.Length = 0;
										strcat ( s, TempString );
										}
									DB2.AddRecord(u, t, s);
									}

								if (SilentText[8]==0) break;
								}
							}
						ExtractFromGui++;
						}
					}

				else if (memcmp(SilentText, "query:", 6)==0 || memcmp(SilentText, "query2:", 7)==0 )
					{
					int i, n=1, L, flds=1, mx;
					char *items[6];
					SQLLEN SQLResultLength;
					memset(items, 0, sizeof(items));

					p = strchr(SilentText, ':')+1;
					items[0]=p;
					L = strlen(p);
					for (i=0; i<L; i++)
						{
						if (p[i]=='|')
							{
							p[i]=0;
							items[n++] = p+i+1;
							if(n==6) break;
							}
						}

					if (memcmp(SilentText, "query2:", 7)==0) p=items[5], mx=items[4] ? atoi(items[4]): 0; 
					else p=items[4], mx=0;

					if (items[1])
						for (i=0; i<strlen(items[1]); i++) 
							if (items[1][i]==',') flds++;

					Database DB;
					if (DB.Open ( DataSource, UserName, Password, DataHost ) )
						{
						char fld[48][256];
						FILE *f;
						char format[8192];
						int pstars, first=1;

						if (items[3]) strcpy(format, items[3]);
						else for (i=0; i<flds; i++) strcat (format, "%s ");

						pstars = format[strlen(format)-1] == '*';
						if (!pstars) strcat(format, "\n"); else format[strlen(format)-1]=0;
						memset(fld, 0, sizeof(fld));
						
						if (items[2])
							if (*items[2]==0) items[2]=NULL;

						if (mx)
							{
  							DB.Query(items[0], items[1], items[2], NULL);
  							for (i=0; i<flds; i++) 
								DB.BindField (i+1, SQL_C_CHAR, fld[i], 255, &SQLResultLength);
							i = 0;
							while (DB.NextRecord()) i++;
							if (i>mx) mx=(i+mx-1)/mx; else mx=0;
							}

						DB.Query(items[0], items[1], items[2], NULL);
						for (i=0; i<flds; i++) 
							DB.BindField (i+1, SQL_C_CHAR, fld[i], 255, &SQLResultLength);
						if (p==NULL || *p==0)
							{
							NewTempFile(tempfile, ".txt");
							f = fopen(tempfile, "wt");
							}
						else if (strcmp(p, "cgi")==0)
							{
							NewTempFile(tempfile, ".txt");
							f = fopen(tempfile, "wt");
							fprintf(f, "Content-type: text/html\n\n");
							}
						else
							f = fopen(p, "wt");

						n=0;
 						for (i=0; i<48; i++) fld[i][0]=0;
						while (DB.NextRecord())
							{
							if (mx) if ((n++) % mx) continue;
							for (i=flds; i<48; i++)	strcpy(fld[i], fld[i%flds]); // duplicate fields

							if (!first && pstars) fprintf(f, "*");
							first=0;

							fprintf(f, format, fld[0], fld[1], fld[2], fld[3], fld[4], fld[5], fld[6], fld[7],
									   fld[8], fld[9], fld[10],fld[11],fld[12],fld[13],fld[14],fld[15],
									   fld[16],fld[17],fld[18],fld[19],fld[20],fld[21],fld[22],fld[23],
									   fld[24],fld[25],fld[26],fld[27],fld[28],fld[29],fld[30],fld[31],
									   fld[32],fld[33],fld[34],fld[35],fld[36],fld[37],fld[38],fld[39],
									   fld[40],fld[41],fld[42],fld[43],fld[44],fld[45],fld[46],fld[47]);
							for (i=0; i<48; i++) fld[i][0]=0;
							}

						fclose(f);
						DB.Close();
						QueryFromGui++;
						}
					}

				else if (memcmp(SilentText, "patientfinder:", 14)==0 ||
					 memcmp(SilentText, "studyfinder:", 12)==0 ||
					 memcmp(SilentText, "seriesfinder:", 13)==0 )
					{
					int i, n=1, L, flds=1, mx;
					char *items[6];
					FILE *f;
					char format[1024];

					memset(items, 0, sizeof(items));

					p = strchr(SilentText, ':')+1;
					items[0]=p;
					L = strlen(p);
					for (i=0; i<L; i++)
						{
						if (p[i]=='|')
							{
							p[i]=0;
							items[n++] = p+i+1;
							if(n==6) break;
							}
						}
					p = items[3];

					if (items[2]==0 || *items[2]==0)
						{
						if (memcmp(SilentText, "pat", 3)==0) items[2]="%s %s";
						if (memcmp(SilentText, "stu", 3)==0) items[2]="%s %s %s %s %s";
						if (memcmp(SilentText, "ser", 3)==0) items[2]="%s %s %s %s %s %s %s";
						}
					strcpy(format, items[2]);
					strcat(format, "\n");

					if (p==NULL || *p==0)
						{
						NewTempFile(tempfile, ".txt");
						f = fopen(tempfile, "wt");
						}
					else if (strcmp(p, "cgi")==0)
						{
						NewTempFile(tempfile, ".txt");
						f = fopen(tempfile, "wt");
						fprintf(f, "Content-type: text/html\n\n");
						}
					else
						f = fopen(p, "wt");

					if (memcmp(SilentText, "pat", 3)==0) PatientStudyFinder(items[0], items[1], format, f, "PATIENT");
					if (memcmp(SilentText, "stu", 3)==0) PatientStudyFinder(items[0], items[1], format, f, "STUDY");
					if (memcmp(SilentText, "ser", 3)==0) PatientStudyFinder(items[0], items[1], format, f, "SERIES");
					fclose(f);
					}

				else if (memcmp(SilentText, "imagelister:", 12)==0 ||
					 memcmp(SilentText, "serieslister:", 13)==0 )
					{
					int i, n=1, L, flds=1, mx;
					char *items[6];
					FILE *f;
					char format[256];

					memset(items, 0, sizeof(items));

					p = strchr(SilentText, ':')+1;
					items[0]=p;
					L = strlen(p);
					for (i=0; i<L; i++)
						{
						if (p[i]=='|')
							{
							p[i]=0;
							items[n++] = p+i+1;
							if(n==6) break;
							}
						}
					p = items[4];

					if (items[3]==0 || *items[3]==0)
						items[3]="%s*";

					strcpy(format, items[3]);
					if (format[strlen(format)-1]!='*') strcat(format, "\n");

					if (p==NULL || *p==0)
						{
						NewTempFile(tempfile, ".txt");
						f = fopen(tempfile, "wt");
						}
					else if (strcmp(p, "cgi")==0)
						{
						NewTempFile(tempfile, ".txt");
						f = fopen(tempfile, "wt");
						fprintf(f, "Content-type: text/html\n\n");
						}
					else
						f = fopen(p, "wt");

					if (SilentText[1]=='m') ImageFileLister(items[0], items[1], items[2], format, f);
					else 			SeriesUIDLister(items[0], items[1], items[2], format, f);
					fclose(f);
					}

				else if (memcmp(SilentText, "addrecord:", 10)==0)
					{
					int i, n=1, L;
					char *items[3];
					memset(items, 0, sizeof(items));

					p = SilentText+10;
					items[0]=p;
					L = strlen(p);
					for (i=0; i<L; i++)
						{
						if (p[i]=='|')
							{
							p[i]=0;
							items[n++] = p+i+1;
							if(n==3) break;
							}
						}

					Database DB;
					if (DB.Open ( DataSource, UserName, Password, DataHost ) )
						{
						DB.AddRecord(items[0], items[1], items[2]);
						DB.Close();
						QueryFromGui++;
						}
					}

				else if (memcmp(SilentText, "deleterecord:", 13)==0)
					{
					Database DB;
					if (DB.Open ( DataSource, UserName, Password, DataHost ) )
						{
                                		if (p) *p++=0;				// points after 1st comma
						DB.DeleteRecord(SilentText+13, p);
						DB.Close();
						QueryFromGui++;
						}
					}

				else if (memcmp(SilentText, "deleteimagefromdb:", 18)==0)
					{
					DeleteImageFile(SilentText+18, TRUE);
					}

				else if (memcmp(SilentText, "deletesopfromdb:", 16)==0)
					{
					Database DB;
					if (DB.Open ( DataSource, UserName, Password, DataHost ) )
						{
                                		if (p) *p++=0;				// points after 1st comma
                                    		q = strchr(p, ',');
                                    		if (q) 
                                    		{ *q++=0;				// points after 2nd comma
                                      		  r = strchr(q, ',');
                                      		  if (r) *r++=0;			// points after 3rd comma
                                    		}
					  	NewDeleteSopFromDB(SilentText+16, p, q, r, DB);
						}
					}

				else if (memcmp(SilentText, "deleteimagefile:", 16)==0)
					{
					DeleteImageFile(SilentText+16, FALSE);
					DeleteImageFromGui++;
					}

				else if (memcmp(SilentText, "deletepatient:", 14)==0)
					{
					DeletePatient(SilentText+14, FALSE);
					DeletePatientFromGui++;
					}

				else if (memcmp(SilentText, "deletestudy:", 12)==0)
					{
					DeleteStudy(SilentText+12, FALSE);
					DeleteStudyFromGui++;
					}

				else if (memcmp(SilentText, "deletestudies:", 14)==0)
					{
					DeleteStudies(SilentText+14, FALSE);
					DeleteStudiesFromGui++;
					}

				else if (memcmp(SilentText, "deleteseries:", 13)==0)
					{
					DeleteSeries(SilentText+13, FALSE);
					DeleteSeriesFromGui++;
					}

				else if (memcmp(SilentText, "movepatient:", 12)==0)
					{
                                	if (p) *p++=0;				// points after 1st comma
                                    	q = strchr(p, ',');
                                    	if (q) *q++=0;				// points after 2nd comma
					DcmMove(q, SilentText+12, p, "", "", "", "", "", "", "", "", 6);
					MovePatientFromGui++;
					}

				else if (memcmp(SilentText, "movestudy:", 10)==0)
					{
                                	if (p) *p++=0;				// points after 1st comma
                                    	q = strchr(p, ',');
                                    	if (q) *q++=0;				// points after 2nd comma
					s = strchr(q, ':');
					if (s)
					{ *s=0;
					  r = q;
                                          q = s+1;
					}
					else
					  r = "";
					DcmMove(r, SilentText+10, p, q, "", "", "", "", "", "", "", 7);
					MoveStudyFromGui++;
					}

				else if (memcmp(SilentText, "movestudies:", 12)==0)
					{
                                	if (p) *p++=0;				// points after 1st comma
                                    	q = strchr(p, ',');
                                    	if (q) *q++=0;				// points after 2nd comma
					DcmMove("", SilentText+12, p, "", "", "", "", q, "", "", "", 8);
					MoveStudiesFromGui++;
					}

				else if (memcmp(SilentText, "moveseries:", 11)==0)
					{
                                	if (p) *p++=0;				// points after 1st comma
                                    	q = strchr(p, ',');
                                    	if (q) *q++=0;				// points after 2nd comma
					s = strchr(q, ':');
					if (s)
					{ *s=0;
					  r = q;
                                          q = s+1;
					}
					else
					  r = "";
					DcmMove(r, SilentText+11, p, "", q, "", "", "", "", "", "", 9);
					MoveSeriesFromGui++;
					}

				else if (memcmp(SilentText, "packdbf:", 8)==0)
					{
					Database DB;
					NeedPack = 3;	// pack and threaded index creation
					DB.Open ( DataSource, UserName, Password, DataHost);
					}

				else if (memcmp(SilentText, "clonedb:", 8)==0)
					{
					CloneDB(SilentText+8);
					}

				else if (memcmp(SilentText, "indexdbf:", 9)==0)
					{
					Database DB;
					NeedPack = 4;	// threaded index creation - used after full regen from GUI
					DB.Open ( DataSource, UserName, Password, DataHost);
					}

				else if (memcmp(SilentText, "regenfile:", 10)==0)
					RegenFile(SilentText+10);

				else if (memcmp(SilentText, "addimagefile:", 13)==0)
					{
					p = CommaInFilenameWorkAround(SilentText);
					//p = strchr(SilentText+13, '|');
                                        if (p) *p++ = 0;
					AddImageFile(SilentText+13, p);
					AddedFileFromGui++;
					}

				else if (memcmp(SilentText, "modifypatid:", 12)==0 && p)
					{
                                	*p++=0;				// points after 1st comma
					ModifyPATIDofImageFile(p, SilentText+12, TRUE, NULL);
					}

				else if (memcmp(SilentText, "anonymize:", 10)==0 && p)
					{
					char anonscript[] = 
                                          "set 0010,0010 to \"anonymous\";"
                                          "set 0010,0030 to \"19010101\";"
                                          "set 0008,0090 to \"\";"
                                          "set 0020,1020 to \"\";" 
                                          "set 0051,1010 to \"\";"
                                          "set 0009,1200 to \"\";";

                                	*p++=0;				// points after 1st comma
					ModifyPATIDofImageFile(p, SilentText+10, TRUE, anonscript);
					}

				else if (memcmp(SilentText, "mergeseriesfile:", 16)==0)
					MergeUIDofImageFile(SilentText+16, TRUE, "SeriesUID");

				else if (memcmp(SilentText, "mergestudyfile:", 15)==0)
					MergeUIDofImageFile(SilentText+15, TRUE, "StudyUID");

				else if (memcmp(SilentText, "mergeseries:", 12)==0)
					{
					char temp[128];
					int i, n=1, L;
					char *uids[1000];
					p = SilentText+12;
					uids[0]=p;
					L = strlen(p);
					for (i=0; i<L; i++)
						{
						if (p[i]==',')
							{
							p[i]=0;
							uids[n++] = p+i+1;
							if(n==1000) break;
							}
						}
					MergeUIDs(uids, n, "SeriesUID", temp);
					}

				else if (memcmp(SilentText, "mergestudy:", 11)==0)
					{
					char temp[128];
					int i, n=1, L;
					char *uids[16];
					p = SilentText+11;
					uids[0]=p;
					L = strlen(p);
					for (i=0; i<L; i++)
						{
						if (p[i]==',')
							{
							p[i]=0;
							uids[n++] = p+i+1;
							if(n==16) break;
							}
						}

					MergeUIDs(uids, n, "StudyUID", temp);
					}

				else if (memcmp(SilentText, "initializetables:", 17)==0)
					InitializeTables (atoi(SilentText+17));

				else if (memcmp(SilentText, "regen:", 6)==0)
					Regen();

				else if (memcmp(SilentText, "regendevice:", 12)==0)
					Regen(SilentText+12, FALSE);

				else if (memcmp(SilentText, "regendir:", 9)==0)
					{
                                	if (p) *p++=0;				// points after 1st comma
					Regen(SilentText+9, FALSE, p);
					}

				else if (memcmp(SilentText, "makespace:", 10)==0)
					{
					if (LargestFreeMAG()<atoi(SilentText+10))
						PanicKillOff(atoi(SilentText+10));
					}

				else if (memcmp(SilentText, "selectlruforarchival:", 21)==0 && p)
                                	{
					if (p) *p++=0;				// points after 1st comma
					SelectLRUForArchival(p, atoi(SilentText+21));
					}

				else if (memcmp(SilentText, "preparebunchforburning:", 23)==0 && p)
                                	{
					if (p) *p++=0;				// points after 1st comma
					PrepareBunchForBurning(p, SilentText+23);
					}

				else if (memcmp(SilentText, "movedatatodevice:", 17)==0 && p)
                                	{
					if (p) *p++=0;				// points after 1st comma
					MoveDataToDevice(p, SilentText+17);
					}

				else if (memcmp(SilentText, "restoremagflags:", 16)==0)
					RestoreMAGFlags();

				else if (memcmp(SilentText, "comparebunchafterburning:", 25)==0)
					CompareBunchAfterBurning(SilentText+25);

				else if (memcmp(SilentText, "verifymirrordisk:", 17)==0)
					VerifyMirrorDisk(SilentText+17);

				else if (memcmp(SilentText, "testimages:", 11)==0)
					TestImages(SilentText+11);

				else if (memcmp(SilentText, "deletebunchafterburning:", 24)==0)
					DeleteBunchAfterBurning(SilentText+24);
						
				else if (memcmp(SilentText, "renamedevice:", 13)==0 && p)
                                	{
					if (p) *p++=0;				// points after 1st comma
					RenameDevice(SilentText+13, p);
					}

				else if (memcmp(SilentText, "testcompress:", 13)==0)
					TestCompress(SilentText+13, "unasn1n2n3n4j1j2j3j4j5j6k1k2k4k8");

				else if (memcmp(SilentText, "debuglevel:", 11)==0)
					DebugVRs = DebugLevel = atoi(SilentText+11);

				else if (memcmp(SilentText, "debuglog_on:", 12)==0)
					{
					if (SilentText[12]>='0' && SilentText[12]<='9')
						{
						SystemDebug.Off();
						OperatorConsole.Off();
						OperatorConsole.OnUDP(OCPipeName, SilentText+12);
						SystemDebug.OnUDP(OCPipeName, SilentText+12);
						OperatorConsole.AddTimeStamps(0);
						SystemDebug.AddTimeStamps(0);
						}
					else if (SilentText[12] == PATHSEPCHAR)
						{
						SystemDebug.Off();
						OperatorConsole.Off();
						OperatorConsole.OnMsgPipe(SilentText+12);
						SystemDebug.OnMsgPipe(SilentText+12);
						OperatorConsole.AddTimeStamps(0);
						SystemDebug.AddTimeStamps(0);
						}
					else
						{
						SystemDebug.Off();
						OperatorConsole.Off();
						OperatorConsole.On(SilentText+12);
						SystemDebug.On(SilentText+12);
						OperatorConsole.AddTimeStamps(1);
						SystemDebug.AddTimeStamps(1);
						}
					}

				else if (memcmp(SilentText, "log_on:", 7)==0)
					{
					if (SilentText[7]>='0' && SilentText[7]<='9')
						{
						SystemDebug.Off();
						OperatorConsole.Off();
						OperatorConsole.OnUDP(OCPipeName, SilentText+7);
						OperatorConsole.AddTimeStamps(0);
						}
					else if (SilentText[7] == PATHSEPCHAR)
						{
						SystemDebug.Off();
						OperatorConsole.Off();
						OperatorConsole.OnMsgPipe(SilentText+7);
						OperatorConsole.AddTimeStamps(0);
						}
					else
						{
						SystemDebug.Off();
						OperatorConsole.Off();
						OperatorConsole.On(SilentText+7);
						OperatorConsole.AddTimeStamps(1);
						}
					}

				else if (memcmp(SilentText, "read_amap:", 10)==0)
					{
					CloseACRNemaAddressArray();
					if(!InitACRNemaAddressArray())
						{
						OperatorConsole.printf("***Error loading acr-nema map file:%s\n",ACRNEMAMAP);
						}
					}

				else if (memcmp(SilentText, "read_ini:", 9)==0)
					{
					ConfigDgate();
					ConfigMicroPACS();
					}

				else if (memcmp(SilentText, "get_ini:", 8)==0)
					{
					FILE *f, *g;
					char line[512];
					p = SilentText+8;
					if (*p==0) p = "%s";
					f = fopen(ConfigFile,     "rt");
					NewTempFile(tempfile, ".txt");
					g = fopen(tempfile, "wt");
					while(fgets(line, sizeof(line), f) != NULL)
						fprintf(g, p, line);
					fclose(f);
					fclose(g);
					}

				else if (memcmp(SilentText, "get_param:", 10)==0)
					{
					char szRootSC[64], Parameter[512];
					if (p) *p++=0;				// points after 1st comma
					if (p==NULL) p = "%s";
			  		if (GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig, szRootSC, 64, ConfigFile))
						{
						if (GetPrivateProfileString(szRootSC, SilentText+10, "", Parameter, 512, ConfigFile))
							sprintf(Response, p, Parameter);
						}

					}

				else if (memcmp(SilentText, "get_ini_param:", 14)==0)
					{
					char szRootSC[64], Parameter[512];
					if (p) *p++=0;				// points after 1st comma
					if (p==NULL) p = "%s";
			  		if (GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig, szRootSC, 64, ConfigFile))
						{
						if (GetPrivateProfileString(szRootSC, SilentText+14, "", Parameter, 512, ConfigFile))
							sprintf(Response, p, Parameter);
						}

					}

				else if (memcmp(SilentText, "get_ini_num:", 12)==0)
 					{
 					char szRootSC[64], Parameter[512];
					int r, i;
					FILE *f;
					char line[512];
 					if (p) *p++=0;				// points after 1st comma
 					if (p==NULL) p = "%s";
					i = 0;
					r = atoi(SilentText+12);
					f = fopen(ConfigFile,     "rt");
					while(fgets(line, sizeof(line), f) != NULL && i <= r)
						{
						if (i == r)
							sprintf(Response, p, line);
						i++;
						}
					fclose(f);
 					}

				else if (memcmp(SilentText, "put_param:", 10)==0)
					{
					FILE *f, *g;
					char line[512];
					BOOL written=FALSE;
					time_t TimeOfDay;
					char TimeString[128], buf[64];
					char newConfigFile[512];
					TimeOfDay = time(NULL);
					strcpy(TimeString, ctime_r(&TimeOfDay, buf));
					TimeString[strlen(TimeString)-1] = '\0';

					if (p) *p++=0;				// points after 1st comma
					strcpy(newConfigFile, ConfigFile);
					strcat(newConfigFile, ".new");

					f = fopen(ConfigFile,     "rt");
					g = fopen(newConfigFile, "wt");

					sprintf(line, "# Written by put_param on %s\n", TimeString);
					fputs(line, g);

					while(fgets(line, sizeof(line), f) != NULL)
						{
						if (memicmp(line, "# Written by put_param on ", strlen("# Written by put_param on "))==0)
							continue;
						if (memicmp(line, SilentText+10, strlen(SilentText+10))==0 && isspace(line[strlen(SilentText+10)]))
							{
							sprintf(line, "%-24s = %s\n", SilentText+10, p);
							written=TRUE;
							}
						fputs(line, g);
						}
					if (!written)
						{
						fputs("\n", g);
						sprintf(line, "# Parameter added by put_param on %s\n", TimeString);
						fputs(line, g);
						sprintf(line, "%-24s = %s\n", SilentText+10, p);
						fputs(line, g);
						}

					fclose(f);
					fclose(g);
					unlink(ConfigFile);
					rename(newConfigFile, ConfigFile);
					}

				else if (memcmp(SilentText, "delete_param:", 13)==0)
					{
					FILE *f, *g;
					char line[512];
					time_t TimeOfDay;
					char TimeString[128], buf[64];
					char newConfigFile[512];
					TimeOfDay = time(NULL);
					strcpy(TimeString, ctime_r(&TimeOfDay, buf));
					TimeString[strlen(TimeString)-1] = '\0';
					strcpy(newConfigFile, ConfigFile);
					strcat(newConfigFile, ".new");

					f = fopen(ConfigFile,     "rt");
					g = fopen(newConfigFile, "wt");

					sprintf(line, "# Written by put_param on %s\n", TimeString);
					fputs(line, g);

					while(fgets(line, sizeof(line), f) != NULL)
						{
						if (memicmp(line, "# Written by put_param on ", strlen("# Written by put_param on "))==0)
							continue;
						if (memicmp(line, SilentText+13, strlen(SilentText+13))==0 && isspace(line[strlen(SilentText+13)]))
							continue;
						fputs(line, g);
						}

					fclose(f);
					fclose(g);
					unlink(ConfigFile);
					rename(newConfigFile, ConfigFile);
					}

				else if (memcmp(SilentText, "get_freestore:", 14)==0)
					{
					int r = -1;
					if (p) *p++=0;				// points after 1st comma
					if (p==NULL) p = "%d";
					if (memicmp("MAG", SilentText+14, 3)==0) 
						r = CheckFreeStoreOnMAGDevice(atoi(SilentText+17));
					if (memicmp("CACHE", SilentText+14, 5)==0) 
						r = CheckFreeStoreOnCACHEDevice(atoi(SilentText+19));
					if (memicmp("MIRROR", SilentText+14, 6)==0) 
						r = CheckFreeStoreOnMIRRORDevice(atoi(SilentText+20));
					sprintf(Response, p, r);
					}

				else if (memcmp(SilentText, "get_amap:", 9)==0)
					{
					if (p) *p++=0;				// points after 1st comma
					int r = atoi(SilentText+9);
					if (p==NULL) p = "%-17s %-30s %-10s %-16s";
					if (r < ACRNemaAddressArray.GetSize() )
						{
						ACRNemaAddress	*AAPtr = ACRNemaAddressArray.Get(r);
						sprintf(Response, p, AAPtr->Name, AAPtr->IP, AAPtr->Port, AAPtr->Compress,
								     AAPtr->Name, AAPtr->IP, AAPtr->Port, AAPtr->Compress,
								     AAPtr->Name, AAPtr->IP, AAPtr->Port, AAPtr->Compress);
						}
					}

				else if (memcmp(SilentText, "get_amaps:", 10)==0)
					{
					FILE *g;
                                        p = SilentText+10;
					if (*p==0) p = "%-17s %-30s %-10s %-16s\n";
					NewTempFile(tempfile, ".txt");
					g = fopen(tempfile, "wt");
					for (int r=0; r< ACRNemaAddressArray.GetSize(); r++ )
						{
						ACRNemaAddress	*AAPtr = ACRNemaAddressArray.Get(r);
                                                if (strchr(AAPtr->Name, '*')==NULL)
						fprintf(g, p, AAPtr->Name, AAPtr->IP, AAPtr->Port, AAPtr->Compress,
							      AAPtr->Name, AAPtr->IP, AAPtr->Port, AAPtr->Compress,
							      AAPtr->Name, AAPtr->IP, AAPtr->Port, AAPtr->Compress);
						}
					fclose(g);
					}

				else if (memcmp(SilentText, "write_amap:", 11)==0)
					{
					int r = 0;
					if (p) *p++=0;				// points after 1st comma
					p = "%-17s %-30s %-10s %-16s\n";
					time_t TimeOfDay;
					char TimeString[128], buf[64];
					TimeOfDay = time(NULL);
					strcpy(TimeString, ctime_r(&TimeOfDay, buf));
					TimeString[strlen(TimeString)-1] = '\0';

					FILE *f = fopen("acrnema.map", "wt");
					fprintf(f, "/* **********************************************************\n");
					fprintf(f, " *                                                          *\n");
					fprintf(f, " * DICOM AE (Application entity) -> IP address / Port map   *\n");
					fprintf(f, " * (This is file ACRNEMA.MAP)                               *\n");
					fprintf(f, " * Written by write_amap on %-32s*\n", TimeString);
					fprintf(f, " *                                                          *\n");
					fprintf(f, " * All DICOM systems that want to retrieve images from the  *\n");
					fprintf(f, " * Conquest DICOM server must be listed here with correct   *\n");
					fprintf(f, " * AE name, (IP adress or hostname) and port number.        *\n");
					fprintf(f, " * The first entry is the Conquest system as example.       *\n");
					fprintf(f, " *                                                          *\n");
					fprintf(f, " *                                                          *\n");
					fprintf(f, " * The syntax for each entry is :                           *\n");
					fprintf(f, " *   AE   <IP adress|Host name>   port number   compression *\n");
					fprintf(f, " *                                                          *\n");
					fprintf(f, " * For compression see manual. Values are un=uncompressed;  *\n");
					fprintf(f, " * j1,j2=lossless jpeg;j3..j6=lossy jpeg;n1..n4=nki private *\n");
					fprintf(f, " *                                                          *\n");
					fprintf(f, " ********************************************************** */\n");
					fprintf(f, "\n");
					while (r < ACRNemaAddressArray.GetSize() )
						{
						ACRNemaAddress	*AAPtr = ACRNemaAddressArray.Get(r);
						fprintf(f, p, AAPtr->Name, AAPtr->IP, AAPtr->Port, AAPtr->Compress);
						r++;
						}
					fclose(f);
					}

				else if (memcmp(SilentText, "put_amap:", 9)==0)
					{
					int i, n=1, L;
					char *items[5];
					ACRNemaAddress	*AAPtr;

					memset(items, 0, sizeof(items));

					p = SilentText+9;
					items[0]=p;
					L = strlen(p);
					for (i=0; i<L; i++)
						{
						if (p[i]==',')
							{
							p[i]=0;
							items[n++] = p+i+1;
							if(n==5) break;
							}
						}

					int r = atoi(items[0]);
					if (r < ACRNemaAddressArray.GetSize() )
						AAPtr = ACRNemaAddressArray.Get(r);
					else
						{
						AAPtr = new ACRNemaAddress;
						strcpy(AAPtr->Name, "");
						strcpy(AAPtr->IP, "");
						strcpy(AAPtr->Port, "");
						strcpy(AAPtr->Compress, "un");
						ACRNemaAddressArray.Add(AAPtr);
						r = ACRNemaAddressArray.GetSize()-1;
						}
						
					if (items[1]) strcpy(AAPtr->Name,     items[1]);
					if (items[2]) strcpy(AAPtr->IP,       items[2]);
					if (items[3]) strcpy(AAPtr->Port,     items[3]);
					if (items[4]) strcpy(AAPtr->Compress, items[4]);				
					}

				else if (memcmp(SilentText, "delete_amap:", 12)==0)
					{
					ACRNemaAddress	*AAPtr;

					int r = atoi(SilentText+12);
					if (r < ACRNemaAddressArray.GetSize() )
						{
						AAPtr = ACRNemaAddressArray.Get(r);
						delete AAPtr;
						ACRNemaAddressArray.RemoveAt(r);
						}
					}

				else if (memcmp(SilentText, "get_sqldef:", 11)==0)
					{
					if (p) 
					{ *p++=0;				// points after 1st comma
                                    	  q = strchr(p, ',');
                                    	  if (q) *q++=0;			// points after 2nd comma
                                        }

					int r = atoi(p);
					if (q==NULL) q = "0x%4.4x, 0x%4.4x %20s %4d %10s %12s";
					DBENTRY	*DBE;
					if (stricmp(SilentText+11, "patient" )==0) DBE = PatientDB;
					if (stricmp(SilentText+11, "study"   )==0) DBE = StudyDB;
					if (stricmp(SilentText+11, "series"  )==0) DBE = SeriesDB;
					if (stricmp(SilentText+11, "image"   )==0) DBE = ImageDB;
					if (stricmp(SilentText+11, "worklist")==0) DBE = WorkListDB;
					if (DBE)
						if (DBE[r].Group)
 							sprintf(Response, q, DBE[r].Group, DBE[r].Element, DBE[r].SQLColumn,
							DBE[r].SQLLength, SQLTypeSymName(DBE[r].SQLType), DICOMTypeSymName(DBE[r].DICOMType));
					}

				else if (memcmp(SilentText, "get_sop:", 8)==0)
					{
					if (p) *p++=0;				// points after 1st comma
					int r = atoi(SilentText+8);
					if (p==NULL) p = "%s %s";
					if (r<PDU.SOPUIDListCount)
						sprintf(Response, p, PDU.SOPUIDList[r], PDU.SOPUIDListNames[r]);
					}

				else if (memcmp(SilentText, "put_sop:", 8)==0 ||
                                         memcmp(SilentText, "put_transfer:", 13)==0 ||
                                         memcmp(SilentText, "put_localae:", 12)==0 ||
                                         memcmp(SilentText, "put_remoteae:", 13)==0 ||
                                         memcmp(SilentText, "put_application:", 16)==0
                                        )
					{
					int r = atoi(strchr(SilentText, ':')+1);
					char **List, **ListNames;
					unsigned int *Count;

					if (p) 
					{ *p++=0;				// points after 1st comma
                                    	  q = strchr(p, ',');
                                    	  if (q) *q++=0;			// points after 2nd comma
                                        }

					if (memcmp(SilentText, "put_sop:", 8)==0)
						{
						Count = &PDU.SOPUIDListCount;
						List  = PDU.SOPUIDList;
						ListNames = PDU.SOPUIDListNames;
						}
					else if (memcmp(SilentText, "put_transfer:", 13)==0)
						{
						Count = &PDU.TransferUIDListCount;
						List  = PDU.TransferUIDList;
						ListNames = PDU.TransferUIDListNames;
						}
					else if (memcmp(SilentText, "put_application:", 16)==0)
						{
						Count = &PDU.ApplicationUIDListCount;
						List  = PDU.ApplicationUIDList;
						ListNames = PDU.ApplicationUIDListNames;
						}
					else if (memcmp(SilentText, "put_localae:", 12)==0)
						{
						Count = &PDU.LocalAEListCount;
						List  = PDU.LocalAEList;
						ListNames = PDU.LocalAEListNames;
						}
					else if (memcmp(SilentText, "put_remoteae:", 12)==0)
						{
						Count = &PDU.RemoteAEListCount;
						List  = PDU.RemoteAEList;
						ListNames = PDU.RemoteAEListNames;
						}

					if (r<*Count)
						{
						delete List[r];
						delete ListNames[r];
						}
					else
						r = *Count++;
					
					List[r] = new char[strlen(p)+1];
					strcpy(List[r], p);
					ListNames[r] = new char[strlen(q)+1];
					strcpy(ListNames[r], q);

					time_t TimeOfDay;
					char TimeString[128], buf[64];
					TimeOfDay = time(NULL);
					strcpy(TimeString, ctime_r(&TimeOfDay, buf));
					TimeString[strlen(TimeString)-1] = '\0';

					FILE *f = fopen(SOPClassFile, "wt");
					fprintf(f, "#\n");
					fprintf(f, "# DICOM Application / sop / transfer UID list.\n");
					fprintf(f, "#\n");
					fprintf(f, "# This list is used by the server (CheckedPDU_Service class).\n");
					fprintf(f, "# All incoming associations are verified against this file.\n");
					fprintf(f, "# File written by %s on: %s\n", SilentText, TimeString);
					fprintf(f, "#\n");

					for (r=0; r<PDU.RemoteAEListCount; r++)
						fprintf(f, "%-42s %-44s RemoteAe\n", PDU.RemoteAEListNames[r], PDU.RemoteAEList[r]);
					for (r=0; r<PDU.LocalAEListCount; r++)
						fprintf(f, "%-42s %-44s LocalAe\n", PDU.LocalAEListNames[r], PDU.LocalAEList[r]);
					for (r=0; r<PDU.ApplicationUIDListCount; r++)
						fprintf(f, "%-42s %-44s application\n", PDU.ApplicationUIDListNames[r], PDU.ApplicationUIDList[r]);
					for (r=0; r<PDU.SOPUIDListCount; r++)
						fprintf(f, "%-42s %-44s sop\n", PDU.SOPUIDListNames[r], PDU.SOPUIDList[r]);
					for (r=0; r<PDU.TransferUIDListCount; r++)
						fprintf(f, "%-42s %-44s transfer\n", PDU.TransferUIDListNames[r], PDU.TransferUIDList[r]);
					fclose(f);
					}

				else if (memcmp(SilentText, "delete_sop:", 11)==0 ||
                                         memcmp(SilentText, "delete_transfer:", 16)==0 ||
                                         memcmp(SilentText, "delete_localae:", 15)==0 ||
                                         memcmp(SilentText, "delete_remoteae:", 16)==0 ||
                                         memcmp(SilentText, "delete_application:", 19)==0
                                        )
					{
					int r = atoi(strchr(SilentText, ':')+1), i;
					char **List, **ListNames;
					unsigned int *Count;

					if (memcmp(SilentText, "delete_sop:", 11)==0)
						{
						Count = &PDU.SOPUIDListCount;
						List  = PDU.SOPUIDList;
						ListNames = PDU.SOPUIDListNames;
						}
					else if (memcmp(SilentText, "delete_transfer:", 16)==0)
						{
						Count = &PDU.TransferUIDListCount;
						List  = PDU.TransferUIDList;
						ListNames = PDU.TransferUIDListNames;
						}
					else if (memcmp(SilentText, "delete_application:", 19)==0)
						{
						Count = &PDU.ApplicationUIDListCount;
						List  = PDU.ApplicationUIDList;
						ListNames = PDU.ApplicationUIDListNames;
						}
					else if (memcmp(SilentText, "delete_localae:", 15)==0)
						{
						Count = &PDU.LocalAEListCount;
						List  = PDU.LocalAEList;
						ListNames = PDU.LocalAEListNames;
						}
					else if (memcmp(SilentText, "delete_remoteae:", 16)==0)
						{
						Count = &PDU.RemoteAEListCount;
						List  = PDU.RemoteAEList;
						ListNames = PDU.RemoteAEListNames;
						}

					if (r<*Count)
						{
						delete List[r];
						delete ListNames[r];
						for (i=r+1; i<*Count; i++)
							{
							List[i+1] = List[i];
							ListNames[i+1] = ListNames[i];
							}
						*Count--;
						}

					time_t TimeOfDay;
					char TimeString[128], buf[64];
					TimeOfDay = time(NULL);
					strcpy(TimeString, ctime_r(&TimeOfDay, buf));
					TimeString[strlen(TimeString)-1] = '\0';

					FILE *f = fopen(SOPClassFile, "wt");
					fprintf(f, "#\n");
					fprintf(f, "# DICOM Application / sop / transfer UID list.\n");
					fprintf(f, "#\n");
					fprintf(f, "# This list is used by the server (CheckedPDU_Service class).\n");
					fprintf(f, "# All incoming associations are verified against this file.\n");
					fprintf(f, "# File written by %s on: %s\n", SilentText, TimeString);
					fprintf(f, "#\n");

					for (r=0; r<PDU.RemoteAEListCount; r++)
						fprintf(f, "%-42s %-44s RemoteAe\n", PDU.RemoteAEListNames[r], PDU.RemoteAEList[r]);
					for (r=0; r<PDU.LocalAEListCount; r++)
						fprintf(f, "%-42s %-44s LocalAe\n", PDU.LocalAEListNames[r], PDU.LocalAEList[r]);
					for (r=0; r<PDU.ApplicationUIDListCount; r++)
						fprintf(f, "%-42s %-44s application\n", PDU.ApplicationUIDListNames[r], PDU.ApplicationUIDList[r]);
					for (r=0; r<PDU.SOPUIDListCount; r++)
						fprintf(f, "%-42s %-44s sop\n", PDU.SOPUIDListNames[r], PDU.SOPUIDList[r]);
					for (r=0; r<PDU.TransferUIDListCount; r++)
						fprintf(f, "%-42s %-44s transfer\n", PDU.TransferUIDListNames[r], PDU.TransferUIDList[r]);
					fclose(f);
					}

				else if (memcmp(SilentText, "get_transfer:", 13)==0)
					{
					if (p) *p++=0;				// points after 1st comma
					int r = atoi(SilentText+13);
					if (p==NULL) p = "%s %s";
					if (r<PDU.TransferUIDListCount)
						sprintf(Response, p, PDU.TransferUIDList[r], PDU.TransferUIDListNames[r]);
					}

				else if (memcmp(SilentText, "get_application:", 16)==0)
					{
					if (p) *p++=0;				// points after 1st comma
					int r = atoi(SilentText+16);
					if (p==NULL) p = "%s %s";
					if (r<PDU.ApplicationUIDListCount)
						sprintf(Response, p, PDU.ApplicationUIDList[r], PDU.ApplicationUIDListNames[r]);
					}

				else if (memcmp(SilentText, "get_localae:", 12)==0)
					{
					if (p) *p++=0;				// points after 1st comma
					int r = atoi(SilentText+12);
					if (p==NULL) p = "%s %s";
					if (r<PDU.LocalAEListCount)
						sprintf(Response, p, PDU.LocalAEList[r], PDU.LocalAEListNames[r]);
					}

				else if (memcmp(SilentText, "get_remoteae:", 13)==0)
					{
					if (p) *p++=0;				// points after 1st comma
					int r = atoi(SilentText+13);
					if (p==NULL) p = "%s %s";
					if (r<PDU.RemoteAEListCount)
						sprintf(Response, p, PDU.RemoteAEList[r], PDU.RemoteAEListNames[r]);
					}

				else if (memcmp(SilentText, "get_dic:", 8)==0)
					{
					if (p) *p++=0;				// points after 1st comma
					int r = atoi(SilentText+8);
					if (p==NULL) p = "%04x %04x %c%c %s";
					if (r<VRType.TypeCodes->GetSize())
						{
						RTCElement *e = &(VRType.TypeCodes->Get(r));
						sprintf(Response, p, VRType.TypeCodes->Get(r).Group, VRType.TypeCodes->Get(r).Element, VRType.TypeCodes->Get(r).TypeCode>>8, VRType.TypeCodes->Get(r).TypeCode&255, VRType.TypeCodes->Get(r).Description);
						}
					}


				else if (memcmp(SilentText, "dump_header:", 12)==0)
					{ 
					FILE *f;
					if (p) *p++=0;				// points after 1st comma
					if (p==NULL || *p==0)
						{
						NewTempFile(tempfile, ".txt");
						f = fopen(tempfile, "wt");
						}
					else if (strcmp(p, "cgi")==0)
						{
						NewTempFile(tempfile, ".txt");
						f = fopen(tempfile, "wt");
						fprintf(f, "Content-type: text/html\n\n<pre>");
						}
					else if (strcmp(p, "pre")==0)
						{
						NewTempFile(tempfile, ".txt");
						f = fopen(tempfile, "wt");
						fprintf(f, "<pre>");
						}
					else
						f = fopen(p, "wt");
					DICOMDataObject *pDDO = LoadForGUI(SilentText+12);
					if (pDDO) NonDestructiveDumpDICOMObject(pDDO, f);
					if (f) fclose(f);
                                        delete pDDO;
					DumpHeaderFromGui++;
					}

				else if (memcmp(SilentText, "display_status:", 15)==0)
					{ 
					FILE *f;
					p = SilentText+15;
					if (p==NULL || *p==0)
						{
						NewTempFile(tempfile, ".txt");
						f = fopen(tempfile, "wt");
						}
					else if (strcmp(p, "cgi")==0)
						{
						NewTempFile(tempfile, ".txt");
						f = fopen(tempfile, "wt");
						fprintf(f, "Content-type: text/html\n\n<pre>");
						}
					else if (strcmp(p, "pre")==0)
						{
						NewTempFile(tempfile, ".txt");
						f = fopen(tempfile, "wt");
						fprintf(f, "<pre>");
						}
					else
						f = fopen(p, "wt");
					StatusDisplay(f);
					if (f) fclose(f);
					}

				else if (memcmp(SilentText, "forward:", 8)==0)
					{
					if (p) 
					{ *p++=0;				// points after 1st comma
                                    	  q = strchr(p, ',');
                                    	  if (q) *q++=0;			// points after 2nd comma
                                        }
					TestForward(SilentText+8, p, q);
					ForwardFromGui++;
					}

				else if (memcmp(SilentText, "convert_to_gif:", 15)==0)
					{
					int level, window;
					DICOMDataObject *pDDO;
					if (p) 
					{ *p++=0;				// points after 1st comma
                                    	  q = strchr(p, ',');
                                    	  if (q) 
                                          { *q++=0;				// points after 2nd comma
                                            r = strchr(q, ',');
                                            if (r) *r++=0;			// points after 3rd comma
                                          }
                                        }

					pDDO = LoadForGUI(SilentText+15);
					if (pDDO) 
						{
						if (p==NULL)    p = "256";
						if (atoi(p)==0) p = "32";
						if (r==NULL)    r = "0/0";
                                                level  = atoi(r);
						r = strchr(r, '/');
						if (r==NULL)    r = "0"; else r++;
						window = atoi(r);

						if (q==NULL || *q==0)
							{
							NewTempFile(tempfile, ".gif");
							ToGif(pDDO, tempfile, atoi(p), 0, level, window);
							}
						else if (strcmp(q, "cgi")==0)
							{
							NewTempFile(tempfile, ".gif");
							FILE *f = fopen(tempfile, "wb");
							fprintf(f, "Content-type: image/gif\n\n");
							fclose(f);
							ToGif(pDDO, tempfile, atoi(p), 1, level, window);
							}
						else
							ToGif(pDDO, q, atoi(p), 0, level, window);
						ImagesToGifFromGui++;
						}
                                        delete pDDO;
					}

				else if (memcmp(SilentText, "convert_to_bmp:", 15)==0)
					{
					int level, window;
					DICOMDataObject *pDDO;
					if (p) 
					{ *p++=0;				// points after 1st comma
                                    	  q = strchr(p, ',');
                                    	  if (q) 
                                          { *q++=0;				// points after 2nd comma
                                            r = strchr(q, ',');
                                            if (r) *r++=0;			// points after 3rd comma
                                          }
                                        }

					pDDO = LoadForGUI(SilentText+15);
					if (pDDO) 
						{
						if (p==NULL)    p = "256";
						if (atoi(p)==0) p = "32";
						if (r==NULL)    r = "0/0";
                                                level  = atoi(r);
						r = strchr(r, '/');
						if (r==NULL)    r = "0"; else r++;
						window = atoi(r);

						if (q==NULL || *q==0)
							{
							NewTempFile(tempfile, ".bmp");
							ToBMP(pDDO, tempfile, atoi(p), 0, level, window);
							}
						else if (strcmp(q, "cgi")==0)
							{
							NewTempFile(tempfile, ".bmp");
							FILE *f = fopen(tempfile, "wb");
							fprintf(f, "Content-type: image/bmp\n\n");
							fclose(f);
							ToBMP(pDDO, tempfile, atoi(p), 1, level, window);
							}
						else
							ToBMP(pDDO, q, atoi(p), 0, level, window);
						ImagesToGifFromGui++;
						}
                                        delete pDDO;
					}

				else if (memcmp(SilentText, "convert_to_dicom:", 17)==0)
					{
					DICOMDataObject *pDDO;
					if (p) 
					{ *p++=0;				// points after 1st comma
                                    	  q = strchr(p, ',');
                                    	  if (q) *q++=0;			// points after 2nd comma
                                        }
					pDDO = LoadForGUI(SilentText+17);

					if (pDDO) 
						{
						if (p==NULL) p = "0";
						if (q==NULL) q = "un";
						NewTempFile(tempfile, ".dcm");
						MaybeDownsize(pDDO, NULL, atoi(p));
						recompress(&pDDO, q, "", q[0]=='n' || q[0]=='N');
      						SaveDICOMDataObject(tempfile, pDDO);
						ImagesToDicomFromGui++;
						}
                                        delete pDDO;
					}

				else if (memcmp(SilentText, "grabimagesfromserver:", 21)==0)
					{
					if (p) *p++=0;				// points after 1st comma
					GrabImagesFromServer((unsigned char *)SilentText+21, p, (char *)MYACRNEMA);
					GrabFromGui++;
					}
				else if (memcmp(SilentText, "prefetch:", 9)==0)
					{
					PrefetchPatientData((char *)SilentText+9, 0);
					}
				else if (memcmp(SilentText, "loadhl7:", 8)==0)
					{
					int len = DFileSize(SilentText+8);
					if (len)
						{
						char *p=(char*) malloc(len+1);
						FILE *f;
						f = fopen(SilentText+8, "rb");
						fread(p, 1, len, f);
 						p[len]=0;
        	                        	fclose(f); 
						ProcessHL7Data(p);
						free(p);
						}
					}
				else if (memcmp(SilentText, "quit:", 5)==0)
					{
					exit(0);
					}
				else if (memcmp(SilentText, "checklargestmalloc:", 19)==0)
					{
					for(int i=0; i<1000; i++)
						{ char *p = (char *)malloc(i*10*1024*1024);
						  if (p) free(p);
						  else
							{
							OperatorConsole.printf("Largest malloc = %d MB\n", i*10);
							break;
							}
						}
					}
				}

			if (tempfile[0])
				{
				int len = DFileSize(tempfile);
				if (len)
					{
					int extra=0;
					if (memcmp(SilentText, "convert_to_dicom:", 17)==0) extra=27;
					VR *vr = new VR(0x9999, 0x0401, len+extra, TRUE);	// better
					FILE *f;
					f = fopen(tempfile, "rb");
					if (extra) sprintf((char *)(vr->Data), "Content-type: image/dicom\n\n");
					fread((char*)(vr->Data)+extra, 1, len, f);
                                	fclose(f); 
					SOPVerification.WriteResponse(&PDU, &DCO, vr);
					}
				else
					SOPVerification.WriteResponse(&PDU, &DCO, NULL);
				unlink(tempfile);
				}
			else if (Response[0])
				{
				VR *vr = new VR(0x9999, 0x0401, strlen(Response), (void *)Response, FALSE);
				SOPVerification.WriteResponse(&PDU, &DCO, vr);
				}
			else
				SOPVerification.WriteResponse(&PDU, &DCO, NULL);
			continue;
			}

		if(SOPPatientRootQuery.Read (&PDU, &DCO))
			{
#ifdef	DEBUG_MODE
			if (!vrsilent)
				OperatorConsole.printf("C-Find (PatientRoot) located %d records\n", SOPPatientRootQuery.RecordsFound);
#endif
			C_Find_PatientRoot++;
			continue;
			}
		if(SOPPatientRootRetrieveNKI.Read (&PDU, &DCO, (void *)&ratd))
			{
#ifdef	DEBUG_MODE
			if (!vrsilent)
				OperatorConsole.printf("C-Move (PatientRootNKI)\n");
#endif
			LogUser("C-Move ", &PDU, &DCO);
			C_Move_PatientRootNKI++;
			continue;
			}
		if(SOPPatientRootRetrieveGeneric.Read (&PDU, &DCO, (void *)&ratd))
			{
#ifdef	DEBUG_MODE
			if (!vrsilent)
				OperatorConsole.printf("C-Move (PatientRoot)\n");
#endif
			LogUser("C-Move ", &PDU, &DCO);
			C_Move_PatientRoot++;
			continue;
			}

		if(SOPStudyRootQuery.Read (&PDU, &DCO))
			{
#ifdef	DEBUG_MODE
			if (!vrsilent)
				OperatorConsole.printf("C-Find (StudyRoot) located %d records\n", SOPStudyRootQuery.RecordsFound);
#endif
			C_Find_StudyRoot++;
			continue;
			}
		if(SOPStudyRootRetrieveNKI.Read (&PDU, &DCO, (void *)&ratd))
			{
#ifdef	DEBUG_MODE
			if (!vrsilent)
				OperatorConsole.printf("C-Move (StudyRootNKI)\n");
#endif
			LogUser("C-Move ", &PDU, &DCO);
			C_Move_StudyRootNKI++;
			continue;
			}
		if(SOPStudyRootRetrieveGeneric.Read (&PDU, &DCO, (void *)&ratd))
			{
#ifdef	DEBUG_MODE
			if (!vrsilent)
				OperatorConsole.printf("C-Move (StudyRoot)\n");
#endif
			LogUser("C-Move ", &PDU, &DCO);
			C_Move_StudyRoot++;
			continue;
			}

		if(SOPPatientStudyOnlyQuery.Read (&PDU, &DCO))
			{
#ifdef	DEBUG_MODE
			if (!vrsilent)
				OperatorConsole.printf("C-Find (PatientStudyOnly) located %d records\n", SOPPatientStudyOnlyQuery.RecordsFound);
#endif
			C_Find_PatientStudyOnly++;
			continue;
			}

		if(SOPModalityWorkListQuery.Read (&PDU, &DCO))
			{
#ifdef	DEBUG_MODE
			if (!vrsilent)
				OperatorConsole.printf("C-Find (Modality Work List) located %d records\n", SOPModalityWorkListQuery.RecordsFound);
#endif
			C_Find_ModalityWorkList++;
			continue;
			}

		if(SOPPatientStudyOnlyRetrieveNKI.Read (&PDU, &DCO, (void *)&ratd))
			{
#ifdef	DEBUG_MODE
			if (!vrsilent)
				OperatorConsole.printf("C-Move (PatientStudyOnlyNKI)\n");
#endif
			LogUser("C-Move ", &PDU, &DCO);
			C_Move_PatientStudyOnlyNKI++;
			continue;
			}
		if(SOPPatientStudyOnlyRetrieveGeneric.Read (&PDU, &DCO, (void *)&ratd))
			{
#ifdef	DEBUG_MODE
			if (!vrsilent)
				OperatorConsole.printf("C-Move (PatientStudyOnly)\n");
#endif
			LogUser("C-Move ", &PDU, &DCO);
			C_Move_PatientStudyOnly++;
			continue;
			}

		vr = DCO.GetVR(0x0000, 0x0100);
		if(vr && vr->Data)
			{
#if NATIVE_ENDIAN == LITTLE_ENDIAN //Little Endian
			val = (* ((UINT16*)vr->Data) );
#else //Big Endian like Apple power pc
			val = SwitchEndian((UINT16)(* ((UINT16*)vr->Data) ));
#endif //BigEndian
			}
		else
			val = 0xffff;

		if(val==0x0fff)	// C-CANCEL = cancel request
			{
#ifdef	DEBUG_MODE
			if (!vrsilent)
				OperatorConsole.printf("Cancel request (ignored)\n");
#endif
			continue;
			}

		if(PrinterSupport (&PDU, &DCO, PrintData))
			{
			continue;
			}

		
		DICOMDataObject *CommitData = NULL;
		if(StorageCommitmentSupport (&PDU, &DCO, &CommitData))
			{
			continue;
			}

		if (CommitData)
			{
			PDU.Close();
OperatorConsole.printf("hi\n");
			StorageCommitmentSupport (&PDU, &DCO, &CommitData);
			break;
			}

		//Note: Changed print val to hex, easier to read.
		OperatorConsole.printf("\n***Client Error: command %4.4x failed **\n", val);
		OperatorConsole.printf("***Connection Terminated\n");
		UnknownRequest++;

		DebugLevel += 4;
		NonDestructiveDumpDICOMObject(&DCO);
		DebugLevel -= 4;

		PDU.Close();
		return ( FALSE );
		}

		// wait for the read ahead thread to end (otherwise ratd would point to nothing)

		ratd.ForceEnd = TRUE;
		while (ratd.Busy) Sleep(50);

		// free memory associated with thread if not already done so
		if (ratd.Devices)
			{
			if (ratd.pDCO) delete ratd.pDCO; ratd.pDCO = NULL;
			for (int i=0; i<ratd.NumToRead; i++) 
				if (ratd.DDOOutPtr[i])
					delete ratd.DDOOutPtr[i];
			free(ratd.DDOOutPtr);  ratd.DDOOutPtr   = NULL;
			free(ratd.Filenames);  ratd.Filenames   = NULL;
			free(ratd.ObjectFiles);ratd.ObjectFiles = NULL;
			free(ratd.Devices);    ratd.Devices     = NULL;
			// 20060405	mvh	Fix for multiple moves on same assoc (thanks Andrej Savelov)
			free(ratd.SOPs);       ratd.SOPs       = NULL;
			free(ratd.Series);     ratd.Series     = NULL;
			free(ratd.Studies);    ratd.Studies    = NULL;
			free(ratd.pats);       ratd.pats       = NULL;

			CloseHandle(ratd.Handle); ratd.Handle = NULLHANDLE;
			}

		// 20060405	mvh	Fix for multiple moves on same assoc (thanks Andrej Savelov)
		ratd.TimeOut = FALSE;
		ratd.Busy    = FALSE;

		TimeOfDay2 = time(NULL);
		TotalTime += TimeOfDay2 - TimeOfDay;
		if (!vrsilent)
			{
			char buf[64];
			strcpy(TimeString, ctime_r(&TimeOfDay2, buf));
			TimeString[strlen(TimeString)-1] = '\0';
			OperatorConsole.printf("UPACS THREAD %d: ENDED AT: %s\n", ThreadNum, TimeString);
			OperatorConsole.printf("UPACS THREAD %d: TOTAL RUNNING TIME: %d SECONDS\n", ThreadNum, TimeOfDay2 - TimeOfDay);
			}
#ifdef __BORLANDC__
			{ 
			int i = _heapchk();
			if (i<0) OperatorConsole.printf("*** heap error *** : %d", i);
			}
#endif

	return ( TRUE );
	}

StorageApp		ServerApp;

void ConfigDgate(void)
	{
	char	szRootSC[64];
	char	szTemp[32];
	int	i;

	// allow enable/disable of read ahead thread
  	if (!GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig,
			szRootSC, 64, ConfigFile)) return;

	if (!GetPrivateProfileString(szRootSC, "EnableReadAheadThread", "1",
			szTemp, 32, ConfigFile)) return;
	EnableReadAheadThread = atoi(szTemp);

	if (!GetPrivateProfileString(szRootSC, "WorkListMode", "0",
			szTemp, 32, ConfigFile)) return;
	WorkListMode = atoi(szTemp);

	// archive compression setting (new); note: returns length of entry; can be zero - is still OK!
	GetPrivateProfileString(szRootSC, "ArchiveCompression", "", ArchiveCompression, 16, ConfigFile);

	// compression default empty (same as as is)
	DroppedFileCompression[0] = 0;
	IncomingCompression   [0] = 0;

	// still accept obsolete FileCompressMode - 1..4 interpreted as nki compression n1..n4
	GetPrivateProfileString(szRootSC, "FileCompressMode", "0", szTemp, 32, ConfigFile);
	FileCompressMode = atoi(szTemp);
	if (FileCompressMode)
		{
		sprintf(DroppedFileCompression, "n%d", FileCompressMode);
		sprintf(IncomingCompression, 	"n%d", FileCompressMode);
		}

	// default no change of these settings
	GetPrivateProfileString(szRootSC, "DroppedFileCompression", 
				DroppedFileCompression, DroppedFileCompression, 16, ConfigFile);
	GetPrivateProfileString(szRootSC, "IncomingCompression",    
				IncomingCompression,    IncomingCompression,    16, ConfigFile);

	GetPrivateProfileString(szRootSC, "StorageFailedErrorCode", "272", szTemp, 32, ConfigFile);
        StorageFailedErrorCode = atoi(szTemp);

	GetPrivateProfileString(szRootSC, "TCPIPTimeOut", "300", szTemp, 32, ConfigFile);
        TCPIPTimeOut = atoi(szTemp);

	GetPrivateProfileString(szRootSC, "FailHoldOff", "60", szTemp, 32, ConfigFile);
        FailHoldOff = atoi(szTemp);

	GetPrivateProfileString(szRootSC, "RetryDelay", "100", szTemp, 32, ConfigFile);
        RetryDelay = atoi(szTemp);

	GetPrivateProfileString(szRootSC, "QueueSize", "128", szTemp, 32, ConfigFile);
        QueueSize = atoi(szTemp);

	GetPrivateProfileString(szRootSC, "ForwardCollectDelay", "600", szTemp, 32, ConfigFile);
        ForwardCollectDelay = atoi(szTemp);

	for (i=0; i<10; i++)
		{
		sprintf(szTemp, "VirtualServerFor%d", i);
		GetPrivateProfileString(szRootSC, szTemp, "", VirtualServerFor[i], 48, ConfigFile);
		}

	GetPrivateProfileString(szRootSC, "CacheVirtualData", "1", szTemp, 32, ConfigFile);
	CacheVirtualData = atoi(szTemp);

	GetPrivateProfileString(szRootSC, "PadAEWithZeros", "0", szTemp, 32, ConfigFile);
        ConfigPadAEWithZeros = atoi(szTemp);
	}

// main as console app or child process
static void DgateCgi(char *query_string, char *ext); // forward reference

int
main ( int	argc, char	*argv[] )
	{
	char	TimeString[128];
	time_t	TimeOfDay, TimeOfDay2;

	char	szRootSC[64];
	char	szTemp[32];
        int     i;
	BOOL	MissingDict=FALSE;

	char	*query_string = getenv( "QUERY_STRING" );
	if (query_string && argc==1) 
		{
		char *ext=strrchr(argv[0], '.');
	        DgateCgi(query_string, ext);
		exit(0);
		}
		
	StartTime = time(NULL);

	SetDicomErrorHandler(MyDcmError);

#ifdef UNIX
	if (strrchr(argv[0], '/'))
		{
		strcpy(DicomDict, argv[0]);
		*strrchr(DicomDict, '/') = 0;
		strcat(DicomDict, "/dgate.dic");
		}
#endif

	if (!VRType.AttachRTC(DicomDict))
		{
		MissingDict = TRUE;
		printf("File dgate.dic is missing, some server functions will not work correctly\n");
		}

	// allow enable/disable of read ahead thread
  	if (!GetPrivateProfileString(RootConfig, "MicroPACS", RootConfig,
			szRootSC, 64, ConfigFile)) return 0;

	ConfigDgate();

#ifdef UNIX
	// set that pesky broken pipe signal to be ignored
	// I/O that would have generated a SIGPIPE will return EPIPE instead
	// this applies to named pipe writes and read/write of sockets

	signal(SIGPIPE, SIG_IGN);

	// get ini file from same directory as binary
	if (strrchr(argv[0], '/'))
		{
		strcpy(ConfigFile, argv[0]);
		*strrchr(ConfigFile, '/') = 0;
		strcat(ConfigFile, "/dicom.ini");
		}
#endif

	int	Socketfd;

#if defined(DO_LEAK_DETECTION) || defined(DO_VIOLATION_DETECTION)
      int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

#ifdef DO_LEAK_DETECTION
      tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
#endif

#ifdef DO_VIOLATION_DETECTION
      tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
#endif

      _CrtSetDbgFlag(tmpDbgFlag);
      /* If a leak is detected, run again after specifying the serial num of the
         leak using _CrtSetBreakAlloc(<id>); */
//      _CrtSetBreakAlloc(37);
#endif

	if ( ! (Socketfd = ParseArgs(argc, argv) ))
		{
		if (RunServer)
			{
			struct stat statbuf;
			char TimeString[100], buf[64];
			int iNbConverters;

			memset((void *)&statbuf, 0, sizeof(statbuf));

		        // Start queue for export converters (may reprocess previously failed requests)
		        stat(argv[0], &statbuf);
                        strcpy(TimeString, ctime_r(&statbuf.st_mtime, buf));
			TimeString[strlen(TimeString)-1] = '\0';
			if (!(statbuf.st_mode & S_IFREG)) strcpy(TimeString, "?");
			if (NoThread)
				OperatorConsole.printf("DGATE (%s, build %s) runs in non-threaded debug mode\n", VERSION, TimeString);
			else
				OperatorConsole.printf("DGATE (%s, build %s) is running as threaded server\n", VERSION, TimeString);

			if (MissingDict)
				OperatorConsole.printf("*** File dgate.dic is missing, some server functions will not work correctly\n");

		        GetPrivateProfileString(szRootSC, "ExportConverters", "0", szTemp, 32, ConfigFile);
			iNbConverters = atoi(szTemp);
                        if (iNbConverters>MAXExportConverters) iNbConverters=MAXExportConverters;

		        if (iNbConverters && !NoThread)
		        { eqN = (struct conquest_queue **)malloc(iNbConverters * sizeof(struct conquest_queue *));
		          memset(eqN, 0, iNbConverters * sizeof(struct conquest_queue *));
		          for(i=0; i<iNbConverters; i++)
		            export_queueN(&eqN[i], NULL, i, NULL, NULL, NULL, NULL, NULL, NULL);
			  OperatorConsole.printf("Started %d export queue thread(s)\n", iNbConverters);
		        }

		        // Start queue for mirror copy (may reprocess previously failed requests)
		        GetPrivateProfileString(szRootSC, "MIRROR0", "-1", szTemp, 32, ConfigFile);
		        if (atoi(szTemp)!=-1 && !NoThread) 
			{ mirrorcopy_queue(NULL, NULL);
			  OperatorConsole.printf("Started mirror copy queue thread\n");
                        }

		        // Start prefetcher queue 
		        GetPrivateProfileString(szRootSC, "Prefetcher", "0", szTemp, 32, ConfigFile);
		        if (atoi(szTemp) && !NoThread) 
			{ prefetchermode = atoi(szTemp);
                          prefetcher(NULL, FALSE);
			  OperatorConsole.printf("Started prefetcher queue thread\n");
                        }

			if (NoThread)
				while ( TRUE )
					ServerApp.Server(Port);
			else
				ServerApp.Server(Port);
			}
		return(0);
		}

	// run a child from a driver application
	OperatorConsole.printf("Child: Socketfd = %d\n", Socketfd);

	// Print Date / Time in Log file
	char buf[64];
	TimeOfDay = time(NULL);
	strcpy(TimeString, ctime_r(&TimeOfDay, buf));
	TimeString[strlen(TimeString)-1] = '\0';
	OperatorConsole.printf("UPACS: %s: STARTED AT: %s\n", argv[1],  TimeString);

	ServerApp.ServerChild(Socketfd);

	TimeOfDay2 = time(NULL);
	strcpy(TimeString, ctime_r(&TimeOfDay2, buf));
	TimeString[strlen(TimeString)-1] = '\0';
	OperatorConsole.printf("UPACS: %s: ENDED AT: %s\n", argv[1],  TimeString);
	OperatorConsole.printf("UPACS: %s: TOTAL RUNNING TIME: %d SECONDS\n", argv[1], TimeOfDay2 - TimeOfDay );

	return ( 0 );
	}

///////////////////////////////////////////////////////////////////////////////
// grabbing code
///////////////////////////////////////////////////////////////////////////////

void SetStringVR(VR **vr, int g, int e, char *String)
{ int		len;
  char*		pCopy;

  /* NOTE:
	Uneven length strings must be padded with a space.
	Uneven length UIDs must be padded with \0.
  */
  len = strlen(String);
  pCopy = strdup(String);
  if (len & 1)
    pCopy[len++] = ' ';
  *vr = new VR(g, e, len, (void*)pCopy, (BOOL) TRUE); 
}


BOOL GrabImagesFromServer(BYTE *calledae, char *studydate, char *destination)
	{
	BOOL			quit, quit2;
	PDU_Service		PDU, PDU2;
	DICOMCommandObject	DCO;
	DICOMCommandObject	DCOR, DCOR2;
	DICOMDataObject		DDO;
	DICOMDataObject		DDOR, DDOR2;
	UID			uid, uid2;
	VR			*vr;
	LE_UINT16		command, datasettype, messageid, priority, tempid;
	BYTE			SOP[66];
  	unsigned char 		ip[64], port[64], compress[64];

	PDU.AttachRTC(&VRType);
	PDU2.AttachRTC(&VRType);

	if(!GetACRNema((char *)calledae, (char *)ip, (char *)port, (char *)compress))
		return FALSE;
		
	// Start setting up connection for C-FIND
	PDU.ClearAbstractSyntaxs();
	PDU.SetLocalAddress(MYACRNEMA);
	PDU.SetRemoteAddress(calledae);

	uid.Set("1.2.840.10008.3.1.1.1");	// Application context (DICOM App)
	PDU.SetApplicationContext(uid);

	uid.Set("1.2.840.10008.5.1.4.1.2.2.1"); // StudyRootQuery (C-Find SOP)
	PDU.AddAbstractSyntax(uid);

	// idem for the C-MOVE
	PDU2.ClearAbstractSyntaxs();
	PDU2.SetLocalAddress(MYACRNEMA);
	PDU2.SetRemoteAddress(calledae);

	uid2.Set("1.2.840.10008.3.1.1.1");	// Application context (DICOM App)
	PDU2.SetApplicationContext(uid2);

	uid2.Set("1.2.840.10008.5.1.4.1.2.2.2"); // StudyRootRetrieve (C-Move SOP)
	PDU2.AddAbstractSyntax(uid2);

        PDU.SetTimeOut(TCPIPTimeOut);
        PDU2.SetTimeOut(TCPIPTimeOut);

	// Make the association for the FIND on port/ip
	if(!PDU.Connect(ip, port))
		{
		OperatorConsole.printf("*** Grab - failed to connect for C-FIND\n");
		return ( FALSE );
		}

	// Make the association for the MOVE on port/ip
	if(!PDU2.Connect(ip, port))
		{
		PDU.Close();
		OperatorConsole.printf("*** Grab - failed to connect for C-MOVE\n");
		return(FALSE);
		}

	// Start a StudyRootQuery

	strcpy((char*) SOP, "1.2.840.10008.5.1.4.1.2.2.1"); // StudyRootQuery (C-Find SOP)
	vr = new VR (0x0000, 0x0002, strlen((char*)SOP), (void*) SOP, FALSE);
	DCO.Push(vr);
	command = EN0020;
	vr = new VR (0x0000, 0x0100, 0x0002, &command, FALSE);
	DCO.Push(vr);
	priority = 0;	// MEDIUM
	vr = new VR (0x0000, 0x0700, 0x0002, &priority, FALSE);
	DCO.Push(vr);
	datasettype = EN0102;	
	vr = new VR (0x0000, 0x0800, 0x0002, &datasettype, FALSE);
	DCO.Push(vr);
	messageid = EN0003;
	vr = new VR (0x0000, 0x0110, 0x0002, &messageid, FALSE);
	DCO.Push(vr);

	// Start of data object. Query level is image; ask for image UIDs and patid

	SetStringVR(&vr, 0x0008, 0x0052, "IMAGE"); 
	DDO.Push(vr); 
	SetStringVR(&vr, 0x0008, 0x0020, studydate); 
	DDO.Push(vr); 
	SetStringVR(&vr, 0x0008, 0x0018, "");
	DDO.Push(vr); 
	SetStringVR(&vr, 0x0010, 0x0020, "");
	DDO.Push(vr); 

	// Write query command object and data object

	PDU.Write(&DCO, uid);
	PDU.Write(&DDO, uid);

	// Get response

	quit = 0;
	while(!quit)
		{
		if(!PDU.Read(&DCOR))
			{
			PDU2.Close();
			OperatorConsole.printf("*** Grab - association lost in C-FIND\n");
			return ( FALSE );	// association lost
			}
		// is this a C-Find-RSP ?
		if(DCOR.GetUINT16(0x0000, 0x0100)!=0x8020)
			{
			PDU.Close();
			PDU2.Close();
			OperatorConsole.printf("*** Grab - wrong response for C-FIND\n");
			return(FALSE);
			}
		// is there a data set?
		if(DCOR.GetUINT16(0x0000, 0x0800)==0x0101)
			{
			PDU.Close();
			PDU2.Close();
			OperatorConsole.printf("Grab - finished\n");
			return( TRUE );
			}
		// no success
		if(DCOR.GetUINT16(0x0000, 0x0900)==0x0000)
			{
			PDU.Close();
			PDU2.Close();
			OperatorConsole.printf("*** Grab - C-FIND failed\n");
			return ( FALSE );
			}
		// read response data
		if(!PDU.Read(&DDOR))
			{
			PDU.Close();
			PDU2.Close();
			OperatorConsole.printf("*** Grab - C-FIND failed\n");
			return(FALSE);
			}

		while(vr = DCOR.Pop())
			delete vr;

		while(vr = DDOR.Pop())
			{
			if(vr->Group == 0x0000)	// error response
				{
				PDU.Close();
				PDU2.Close();
				OperatorConsole.printf("*** Grab - C-FIND error\n");
				return ( TRUE );
				}

			// recieving a SOPInstanceUID
			if (vr->Group == 0x0008 && vr->Element == 0x0018)
				{
				char s[255];
				memset(s, 0, 255);
				memcpy(s, vr->Data, vr->Length);

				// is it on the local server (note: test may be speeded by passing patid) ?		
				if (!TestFilter("", s, 10))
					{
					VR	*vr;

					// move it to the destination if it is not
					strcpy((char*) SOP, "1.2.840.10008.5.1.4.1.2.2.2"); // StudyRootMove (C-Move SOP)
					vr = new VR (0x0000, 0x0002, strlen((char*)SOP), (void*) SOP, FALSE);
					DCO.Push(vr);
					command = EN0021;
					vr = new VR (0x0000, 0x0100, 0x0002, &command, FALSE);
					DCO.Push(vr);
					priority = 0;	// MEDIUM
					vr = new VR (0x0000, 0x0700, 0x0002, &priority, FALSE);
					DCO.Push(vr);
					datasettype = EN0102;	
					vr = new VR (0x0000, 0x0800, 0x0002, &datasettype, FALSE);
					DCO.Push(vr);	
					messageid++;
#if NATIVE_ENDIAN == LITTLE_ENDIAN //Little Endian
					tempid = messageid;
#else //Big Endian like Apple power pc
					tempid = SwitchEndian((UINT16) messageid);
#endif //Big Endian
					vr = new VR (0x0000, 0x0110, 0x0002, &tempid, FALSE);
					DCO.Push(vr);
					vr = new VR(0x0000, 0x0600, 16, (void*)destination, FALSE);
					DCO.Push(vr);

					// Data object. Query level is image; ask for date and SopInstanceUID 
					SetStringVR(&vr, 0x0008, 0x0052, "IMAGE"); 
					DDO.Push(vr); 
					SetStringVR(&vr, 0x0008, 0x0020, studydate); 
					DDO.Push(vr); 
					SetStringVR(&vr, 0x0008, 0x0018, s);
					DDO.Push(vr); 

					// Write command object and data object
					PDU2.Write(&DCO, uid2);
					PDU2.Write(&DDO, uid2);

					// Get response
					quit2 = 0;
					while(!quit2)
						{
						if(!PDU2.Read(&DCOR2))
							{
							PDU.Close();
							OperatorConsole.printf("*** Grab - C-MOVE association lost\n");
							return ( FALSE );	// associate lost
							}
						if(DCOR2.GetUINT16(0x0000, 0x0100)!=0x8021)
							{
							PDU.Close();
							PDU2.Close();
							OperatorConsole.printf("*** Grab - C-MOVE got a wrong response\n");
							return(FALSE);
							}
						// ignore the data set
						if(DCOR2.GetUINT16(0x0000, 0x0800)!=0x0101)
							{
							PDU2.Read(&DDOR2);
							DDOR2.Reset();
							}
						if(DCOR2.GetUINT16(0x0000, 0x0900)==0x0000)	// success ?
							{
							OperatorConsole.printf("Grabbed %s\n", s);
							quit2=1;
							}
						else if (DCOR2.GetUINT16(0x0000, 0x0900)==0xff00)	// continuing no problem
							{
							}
						else
							{
							UINT16	RemoteError = DCOR2.GetUINT16(0x0000, 0x0900);	// mvh 20051208
							OperatorConsole.printf("*** Grab - remote C-MOVE DICOM Error code %x (%d)\n", RemoteError, RemoteError);
							PDU.Close();
							PDU2.Close();
							return ( FALSE );
							}
						DCOR2.Reset();
						}

					}
				}

			delete vr;
			}
		}

	PDU.Close();
	PDU2.Close();
	return ( TRUE );
	}

/////////////////////////////////////////////////////////////////////////////////////////////
// Used to send commands (e.g, from linux gui to be developed) to running server without reindexing
/////////////////////////////////////////////////////////////////////////////////////////////

int processhtml(char *out, char *in, int len)
	{	
	int i, outlen=0, instring=0;
	for (i=0; i<len; i++)
		{
		if (i>3 && in[i-3]=='<' && in[i-2]=='A' && instring==0) instring=1;
		if (in[i]=='>' && instring==1) instring=0;

		if (in[i]==' ' && instring) { out[outlen++] = '%'; out[outlen++] = '2'; out[outlen++] = '0'; }
		else 		              out[outlen++] = in[i];
		}
	return outlen;
	}

static char ServerCommandAddress[64] = "127.0.0.1";

static BOOL SendServerCommand(char *NKIcommand1, char *NKIcommand2, int con, char *buf, BOOL html)
	{
	PDU_Service		PDU;
	DICOMCommandObject	DCO;
	DICOMCommandObject	DCOR;
	UID			uid;
	VR			*vr;
	LE_UINT16		tuint16, command, datasettype, messageid;
	BYTE			SOP[64];

	if (buf) *buf=0;

	PDU.ClearAbstractSyntaxs();
	PDU.SetLocalAddress(MYACRNEMA);
	PDU.SetRemoteAddress(MYACRNEMA);
	uid.Set("1.2.840.10008.3.1.1.1");	// Dicom APP
	PDU.SetApplicationContext(uid);
	uid.Set("1.2.840.10008.1.1");		// Verification
	PDU.AddAbstractSyntax(uid);

	if(!PDU.Connect((unsigned char *)ServerCommandAddress, Port))
		return ( FALSE );
	
	strcpy((char*) SOP, "1.2.840.10008.1.1"); // Verification
	vr = new VR (0x0000, 0x0002, strlen((char*)SOP), (void*) SOP, FALSE);
	DCO.Push(vr);

	if (strlen(NKIcommand1))
	{ vr = new VR (0x9999, 0x0300, strlen((char*)NKIcommand1), (void*)NKIcommand1, FALSE);
	  DCO.Push(vr);
	}

	if (strlen(NKIcommand2))
	{ vr = new VR (0x9999, 0x0400, strlen((char*)NKIcommand2), (void*)NKIcommand2, FALSE);
	  DCO.Push(vr);
	}

	command = EN0030;
	vr = new VR (0x0000, 0x0100, 0x0002, &command, FALSE);
	DCO.Push(vr);

	datasettype = 0x0101;	
	vr = new VR (0x0000, 0x0800, 0x0002, &datasettype, FALSE);
	DCO.Push(vr);

	messageid = EN0001;
	vr = new VR (0x0000, 0x0110, 0x0002, &messageid, FALSE);
	DCO.Push(vr);

	PDU.Write(&DCO, uid);

	if(!PDU.Read(&DCOR))
		return ( FALSE );	// associate lost

	while(vr = DCOR.Pop())
		{
		if (vr->Group == 0x9999 && vr->Element == 0x0401)
			{
			if (buf)
				{
				int len;
				if (html) len = processhtml(buf, (char *)vr->Data, vr->Length);
                                else      memcpy(buf, vr->Data, len = vr->Length);
				buf[len]=0;
				if (len>1 && buf[len-1]==' ') buf[len-1]=0;
				}
			else
				{
				if (con==0)
					{
					con = fileno(stdout);
#ifdef WIN32
					setmode(con, O_BINARY);
#endif
					}

				if (html)
				{ char *buf = (char *)malloc(1000000);
                                  int len = processhtml(buf, (char *)vr->Data, vr->Length);
				  write(con, buf, len);
                                  free(buf);
                                }
				else
				  write(con, vr->Data, vr->Length);
				}
			}
		delete vr;
		}

	PDU.Close();
	return ( TRUE );
	}

/////////////////////////////////////////////////////////////////////////////////////////////
// Elementary web interface
/////////////////////////////////////////////////////////////////////////////////////////////

static int isxdig(char ch)
{ if ((ch>='0' && ch<='9') ||
      (ch>='A' && ch<='F') ||
      (ch>='a' && ch<='f'))
    return 1;
  else
    return 0;
}

static int xdigit(char ch)
{  if (ch>='0' && ch<='9')
    return ch - '0';
  else
    return (ch & 15) + 9;
}

static int htoin(char *value, int len)
{ int i, result;

  result = 0;
  i      = 0;
  while (i < len && value[i] == ' ') i++;

  while (i < len && isxdig(value[i]))
  { result = result * 16 + xdigit(value[i]);
    i++;
  }

  return result;
}

static int CGI(char *out, char *name, char *def)
{ char *p = getenv( "QUERY_STRING" );
  char tmp[512];
  char *q;
  int  i, j;

  if (out!=def) *out = 0;
  if (p==NULL) return 0;

  strcpy(tmp, "&");	// & only hits on second item
  strcat(tmp, name);
  strcat(tmp, "=");

  q = strstr(p, tmp);

  if (q==NULL)		// check first item
    if (memcmp(p, tmp+1, strlen(tmp+1))==0) q=p-1;

  if (q==NULL)
  { if (out!=def) strcpy(out, def);
    return 0;
  }

  q = q + strlen(tmp);

  i = 0; j = 0;
  while (q[i] != 0  && q[i] != '&')
  { if (q[i] == '%')
    { tmp[j++] = (char)(htoin(q+i+1, 2));
      i = i+3;
    }
    else if (q[i] == '+')
    { tmp[j++] = ' ';
      i++;
    }
    else
      tmp[j++] = q[i++];
  }
  tmp[j++] = 0;

  strcpy(out, tmp);

  return 0;
}

int console;

static void HTML(char *string, ...)
{ char n[2]="\n";
  char s[1250];
  va_list vargs;

  va_start(vargs, string);
  vsprintf(s, string, vargs);
  va_end(vargs);

  write(console, s, strlen(s));
  write(console, n, 1);
}

static BOOL Tabulate(char *c1, char *par, char *c4, BOOL edit=FALSE)
{ char *p=strchr(par, ':');
  char buf[512];

  SendServerCommand("", par, 0, buf);
  if (buf[0])
  { if (edit) HTML("<TR><TD>%s<TD>%s<TD><INPUT SIZE=64 NAME=\"%s\" VALUE=\"%s\"><TD>%s<TD></TR>", c1, p+1, p+1, buf, c4, p+1);
    else      HTML("<TR><TD>%s<TD>%s<TD>%s<TD>%s<TD></TR>", c1, p+1, buf, c4, p+1);
    return TRUE;
  }
  else
    return FALSE;
}

static void replace(char *string, char *key, char *value)
{ char temp[1000];
  char *p = strstr(string, key), *q = string;

  if (p==NULL) return;
  if (value==NULL) return;

  temp[0] = 0;
  while(p)
  { *p = 0;
    strcat(temp, string);
    *p = key[0];
    strcat(temp, value);
    string = p + strlen(key);
    p = strstr(string, key);
  };

  strcat(temp, string);
  strcpy(q, temp);
}

static void DgateCgi(char *query_string, char *ext)
{ char mode[512], command[8192], size[32], dsize[32], iconsize[32], slice[512], slice2[512], query[512], buf[512], 
       patientidmatch[512], patientnamematch[512], studydatematch[512], startdatematch[512], 
       db[256], series[512], study[512], compress[64], WebScriptAddress[256], WebMAG0Address[256], 
       WebServerFor[64], WebCodeBase[512], lw[64], source[64], dest[64], series2[512], study2[512];
  char ex[128], extra[256], graphic[32], viewer[128];
  int  i, j;
  DBENTRY *DBE;
  char  RootSC[64];
  char  Temp[128];
  char  *p;

  BOOL ReadOnly=FALSE;

  console = fileno(stdout);
#ifdef WIN32
  strcpy(ex, ".exe");
  if (ext) strcpy(ex, ext);
  setmode(console, O_BINARY);
#else
  ex[0]=0;
  if (ext) strcpy(ex, ext);
#endif

  ConfigMicroPACS();
  LoadKFactorFile((char*)KFACTORFILE);

  GetPrivateProfileString ( RootConfig, "MicroPACS", RootConfig, RootSC, 64, ConfigFile);
  GetPrivateProfileString ( RootSC, "WEBReadOnly", "1", Temp, 64, ConfigFile);
  ReadOnly = atoi(Temp);

  sprintf(WebScriptAddress, "http://%s/cgi-bin/dgate%s", getenv("SERVER_NAME"), ex);
  GetPrivateProfileString ( RootSC, "WEBScriptAddress", WebScriptAddress, WebScriptAddress, 256, ConfigFile);

  sprintf(WebMAG0Address, "http://%s/mag0", getenv("SERVER_NAME"), ex);
  GetPrivateProfileString ( RootSC, "WEBMAG0Address", WebMAG0Address, WebMAG0Address, 256, ConfigFile);

  sprintf(WebServerFor, "127.0.0.1");
  GetPrivateProfileString ( RootSC,        "WebServerFor", WebServerFor, WebServerFor, 256, ConfigFile);
  GetPrivateProfileString ( "webdefaults", "address",      WebServerFor, WebServerFor, 256, ConfigFile);
  strcpy(ServerCommandAddress, WebServerFor);

  strcpy(WebCodeBase, WebScriptAddress);		// note: WebCodeBase should include trailing slash or backslash
  p = strrchr(WebCodeBase, '/'); 
  if (p) p[0]=0;
  p = strrchr(WebCodeBase, '/'); 
  if (p) p[1]=0;
  GetPrivateProfileString ( RootSC, "WebCodeBase", WebCodeBase, (char*)WebCodeBase, 256, ConfigFile);

  GetPrivateProfileString ( "webdefaults", "port", (char *)Port, (char*)Port,       256, ConfigFile);

  CGI((char *)Port,         "port",    (char *)Port);	// allow serving any server
  CGI(ServerCommandAddress, "address", ServerCommandAddress);

  CGI(query,   "query",    "");		// query for most db selectors
  CGI(slice2,  "slice",    "");		// patid:sop for slice
  CGI(series2, "series",   "");		// patid:seriesuid for seriesviewer/move/delete
  CGI(study2,  "study",    "");		// patid:studyuid for move/delete
  CGI(mode,    "mode",     "");		// web page
  CGI(db,      "db",       "");		// database to edit or list
  CGI(lw,      "lw",       "0/0");	// level/window
  CGI(source,  "source",   "(local)");  // source for move
  CGI(dest,    "dest",     "");		// destination for move

  j = 0;
  for(i=0; i<strlen(slice2); i++) if (slice2[i]==' ') { slice[j++]='%'; slice[j++]='2'; slice[j++]='0'; } else slice[j++]=slice2[i];
  slice[j++]=0;

  j = 0;
  for(i=0; i<strlen(series2); i++) if (series2[i]==' ') { series[j++]='%'; series[j++]='2'; series[j++]='0'; } else series[j++]=series2[i];
  series[j++]=0;

  j = 0;
  for(i=0; i<strlen(study2); i++) if (study2[i]==' ') { study[j++]='%'; study[j++]='2'; study[j++]='0'; } else study[j++]=study2[i];
  study[j++]=0;

  GetPrivateProfileString ( "webdefaults", "size",     "512", size,     256, ConfigFile);
  GetPrivateProfileString ( "webdefaults", "dsize",    "0",   dsize,    256, ConfigFile);
  GetPrivateProfileString ( "webdefaults", "compress", "n4",  compress, 256, ConfigFile);
  GetPrivateProfileString ( "webdefaults", "iconsize", "48",  iconsize, 256, ConfigFile);
  GetPrivateProfileString ( "webdefaults", "graphic",  "bmp", graphic,   32, ConfigFile);
  GetPrivateProfileString ( "webdefaults", "viewer",   "seriesviewer", viewer, 128, ConfigFile);

  CGI(size,    "size",     size);	// size of viewer in pixels
  CGI(dsize,   "dsize",    dsize);	// max size of transmitted dicom images in pixels, 0=original
  CGI(compress,"compress", compress);	// compression of transmitted dicom images to (our) web viewer
  CGI(iconsize,"iconsize", iconsize);	// size of icons in image table
  CGI(graphic, "graphic",  graphic);	// style of transmitting thumbnails and slices (gif or bmp)
  CGI(viewer,  "viewer",   viewer);	// mode of used viewer

  CGI(patientidmatch,   "patientidmatch",   "");	// search strings
  CGI(patientnamematch, "patientnamematch", "");
  CGI(studydatematch,   "studydatematch",   "");
  CGI(startdatematch,   "startdatematch",   "");

  sprintf(extra, "port=%s&address=%s", Port, ServerCommandAddress);

  if (patientidmatch[0]!=0)
  { if (query[0]) strcat(query, " and ");
    strcat(query, "PatientID like '%");
    strcat(query, patientidmatch);
    strcat(query, "%'");
  };

  if (patientnamematch[0]!=0)
  { if (query[0]) strcat(query, " and ");
    strcat(query, "PatientNam like '%");
    strcat(query, patientnamematch);
    strcat(query, "%'");
  };

  if (studydatematch[0]!=0)
  { if (query[0]) strcat(query, " and ");
    strcat(query, "StudyDate like '%");
    strcat(query, studydatematch);
    strcat(query, "%'");
  };

  if (startdatematch[0]!=0)
  { if (query[0]) strcat(query, " and ");
    strcat(query, "StartDate like '%");
    strcat(query, startdatematch);
    strcat(query, "%'");
  };

  if      (stricmp(db, "dicomworklist")==0) DBE=WorkListDB;
  else if (stricmp(db, "dicompatients")==0) DBE=PatientDB;
  else if (stricmp(db, "dicomstudies" )==0) DBE=StudyDB;
  else if (stricmp(db, "dicomseries"  )==0) DBE=SeriesDB;
  else if (stricmp(db, "dicomimages"  )==0) DBE=ImageDB;
  else                                      DBE=WorkListDB;

  /************************** top page **************************/

  if (strcmp(mode, "")==0 || strcmp(mode, "top")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H1>Welcome to the Conquest DICOM server - version %s</H1>", VERSION);
    HTML("<IMG SRC='%sconquest.jpg' ALT='Developed in the Conquest project'>", WebCodeBase);
    HTML("<HR>");
    HTML("<PRE>");
    SendServerCommand("", "display_status:", console);
    HTML("</PRE>");


    HTML("<HR>");

    HTML("<table>");
    HTML("<tr>");
    HTML("<FORM ACTION=\"dgate%s\">", ex);
    HTML("<INPUT NAME=mode    TYPE=HIDDEN VALUE=querypatients>");
    HTML("<INPUT NAME=port    TYPE=HIDDEN VALUE=%s>", (char *)Port);
    HTML("<INPUT NAME=address TYPE=HIDDEN VALUE=%s>", ServerCommandAddress);
    HTML("<td>List local patients");
    HTML("<td>Patient ID: <INPUT NAME=patientidmatch TYPE=Text VALUE=>");
    HTML("<td>Name: <INPUT NAME=patientnamematch TYPE=Text VALUE=>");
    HTML("<td>");
    HTML("<td><INPUT TYPE=SUBMIT VALUE=Go>");
    HTML("</FORM>");
    HTML("</tr>");

    HTML("<tr>");
    HTML("<FORM ACTION=\"dgate%s\">", ex);
    HTML("<INPUT NAME=mode    TYPE=HIDDEN VALUE=querystudies>");
    HTML("<INPUT NAME=port    TYPE=HIDDEN VALUE=%s>", (char *)Port);
    HTML("<INPUT NAME=address TYPE=HIDDEN VALUE=%s>", ServerCommandAddress);
    HTML("<td>List local studies");
    HTML("<td>Patient ID: <INPUT NAME=patientidmatch TYPE=Text VALUE=>");
    HTML("<td>Name: <INPUT NAME=patientnamematch TYPE=Text VALUE=>");
    HTML("<td>Date: <INPUT NAME=studydatematch TYPE=Text VALUE=>");
    HTML("<td><INPUT TYPE=SUBMIT VALUE=Go>");
    HTML("</FORM>");
    HTML("</tr>");

    HTML("<tr>");
    HTML("<FORM ACTION=\"dgate%s\">", ex);
    HTML("<INPUT NAME=mode    TYPE=HIDDEN VALUE=queryworklist>");
    HTML("<INPUT NAME=port    TYPE=HIDDEN VALUE=%s>", (char *)Port);
    HTML("<INPUT NAME=address TYPE=HIDDEN VALUE=%s>", ServerCommandAddress);
    HTML("<td>List local worklist");
    HTML("<td>Patient ID: <INPUT NAME=patientidmatch TYPE=Text VALUE=>");
    HTML("<td>Name: <INPUT NAME=patientnamematch TYPE=Text VALUE=>");
    HTML("<td>Date: <INPUT NAME=startdatematch TYPE=Text VALUE=>");
    HTML("<td><INPUT TYPE=SUBMIT VALUE=Go>");
    HTML("</FORM>");
    HTML("</tr>");

    HTML("<tr>");
    HTML("<FORM ACTION=\"dgate%s\">", ex);
    HTML("<td>Find ");
    HTML("<select name=mode>");
    HTML("  <option value='patientfinder'>Patient</option>");
    HTML("  <option value='studyfinder'>Study</option>");
    HTML("  <option value='seriesfinder'>Series</option>");
    HTML("</select>");
    HTML("<td>on server");
    HTML("<select name=dest>");
    SendServerCommand("", "get_amaps:<option value='%s'>%0.0s%0.0s%0.0s%s</option>", console);
    HTML("</select>");
    HTML("<INPUT NAME=port    TYPE=HIDDEN VALUE=%s>", (char *)Port);
    HTML("<INPUT NAME=address TYPE=HIDDEN VALUE=%s>", ServerCommandAddress);
    HTML("<INPUT NAME=source  TYPE=HIDDEN VALUE=%s>", source);
    HTML("<INPUT NAME=dest    TYPE=HIDDEN VALUE=%s>", dest);
    HTML("<td>Search: <INPUT NAME=query TYPE=Text VALUE=>");
    HTML("<td><td><INPUT TYPE=SUBMIT VALUE=Go>");
    HTML("</FORM>");
    HTML("</tr>");

    HTML("</table>");

    HTML("<HR>");
    HTML("<BR><A HREF=\"dgate%s?%s&mode=showconfig\">Show server configuration</A>", ex, extra);
    HTML("<BR><A HREF=\"dgate%s?%s&mode=showsops\">Show server accepted SOPs</A>", ex, extra);
    HTML("<BR><A HREF=\"dgate%s?%s&mode=showdb\">Show database layout</A>", ex, extra);
    HTML("<BR><A HREF=\"dgate%s?%s&mode=showdictionary\">Show DICOM dictionary (Long!)</A>", ex, extra);
    HTML("</BODY>");
    exit(0);
  };

  /************************** configuration **************************/

  if (strcmp(mode, "showconfig")==0)
  { ReadOnly = TRUE;

    HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H1>Welcome to the Conquest DICOM server - version %s</H1>", VERSION);
    HTML("<FORM ACTION=\"dgate%s\">", ex);
    if (!ReadOnly) HTML("<INPUT NAME=mode TYPE=HIDDEN VALUE=updateconfig>");
    HTML("<INPUT NAME=port    TYPE=HIDDEN VALUE=%s>", (char *)Port);
    HTML("<INPUT NAME=address TYPE=HIDDEN VALUE=%s>", ServerCommandAddress);
    HTML("<HR>");
    HTML("<table RULES=ALL BORDER=1>");

    HTML("<Caption>List of configuration parameters</caption>");
    HTML("<TR><TD>File<TD>Parameter<TD>Value<TD>Description</TR>");
    Tabulate("dicom.ini", "get_param:MyACRNema", "DICOM name of server", !ReadOnly);
    Tabulate("dicom.ini", "get_param:TCPPort",   "TCP/IP port where server listens", !ReadOnly);
    Tabulate("dicom.ini", "get_param:FileNameSyntax", "Determines name for new files", !ReadOnly);
    Tabulate("dicom.ini", "get_param:FixPhilips", "Strip leading zeros from 10 digit patient ID", !ReadOnly);
    Tabulate("dicom.ini", "get_param:FixKodak", "Strip leading zeros from 8 digit patient ID", !ReadOnly);
    Tabulate("dicom.ini", "get_param:IncomingCompression", "Compression mode for incoming files", !ReadOnly);
    Tabulate("dicom.ini", "get_param:ArchiveCompression", "Compression mode for acrhived files", !ReadOnly);
    Tabulate("dicom.ini", "get_param:MAGDevices", "Number of storage devices", !ReadOnly);
    for (i=0; i<100; i++)
    { sprintf(command, "get_param:MAGDevice%d", i);
      if (!Tabulate("dicom.ini", command, "Directory where data is stored", !ReadOnly)) break;
    }   
    HTML("<TR></TR>");
    for (i=0; i<1000; i++)
    { sprintf(command, "get_amap:%d", i);
      if (!Tabulate("acrnema.map", command, "Known DICOM provider")) break;
    }

    HTML("</table>");
    if (!ReadOnly) HTML("<INPUT TYPE=SUBMIT VALUE=Update>");
    HTML("</FORM>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "showsops")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H1>Welcome to the Conquest DICOM server - version %s</H1>", VERSION);
    HTML("<HR>");
    HTML("<table RULES=ALL BORDER=1>");

    HTML("<Caption>List of uids</caption>");
    HTML("<TR><TD>File<TD>Parameter<TD>Value<TD>Description</TR>");
    for (i=0; i<1000; i++)
    { sprintf(command, "get_sop:%d", i);
      if (!Tabulate("dgatesop.lst", command, "Accepted SOP class uid")) break;
    }
    for (i=0; i<1000; i++)
    { sprintf(command, "get_transfer:%d", i);
      if (!Tabulate("dgatesop.lst", command, "Accepted transfer syntax ")) break;
    }
    for (i=0; i<1000; i++)
    { sprintf(command, "get_application:%d", i);
      if (!Tabulate("dgatesop.lst", command, "Accepted application uid")) break;
    }
    for (i=0; i<1000; i++)
    { sprintf(command, "get_localae:%d", i);
      if (!Tabulate("dgatesop.lst", command, "Accepted called AE title")) break;
    }
    for (i=0; i<1000; i++)
    { sprintf(command, "get_remotea:%d", i);
      if (!Tabulate("dgatesop.lst", command, "Accepted calling AE title")) break;
    }
    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "showdb")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H1>Welcome to the Conquest DICOM server - version %s</H1>", VERSION);
    HTML("<HR>");
    HTML("<table RULES=ALL BORDER=1>");

    HTML("<Caption>List of configuration parameters</caption>");
    HTML("<TR><TD>File<TD>Parameter<TD>Value<TD>Description</TR>");
    for (i=0; i<1000; i++)
    { sprintf(command, "get_sqldef:PATIENT,%d", i);
      if (!Tabulate("dicom.sql", command, "Patient database field definition")) break;
    }
    for (i=0; i<1000; i++)
    { sprintf(command, "get_sqldef:STUDY,%d", i);
      if (!Tabulate("dicom.sql", command, "Study database field definition")) break;
    }
    for (i=0; i<1000; i++)
    { sprintf(command, "get_sqldef:SERIES,%d", i);
      if (!Tabulate("dicom.sql", command, "Series database field definition")) break;
    }
    for (i=0; i<1000; i++)
    { sprintf(command, "get_sqldef:IMAGE,%d", i);
      if (!Tabulate("dicom.sql", command, "Image database field definition")) break;
    }
    for (i=0; i<1000; i++)
    { sprintf(command, "get_sqldef:WORKLIST,%d", i);
      if (!Tabulate("dicom.sql", command, "Worklist database field definition")) break;
    }
    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "showdictionary")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H1>Welcome to the Conquest DICOM server - version %s</H1>", VERSION);
    HTML("<HR>");
    HTML("<table RULES=ALL BORDER=1>");

    HTML("<Caption>List of configuration parameters</caption>");
    HTML("<TR><TD>File<TD>Parameter<TD>Value<TD>Description</TR>");
    for (i=0; i<9000; i++)
    { sprintf(command, "get_dic:%d", i);
      if (!Tabulate("dgate.dic", command, "Dictionary item")) break;
    }
    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "updateconfig")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H1>Welcome to the Conquest DICOM server - version %s</H1>", VERSION);
    HTML("<HR>");
    if (!ReadOnly) HTML("Updating the configuration through the WEB interface is not yet supported");
    else           HTML("Updating the configuration through the WEB interface is not allowed");
    HTML("</BODY>");
    exit(0);
  };

  /************************** local browsers **************************/

  if (strcmp(mode, "querypatients")==0)
  { HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<table RULES=ALL BORDER=1>");
    if (query[0]) HTML("<Caption>List of selected patients on local server</caption>");
    else          HTML("<Caption>List of all patients on local server</caption>");
    HTML("<TR><TD>Patient ID<TD>Name<TD>Sex<TD>Birth date</TR>");

    strcpy(command,                 "query:DICOMPatients|patientid,patientnam,patientsex,patientbir|");
    strcpy(command+strlen(command),  query);
    sprintf(command+strlen(command), "|<TR><TD><A HREF=dgate%s?%s&mode=querystudies&query=patientid+=+'%%s'>%%0.0s %%0.0s %%0.0s", ex, extra);
    strcpy(command+strlen(command), "%s</A><TD>%s<TD>%s<TD>%s</TR>");
    SendServerCommand("", command, console);

    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "querystudies")==0)
  { HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<table RULES=ALL BORDER=1>");
    if (query[0]) HTML("<Caption>List of selected studies on local server</caption>");
    else          HTML("<Caption>List of all studies on local server</caption>");
    HTML("<TR><TD>Patient ID<TD>Name<TD>Study date<TD>Study description<TD>Study modality</TR>");

    strcpy(command,                 "query:DICOMStudies|patientid,studyinsta,patientnam,studydate,studydescr,studymodal|");
    strcpy(command+strlen(command),  query);
    sprintf(command+strlen(command), "|<TR><TD><A HREF=dgate%s?%s&mode=queryseries&query=DICOMStudies.patientid+=+'%%s'+and+DICOMSeries.studyinsta+=+'%%s'>%%0.0s %%0.0s %%0.0s %%0.0s", ex, extra);
    strcpy(command+strlen(command), "%s</A><TD>%0.0s %s<TD>%s<TD>%s<TD>%s");
    sprintf(command+strlen(command), "<TD><A HREF=dgate%s?%s&mode=studymover&study=%%s:%%s&source=%s>push%%0.0s %%0.0s %%0.0s %%0.0s</A>", ex, extra, source);
    if (!ReadOnly) 
      sprintf(command+strlen(command), "<TD><A HREF=dgate%s?%s&mode=studydeleter&study=%%s:%%s>delete%%0.0s %%0.0s %%0.0s %%0.0s</A>", ex, extra);
    strcpy(command+strlen(command), "</TR>");
    SendServerCommand("", command, console);

    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "queryseries")==0)
  { HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<table RULES=ALL BORDER=1>");
    HTML("<Caption>List of series on local server</caption>");
    HTML("<TR><TD>Patient ID<TD>Name<TD>Series date<TD>Series time<TD>Series description<TD>Modality</TR>");

    strcpy(command,                 "query:DICOMSeries,DICOMStudies|DICOMStudies.patientid,DICOMSeries.seriesinst,DICOMStudies.patientnam,DICOMSeries.seriesdate,DICOMSeries.seriestime,DICOMSeries.seriesdesc,DICOMSeries.modality|");
    strcpy(command+strlen(command),  query);
    strcpy(command+strlen(command), " and DICOMStudies.studyinsta=DICOMSeries.studyinsta");
    sprintf(command+strlen(command), "|<TR><TD><A HREF=dgate%s?%s&mode=queryallimages&query=DICOMStudies.patientid+=+'%%s'+and+DICOMSeries.seriesinst+=+'%%s'>%%0.0s %%0.0s %%0.0s %%0.0s %%0.0s %%s %%0.0s</A>", ex, extra);
    strcpy(command+strlen(command), "<TD>%s<TD>%s<TD>%s<TD>%s<TD>%s");
    sprintf(command+strlen(command), "<TD><A HREF=dgate%s?%s&mode=queryimages&query=DICOMStudies.patientid+=+'%%s'+and+DICOMSeries.seriesinst+=+'%%s'&size=%s>thumbs%%0.0s %%0.0s %%0.0s %%0.0s %%0.0s</A>", ex, extra, size);
    sprintf(command+strlen(command), "<TD><A HREF=dgate%s?%s&mode=%s&series=%%s:%%s&size=%s>view%%0.0s %%0.0s %%0.0s %%0.0s %%0.0s</A>", ex, extra, viewer, size);
    sprintf(command+strlen(command), "<TD><A HREF=dgate%s?%s&mode=seriesmover&series=%%s:%%s&source=%s>push%%0.0s %%0.0s %%0.0s %%0.0s %%0.0s</A>", ex, extra, source);
    if (!ReadOnly) 
      sprintf(command+strlen(command), "<TD><A HREF=dgate%s?%s&mode=seriesdeleter&series=%%s:%%s>delete%%0.0s %%0.0s %%0.0s %%0.0s %%0.0s</A>", ex, extra);
    strcpy(command+strlen(command), "</TR>");
    SendServerCommand("", command, console);

    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "queryimages")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<table RULES=ALL BORDER=1>");
    HTML("<Caption>List of images with thumbnails on local server (maximal 50)</caption>");
    HTML("<TR><TD>Patient ID<TD>Name<TD>Date<TD>Image number<TD>Slice location<TD>Icon</TR>");

    strcpy(command,                 "query2:DICOMImages,DICOMSeries,DICOMStudies|DICOMStudies.patientid,DICOMImages.sopinstanc,DICOMStudies.patientnam,DICOMSeries.seriesdate,DICOMImages.imagenumbe,DICOMImages.slicelocat|");
    strcpy(command+strlen(command),  query);
    strcpy(command+strlen(command), " and DICOMStudies.studyinsta=DICOMSeries.studyinsta and DICOMSeries.seriesinst=DICOMImages.seriesinst");
    sprintf(command+strlen(command), "|<TR><TD><A HREF=dgate%s?%s&mode=sliceviewer&slice=%%s:%%s&size=%s&graphic=%s>%%0.0s %%0.0s %%0.0s %%0.0s", ex, extra, size, graphic);
    strcpy(command+strlen(command), "%s</A><TD>%0.0s %s<TD>%s<TD>%s<TD>%s");
    sprintf(command+strlen(command), "<TD><A HREF=dgate%s?%s&mode=headerdump&slice=%%s:%%s %%0.0s %%0.0s %%0.0s %%0.0s>", ex, extra);
    sprintf(command+strlen(command), "<IMG SRC=dgate%s?%s&mode=slice&slice=%%s:%%s&size=%s&graphic=%s></A></TR>", ex, extra, iconsize, graphic);
    strcpy(command+strlen(command), "|50");
    SendServerCommand("", command, console);

    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "queryallimages")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<table RULES=ALL BORDER=1>");
    HTML("<Caption>List of images on local server (all)</caption>");
    HTML("<TR><TD>Patient ID<TD>Name<TD>Date<TD>Image number<TD>Slice location<TD>Header</TR>");

    strcpy(command,                 "query:DICOMImages,DICOMSeries,DICOMStudies|DICOMStudies.patientid,DICOMImages.sopinstanc,DICOMStudies.patientnam,DICOMSeries.seriesdate,DICOMImages.imagenumbe,DICOMImages.slicelocat|");
    strcpy(command+strlen(command),  query);
    strcpy(command+strlen(command), " and DICOMStudies.studyinsta=DICOMSeries.studyinsta and DICOMSeries.seriesinst=DICOMImages.seriesinst");
    sprintf(command+strlen(command), "|<TR><TD><A HREF=dgate%s?%s&mode=sliceviewer&slice=%%s:%%s&size=%s&graphic=%s>%%0.0s %%0.0s %%0.0s %%0.0s", ex, extra, size, graphic);
    strcpy(command+strlen(command), "%s</A><TD>%0.0s %s<TD>%s<TD>%s<TD>%s");
    sprintf(command+strlen(command), "<TD><A HREF=dgate%s?%s&mode=headerdump&slice=%%s:%%s %%0.0s %%0.0s %%0.0s %%0.0s>", ex, extra);
    strcpy(command+strlen(command), "*</A></TR>");
    SendServerCommand("", command, console);

    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  /************************** remote query **************************/

  if (strcmp(mode, "patientfinder")==0)
  { HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<table RULES=ALL BORDER=1>");
    if (query[0]) HTML("<Caption>List of selected patients on %s</caption>", dest);
    else          HTML("<Caption>List of all patients on %s</caption>", dest);
    HTML("<TR><TD>ID<TD>Name</TR>");

    sprintf(command,                 "patientfinder:%s|%s", dest, query);
    sprintf(command+strlen(command), "|<TR><TD><A HREF=dgate%s?%s&mode=studyfinder&dest=%s&query==%%0.0s%%s>", ex, extra, dest);
    strcpy(command+strlen(command),  "%0.0s%s</A><TD>%s");
    SendServerCommand("", command, console);

    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "studyfinder")==0)
  { HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<table RULES=ALL BORDER=1>");
    if (query[0]) HTML("<Caption>List of selected studies on %s</caption>", dest);
    else          HTML("<Caption>List of all studies on %s</caption>", dest);
    HTML("<TR><TD>Date<TD>Modality<TD>Name<TD>Id<TD>UID</TR>");

    sprintf(command,                 "studyfinder:%s|%s", dest, query);
    sprintf(command+strlen(command), "|<TR><TD><A HREF=dgate%s?%s&mode=seriesfinder&dest=%s&query==%%0.0s%%0.0s%%0.0s%%0.0s%%s>", ex, extra, dest);
    strcpy(command+strlen(command),  "%s</A><TD>%s<TD>%s<TD>%s<TD>%s");
    sprintf(command+strlen(command), "<TD><A HREF=dgate%s?%s&mode=studymover&study=%%0.0s%%0.0s%%0.0s%%s:%%s&source=%s>push</A>", ex, extra, dest);
    strcpy(command+strlen(command), "</TR>");
    SendServerCommand("", command, console);

    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "seriesfinder")==0)
  { HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<table RULES=ALL BORDER=1>");
    if (query[0]) HTML("<Caption>List of selected series on %s</caption>", dest);
    else          HTML("<Caption>List of all series on %s</caption>", dest);
    HTML("<TR><TD>Date<TD>Time<TD>Modality<TD>Name<TD>ID<TD>UID</TR>");

    sprintf(command,                 "seriesfinder:%s|%s", dest, query);
    strcpy(command+strlen(command),  "|<TR><TD>%s<TD>%s<TD>%s<TD>%s<TD>%s%0.0s<TD>%s</TR>");
    sprintf(command+strlen(command), "<TD><A HREF=dgate%s?%s&mode=seriesmover&series=%%0.0s%%0.0s%%0.0s%%0.0s%%s:%%0.0s%%s&source=%s>push</A>", ex, extra, dest);
    SendServerCommand("", command, console);

    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  /* unused */
  if (strcmp(mode, "dicomfind")==0)
  { HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<FORM ACTION=\"dgate%s\">", ex);

    HTML("Find ");
    HTML("<select name=mode>");
    HTML("  <option value='patientfinder'>Patient</option>");
    HTML("  <option value='studyfinder'>Study</option>");
    HTML("  <option value='seriesfinder'>Series</option>");
    HTML("</select>");
    HTML("on server");

    HTML("<select name=dest>");
    SendServerCommand("", "get_amaps:<option value='%s'>%0.0s%0.0s%0.0s%s</option>", console);
    HTML("</select>");

    HTML("<INPUT NAME=port    TYPE=HIDDEN VALUE=%s>", (char *)Port);
    HTML("<INPUT NAME=address TYPE=HIDDEN VALUE=%s>", ServerCommandAddress);
    HTML("<INPUT NAME=source  TYPE=HIDDEN VALUE=%s>", source);
    HTML("<INPUT NAME=dest    TYPE=HIDDEN VALUE=%s>", dest);
    HTML("Search: <INPUT NAME=query TYPE=Text VALUE=>");
    HTML("<INPUT TYPE=SUBMIT VALUE=Go>");
    HTML("</FORM>");
    HTML("</BODY>");
    exit(0);
  };

  /************************** movers **************************/

  if (strcmp(mode, "studymover")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<table RULES=ALL BORDER=1>");
    HTML("<Caption>Send study: %s</caption>", study2);
    HTML("<TR><TD>Source<TD>Destination</TR>");

    for (i=0; i<1000; i++)
    { char com[80], buf[80];
      sprintf(com, "get_amap:%d,%%s", i);
      SendServerCommand("", com, 0, buf);
      if (buf[0]==0) break;
      if (strchr(buf, '*')==NULL)
      { sprintf(command, "<TR><TD>%s<TD><A HREF=dgate%s?%s&mode=movestudy&study=%s&source=%s&dest=%s>%s</A></TR>", source, ex, extra, study, source, buf, buf);
        HTML("%s", command);
      }
    }

    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "seriesmover")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<table RULES=ALL BORDER=1>");
    HTML("<Caption>Send series: %s</caption>", series2);
    HTML("<TR><TD>Source<TD>Destination</TR>");

    for (i=0; i<1000; i++)
    { char com[80], buf[80];
      sprintf(com, "get_amap:%d,%%s", i);
      SendServerCommand("", com, 0, buf);
      if (buf[0]==0) break;
      if (strchr(buf, '*')==NULL)
      { sprintf(command, "<TR><TD>%s<TD><A HREF=dgate%s?%s&mode=moveseries&series=%s&source=%s&dest=%s>%s</A></TR>", source, ex, extra, series, source, buf, buf);
        HTML("%s", command);
      }
    }

    HTML("</table>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "movestudy")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);
    sprintf(command, "movestudy:%s,%s,%s", source, dest, study2);
    SendServerCommand("", command, 0, buf);
    HTML("%s", command);
    HTML("<BR>");
    HTML("Done %s", study2);
    HTML("</BODY>");
    exit(0);
  }

  if (strcmp(mode, "moveseries")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);
    sprintf(command, "moveseries:%s,%s,%s", source, dest, series2);
    SendServerCommand("", command, 0, buf);
    HTML("%s", command);
    HTML("<BR>");
    HTML("Done %s", series2);
    HTML("</BODY>");
    exit(0);
  }

  /************************** delete **************************/

  if (!ReadOnly) 
  { if (strcmp(mode, "studydeleter")==0)
    { HTML("Content-type: text/html\n");
      HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
      HTML("<BODY BGCOLOR='CFDFCF'>");
      HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);
      HTML("<Caption>Delete study: %s</caption>", study);
  
      HTML("<FORM ACTION=\"dgate%s\">", ex);
      HTML("<INPUT NAME=mode    TYPE=HIDDEN VALUE=deletestudy>");
      HTML("<INPUT NAME=port    TYPE=HIDDEN VALUE=%s>", (char *)Port);
      HTML("<INPUT NAME=address TYPE=HIDDEN VALUE=%s>", ServerCommandAddress);
      HTML("<INPUT NAME=study   TYPE=HIDDEN VALUE='%s'>", study);
      HTML("<INPUT TYPE=SUBMIT VALUE='Delete the study'>");
      HTML("</FORM>");
      HTML("</BODY>");
      exit(0);
    };
  
    if (strcmp(mode, "seriesdeleter")==0)
    { HTML("Content-type: text/html\n");
      HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
      HTML("<BODY BGCOLOR='CFDFCF'>");
      HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);
      HTML("<Caption>Delete series: %s</caption>", series);
  
      HTML("<FORM ACTION=\"dgate%s\">", ex);
      HTML("<INPUT NAME=mode    TYPE=HIDDEN VALUE=deleteseries>");
      HTML("<INPUT NAME=port    TYPE=HIDDEN VALUE=%s>", (char *)Port);
      HTML("<INPUT NAME=address TYPE=HIDDEN VALUE=%s>", ServerCommandAddress);
      HTML("<INPUT NAME=series  TYPE=HIDDEN VALUE='%s'>", series);
      HTML("<INPUT TYPE=SUBMIT VALUE='Delete the series'>");
      HTML("</FORM>");
      HTML("</BODY>");
      exit(0);
    };
  
    if (strcmp(mode, "deletestudy")==0)
    { HTML("Content-type: text/html\n");
      HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
      HTML("<BODY BGCOLOR='CFDFCF'>");
      HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);
      sprintf(command, "deletestudy:%s", study2);
      SendServerCommand("", command, 0, buf);
      HTML(command);
      HTML("<BR>");
      HTML("Done");
      HTML("</BODY>");
      exit(0);
    }
  
    if (strcmp(mode, "deleteseries")==0)
    { HTML("Content-type: text/html\n");
      HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
      HTML("<BODY BGCOLOR='CFDFCF'>");
      HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);
      sprintf(command, "deleteseries:%s", series2);
      SendServerCommand("", command, 0, buf);
      HTML(command);
      HTML("<BR>");
      HTML("Done");
      HTML("</BODY>");
      exit(0);
    }
  }

  /************************** worklist browser and editor **************************/

  if (strcmp(mode, "queryworklist")==0)
  { HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<table RULES=ALL BORDER=1>");
    if (query[0]) HTML("<Caption>List of selected worklist entries</caption>");
    else          HTML("<Caption>List of all worklist entries (not all fields displayed)</caption>");
    HTML("<TR><TD>AccessionN<TD>Patient ID<TD>Name<TD>Birth date<TD>Sex<TD>Physician<TD>Description<TD>Modality<TD>Date<TD>Time</TR>");

    strcpy(command,                  "query:dicomworklist|AccessionN,PatientID,PatientNam,PatientBir,PatientSex,ReqPhysici,ReqProcDes,Modality,StartDate,StartTime|");
    strcpy(command+strlen(command),  query);
    sprintf(command+strlen(command), "|<TR><TD><A HREF=dgate%s?%s&mode=editrecord&db=dicomworklist&query=AccessionN+=+'%%s'>%%0.0s %%0.0s %%0.0s %%0.0s %%0.0s %%0.0s %%0.0s %%0.0s %%0.0s", ex, extra);
    sprintf(command+strlen(command), "%%s</A><TD>%%s<TD>%%s<TD>%%s<TD>%%s<TD>%%s<TD>%%s<TD>%%s<TD>%%s<TD>%%s<TD><A HREF=dgate%s?%s&mode=deleterecord&db=dicomworklist&query=AccessionN+=+'%%s'>Delete</A></TR>", ex, extra);
    SendServerCommand("", command, console);

    HTML("</table>");

    sprintf(command, "<A HREF=dgate%s?%s&mode=addrecord&db=dicomworklist>Add worklist entry</A>", ex, extra);
    HTML(command);
    HTML("</BODY>");
    exit(0);
  };

  /************************** general purpose database editing **************************/

  if (strcmp(mode, "editrecord")==0)
  { HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    if (!DBE)
    { HTML("<H2>File DICOM.SQL not found - please check configuration</H2>");
      exit(0);
    }

    HTML("<FORM ACTION=\"dgate%s\">", ex);
    if (!ReadOnly) HTML("<INPUT TYPE=HIDDEN NAME=mode  VALUE=saverecord>");
    HTML("<INPUT NAME=port    TYPE=HIDDEN VALUE=%s>", (char *)Port);
    HTML("<INPUT NAME=address TYPE=HIDDEN VALUE=%s>", ServerCommandAddress);
    HTML("<INPUT NAME=db      TYPE=HIDDEN VALUE=%s>", db);
    HTML("<INPUT NAME=query   TYPE=HIDDEN VALUE=%s>", query);

    HTML("<table RULES=ALL BORDER=2>");
    HTML("<Caption>Edit %s entry</caption>", db);
    HTML("<TR><TD>Field<TD>Value</TR>");

    strcpy(command, "query2:");
    strcat(command, db);
    strcat(command, "|");
    i = 0;
    while ( TRUE )
    { if(!DBE[i].Element) break;			// end of fields
      if (DBE[i].DICOMType!=DT_STARTSEQUENCE && DBE[i].DICOMType!=DT_ENDSEQUENCE)
      {	strcat(command, DBE[i].SQLColumn);
	strcat(command, ",");
      }
      ++i;
    }
    command[strlen(command)-1]=0;			// remove trailing ,

    strcat(command, "|");
    strcat(command, query);
    strcat(command, "|");
    
    i = 0;
    while ( TRUE )
    { if(!DBE[i].Element) break;			// end of fields
      if (DBE[i].DICOMType!=DT_STARTSEQUENCE && DBE[i].DICOMType!=DT_ENDSEQUENCE)
      {	strcat(command, "<TR><TD>");
      	strcat(command, DBE[i].SQLColumn);
	strcat(command, "<TD><INPUT NAME=");
      	strcat(command, DBE[i].SQLColumn);
      	strcat(command, " TYPE=Text VALUE='%s'></TR>");
      }
      ++i;
    };
    strcat(command, "|1");	// max 1 record !!!!!
    
    SendServerCommand("", command, console);

    HTML("</table>");
    if (!ReadOnly) HTML("<INPUT TYPE=SUBMIT VALUE=Save>");
    HTML("</FORM>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "addrecord")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    if (!DBE)
    { HTML("<H2>File DICOM.SQL not found - please check configuration</H2>");
      exit(0);
    }

    HTML("<FORM ACTION=\"dgate%s\">", ex);
    if (!ReadOnly) HTML("<INPUT TYPE=HIDDEN NAME=mode VALUE=saverecord>");
    HTML("<INPUT NAME=db      TYPE=HIDDEN VALUE=%s>", db);
    HTML("<INPUT NAME=port    TYPE=HIDDEN VALUE=%s>", (char *)Port);
    HTML("<INPUT NAME=address TYPE=HIDDEN VALUE=%s>", ServerCommandAddress);

    HTML("<table RULES=ALL BORDER=2>");
    sprintf(command, "<Caption>Add %s entry</caption>", db);
    HTML(command);

    HTML("<TR><TD>Field<TD>Value</TR>");

    i = 0;
    while ( TRUE )
    { if(!DBE[i].Element) break;			// end of fields
      if (DBE[i].DICOMType!=DT_STARTSEQUENCE && DBE[i].DICOMType!=DT_ENDSEQUENCE)
      {	strcpy(command, "<TR><TD>");
      	strcat(command, DBE[i].SQLColumn);
	strcat(command, "<TD><INPUT NAME=");
      	strcat(command, DBE[i].SQLColumn);
      	strcat(command, " TYPE=Text VALUE=''></TR>");
      	HTML(command);
      }
      ++i;
    }

    HTML("</table>");
    if (!ReadOnly) HTML("<INPUT TYPE=SUBMIT VALUE=Save>");
    HTML("</FORM>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "saverecord")==0)
  { HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    if (!DBE)
    { HTML("<H2>File DICOM.SQL not found - please check configuration</H2>");
      exit(0);
    }

    if (ReadOnly || DBE!=WorkListDB)
    { HTML("<H2>This table is readonly</H2>");
      exit(0);
    }
    
    if (strlen(query))
    { sprintf(command, "deleterecord:%s,%s", db, query); 
      SendServerCommand("", command, console);
      sprintf(command, "Updated/added %s entry for %s", db, query);
      HTML(command);
    }
    else
    { sprintf(command, "Added new record in %s", db);
      HTML(command);
    };
     
    sprintf(command, "addrecord:%s|", db);
    i = 0;
    while ( TRUE )
    { if(!DBE[i].Element) break;			// end of fields
      if (DBE[i].DICOMType!=DT_STARTSEQUENCE && DBE[i].DICOMType!=DT_ENDSEQUENCE)
      {	strcat(command, DBE[i].SQLColumn);
	strcat(command, ",");
      }
      ++i;
    }
    command[strlen(command)-1]=0;			// remove trailing ,
    strcat(command, "|");

    i = 0;
    while ( TRUE )
    { if(!DBE[i].Element) break;			// end of fields
      if (DBE[i].DICOMType!=DT_STARTSEQUENCE && DBE[i].DICOMType!=DT_ENDSEQUENCE)
      {	strcat(command, "'"); 
        CGI(buf, DBE[i].SQLColumn, ""); 
        strcat(command, buf); 
        strcat(command, "', "); 
      }
      ++i;
    }
    command[strlen(command)-2]=0;			// remove trailing , and space

    SendServerCommand("", command, console);
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "deleterecord")==0)
  { HTML("Content-type: text/html\nCache-Control: no-cache\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    if (ReadOnly || DBE!=WorkListDB)
    { HTML("<H2>This table is readonly</H2>");
      exit(0);
    }

    sprintf(command, "deleterecord:%s,%s", db, query); 
    SendServerCommand("", command, console);
    HTML("Deleted record entry for ");
    HTML(db);
    HTML("Where ");
    HTML(query);
    HTML("</BODY>");
    exit(0);
  };

  /************************** viewers **************************/

  /* page with one slice */
  if (strcmp(mode, "sliceviewer")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<A HREF=dgate%s?%s&mode=headerdump&slice=%s>", ex, extra, slice);
    HTML("<IMG SRC=dgate%s?%s&mode=slice&slice=%s&size=%s&graphic=%s&lw=%s>", ex, extra, slice, size, graphic, lw);
    HTML("</A>");
    HTML("</BODY>");
    exit(0);
  };

  if (strcmp(mode, "headerdump")==0)
  { HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);
    HTML("<BR>Header dump of DICOM object:<BR>");
    HTML("<PRE>");

    sprintf(command, "dump_header:%s", slice2);
    SendServerCommand("", command, console, NULL, FALSE);

    HTML("</PRE>");
    HTML("</BODY>");
    exit(0);
  };

  /* just generates the bitmap */
  if (strcmp(mode, "slice")==0)
  { sprintf(command, "convert_to_%s:%s,%s,cgi,%s", graphic, slice2, size, lw);
    SendServerCommand("", command, console, NULL, FALSE);
    exit(0);
  };

  /* transmits the image contents in dicom format */
  if (strcmp(mode, "dicom")==0)
  { sprintf(command, "convert_to_dicom:%s,%s,%s", slice2, dsize, compress);
    SendServerCommand("", command, console, NULL, FALSE);
    exit(0);
  };

  /* k-pacs viewer in an OCX; internet explorer only */
  if (strcmp(mode, "seriesviewer")==0)
  { char *p;

    HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);

    HTML("<OBJECT");
    HTML("          classid=\"clsid:0AA480F2-78EB-4A31-B4C8-F981C5004BBB\"");
    HTML("          codebase=\"%sActiveFormProj1.ocx\"", WebCodeBase);
    HTML("          width=%s", size);
    HTML("          height=%s", size);
    HTML("          align=center");
    HTML("          hspace=0");
    HTML("          vspace=0");
    HTML(">");
    HTML("<PARAM name=DCMFilelist value=");

    p = strchr(series2, ':');
    if (p) *p = '|';
    sprintf(command, "imagelister:local|%s|:%s?%s&mode=dicom&slice=%%s:%%s&dsize=%s&compress=%s*", series2, WebScriptAddress, extra, dsize, compress);
    SendServerCommand("", command, console);

    HTML(">");
    HTML("</OBJECT>");
    HTML("</BODY>");
    exit(0);
  };

  /* k-pacs viewer in an OCX; internet explorer only; data access through http served image files */
  if (strcmp(mode, "seriesviewer2")==0)
  { char wwwserveraddress[512] = "http://";
    strcat(wwwserveraddress, getenv("SERVER_NAME"));

    HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);
    HTML("<OBJECT");
    HTML("          classid=\"clsid:0AA480F2-78EB-4A31-B4C8-F981C5004BBB\"");
    HTML("          codebase=\"%sActiveFormProj1.ocx\"", WebCodeBase);
    HTML("          width=%s", size);
    HTML("          height=%s", size);
    HTML("          align=center");
    HTML("          hspace=0");
    HTML("          vspace=0");
    HTML(">");
    HTML("<PARAM name=DCMFilelist value=");

    char *p = strchr(series2, ':');
    if (p) *p = '|';

    if (memcmp(wwwserveraddress, "file", 4)==0)
      sprintf(command, "imagelister:local|%s", series2);
    else
      sprintf(command, "imagelister:local|%s|@%s/%%0.0s%%s*", series2, WebMAG0Address);

    SendServerCommand("", command, console);

    HTML(">");
    HTML("</OBJECT>");
    HTML("</BODY>");
    exit(0);
  };

  /* no viewer */
  if (strcmp(mode, "noviewer")==0)
  { char *p;

    HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s</H2>", VERSION);
    HTML("<HR>");
    HTML("<H2>No viewer is installed</H2>");
    HTML("</OBJECT>");
    HTML("</BODY>");
    exit(0);
  };

  /* The Japanese java-based viewer; requires some modifications to work */
  if (strcmp(mode, "aiviewer")==0)
  { char *p;

    HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s with AiViewer</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s with AiViewer</H2>", VERSION);

    HTML("<APPLET");
    HTML("  CODEBASE = '%sdicomviewer100'", WebCodeBase);
    HTML("  CODE = 'dicomviewer.Viewer.class'");
    HTML("  NAME = 'Viewer.java'");
    HTML("  WIDTH = '100%%'");
    HTML("  HEIGHT = '90%%'");
    HTML("  HSPACE = 0");
    HTML("  VSPACE = 0");
    HTML("  ALIGN = middle >");
    HTML("<PARAM NAME = tmpSize VALUE = 1>");
    HTML("<PARAM NAME = NUM VALUE = 0>");
    HTML("<PARAM NAME = currentNo VALUE = 0>");
    HTML("<PARAM NAME = dicURL VALUE = '%sdicomviewer100/Dicom.dic'>", WebCodeBase);
    HTML("<PARAM NAME = imgURL0 VALUE = ");

    p = strchr(series2, ':');
    if (p) *p = '|';
    sprintf(command, "imagelister:local|%s|:%s?%s&mode=dicom&slice=%%s:%%s&dsize=%s&compress=%s*", series2, WebScriptAddress, extra, dsize, "un");
    SendServerCommand("", command, console);

    HTML(">");

    HTML("</APPLET>");
    HTML("</BODY>");
    exit(0);
  };

  /* very simple jave-script based viewer with server side processing */
  if (strcmp(mode, "serversideviewer")==0)
  { char *p;

    HTML("Content-type: text/html\n");
    HTML("<HEAD><TITLE>Conquest DICOM server - version %s server side viewer</TITLE></HEAD>", VERSION);
    HTML("<BODY BGCOLOR='CFDFCF'>");
    HTML("<H2>Conquest DICOM server - version %s server side viewer</H2>", VERSION);

    HTML("<SCRIPT language=JavaScript>");
    HTML("function load()");
    HTML("{ document.images[0].src = 'dgate%s?%s&mode=slice'+", ex, extra);
    HTML("                           '&graphic=' + document.forms[0].graphic.value +");
    HTML("                           '&slice='   + document.forms[0].slice.value +");
    HTML("                           '&lw='      + document.forms[0].level.value + '/' + document.forms[0].window.value");
    HTML("}");
    HTML("</SCRIPT>");

    HTML("<IMG SRC=loading.jpg BORDER=0 HEIGHT=%s>", size);

    HTML("<FORM>");
    HTML("Slice: ");
    HTML("  <select name=slice onchange=load()>");

    p = strchr(series2, ':');
    if (p) *p = '|';
    sprintf(command, "imagelister:local|%s|:<option value=%%s:%%s>%%04d</option>", series2);
    SendServerCommand("", command, console);

    HTML("  </select>");

    HTML("Level:");
    HTML("  <INPUT name=level size=4 onchange=load() >");
    HTML("Window:");
    HTML("  <INPUT name=window size=4 onchange=load() >");
    HTML("Color:");
    HTML("  <select name=graphic onchange=load()>");
    if (strcmp(graphic, "bmp")==0)
    { HTML("    <option value=bmp>yes</option>");
      HTML("    <option value=gif>no</option>");
    }
    else
    { HTML("    <option value=gif>no</option>");
      HTML("    <option value=bmp>yes</option>");
    }
    HTML("  </select>");
    HTML("  <INPUT TYPE=BUTTON VALUE=load onclick=load() >");
    HTML("</FORM>");

    HTML("<SCRIPT language=JavaScript>");
    HTML("  document.onload=load()");
    HTML("</SCRIPT>");

    HTML("</BODY>");
    exit(0);
  };

  /************************** general purpose web scripting **************************/

/*
This is a general purpose web script processor; it can be used to create any web page, not just viewers.

This is a sample from dicom.ini:

[flexviewer]
line0 = <HEAD><TITLE>Conquest DICOM server - version %version% and %windowname%</TITLE></HEAD>
line1 = <BODY BGCOLOR='CFDFCF'>
line2 = <H2>Conquest DICOM server - version %version% and AiViewer v1.00</H2>
line3 = <APPLET CODEBASE=%webcodebase%dicomviewer100 CODE='dicomviewer.Viewer.class'
line4 = NAME='Viewer.java' WIDTH='100%' HEIGHT='90%' HSPACE=0 VSPACE=0 ALIGN=middle >
line5 = <PARAM NAME = tmpSize VALUE = 10>
line6 = <PARAM NAME = NUM VALUE = 0>
line7 = <PARAM NAME = currentNo VALUE = 0>
line8 = <PARAM NAME = dicURL VALUE = '%webcodebase%dicomviewer100/Dicom.dic'>
line9 = <PARAM NAME = imgURL0 VALUE = 
line10 = --imagelister:local|%patid%|%seruid%|:%webscriptadress%?%extra%&mode=dicom&slice=%s:%s&compress=un*
line11 = >
line12 = </APPLET>
line13 = </BODY>
# this is the default; the variable can be passed in the url
windowname = AiViewer V1.00

[flexviewer2]
source = flexviewer2.cq
windowname = AiViewer V1.00
*/

  /* check mode of web page against dicom.ini sections */
  GetPrivateProfileString ( mode, "source", "", buf, 1000, ConfigFile);
  GetPrivateProfileString ( mode, "line0", buf, buf, 1000, ConfigFile);
  if (buf[0])
  { char *p;
    char string[1000], temp[1000], server[1000], patid[66], patid2[200], seruid[66], studyuid[66], sopuid[66];
    FILE *f=NULL;

    /* create several variables useful for scripting */

    sprintf(server, "%s?%s", WebScriptAddress, extra);

    patid[0] = seruid[0] = studyuid[0] = sopuid[0] = 0;

    if (study2[0])
    { strcpy(temp, study2);
      p = strchr(temp, ':');
      if (p) 
      { *p = 0;
        strcpy(patid2, temp);
        *p = '|';
        strcpy(studyuid, p+1);
      }
    
      strcpy(temp, study);
      p = strchr(temp, ':');
      if (p) 
      { *p = 0;
        strcpy(patid, temp);
      }
    }

    if (series2[0])
    { strcpy(temp, series2);
      p = strchr(temp, ':');
      if (p) 
      { *p = 0;
        strcpy(patid2, temp);
        *p = '|';
        strcpy(seruid, p+1);
      }
    
      strcpy(temp, series);
      p = strchr(temp, ':');
      if (p) 
      { *p = 0;
        strcpy(patid, temp);
      }
    }

    if (slice2[0])
    { strcpy(temp, slice2);
      p = strchr(temp, ':');
      if (p) 
      { *p = 0;
        strcpy(patid2, temp);
        *p = '|';
        strcpy(sopuid, p+1);
      }
    
      strcpy(temp, slice);
      p = strchr(temp, ':');
      if (p) 
      { *p = 0;
        strcpy(patid, temp);
      }
    }

    /* contents may come from file or from dicom.ini */
    GetPrivateProfileString ( mode, "source", "", temp, 1000, ConfigFile);
    if (temp[0]) f = fopen(temp, "rt");

    GetPrivateProfileString ( mode, "header", "Content-type: text/html\n", temp, 1000, ConfigFile);
    while (p = strstr(temp, "\\")) p[0]='\n';
    HTML(temp);

    for (i=0; i<10000; i++)
    { if (f)
      { if (fgets(string, sizeof(string), f) == NULL)
          break;
        if (string[strlen(string)-1]=='\n') string[strlen(string)-1]=0;
      }
      else
      { sprintf(temp, "line%d", i);
        GetPrivateProfileString ( mode, temp, "", string, 1000, ConfigFile);
        if (string[0]==0) break;
      }

      /* fill in predefined scripting variables */

      replace(string, "%query_string%",    getenv( "QUERY_STRING" ));
      replace(string, "%server_name%",     getenv( "SERVER_NAME" ));
      replace(string, "%script_name%",     getenv( "SCRIPT_NAME" ));
      replace(string, "%path_translated%", getenv( "PATH_TRANSLATED" ));

      replace(string, "%port%",            (char *)Port);
      replace(string, "%address%",         ServerCommandAddress);
      replace(string, "%webcodebase%",     WebCodeBase);
      replace(string, "%webscriptadress%", WebScriptAddress);
      replace(string, "%extra%",           extra);
      replace(string, "%server%",          server);
      replace(string, "%version%",         VERSION);
      replace(string, "%mode%",            mode);

      replace(string, "%series%",          series2); // unprocessed
      replace(string, "%series2%",         series);  // replaced spaces by %20
      replace(string, "%slice%",           slice2);  // unprocessed
      replace(string, "%slice2%",          slice);   // replaced spaces by %20
      replace(string, "%study%",           study2);  // unprocessed
      replace(string, "%study2%",          study);   // replaced spaces by %20
      replace(string, "%patid%",           patid2);  // unprocessed
      replace(string, "%patid2%",          patid);   // replaced spaces by %20
      replace(string, "%seruid%",          seruid);
      replace(string, "%studyuid%",        studyuid);
      replace(string, "%sopuid%",          sopuid);

      replace(string, "%size%",            size);
      replace(string, "%dsize%",           dsize);
      replace(string, "%compress%",        compress);
      replace(string, "%iconsize%",        iconsize);
      replace(string, "%graphic%",         graphic);
      replace(string, "%viewer%",          viewer);
      replace(string, "%lw%",              lw);

      replace(string, "%query%",           query);
      replace(string, "%db%",              db);
      replace(string, "%source%",          source);
      replace(string, "%dest%",            dest);
      replace(string, "%patientidmatch%",  patientidmatch);
      replace(string, "%patientnamematch%",patientnamematch);
      replace(string, "%studydatematch%",  studydatematch);
      replace(string, "%startdatematch%",  startdatematch);

      /* this code will substitute any other %var% with a cgi variable 
         with a default given in section for this server mode in dicom.ini 
      */

      char *p = strchr(string, '%');
      if (p)
      { char *q = strchr(p+1, '%');
        if (q && q!=p+1)
        { char var[512], val[512], var2[512];
          *q=0;
          strcpy(var, p+1);
          *q='%';;
          strcpy(var2, "%");
          strcat(var2, var);
          strcat(var2, "%");

          GetPrivateProfileString ( mode, var, var2, val, 512, ConfigFile);
          CGI(val, var, val);
          replace(string, var2, val);
        }
      }

      /* runs: #comment, --servercommand, or HTML output */
      if (string[0]=='#')                        strcpy(string, "");
      else if (string[0]=='-' && string[1]=='-') SendServerCommand("", string+2, console);
      else                                       HTML("%s", string);
    };

    if (f) fclose(f);
    exit(0);
  };

  exit(0);
}

/* Dicom web viewer works as follows:

client		-> 	webserver			url		http://127.0.0.1/scripts/dgate.exe?mode=seriesviewer&series=...
webserver 	-> 	dicomserver 			query		imagelister:local|.....
client  	<- 	webserver <- dicomserver	query results	(to build list of urls of dicom slices)
client  	<- 	webserver			activex control http://127.0.0.1/ActiveFormProj1.ocx

Then for each slice that is required:

control 	-> 	webserver			url of slice	http://127.0.0.1/scripts/dgate.exe?mode=dicom&slice=......
webserver	-> 	dicomserver			dicom request   convert_to_dicom:....
control 	<- 	webserver <- dicomserver	dicom data

*/

//////////////////////////////////////////////////////////////////
// Elementary HL7 interface
//////////////////////////////////////////////////////////////////

// These contain HL7 DateTime code: provide .DATE and .TIME split, note: should start/end with |

static char HL7DateTimeTypes[]=
"|OBR.7|OBR.8|OBR.22|PID.7|PID.29|PV1.44|PV1.45|TXA.4|TXA.6|TXA.7|TXA.8|OBR.6|OBR.36|MSH.6|OBX.12|OBX.14|ORC.9|ORC.15|";

// read next item from data passed in p; data->item, type->name, tmp->internal
// note that data, type, and tmp are also used to maintain the parser state

void parseHL7(char **p, char *data, char *type, char *tmp, char *HL7FieldSep, char *HL7SubFieldSep, char *HL7RepeatSep)
{ int field;
  char *q;
  char t[32];
  int i, dots=0;

  sprintf(t, "|%s|", type);								// to seek types
  if (strlen(type)>2)									// count .N
    for (i=0; i<strlen(type)-1; i++) dots += (type[i]=='.' && type[i+1]>='0' && type[i+1]<='9');

  if (**p==0)
  { strcpy(type, "EOM");								// end of message
    data[0]=0;
  }
  else if (strstr(HL7DateTimeTypes, t))							// translate datetime type
  { tmp[0] = 0;
    if (strlen(data)>8) strcpy(tmp, data+8);						// get time for later use
    data[8]=0;
    strcat(type, ".DATE");								// XXX.N.DATE
  }
  else if (strstr(type, ".DATE")>0)							// date was returned, now get time
  { strcpy(data, tmp);									// time
    strcpy(type+strlen(type)-4, "TIME");						// XXX.N.TIME
  }
  else if (strchr(data, *HL7SubFieldSep))						// translate field type, first entry
  { q = strchr(data, *HL7SubFieldSep);
    if (q) *q=0;
    if (q) strcpy(tmp, q+1);
    else   tmp[0]=0;

    strcat(type, ".0");									// XXX.N.0
  }
  else if (strchr(data, *HL7RepeatSep))							// translate repeat type, first entry
  { q = strchr(data, *HL7RepeatSep);
    if (q) *q=0;
    if (q) strcpy(tmp, q+1);
    else   tmp[0]=0;

    strcat(type, ".0");									// XXX.N.0
  }
  else if (dots>1)									// process subfields/repeats
  { if (tmp[0]==0)
    { q = strrchr(type, '.');
      if (q) *q=0;
      strcat(type, ".END");								// internal token
      data[0]=0;
    }
    else
    { strcpy(data, tmp);
              q = strchr(data, *HL7SubFieldSep);
      if (!q) q = strchr(data, *HL7RepeatSep);
      if (q) *q=0;
      if (q) strcpy(tmp, q+1);
      else   tmp[0]=0;

      q = strrchr(type, '.');
      sprintf(q+1, "%d", atoi(q+1)+1);							// XXX.N.M
    }
  }
  else if (**p==0x0d)
  { strcpy(type, "EOS"); strcpy(data, ""); (*p)+=1;					// end of segment
    if (**p==0x0a) (*p)+=1;
    if (strncmp(*p, "MSH", 3)==0) strcpy(type, "EOM");					// peek ahead for end of message
  }
  else if (strcmp(type, "EOS")==0 || strcmp(type, "EOM")==0 || strcmp(type, "")==0)	// new segment
  { field = 0;
    if (strncmp(*p, "MSH", 3)==0)
    { *HL7FieldSep    = (*p)[3];
      *HL7SubFieldSep = (*p)[4];
      *HL7RepeatSep   = (*p)[5];
      strncpy(type, *p, 3); strcat(type, ".0");
      strncpy(data, *p, 3); 
      (*p)+=4; 
    }
    else if ((*p)[0]>='A' && (*p)[0] <='Z' && 						// message header 
        (*p)[1]>='A' && (*p)[1] <='Z' &&
        (((*p)[2]>='A' && (*p)[2] <='Z') || ((*p)[2]>='0' && (*p)[2] <='9')))
    { strncpy(type, *p, 3); strcat(type, ".0");
      strncpy(data, *p, 3); 
      (*p)+=4; 
    }
    else
    { strcpy(type, "UNK.0");
      data[0]=0;
    }
  }
  else
  { field = atoi(type+4);								// genererate new segment
    sprintf(type+4, "%d", field+1);

    q = strchr(*p, *HL7FieldSep);							// upto segment separator
    if (q)
    { *q=0;
      strncpy(data, *p, 255);
      data[255]=0;
      *q = *HL7FieldSep;
      *p = q+1;
    }
    else
    { q = strchr(*p, 0x0d);								// or 0x0d
      if (q)
      { *q = 0;
        strncpy(data, *p, 255);
        data[255]=0;
        *q = 0x0d;
        *p = q;										// process 0x0d again
      }
      else
      { strcpy(data, "");
        strcpy(type, "ERR");								// internal token
      }
    }
  }

  SystemDebug.printf("HL7 item: %s, contents: %s\n", type, data);  
}

// load HL7 data into modality worklist using the translation table from dicom.sql

void ProcessHL7Data(char *data)
{ char *p=data;
  char fields[1024], values[4096], command[8192], type[32], item[256], uid[66], tmp[256];
  int i;
  
  char HL7FieldSep    = '|';
  char HL7SubFieldSep = '^';
  char HL7RepeatSep   = '~';

  Database DB;
  if (!DB.Open ( DataSource, UserName, Password, DataHost ) ) 
  { OperatorConsole.printf("*** HL7 import: cannot open database");
    return;
  }

  fields[0]=0;
  values[0]=0;
  type[0]=0;	// used for context of parser
  tmp[0]=0;

  while (TRUE)
  { parseHL7(&p, item, type, tmp, &HL7FieldSep, &HL7SubFieldSep, &HL7RepeatSep);
    if (strcmp(type, "ERR")==0) break;			// error

    // search type in database; if found prepare strings to write
    i = 0;
    while ( TRUE )
    { if(!WorkListDB[i].Element) break;
      if (WorkListDB[i].DICOMType!=DT_STARTSEQUENCE && WorkListDB[i].DICOMType!=DT_ENDSEQUENCE)
      {	if (strcmp(type, WorkListDB[i].HL7Tag)==0)
        { strcat(fields, WorkListDB[i].SQLColumn);
          strcat(fields, ",");

          strcat(values, "'");
          item[WorkListDB[i].SQLLength]=0;		// truncate data to make fit
          strcat(values, item);
          strcat(values, "',");
        }
      }
      ++i;
    }

    // on end of message save the data into the database

    if (strcmp(type, "EOM")==0)
    { // search for special items (start with *) that are not read from the HL7 parser
      i = 0;
      while ( TRUE )
      { if(!WorkListDB[i].Element) break;

        if (WorkListDB[i].DICOMType!=DT_STARTSEQUENCE && WorkListDB[i].DICOMType!=DT_ENDSEQUENCE)
        { if (WorkListDB[i].HL7Tag[0]=='*')
          { strcat(fields, WorkListDB[i].SQLColumn);
            strcat(fields, ",");

            strcat(values, "'");		

            if (strcmp("*AN", WorkListDB[i].HL7Tag)==0)		// generate new accession number
            { GenUID(uid);
              strcat(values, uid + strlen(uid)-16);
            }
            else if (strcmp("*UI", WorkListDB[i].HL7Tag)==0)	// generate new uid
            { GenUID(uid);
              strcat(values, uid);
            }

            strcat(values, "',");
          }
        }
        ++i;
      }

      fields[strlen(fields)-1]=0;	// remove trailing ,
      values[strlen(values)-1]=0;	// remove trailing ,

      DB.AddRecord("dicomworklist", fields, values);
      SystemDebug.printf("Entering modality worklist fields: %s\n", fields);  
      QueryFromGui++;

      fields[0]=0;
      values[0]=0;

      HL7FieldSep    = '|';
      HL7SubFieldSep = '^';
      HL7RepeatSep   = '~';
    }

    if (strcmp(type, "EOM")==0)		// end of message
      if (*p==0) break;			// end of file
  }

  DB.Close();
}


