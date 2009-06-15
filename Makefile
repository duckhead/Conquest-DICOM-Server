CC = g++
CFLAGS = -I/usr/include/postgresql -DUNIX -DNATIVE_ENDIAN=1 -DNOINTJPEG -DPOSTGRES
LDFLAGS = -L/usr/local/pgsql/lib
LIBS    = -lpthread -lpq
SOURCES = total.cxx

all:    dgate

dgate:  total.cxx
        $(CC) $(CFLAGS) total.cxx $(LDFLAGS) $(LIBS) -o dgate

install: dgate
        pkill -9 dgate || true
        sleep 0.2s
        cp dicom.ini.postgres dicom.ini
        cp dicom.sql.postgres dicom.sql
        sudo cp dgate /usr/lib/cgi-bin/dgate.html
        sudo cp dgate /usr/lib/cgi-bin
        sudo cp dicom.sql /usr/lib/cgi-bin
        sudo cp dicom.ini.www /usr/lib/cgi-bin
        sudo /sbin/ldconfig /usr/local/pgsql/lib
