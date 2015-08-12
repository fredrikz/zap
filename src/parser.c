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
 * | TKW_Depends '{' ConstStringList '}';
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
 * | Expression
 *
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
Declaration( Parser* p, Options* o )
{
  (void)p;
  (void)o;
  return 0;
}

int
ParserParse( Parser* p, Options* o )
{
  for (;;)
  {
    if ( Directive( p, o )
      || Declaration( p, o ) )
    {
      continue;
    }

    break;
  }

  // Print what we've found so far, even if we have errors
  printf( "Parser pass:\n" );
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

  EXPECT( TEof );
  return 1;
}

