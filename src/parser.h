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


enum TypeInfoType
{
  TypeInt,
  TypeUInt,
  TypeFloat,
  TypeString,
  TypeUDT
};


enum TypeQualifiers
{
  TFConst     = 1<<0,
  TFOptional  = 1<<1,
  TFRequired  = 1<<2 
};

typedef struct TypeInfo__
{
  const uint8_t*  _id;
  unsigned char   _idLength;
  unsigned char   _type;
  unsigned char   _typeQualifiers;
  uint32_t        _size; // 0 for undefined, 1 for single variables, 2+ for arrays of defined size
} TypeInfo;


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

