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

// Architectually Neutral Endian types.
//
// Note these are not done with C++ templates.  This is a design choice.
UINT16	SwitchEndian(UINT16	x);
UINT32	SwitchEndian(UINT32	x);
INT16	SwitchEndian(INT16	x);
INT32	SwitchEndian(INT32	x);

#   ifdef BSD  // Swap durring file reads and writes
#	define	LE_UINT16	UINT16
#	define	LE_UINT32	UINT32
#	define	LE_INT16	INT16
#	define	LE_INT32	INT32
#	else //Do the original stuff
#	if	NATIVE_ENDIAN	==	LITTLE_ENDIAN

#	define	LE_UINT16	UINT16
#	define	LE_UINT32	UINT32
#	define	LE_INT16	INT16
#	define	LE_INT32	INT32

#	define	XE_UINTX	BE_UINT16
#	define	UINTX		UINT16
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	BE_UINT32
#	define	UINTX		UINT32
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	BE_INT16
#	define	UINTX		INT16
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	BE_INT32
#	define	UINTX		INT32
#	include	"endian.hpd"

#undef	XE_UINTX
#undef	UINTX

#else
#	define	BE_UINT16	UINT16
#	define	BE_UINT32	UINT32
#	define	BE_INT16	INT16
#	define	BE_INT32	INT32

#	define	XE_UINTX	LE_UINT16
#	define	UINTX		UINT16
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	LE_UINT32
#	define	UINTX		UINT32
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	LE_INT16
#	define	UINTX		INT16
#	include	"endian.hpd"
#undef	XE_UINTX
#undef	UINTX
#	define	XE_UINTX	LE_INT32
#	define	UINTX		INT32
#	include	"endian.hpd"

#undef	XE_UINTX
#undef	UINTX

#endif
#endif //BSD

// Define some endian switchable constants
#if NATIVE_ENDIAN == LITTLE_ENDIAN //Little Endian
#	define	EN0001	0x0001
#	define	EN0003	0x0003
#	define	EN0010	0x0010
#	define	EN0020	0x0020
#	define	EN0021	0x0021
#	define	EN0030	0x0030
#	define	EN0100	0x0100
#	define	EN0102	0x0102
#	define	EN0110	0x0110
#	define	EN0120	0x0120
#	define	EN0130	0x0130
#	define	EN0140	0x0140
#	define	EN0150	0x0150
#	define	EN0FFF	0x0FFF
#	define	EN8001	0x8001
#	define	EN8010	0x8010
#	define	EN8020	0x8020
#	define	EN8021	0x8021
#	define	EN8030	0x8030
#	define	EN8100	0x8100
#	define	EN8110	0x8110
#	define	EN8120	0x8120
#	define	EN8130	0x8130
#	define	EN8140	0x8140
#	define	EN8150	0x8150
#else //Big Endian like Apple power pc
#	define	EN0001	0x0100
#	define	EN0003	0x0300
#	define	EN0010	0x1000
#	define	EN0020	0x2000
#	define	EN0021	0x2100
#	define	EN0030	0x3000
#	define	EN0100	0x0001
#	define	EN0102	0x0201
#	define	EN0110	0x1001
#	define	EN0120	0x2001
#	define	EN0130	0x3001
#	define	EN0140	0x4001
#	define	EN0150	0x5001
#	define	EN0FFF	0xFF0F
#	define	EN8001	0x0180
#	define	EN8010	0x1080
#	define	EN8020	0x2080
#	define	EN8021	0x2180
#	define	EN8030	0x3080
#	define	EN8100	0x0081
#	define	EN8110	0x1081
#	define	EN8120	0x2081
#	define	EN8130	0x3081
#	define	EN8140	0x4081
#	define	EN8150	0x5081
#endif //Big Endian


