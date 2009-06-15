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

/*********************************************************************
 *
 * Connection Class
 *
 *
 *********************************************************************/

// The library used to link with the DICOM Lib has a naming conflict with
// socket.  Therefor we simply remap our socket class into MacSocket.
 
#ifdef	MAC
#	define	Socket	MacSocket
#endif

class	Socket
	{
	int					Error;
	unsigned	long	tulong,*tulongptr;
	public:
		int					Socketfd;
		int					ListenSocketfd;
		BOOL				UDP;
		BOOL				Connected;
		BOOL				Listened;
	private:
	struct	sockaddr_in	sa;
	struct	hostent		hes;
	struct	servent		servs;

		int					TimeOut;					
	public:
		virtual	BOOL	SendBinary(BYTE *, UINT);
		virtual	INT		ReadBinary(BYTE *, UINT);
		struct	hostent	*Gethostbyname(char	*);
		struct	servent	*Getservbyname(char	*, char *);
		int		GetLinkError();
				Socket();
		virtual	~Socket();		
		virtual	BOOL	Open ( char *ip, char *port );
		BOOL	Close ();
		int		GetLastError()	{ return ( Error ); };
		BOOL	Listen (char *port);
		BOOL	Accept ();
		BOOL	SetTimeOut(int);
		
		// UDP Extensions
		BOOL	BindUDPServer(char *port);
		BOOL	BindUDPClient(char *host, char *port);

	};

