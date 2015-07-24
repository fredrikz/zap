#ifndef SCANNER_H_
#define SCANNER_H_

#include "commandline.h"
#include "array.h"

enum ScannerToken
{
  TInvalid,
  TKeyword,
  TOperator,
  TName,
  TNumber,
  TType,
  TLParan,
  TRParan,
  TLCurlyB,
  TRCurlyB,
  TLSquareB,
  TRSquareB,
  TEquals,
  TString,
  TComment,
  TDot,
  TColon,
  TSemiColon,
  TComma,
  TDirective,
  TTotalTokenCount
};

typedef struct Token__
{
  uint8_t*        _start;
  unsigned int    _lineNo;
  unsigned char   _length;
  unsigned char   _token;
} Token;

ARRAYTYPE(Token)

typedef struct Scanner__
{
  ArrayToken      _tokens;
} Scanner;


void          ScannerCreate( Scanner* s, Options* o );
void          ScannerDestroy( Scanner* s );
int           ScannerScan( Scanner* s, Options* o );

#endif

