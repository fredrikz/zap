#include "scanner.h"
#include <string.h>
#include <stdio.h>
#include "commandline.h"

enum CharTypes {
  Invalid,
  Whitespace,
  Alpha,
  Numeric,
  Control,
  Underscore
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

static uint8_t 
charType[256] = 
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
  Underscore,   // '_' // Special case
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



void
ScannerInit( Scanner* s, Options* o )
{
  (void)o;
  memset( s, 0, sizeof(Scanner) );
  ArrayCreateToken( &s->_tokens );
}



static inline int
ReadCharacterString( uint8_t** start, uint8_t* end, ArrayToken* arr )
{
  (*start)++; // Skip '"'
  uint8_t* curr = *start;
  while ( curr != end )
  {
    if ( *curr == '"'
      && *(curr-1) != '\\' )
    {
      Token tok;
      tok._token = TString;
      tok._start = *start;
      tok._length = (unsigned char)(curr - *start);
      ArrayAddToken( arr, &tok );
      *start = curr;
      return 0;
    }
    ++curr;
  }

  *start = end;
  return -1;
}



int
ScannerScan( Scanner* s, Options* o )
{
  int retVal = 0;

  // assume some number of tokens to begin with
  ArraySetCapacityToken( &s->_tokens, o->_fileData._size / 5 );

  uint8_t* curr = o->_fileData._data;
  uint8_t* end = o->_fileData._data + o->_fileData._size;

  //uint8_t* first = curr;
  //uint8_t currType = Invalid;

  for (; curr < end; ++curr )
  {
    uint8_t sourceChar = *curr;
    uint8_t sourceType = charType[sourceChar];
    switch ( sourceType )
    {
    case Underscore:

      break;
    case Control:
      switch ( sourceChar )
      {
      case '"':
        retVal = ReadCharacterString( &curr, end, &s->_tokens );
        break;
      }
      break;
    case Numeric:
      break;
    case Alpha:
      printf( "A" );
      break;
    case Whitespace:
      
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

        printf( "Illegal character found in source file, '%s'(0x%x)\n", readable, (unsigned int)sourceChar );
        retVal = -1;
      }
    }
  }

  printf( "\n" );
  for ( unsigned int i = 0; i < s->_tokens._size; ++i )
  {
    Token* tok = &s->_tokens._data[i];
    printf( "Type %u: '%.*s'\n", tok->_token, tok->_length, tok->_start );
  }

  return retVal;
}




