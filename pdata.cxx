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

#	include	"dicom.hpp"


INT		MemoryBuffer	::	ReadBinary ( BYTE * Data, UINT Count )
	{
	INT	RetVal;

	if ( Index == Length )
			return ( -1 );	// end
	if((Length - Index)<Count )
		{
		memcpy((void*)Data, (void*)&this->Data[Index], Length - Index);
		RetVal = (INT) Length - Index;
		Index = Length;
		return ( RetVal );
		}
	memcpy((void*)Data, (void*)&this->Data[Index], Count);
	Index += Count;
	return ( Count );
	}

BOOL	MemoryBuffer	::	SendBinary ( BYTE *, UINT )
	{
	return ( FALSE );
	}

MemoryBuffer	::	MemoryBuffer (
	BYTE *Data,
	UINT Length,
	BOOL	Destruct,
	UINT	InEndian )
	{
	Index = 0;
	this->Length = Length;
	this->Data = Data;
	DestructFlag = Destruct;
	SetIncomingEndian(InEndian);
	
	this->Index = Length;
	BufferSpace	*aBS = new BufferSpace;
	aBS->BufferSize = Length;
	aBS->isTemp = TRUE;
	aBS->Data = Data;
	InSize += Length;
	Incoming.Add(aBS);
	}

MemoryBuffer	::	~MemoryBuffer ()
	{
	if(DestructFlag)
		if(Data)
			delete Data;
	}

BOOL	LinkedBuffer	::	Fill(Buffer	&Link, UINT	Count)
	{
	LinkedTo = &Link;
	return(Buffer :: Fill(Count));
	}

INT		LinkedBuffer	::	ReadBinary(BYTE	*Data, UINT	Count)
	{
	if(LinkedTo->Read(Data, Count))
		return(Count);
	return(-1);
	}

BOOL	LinkedBuffer	::	SendBinary(BYTE	*Data, UINT	Count)
	{
	LinkedTo->Write(Data, Count);
	LinkedTo->Flush();
	return ( TRUE );
	}

UINT	LinkedBuffer	::	GetOutgoingSize()
	{
	return(OutSize);
	}


UINT	LinkedBuffer	::	GetIncomingSize()
	{
	return(InSize);
	}

BOOL	LinkedBuffer	::	Flush(Buffer	&Link, UINT	Count)
	{
	LinkedTo = &Link;
	return(Buffer :: Flush(Count));
	}

BOOL	PDataTF	::	ReadDynamic(Buffer	&Link)
	{
	UINT32		Count;

	if(!Length)
		{	
		Link >> Reserved1;
		Link >> Length;
		}
		
	//fprintf(stderr, "Reading P-DATA-TF: %d Length\n", Length);
	Count = Length;
	MsgStatus = 0;	// continue
	while ( Count > 0)
		{
		Link >> pdv.Length;
		Link >> pdv.PresentationContextID;
		Link >> pdv.MsgHeader;
		//fprintf(stderr, "Reading PDV: %d Length, %d ID, %x Header\n",
		//	pdv.Length, pdv.PresentationContextID, pdv.MsgHeader);
		VRBuffer.Fill(Link, pdv.Length - 2);
		//fprintf(stderr, "Finished reading PDV\n");
		Count = Count - pdv.Length - sizeof(UINT32);
		Length = Length - pdv.Length - sizeof(UINT32);
		//fprintf(stderr, "Count = %d\n", Count);
		if(IsThisTheLastFragment(pdv.MsgHeader))
			{
			MsgStatus = 1;
			PresentationContextID = pdv.PresentationContextID;
			return ( TRUE );
			}				
		}
	//fprintf(stderr, "done reading PDV's\n");
	if(IsThisTheLastFragment(pdv.MsgHeader))
		MsgStatus = 1;

	PresentationContextID = pdv.PresentationContextID;

	return ( TRUE );
	}


BOOL	PDataTF	::	Write(Buffer	&Link)
	{
	// Generate P-DATA-TF Messages.
	UINT	BlockSize, SentSize, TotalSize, TLength;

	TotalSize = VRBuffer.GetOutgoingSize();
	BlockSize = 4096;
	SentSize = 0;
	TLength = Length;
	while(SentSize < TotalSize)
		{
		if((TotalSize - SentSize) < BlockSize)
			BlockSize = TotalSize - SentSize;
		if((BlockSize + SentSize) == TotalSize)
	//	if(BlockSize)
			MsgHeader = MsgHeader | 0x02;
		else
			MsgHeader = MsgHeader & 0x01;
		
		pdv.PresentationContextID = PresentationContextID;
		pdv.MsgHeader = MsgHeader;
		pdv.Length = BlockSize + 2;
		Length = pdv.Length + sizeof(UINT32);
		ItemType = 0x04;
		Reserved1 = 0;
		Link << ItemType;
		Link << Reserved1;
		Link << Length;
		Link << pdv.Length;
		Link << pdv.PresentationContextID;
		Link << MsgHeader;
		if(BlockSize)
			VRBuffer.Flush(Link, BlockSize);
		SentSize += BlockSize;
		Link.Flush();
		}
	Length = TLength;
	return ( TRUE );		
	}

PDataTF	::	PDataTF()
	{
	ItemType = 0x04;
	Length = 0;
	}


PDataTF	::	~PDataTF()
	{

	}

