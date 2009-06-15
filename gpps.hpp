// GetPrivateProfileString
// for UNIX use
#ifndef GPPS_HPP
#define GPPS_HPP
#ifdef UNIX
#include <sys/types.h>
#include "wintypes.hpp"
DWORD
GetPrivateProfileString(char *theSection,   // section name
    char *theKey,                           // search key name
    char *theDefault,                       // default value if not found
    char *theReturnBuffer,                  // return valuse stored here
    size_t theBufferLength,                 // byte length of return buffer
    char *theIniFileName);                  // pathname of ini file to search
#endif
#endif
