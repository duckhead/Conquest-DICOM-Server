/*
20000822	ljz	Created
20000823	ljz	Support cases where the input-file is empty or contains
			only white-space.
20010716	ljz	Fix: Allow TOKENs that are substrings of other TOKENs.
			The largest TOKEN that fits will be the result.
20050103	mvh	Changed include file to lower case (for linux)
20050108	mvh	Removed windows.h for linux compile
20050118	mvh	GNU C++ knows BOOL etc, do not redefine
*/

/*
   Lex.cpp contains the implementation of two classes:
   - TK
   - Lex

   These two classes are defined in Lex.hpp

   The original UCDMC code was shipped with only an object-file 'lex.o', and
   not with the source-code of these classes.

   The only compiler that could cope with lex.o was M$VC++.
   However: project-settings had to include a statement to exclude a standard
   library (msvcrt.lib??). This caused crashes when dgate was not launched by
   ConquestDicomServer.exe, but was running stand-alone.
   Print-statements to the console crashed the program...

   For this reason, and in order to be able to compile dgate with other
   compilers, lex.cpp had to be written.

   Lex.hpp has been left 'as is'

  The code is used in:
  - amap.cpp: read acrnema.map
  - parse.cpp: read dicom.sql

  The class TK is just a dummy.
*/

#ifdef WIN32
#ifndef BOOL
#define BOOL int
#define FALSE 0
#define TRUE 1
#endif
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lex.hpp"

TK::TK()
{
}

TK::TK (const	int i)
{
}

TK::TK (const	int i, int j, char* s)
{
}

TK::~TK ()
{
}


/*---------------------------------------------------------*/

typedef struct TOKEN_LIST
{
  int		val;
  char*		token;
  TOKEN_LIST*	pNext;
} TOKEN_LIST;


Lex::Lex()
{
  fp = NULL;		/* Not used */
  Tree = NULL;		/* Mis-used as a pointer to the root of TOKEN_LIST */
  Tk.Str = NULL;	/* Mis-used as a ponter to the current TOKEN_LIST element */
}

Lex::~Lex()
{
  TOKEN_LIST*	pCur;
  TOKEN_LIST*	pNext;

  pCur = (TOKEN_LIST*)Tree;
  while (pCur)
  { if (pCur->token)
      free(pCur->token);
    pNext = pCur->pNext;
    free(pCur);
    pCur = pNext;
  }
}


BOOL Lex::Start (char	*filename, TOKENSTRUCT	*TKS)
{
  FILE*		f;
  int		i, j, iFileLength;
  char*		pBuf;
  int		iNbTokens;
  TOKENSTRUCT*	pToken;
  char**	ppPos;
  char*		pCurPos;
  int		iFirst, iFirstToken;
  TOKEN_LIST*	pCur = NULL;
//  TOKEN_LIST*	pPrev = NULL;		/* LJ */
  char		cSave;
  char		bWasWhiteSpace;
  char*		pTmp;

  /* Sensible parameters? */
  if ((!filename) || (!TKS))
    return FALSE;

  /* How many tokens are to be identified in the file? */
  pToken = TKS;
  iNbTokens = 0;
  while (TRUE)
  { if (pToken->val != 0)
      if ((pToken->token == NULL) || (pToken->token[0] == 0))
	return FALSE;	/* A search-string must be supplied */
      else
	iNbTokens++;
    else
      if (!iNbTokens)
	return FALSE;	/*  At least one token must be supplied */
      else
	break;
    pToken++;
  }

  /* Read the file in memory */
  f = fopen(filename, "rb");
  if (!f)
    return FALSE;

  fseek(f, 0, SEEK_END);
  iFileLength = ftell(f);
  fseek(f, 0, SEEK_SET);

  pBuf = (char*)malloc(iFileLength + 1);
  if (!pBuf)
  { fclose(f);
    return FALSE;
  }

  if (fread(pBuf, 1, iFileLength, f) != iFileLength)
  { fclose(f);
    free(pBuf);
    return FALSE;
  }
  fclose(f);
  pBuf[iFileLength] = 0;

  /* Search all tokens */
  ppPos = (char**)malloc(iNbTokens * sizeof(char*));
  if (!ppPos)
  { fclose(f);
    free(pBuf);
    return FALSE;
  }
  pToken = TKS;
  for (i=0; i<iNbTokens; i++)
  { ppPos[i] = strstr(pBuf, (pToken + i)->token);
    while (ppPos[i])
    { /* Is this token a substring of another one? */
      for (j=0; j<iNbTokens; j++)
      { if (j == i)
	  continue;
	pTmp = strstr((pToken + j)->token, (pToken + i)->token);
	if (pTmp)
	  /* Yep, current token is a substring of another one...
	     Does the larger token also match here? If so,
	     continue searching.
	  */
	  if (strncmp(ppPos[i] - (pTmp - (pToken + j)->token),
	    (pToken + j)->token, strlen((pToken + j)->token)) == 0)
	  { ppPos[i] = strstr(ppPos[i] + 1, (pToken + i)->token);
	    break;
	  }
      }
      if (j == iNbTokens)
	break;
    }
    
  }

  /* What is the first found token? */
  iFirst = iFileLength + 1;
  iFirstToken = -1;
  for (i=0; i<iNbTokens; i++)
    if (ppPos[i] && (ppPos[i] - pBuf < iFirst))
    { iFirst = ppPos[i] - pBuf;
      iFirstToken = i;
    }

  pCurPos = pBuf;
  while (TRUE)
  {
    /* A word may not start with white-space */
    while ((*pCurPos == '\r') || (*pCurPos == '\n') ||
	   (*pCurPos == '\t') || (*pCurPos == ' '))
      pCurPos++;
    if (*pCurPos == 0)
      break;	/* Finished */

    /* Add a new entry */
    if (!pCur)
    { Tree = malloc(sizeof(TOKEN_LIST));
      pCur = (TOKEN_LIST*)Tree;
    }
    else
    { pCur->pNext = (TOKEN_LIST*)malloc(sizeof(TOKEN_LIST));
      pCur = pCur->pNext;
    }

    if (pCurPos < pBuf + iFirst)
    { /* A USER entry */
      pCur->val = TOKEN_USER;
      /* Entry is terminated at the start of the next TOKEN... */
      if (iFirstToken != -1)
      { cSave = pBuf[iFirst];
        pBuf[iFirst] = 0;
      }
      /* ... or at the first occurance of white-space */
      bWasWhiteSpace = 0;
      for (i=0; i<strlen(pCurPos); i++)
        if ((pCurPos[i] == '\r') || (pCurPos[i] == '\n') ||
	    (pCurPos[i] == '\t') || (pCurPos[i] == ' '))
	{ pCurPos[i] = 0;
	  bWasWhiteSpace = 1;
	  break;
	}
      pCur->token = strdup(pCurPos);
      if (bWasWhiteSpace)
	pCurPos[i] = ' ';
      pCurPos += strlen(pCur->token);
      if (iFirstToken != -1)
        pBuf[iFirst] = cSave;
    }
    else
    { /* A TOKEN entry */
      pCur->val = (pToken + iFirstToken)->val;
      pCur->token = NULL;
      pCurPos += strlen((pToken + iFirstToken)->token);

      /* Update the next occurance of this token */
      ppPos[iFirstToken] = strstr(pCurPos, (pToken + iFirstToken)->token);
      while (ppPos[iFirstToken])
      { /* Is this token a substring of another one? */
	for (j=0; j<iNbTokens; j++)
	{ if (j == iFirstToken)
	    continue;
	  pTmp = strstr((pToken + j)->token, (pToken + iFirstToken)->token);
	  if (pTmp)
	    /* Yep, current token is a substring of another one...
	       Does the larger token also match here? If so,
	       continue searching.
	    */
	    if (strncmp(ppPos[iFirstToken] - (pTmp - (pToken + j)->token),
	      (pToken + j)->token, strlen((pToken + j)->token)) == 0)
	    { ppPos[iFirstToken] = strstr(ppPos[iFirstToken] + 1, (pToken + iFirstToken)->token);
	      break;
	    }
        }
	if (j == iNbTokens)
	  break;
      }

      /* Update the first token */
      iFirst = iFileLength + 1;
      iFirstToken = -1;
      for (i=0; i<iNbTokens; i++)
	if (ppPos[i] && (ppPos[i] - pBuf < iFirst))
	{ iFirst = ppPos[i] - pBuf;
	  iFirstToken = i;
	}
    }
    pCur->pNext = NULL;
  }

  /* Finally add an END token */
  if (!pCur)
  { Tree = malloc(sizeof(TOKEN_LIST));
    pCur = (TOKEN_LIST*)Tree;
  }
  else
  { pCur->pNext = (TOKEN_LIST*)malloc(sizeof(TOKEN_LIST));
    pCur = pCur->pNext;
  }
  pCur->val = TOKEN_END;
  pCur->token = NULL;
  pCur->pNext = NULL;


  /* Cleanup */
  free(ppPos);
  free(pBuf);

  return TRUE;
}

BOOL Lex::Stop ()
{
  return TRUE;
}

int Lex::Peek (TK* pTK)
{
  TOKEN_LIST*	pCur;

  pCur = (TOKEN_LIST*)Tk.Str;
  if (!pCur)
  { /* No entries in token-list */
    pTK->Value = TOKEN_END;
    pTK->Str = NULL;
  }
  else
  { pTK->Value = pCur->val;
    pTK->Str = pCur->token;
  }
  return pTK->Value;
}

int Lex::Get (TK* pTK)
{
  TOKEN_LIST*	pCur;

  pCur = (TOKEN_LIST*)Tk.Str;
  if (!pCur)
    /* The first time */
    pCur = (TOKEN_LIST*)Tree;
  else
    /* Get next token, or stay at EOF */
    if (pCur->pNext)
      pCur = pCur->pNext;
  Tk.Str = (char*)pCur;

  if (!pCur)
  { /* No entries in token-list */
    pTK->Value = TOKEN_END;
    pTK->Str = NULL;
  }
  else
  { pTK->Value = pCur->val;
    pTK->Str = pCur->token;
  }

  return pTK->Value;
}

int Lex::GetString (TK* pTK, char c)
{
  /* Not implemented */
  return 0;
}

