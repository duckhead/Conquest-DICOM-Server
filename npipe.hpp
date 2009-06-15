// npipe.hpp -- define UNIX implementation of a minimal
// windows NT named pipe service
// 20070308	bcb 	Added changes for BSD and DARWIN

#ifndef NPIPE_HPP
#define NPIPE_HPP
#include <sys/types.h>
#include "dicom.hpp"

class NamedPipe
{
	public:
		NamedPipe(char *theName);
		~NamedPipe();
		BOOL Create();
		void Destroy();
		size_t Write(unsigned char *theBuffer, size_t theLength);
		size_t Read(unsigned char *theBuffer, size_t theLength);
		BOOL   Wait(size_t theWaitTime);
		BOOL Connect();			// establish connection
		void Disconnect();		// terminate connection

	private:

		char *itsName;
		BOOL itsCreatedHere;
		int itsControlReadFd;	// connections received here
		int itsControlWriteFd;	// this FD bound to name in filesystem
#ifndef BSD
		int itsConnectionFd;	// this is the FD used for a connection
								// which has been established
#else	//BSD
		FILE *itsConnectionFd;
#endif
};

#endif
