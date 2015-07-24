#include "scanner.h"
#include <string.h>
#include <stdio.h>
#include "commandline.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wstatic-in-inline"
#include "keywords.h"
#pragma clang diagnostic pop

enum CharType {
  Invalid,
  Whitespace,
  Alpha,
  Numeric,
  Control
};

/*
#include <stdio.h>
#include <ctype.h>

void PrintCharSet()
{
  for ( int i = 0; i < 128; ++i )
  {
    const char* type = "Invalid,";

    if ( isalnum( i ) )
    {
      type = "AlphaNumeric,";
    }

    int maxLen = strlen( "AlphaNumeric," );

    printf( "  %-*s // '%c'\n", maxLen, type, i < 32 ? ' ' : (char)i );
  }
}
*/

static enum CharType
charTypes[256] = 
{
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''

  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''

  Whitespace,   // '\n'
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''

  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''

  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''

  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''
  Invalid,      // ''

  Invalid,      // ''
  Invalid,      // ''
  Whitespace,   // ' '
  Control,      // '!'
  Control,      // '"'

  Control,      // '#'
  Control,      // '$'
  Control,      // '%'
  Control,      // '&'
  Control,      // '''

  Control,      // '('
  Control,      // ')'
  Control,      // '*'
  Control,      // '+'
  Control,      // ','

  Control,      // '-'
  Control,      // '.'
  Control,      // '/'
  Numeric,      // '0'
  Numeric,      // '1'

  Numeric,      // '2'
  Numeric,      // '3'
  Numeric,      // '4'
  Numeric,      // '5'
  Numeric,      // '6'

  Numeric,      // '7'
  Numeric,      // '8'
  Numeric,      // '9'
  Control,      // ':'
  Control,      // ';'

  Control,      // '<'
  Control,      // '='
  Control,      // '>'
  Control,      // '?'
  Control,      // '@'

  Alpha,        // 'A'
  Alpha,        // 'B'
  Alpha,        // 'C'
  Alpha,        // 'D'
  Alpha,        // 'E'

  Alpha,        // 'F'
  Alpha,        // 'G'
  Alpha,        // 'H'
  Alpha,        // 'I'
  Alpha,        // 'J'

  Alpha,        // 'K'
  Alpha,        // 'L'
  Alpha,        // 'M'
  Alpha,        // 'N'
  Alpha,        // 'O'

  Alpha,        // 'P'
  Alpha,        // 'Q'
  Alpha,        // 'R'
  Alpha,        // 'S'
  Alpha,        // 'T'

  Alpha,        // 'U'
  Alpha,        // 'V'
  Alpha,        // 'W'
  Alpha,        // 'X'
  Alpha,        // 'Y'

  Alpha,        // 'Z'
  Control,      // '['
  Control,      // '\'
  Control,      // ']'
  Control,      // '^'

  Alpha,        // '_' // Special case
  Control,      // '`'
  Alpha,        // 'a'
  Alpha,        // 'b'
  Alpha,        // 'c'

  Alpha,        // 'd'
  Alpha,        // 'e'
  Alpha,        // 'f'
  Alpha,        // 'g'
  Alpha,        // 'h'

  Alpha,        // 'i'
  Alpha,        // 'j'
  Alpha,        // 'k'
  Alpha,        // 'l'
  Alpha,        // 'm'

  Alpha,        // 'n'
  Alpha,        // 'o'
  Alpha,        // 'p'
  Alpha,        // 'q'
  Alpha,        // 'r'

  Alpha,        // 's'
  Alpha,        // 't'
  Alpha,        // 'u'
  Alpha,        // 'v'
  Alpha,        // 'w'

  Alpha,        // 'x'
  Alpha,        // 'y'
  Alpha,        // 'z'
  Control,      // '{'
  Control,      // '|'

  Control,      // '}'
  Control,      // '~'
  Invalid       // ''
};

static enum ScannerToken
controlTokenTypes[256] = 
{
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''

  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''

  TInvalid,      // '\n'
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''

  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''

  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''

  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ''

  TInvalid,      // ''
  TInvalid,      // ''
  TInvalid,      // ' '
  TInvalid,      // '!'
  TInvalid,      // '"'

  TInvalid,      // '#'
  TInvalid,      // '$'
  TInvalid,      // '%'
  TInvalid,      // '&'
  TInvalid,      // '''

  TLParan,       // '('
  TRParan,       // ')'
  TOperator,     // '*'
  TOperator,     // '+'
  TComma,        // ','

  TOperator,     // '-'
  TDot,          // '.'
  TOperator,     // '/'
  TInvalid,      // '0'
  TInvalid,      // '1'

  TInvalid,      // '2'
  TInvalid,      // '3'
  TInvalid,      // '4'
  TInvalid,      // '5'
  TInvalid,      // '6'

  TInvalid,      // '7'
  TInvalid,      // '8'
  TInvalid,      // '9'
  TColon,        // ':'
  TSemiColon,    // ';'

  TInvalid,      // '<'
  TEquals,       // '='
  TInvalid,      // '>'
  TInvalid,      // '?'
  TInvalid,      // '@'

  TInvalid,      // 'A'
  TInvalid,      // 'B'
  TInvalid,      // 'C'
  TInvalid,      // 'D'
  TInvalid,      // 'E'

  TInvalid,      // 'F'
  TInvalid,      // 'G'
  TInvalid,      // 'H'
  TInvalid,      // 'I'
  TInvalid,      // 'J'

  TInvalid,      // 'K'
  TInvalid,      // 'L'
  TInvalid,      // 'M'
  TInvalid,      // 'N'
  TInvalid,      // 'O'

  TInvalid,      // 'P'
  TInvalid,      // 'Q'
  TInvalid,      // 'R'
  TInvalid,      // 'S'
  TInvalid,      // 'T'

  TInvalid,      // 'U'
  TInvalid,      // 'V'
  TInvalid,      // 'W'
  TInvalid,      // 'X'
  TInvalid,      // 'Y'

  TInvalid,      // 'Z'
  TLSquareB,     // '['
  TInvalid,      // '\'
  TRSquareB,     // ']'
  TInvalid,      // '^'

  TInvalid,      // '_' // Special case
  TInvalid,      // '`'
  TInvalid,      // 'a'
  TInvalid,      // 'b'
  TInvalid,      // 'c'

  TInvalid,      // 'd'
  TInvalid,      // 'e'
  TInvalid,      // 'f'
  TInvalid,      // 'g'
  TInvalid,      // 'h'

  TInvalid,      // 'i'
  TInvalid,      // 'j'
  TInvalid,      // 'k'
  TInvalid,      // 'l'
  TInvalid,      // 'm'

  TInvalid,      // 'n'
  TInvalid,      // 'o'
  TInvalid,      // 'p'
  TInvalid,      // 'q'
  TInvalid,      // 'r'

  TInvalid,      // 's'
  TInvalid,      // 't'
  TInvalid,      // 'u'
  TInvalid,      // 'v'
  TInvalid,      // 'w'

  TInvalid,      // 'x'
  TInvalid,      // 'y'
  TInvalid,      // 'z'
  TLCurlyB,      // '{'
  TInvalid,      // '|'

  TRCurlyB,      // '}'
  TInvalid,      // '~'
  TInvalid       // ''
};


static const char* ScannerTokenStrings[] =
{
  "TInvalid",
  "TKeyword",
  "TOperator",
  "TName",
  "TNumber",
  "TType",
  "TLParan",
  "TRParan",
  "TLCurly",
  "TRCurly",
  "TLSquare",
  "TRSquare",
  "TEquals",
  "TString",
  "TComment",
  "TDot",
  "TColon",
  "TSemiColon",
  "TComma",
  "TDirective"
};



void
ScannerCreate( Scanner* s, Options* o )
{
  (void)o;
  memset( s, 0, sizeof(Scanner) );
  ArrayCreateToken( &s->_tokens );
}



static inline void
AddToken( enum ScannerToken token, 
    uint8_t** start, 
    uint8_t* curr, 
    ArrayToken* arr, 
    unsigned int lineNo )
{
  Token tok;
  tok._token = token;
  tok._start = *start;
  tok._length = (unsigned char)(curr - *start);
  tok._lineNo = lineNo;
  ArrayAddToken( arr, &tok );
  *start = curr;
}



static inline int
ReadCharacterString( uint8_t** start, uint8_t* end, ArrayToken* arr, unsigned int* lineNo )
{
  unsigned int beginLineNo = *lineNo;
  ++(*start);
  uint8_t* curr = *start;
  while ( curr != end )
  {
    if ( *curr == '"'
      && *(curr-1) != '\\' )
    {
      AddToken( TString, start, curr, arr, beginLineNo );
      ++(*start);
      return 0;
    }
    else if ( *curr == '\n' )
    {
      ++(*lineNo);
    }
    ++curr;
  }

  *start = end;
  return -1;
}



static inline int
ReadSingleLineComment( uint8_t** start, uint8_t* end, ArrayToken* arr, unsigned int* lineNo )
{
  unsigned int beginLineNo = *lineNo;
  *start += 2;
  uint8_t* curr = *start;
  while ( curr != end )
  {
    if ( *curr == '\n' )
    {
      AddToken( TComment, start, curr, arr, beginLineNo );
      ++(*start);
      ++(*lineNo);
      return 0;
    }
    ++curr;
  }

  *start = end;
  return -1;
}



static inline int
ReadMultiLineComment( uint8_t** start, uint8_t* end, ArrayToken* arr, unsigned int* lineNo )
{
  unsigned int beginLineNo = *lineNo;
  *start += 2;
  uint8_t* curr = *start;

  int nested = 1;

  while ( curr != end )
  {
    if ( *curr == '*' )
    {
      // Ending '*/' has precedence
      if ( curr+1 != end
        && *(curr+1) == '/' )
      {
        --nested;

        if ( nested == 0 )
        {
          AddToken( TComment, start, curr, arr, beginLineNo );
          *start += 2;
          return 0;
        }
      }
      else if ( *(curr-1) == '/' )
      {
        ++nested;
      }
    }
    else if ( *curr == '\n' )
    {
      ++(*lineNo);
    }
    ++curr;
  }

  *start = end;
  return -1;
}



static inline int
ReadNumber( uint8_t** start, uint8_t* end, ArrayToken* arr, unsigned int* lineNo )
{
  uint8_t* curr = *start;

  unsigned int accepted = 10;
  unsigned int isHex = 0;

  if ( curr+1 != end )
  {
    if ( *curr == 0 )
    {
      if ( *(curr+1) == 'x' )
      {
        isHex = 1;
        curr += 2;
      }
      else if ( *(curr+1) == 'o' )
      {
        accepted = 8;
        curr += 2;
      }
      else if ( *(curr+1) == 'b' )
      {
        accepted = 2;
        curr += 2;
      }
    }
    
    if ( *(curr+1) == '.' ) // 0-9 followed by dot is OK
    {
      curr += 2;
    }
  }

  uint8_t* atLeastOne = curr;

  while ( curr != end )
  {
    uint8_t c = *curr - '0';
  
    if ( c >= accepted )
    {
      if ( isHex )
      {
        uint8_t ch = *curr;
        if ( ( ch >= 'a' && ch <= 'f' )
            || ( ch >= 'A' && ch <= 'F' ) )
        {
          ++curr;
          continue;
        }
      }

      break;
    }

    ++curr;
  }

  if ( curr == atLeastOne )
  {
    printf( "Line %u: Illegal number specification\n", *lineNo );
    *start = curr;
    return -1;
  }

  AddToken( TNumber, start, curr, arr, *lineNo );
  return 0;
}



static inline int
ReadAlphaNumeric( uint8_t** start, uint8_t* end, ArrayToken* arr, unsigned int* lineNo )
{
  uint8_t* curr = *start;
  ++curr; // skip first, it's an 'Alpha'

  while ( curr != end )
  {
    enum CharType t = charTypes[*curr];

    if ( !( t == Alpha || t == Numeric ) )
    {
      break;
    }
    ++curr;
  }

  const struct KeywordInfo* info = KeywordLookup( (const char*)*start, (unsigned int)(curr - *start) );
  enum ScannerToken tok = info ? info->_token : TName;
  AddToken( tok, start, curr, arr, *lineNo );
  return 0;
}



static inline int
ReadDirective( uint8_t** start, uint8_t* end, ArrayToken* arr, unsigned int* lineNo )
{
  uint8_t* curr = *start;
  ++curr; // skip first, it's '#'

  while ( curr != end )
  {
    enum CharType t = charTypes[*curr];

    if ( !( t == Alpha || t == Numeric ) )
    {
      break;
    }
    ++curr;
  }

  AddToken( TDirective, start, curr, arr, *lineNo );
  return 0;
}



int
ScannerScan( Scanner* s, Options* o )
{
  int success = 0;

  // assume some number of tokens to begin with
  ArraySetCapacityToken( &s->_tokens, o->_fileData._size / 5 );

  uint8_t* curr = o->_fileData._data;
  uint8_t* end = o->_fileData._data + o->_fileData._size;
  unsigned int lineNo = 1;

  while ( curr < end )
  {
    int retVal = 0;
    uint8_t sourceChar = *curr;
    uint8_t sourceType = charTypes[sourceChar];

    switch ( sourceType )
    {
    case Control:
      switch ( sourceChar )
      {
      case '"':
        retVal = ReadCharacterString( &curr, end, &s->_tokens, &lineNo );
        break;
      case '/':
        if ( curr+1 != end )
        {
          if ( *(curr+1) == '/' )
          {
            retVal = ReadSingleLineComment( &curr, end, &s->_tokens, &lineNo );
            break;
          }
          else if ( *(curr+1) == '*' )
          {
            retVal = ReadMultiLineComment( &curr, end, &s->_tokens, &lineNo );
            break;
          }
        }
        AddToken( TOperator, &curr, curr+1, &s->_tokens, lineNo );
        break;
      case '#':
        retVal = ReadDirective( &curr, end, &s->_tokens, &lineNo );
        break;
      default:
        {
          enum ScannerToken tokType = controlTokenTypes[sourceChar];
          if ( tokType != TInvalid )
          {
            AddToken( tokType, &curr, curr+1, &s->_tokens, lineNo );
          }
          else
          {      
            printf( "Line %u: Illegal control character found, '%c'\n", lineNo, (char)sourceChar );
            retVal = -1;
            ++curr;
          }
        }
        break;
      }
      break;
    case Numeric:
      retVal = ReadNumber( &curr, end, &s->_tokens, &lineNo );
      break;
    case Alpha:
      retVal = ReadAlphaNumeric( &curr, end, &s->_tokens, &lineNo );
      break;
    case Whitespace:
      if ( sourceChar == '\n' )
      {
        ++lineNo;
      }
      ++curr;
      break;
    case Invalid:
    default:
      {
        char buf[2] = { (char)sourceChar, 0 };
        const char* readable = buf;

        if ( sourceChar == 0x09 )
        {
          readable = "tab";
        }
        else if ( sourceChar == 0x0d )
        {
          readable = "carrige return";
        }

        printf( "Line %u: Illegal character found, '%s'(0x%x)\n", lineNo, readable, (unsigned int)sourceChar );
        retVal = -1;

        ++curr;
      }
      break;
    }

    if ( retVal == -1 )
    {
      success = -1;
    }
  }

  _Static_assert( sizeof(ScannerTokenStrings)/sizeof(ScannerTokenStrings[0]) == TTotalTokenCount,
      "Must be same size" );

  for ( unsigned int i = 0; i < s->_tokens._size; ++i )
  {
    Token* tok = &s->_tokens._data[i];
    printf( "[%-10s,%6u]: '%.*s'\n", 
        ScannerTokenStrings[tok->_token], 
        tok->_lineNo, 
        tok->_length,
        tok->_start );
  }

  return success;
}




