#include "parser.h"
#include "commandline.h"
#include "scanner.h"
#include <stdio.h>

/* grammar
 *
 * Goal
 * | Directive Goal
 * | Definition Goal
 * | Eof
 *
 * Definition
 * | Variable
 * | Function
 *
 * ConstStringList
 * | ConstStringList ConstString 
 * | e
 *
 * Directive
 * | TKW_Depends '{' ConstStringList '}'
 * 
 * Function
 * | TKW_Fn TIdentifier TLParan Args TRParan Block
 *
 * Block
 * | Statement Block
 * | e
 * 
 * Statement
 * | Variable Statement
 * | e
 *
 * VariableDeclaration
 * | Type TIdentifier
 *
 * Variable
 * | VariableDeclaration ';'
 * | VariableDeclaration Assignment Expression ';'
 *
 * Expression
 * | Expression
 *
 * Assignment
 * | '='
 *
 * Type
 * | TInt
 * | TUInt
 * | TFloat
 * | TString
 * | TIdentifier
 *
 */

#define PEEK(NUM) (p->_scanner->_tokens._data[p->_currToken+(NUM)])
#define CURR PEEK(0)
#define NEXT (++p->_currToken)

static inline int 
Accept( Parser* p, enum ScannerToken t )
{
  if ( CURR._token == t )
  {
    NEXT;
    return 1;
  }
  return 0;
}
#define ACCEPT( TOKEN ) Accept( p, (TOKEN) )



extern const char* ScannerTokenStrings[];
static inline int 
Expect( Parser* p, enum ScannerToken t )
{
  if ( CURR._token == t )
  {
    NEXT;
    return 1;
  }
  printf( "Unexpected token '%.*s' encountered; expected '%s'.\n", 
      CURR._length, 
      CURR._start, 
      ScannerTokenStrings[t] );
  return 0;
}
#define EXPECT( TOKEN ) { if ( Expect( p, TOKEN ) == 0 ) { return 0; } }



void
ParserCreate( Parser* p, Options* o )
{
  (void)o;
  memset( p, 0, sizeof(Parser) );
}



void      
ParserDestroy( Parser* p )
{
  ArrayDestroyString( &p->_depends );
}



static int
VariableDeclaration( Parser* p, Options* o )
{
  (void)o;
  // TODO: Accept type qualifiers first
  TypeInfo t = { 0, 0, 0, 0, 0 };
  if ( ACCEPT( TInt ) )
  {
    t._type = TypeInt;
  }
  else if ( ACCEPT( TUInt ) )
  {
    t._type = TypeUInt;
  }
  else if ( ACCEPT( TFloat ) )
  {
    t._type = TypeFloat;
  }
  else if ( ACCEPT( TString ) )
  {
    t._type = TypeString;
  }
  else if ( ACCEPT( TIdentifier ) )
  {
    t._type = TypeUDT;
  }

  if ( ACCEPT( TLSquareB ) )
  {
    ACCEPT( TNumber );
    EXPECT( TRSquareB );
  }

  ACCEPT( TIdentifier );
  return 1;
}



static int 
Variable( Parser* p, Options* o )
{
  (void)p; (void)o;
  return 0;
}



static int 
ArgumentList( Parser* p, Options* o )
{
  while ( VariableDeclaration( p, o ) )
  {
    if ( !ACCEPT( TComma ) )
    {
      break;
    }
  }
  // Always accept empty arg lists, let caller determine validity
  return 1;
}



static int
Statement( Parser* p, Options* o )
{
  (void)p; (void)o;
  return 0;
}



static int 
Block( Parser* p, Options* o )
{
  (void)o;
  EXPECT( TLCurlyB );
  for (;;)
  {
    if ( !Statement( p, o ) )
    {
      break;
    }
  }
  EXPECT( TRCurlyB );
  return 1;
}



static int
Function( Parser* p, Options* o )
{
  (void)o;
  EXPECT( TKW_Fn );
  EXPECT( TIdentifier );
  EXPECT( TLParan );
  if ( ArgumentList( p, o ) )
  {
    if ( ACCEPT( TColon ) )
    {
      if ( ArgumentList( p, o ) )
      {
        EXPECT( TRParan );
      }
    }
    else
    {
      EXPECT( TRParan );
    }

    if ( Block( p, o ) )
    {
      return 1;
    }
  }
  return 0;
}


static int
Directive( Parser* p, Options* o )
{
  (void)o;
  if ( Accept( p, TKW_Depends ) )
  {
    EXPECT( TLCurlyB )
    while ( CURR._token == TConstString )
    {
      String str;
      str._begin = CURR._start;
      str._end = str._begin + CURR._length;
      ArrayAddString( &p->_depends, &str );
      NEXT;
    }
    EXPECT( TRCurlyB )
    return 1;
  }
  return 0;
}



static int
Definition( Parser* p, Options* o )
{
  if ( Variable( p, o )
      || Function( p, o ) )
  {
    return 1;
  }
  return 0;
}


int
ParserParse( Parser* p, Options* o )
{
  printf( "Parser pass:\n" );
  for (;;)
  {
    if ( Directive( p, o )
      || Definition( p, o ) )
    {
      continue;
    }

    break;
  }
  EXPECT( TEof );

  // Print what we've found so far, even if we have errors
  if ( p->_depends._size )
  {
    printf( "[Dependencies]\n" );
    for ( unsigned int i = 0; i < p->_depends._size; ++i )
    {
      String* str = &p->_depends._data[i];
      printf( "\"%.*s\"\n", 
          (int)( str->_end - str->_begin ),
          str->_begin );
    }
  }

  return 1;
}

