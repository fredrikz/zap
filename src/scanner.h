#ifndef SCANNER_H_
#define SCANNER_H_

#include "commandline.h"
#include "array.h"

enum ScannerToken
{
  TKeyword,
  TOperator,
  TName,
  TNumber,
  TType,
  TLeftParanthesis,
  TRightParanthesis,
  TLeftCurly,
  TRightCurly,
  TLeftAngleBracket,
  TRightAngleBracket,
  TEquals,
  TString
};

typedef struct Token__
{
  uint8_t*        _start;
  unsigned char   _length;
  unsigned char   _token;
} Token;

ARRAYTYPE(Token)

typedef struct Scanner__
{
  ArrayToken      _tokens;
} Scanner;


void          ScannerInit( Scanner* s, Options* o );
int           ScannerScan( Scanner* s, Options* o );

#endif

