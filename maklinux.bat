REM COPY SOURCE TO LINUX SYSTEM

set source=\quirt\app\conquest\dicomserver\
set dest=\\Dell\homes\conquest

copy %source%maklinux.bat                                   %dest%
copy %source%ActiveFormProj1.ocx                            %dest%
copy %source%manuals\DicomConformance_FilesLST_Changes.pdf  %dest%
copy %source%manuals\windowsmanual.pdf                      %dest%
copy %source%manuals\linuxmanual.pdf                        %dest%

set source=\quirt\comps\exe\dgate\
set dest=\\Dell\homes\conquest

copy %source%vrtosql.cpp  %dest%
copy %source%regen.cpp    %dest%
copy %source%parse.cpp    %dest%
copy %source%nkiqrsop.cxx %dest%
copy %source%loadddo.cpp  %dest%
copy %source%lex.cpp      %dest%
copy %source%dprintf.cpp  %dest%
copy %source%dgatefn.cpp  %dest%
copy %source%device.cpp   %dest%
copy %source%amap.cpp     %dest%
copy %source%dbsql.cpp    %dest%
copy %source%odbci.cpp    %dest%
copy %source%dgate.cpp    %dest%
copy %source%total.cxx    %dest%
copy %source%xvgifwr.c    %dest%

copy %source%dgate.hpp    %dest%
copy %source%dprintf.hpp  %dest%
copy %source%lex.hpp      %dest%
copy %source%nkiqrsop.hpp %dest%
copy %source%odbci.hpp    %dest%

copy %source%wintypes.hpp %dest%  
copy %source%npipe.hpp    %dest%
copy %source%npipe.cpp    %dest%
copy %source%gpps.hpp     %dest%
copy %source%gpps.cpp     %dest%
copy %source%sqlite3.c    %dest%
copy %source%sqlite3.h    %dest%

REM copy source files from DICOMLIB148.ZIP
set source=\quirt\comps\dll\cqdicom\

copy %source%filepdu.cxx %dest%
copy %source%rtc.cxx     %dest%
copy %source%flpdu.cxx   %dest%
copy %source%qrsop.cxx   %dest%
copy %source%storage.cxx %dest%
copy %source%verify.cxx  %dest%
copy %source%dimsen.cxx  %dest%
copy %source%dimsec.cxx  %dest%
copy %source%util.cxx	 %dest%
copy %source%uniq.cxx	 %dest%
copy %source%endian.cxx  %dest%
copy %source%trnsyn.cxx  %dest%
copy %source%pdu.cxx     %dest%
copy %source%pdata.cxx   %dest%
copy %source%aarj.cxx    %dest%
copy %source%aaac.cxx    %dest%
copy %source%aarq.cxx    %dest%
copy %source%deivr.cxx   %dest%
copy %source%socket.cxx  %dest%
copy %source%unixsock.h  %dest%
copy %source%buffer.cxx  %dest%

copy %source%base.hpp	 %dest%
copy %source%endian.hpp  %dest%
copy %source%constant.h  %dest%
copy %source%version.h   %dest%
copy %source%cctypes.h   %dest%
copy %source%safemem.h   %dest%
copy %source%util.h   %dest%
copy %source%socket.hpp   %dest%
copy %source%deivr.hpp   %dest%
copy %source%rtc.hpp	   %dest%
copy %source%aarq.hpp	   %dest%
copy %source%aaac.hpp	   %dest%
copy %source%aarj.hpp	   %dest%
copy %source%pdata.hpp   %dest%
copy %source%pdu.hpp	   %dest%
copy %source%dimsec.hpp   %dest%
copy %source%array.thh   %dest%
copy %source%array.tcc   %dest%
copy %source%pqueue.tcc   %dest%
copy %source%pqueue.thh   %dest%
copy %source%farray.tcc   %dest%
copy %source%farray.thh   %dest%
copy %source%buffer.thh   %dest%
copy %source%buffer.tcc   %dest%
copy %source%endian.hpd   %dest%
copy %source%endian.cpd   %dest%

copy %source%dimsen.hpp   %dest%
copy %source%verify.hpp   %dest%
copy %source%storage.hpp   %dest%
copy %source%qrsop.hpp   %dest%
copy %source%flpdu.hpp   %dest%
copy %source%dicom.hpp   %dest%
