#ifndef PARSER_H_
#define PARSER_H_

#include "array.h"
#include <stdint.h>

struct Scanner;

typedef struct String__
{
  const uint8_t*   _begin;
  const uint8_t*   _end;
} String;


ARRAYTYPE(String)

typedef struct Parser__
{
  struct Scanner*   _scanner;
  unsigned int      _currToken;

  ArrayString       _depends;
} Parser;

struct Options;

void      ParserCreate( Parser* p, struct Options* o );
int       ParserParse( Parser* p, struct Options* o );
void      ParserDestroy( Parser* p );

#endif // PARSER_H_

