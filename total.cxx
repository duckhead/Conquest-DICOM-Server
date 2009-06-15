/* 
20020822	mvh	This file compiles to TOTAL.EXE = DGATE.EXE with, e.g., BC55 
20050102	mvh	Added NOINTJPEG flag: jpeg8/12/16 sources cannot be compiled at once
20050107	mvh	Changed order of compile
20050111	mvh	Added npipe.cpp and gpps.cpp for LINUX
20050118	mvh	Documented how to compile under linux and with visual c++
20050121	mvh	Changed filename to lower case
20050130	mvh	Added xvgifwr.c
20070315	mvh	bcb added DARWIN stuff
20070330	mvh	bcb added DARWIN compile example
20080820	mvh	Solaris compile example
*/

/* Example to compile with BC55:
   bcc32 -v 							# source level debugging
	 -g255 							# max number of warnings
	 -lS:1000000 						# stack size
         -P 							# force C++ compile
	 -DWINDOWS -DWIN32 -DNATIVE_ENDIAN=1 -DNOINTJPEG	# defines
         -I\bc55\include;\conquest\cqdicom;\conquest\dgate 	# include directories (bcc, dicom lib, and dgate parts)
         -L\bc55\lib						# library directory
         total.cxx 						# this source file
         cw32mt.lib 						# multi-thread library

   Example to compile under linux:
   g++ -DUNIX -DNATIVE_ENDIAN=1 -DNOINTJPEG total.cxx -o dgate -lpthread

   Example to compile with visual c++:
   set path=e:\lang\msdev\bin
   set include=e:\lang\msdev\include
   set lib=e:\lang\msdev\lib
   cl /Zi /MD /O2 /DWIN32 /DWINDOWS /DNATIVE_ENDIAN=1 /DNOINTJPEG /FeDGATE.EXE total.cxx odbc32.lib odbccp32.lib wsock32.lib advapi32.lib user32.lib kernel32.lib
	
   Example to compile under OS X xcode using 10.4u sdk on a PowerPC:
   g++ -isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch ppc -Wno-multichar -I/usr/local/mysql/include 
		-L/usr/local/mysql/lib -DDARWIN -DUSEMYSQL -DNOINTJPEG -DB_DEBUG -o dgate total.cxx
			-lpthread -lgcc_s.10.4 -lstdc++.6 -lmysqlclient -lz

   Example to compile under SOLARIS 10:
   /usr/sfw/bin/g++ -DUNIX -DNATIVE_ENDIAN=1 -DNOINTJPEG -DSOLARIS total.cxx -o dgate -lpthread -lsocket -lnsl -lposix4
*/

#define _SH_DENYRW      0x10    /* deny read/write mode */
#define _SH_DENYWR      0x20    /* deny write mode */
#define _SH_DENYRD      0x30    /* deny read mode */
#define _SH_DENYNO      0x40    /* deny none mode */
#define _P_WAIT         0

#ifdef DARWIN /* a flavor of bsd unix */
#define BSD
#endif
#ifdef BSD
#define UNIX
#endif

#include "filepdu.cxx"
#include "vrtosql.cpp"
#include "parse.cpp"
#include "loadddo.cpp"
#include "lex.cpp"
#include "dprintf.cpp"
#include "dgatefn.cpp"
#include "device.cpp"
#include "amap.cpp"
#include "dbsql.cpp"
#include "rtc.cxx"
#include "flpdu.cxx"
#include "qrsop.cxx"
#include "storage.cxx"
#include "verify.cxx"
#include "dimsen.cxx"
#include "dimsec.cxx"
#include "util.cxx"		// zeromem, spacemem
#include "uniq.cxx"		// uniq16, uniq8
//#include "safemem.cxx"
#include "endian.cxx"
#include "trnsyn.cxx"
#include "pdu.cxx"
#include "pdata.cxx"
#include "aarj.cxx"
#include "aaac.cxx"
#include "aarq.cxx"
#include "deivr.cxx"
#include "socket.cxx"
#include "buffer.cxx"
#include "odbci.cpp"
#include "nkiqrsop.cxx"
#include "regen.cpp"
#include "dgate.cpp"
#include "xvgifwr.c"

#ifdef UNIX
#  include "gpps.cpp"
#  include "npipe.cpp"
#endif
