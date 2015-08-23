#ifndef SCANNER_H_
#define SCANNER_H_

#include "array.h"
#include <stdint.h>

enum ScannerToken
{
  TInvalid,

  TOperator,
  TIdentifier,
  TNumber,
  TConstString,

  // Types
  TInt,
  TUInt,
  TFloat,
  TString,

  // Control characters
  TDot,
  TColon,
  TSemiColon,
  TComma,
  TEquals,
  TLParan,
  TRParan,
  TLCurlyB,
  TRCurlyB,
  TLSquareB,
  TRSquareB,

  // Keywords
  TKW_Constant,
  TKW_Fn,
  TKW_If,
  TKW_Else,
  TKW_For,
  TKW_Return,
  TKW_Null,
  TKW_Break,
  TKW_True,
  TKW_False,

  // Directives
  TKW_Depends,

  TComment,
  TWhitespace,

  TEof,

  TTotalTokenCount
};

typedef struct Token
{
  uint8_t*        _start;
  unsigned int    _lineNo;
  unsigned char   _length;
  unsigned char   _token;
} Token;

ARRAYTYPE(Token)

typedef struct Scanner
{
  ArrayToken      _tokens;
} Scanner;

struct Options;

void          ScannerCreate( Scanner* s, struct Options* o );
void          ScannerDestroy( Scanner* s );
int           ScannerScan( Scanner* s, struct Options* o );

#endif

