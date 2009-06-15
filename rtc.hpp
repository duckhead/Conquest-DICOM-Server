
/* ljz 19980331: Added prototype for writing binary RTCs */

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

// Run-Time-Classing of VR's

BOOL	Pack(BYTE, BYTE, UINT16 &);
BOOL	Pack(UINT16, BYTE &, BYTE &);


int MkBinaryRtc(char* pszTextIn, char* pszBinaryOut, BOOL bCarryDescriptions);


#	define	MAKEUINT32(xxx, yyy)	((UINT32) (((UINT32) xxx << 16) | yyy))
typedef	struct	_RTCElement
	{
	UINT16	Group;
	UINT16	Element;
	UINT16	TypeCode;
	char	*Description;
	}	RTCElement;
	
	
class	RTC
	{
	public:
		FixedArray < UINT32, RTCElement >		*TypeCodes;
		BOOL		CarryDescriptions;
	public:
					RTC (BOOL	CD = TRUE, char *filename = NULL);
					~RTC ();
		BOOL		AttachRTC ( char *filename );
		BOOL		AttachBinaryRTC ( char *filename );
		BOOL		AttachResourceRTC (char* pDict);
		BOOL		DetachRTC ();
		BOOL		RunTimeClass ( DICOMObject * );
		BOOL		RunTimeClass ( VR * );
		UINT16		RunTimeClass ( UINT16, UINT16, char * =NULL);
		BOOL		GetGroupElement(RTCElement* pEntry);
	};



