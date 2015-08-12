#include "parser.h"
#include "commandline.h"
#include "scanner.h"

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


static inline int
Peek( Parser* p, enum ScannerToken t )
{
  if ( PEEK(1)._token == t )
  {
    NEXT;
    return 1;
  }
  return 0;
}



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
  if ( Accept( p, TKW_Depends ) 
      && Accept( p, TLCurlyB ) )
  {
    while ( Peek( p, TString ) )
    {
      String str;
      str._begin = CURR._start;
      str._end = str._begin + CURR._length;
      ArrayAddString( &p->_depends, &str );
    }

    return Accept( p, TRCurlyB );
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

  return CURR._token == TEof ? 0 : -1;
}

