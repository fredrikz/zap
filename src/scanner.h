#ifndef SCANNER_H_
#define SCANNER_H_

#include "array.h"
#include <stdint.h>

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

