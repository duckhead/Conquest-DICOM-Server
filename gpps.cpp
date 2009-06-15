// GetPrivateProfileString() -- approximate implementation of
// Windows NT System Services version of GetPrivateProfileString()
// probably doesn't handle the NULL key for section name or value key
// correctly also, doesn't provide Microsoft backwards compatability
// wrt TAB characters in the value string -- Microsoft terminates value
// at the first TAB, but I couldn't discover what the behavior should
// be regarding TABS in quoted strings so, I treat tabs like any other
// characters -- NO comments following value string separated by a TAB
// are allowed (that is an anachronism anyway)

// 20040111	mvh	Fixes: defaults now work, allow DOS type files, case insensitive
// 20070330	mvh	Change by Mark Pearson: also allow tabs instead of spaces around =

#include <stdio.h>
#ifdef UNIX
#include <unistd.h>
#endif
#include <sys/types.h>
#include <string.h>
#include "gpps.hpp"
#include "dicom.hpp"

DWORD
GetPrivateProfileString(char *theSection,	// section name
	char *theKey,							// search key name
	char *theDefault,						// default value if not found
	char *theReturnBuffer,					// return valuse stored here
	size_t theReturnBufferLength,			// byte length of return buffer
	char *theIniFileName)					// pathname of ini file to search
{
	FILE *aFile = theIniFileName ? fopen(theIniFileName, "r") : NULL;
	size_t aLength = (theDefault == NULL) ? 0 : strlen(theDefault);

	if(theReturnBufferLength == 0 || theReturnBuffer == NULL)
	{
		if(aFile)
		{
			fclose(aFile);
		}
		return 0;
	}

	if(aFile == NULL)
	{
		// no ini file specified, return the default

		++aLength;	// room for NULL char
		aLength = theReturnBufferLength < aLength ?
			theReturnBufferLength : aLength;
		strncpy(theReturnBuffer, theDefault, aLength);
		theReturnBuffer[aLength - 1] = '\0';
		return aLength - 1;
	}

	char aLine[2048];
	char *aValue;
	char *aString;
	size_t aLineLength;
	size_t aReturnLength = 0;
	BOOL aSectionFound = FALSE;

	while(fgets(aLine, sizeof(aLine), aFile) != NULL)
	{
		aLineLength = strlen(aLine);
		// strip final '\n'
		if(aLineLength > 0 && aLine[aLineLength - 1] == '\n')
		{
			aLine[aLineLength - 1] = '\0';
			aLineLength--;
		}
		// strip final '\r'
		if(aLineLength > 0 && aLine[aLineLength - 1] == '\r')
		{
			aLine[aLineLength - 1] = '\0';
			aLineLength--;
		}
		switch(*aLine)
		{
			case ' ': // blank line
			case ';': // comment line
				continue;
			break;
			
			case '[':	// section marker

				if(aString = strchr(aLine, ']'))
				{
					*aString = '\0';

					// accept as matched if NULL key or exact match

					if(!theSection || !stricmp(aLine + 1, theSection))
					{
						aSectionFound = TRUE;
					}
				}

			break;

			default:

				// try to match value keys if in proper section

				if(aSectionFound)
				{
					// try to match requested key
	
					if(aString = aValue = strchr(aLine, '='))
					{
						*aValue = '\0';
						++aValue;

						// strip leading blanks in value field

						while((*aValue == ' ' || *aValue == '\t') && aValue < aLine + sizeof(aLine))
						{
							*aValue++ = '\0';
						}
						if(aValue >= aLine + sizeof(aLine))
						{
							aValue = "";
						}
					}
					else
					{
						aValue = "";
					}

					// strip trailing blanks from key

					if(aString)
					{
						while(--aString >= aLine && (*aString == ' ' || *aString == '\t'))
						{
							*aString = '\0';
						}
					}

					// see if key is matched
					
					if(theKey == NULL || !stricmp(theKey, aLine))
					{
						// matched -- first, terminate value part

						aLineLength = strlen(aValue);

						// remove trailing blanks from aValue if any

						aString = aValue + aLineLength - 1;
						
						while(--aString > aValue && (*aString == ' ' || *aString == '\t'))
						{
							*aString = '\0';
							--aLineLength;
						}

						// unquote value if quoted

						if(aLineLength >= 2 && aValue[0] == '"' &&
							aValue[aLineLength - 1] == '"')
						{
							// string quoted with double quotes

							aValue[aLineLength - 1] = '\0';
							++aValue;
							aLineLength -= 2;
						}
						else
						{
							// single quotes allowed also...

							if(aLineLength >= 2 && aValue[0] == '\'' &&
								aValue[aLineLength - 1] == '\'')
							{
								aValue[aLineLength - 1] = '\0';
								++aValue;
								aLineLength -= 2;
							}
						}

						// compute maximum length copyable

						aLineLength = (aLineLength <
							theReturnBufferLength - aReturnLength) ? aLineLength :
							theReturnBufferLength - aReturnLength;

						// do the copy to return buffer

						if(aLineLength)
						{
							strncpy(&theReturnBuffer[aReturnLength],
								aValue, aLineLength);
							aReturnLength += aLineLength;
							if(aReturnLength < theReturnBufferLength)
							{
								theReturnBuffer[aReturnLength] = '\0';
								++aReturnLength;
								
							}
						}
					}
				}

			break;
		}
	}

	if(aFile)
	{
		fclose(aFile);
	}

	if (aReturnLength==0)
	{
		++aLength;	// room for NULL char
		aLength = theReturnBufferLength < aLength ?
			theReturnBufferLength : aLength;
		strncpy(theReturnBuffer, theDefault, aLength);
		theReturnBuffer[aLength - 1] = '\0';
		return aLength - 1;
	}

	return aReturnLength > 0 ? aReturnLength - 1 : 0;
}

