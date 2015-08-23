#include "commandline.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int (*ReadArgFn_t)( void* userData, void* internalData, int currArg, int argc, const char* const * argv, Options* o );

struct BitFlag
{
  const char*     _name;
  uint32_t        _bitFlag;
};

struct BitFlagData
{
  const struct BitFlag*     _options;
  uint32_t                  _numOptions;
};

static struct BitFlag lexerOptions[] = 
{ 
  { "comment", LEX_COMMENTS },
  { "whitespace", LEX_WHITESPACE },
  { "everything", LEX_EVERYTHING }
};

static struct BitFlagData lexerOptionData = { lexerOptions, sizeof(lexerOptions)/sizeof(lexerOptions[0]) };


typedef struct ArgumentOption__
{
  const char*     _optionLong;
  char            _optionShort;
  const char*     _helpText;
  ReadArgFn_t     _readArgFunc;
  void*           _userData;
  void*           _internalData;
} ArgumentOption;



  
static int
ReadFile( void* userData, void* internalData, int currArg, int argc, const char* const* argv, Options* o )
{
  (void)internalData;
  if ( currArg >= argc - 1 )
  {
    printf( "Expected filename after '%s'.\n", argv[currArg] );
    return -1;
  }

  uint8_t* base = (uint8_t*)o;
  base += (unsigned int)userData;
  memcpy( base, &argv[currArg+1], sizeof(const char*) );
  return 1;
}


/*
static int
SetOption( void* userData, void* internalData, int currArg, int argc, const char* const* argv, Options* o )
{
  (void)currArg; (void)argc; (void)argv; (void)internalData;

  uint8_t* base = (uint8_t*)o;
  base += (unsigned int)userData;
  memset( base, 1, 1 );
  return 1;
} 
*/


static int
SetBitFlag( void* userData, void* internalData, int currArg, int argc, const char* const* argv, Options* o )
{
  if ( currArg >= argc - 1 )
  {
    printf( "Expected 'option' after '%s'.\n", argv[currArg] );
    return -1;
  }

  uint8_t* base = (uint8_t*)o;
  base += (unsigned int)userData;

  const struct BitFlagData* data = (const struct BitFlagData*)internalData;

  for ( uint32_t i = 0; i < data->_numOptions; ++i )
  {
    if ( strcmp( data->_options[i]._name, argv[currArg+1] ) == 0 )
    {
      assert( ( ((uint32_t)base) & 0x00000003 ) == 0 );
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-align"
      uint32_t* bitFlags = (uint32_t*)base;
#pragma clang diagnostic pop
      *bitFlags |= data->_options[i]._bitFlag;
      return 1;
    }
  }

  printf( "Invalid 'option' after '%s'.\n", argv[currArg] );
  return -1;
}


const static ArgumentOption validOptions[] = 
{
  { "file",       'f',      "<arg>: File to be compiled",                         &ReadFile,  (void*)offsetof(Options, _fileName), 0 },
  { "lexer",      'l',      "<arg>: Set extra lexer options, valid args are 'comment', 'whitespace' and/or 'everything'",  &SetBitFlag, (void*)offsetof(Options, _lexerOptions), &lexerOptionData },
  { "output",     'o',      "<arg>: Output file",                                 &ReadFile,  (void*)offsetof(Options, _outputFileName), 0 }
};



void
CommandLineOptionCreate( Options* o )
{
  memset( o, 0, sizeof(Options) );
  ArrayCreateuint8_t( &o->_fileData );
}




void
CommandLineOptionDestroy( Options* o )
{
  ArrayDestroyuint8_t( &o->_fileData );
}



static int
LoadFile( const char* fileName, Arrayuint8_t* arr )
{
  FILE* file = fopen( fileName, "rb" );
  if ( file == 0 )
  {
    printf( "Could not open '%s' for reading.\n", fileName );
    return -1;
  }

  fseek( file, 0, SEEK_END );

  ArraySetSizeuint8_t( arr, (unsigned int)ftell( file ) );
  fseek( file, 0, SEEK_SET );
  fread( (void*)arr->_data, arr->_size, 1, file );
  fclose( file );

  return 0;
}



int
CommandLinePostInitOption( Options* o )
{
  if ( o->_fileName == 0 )
  { 
    printf( "Missing '--file' argument.\n" );
    return -1;
  }

  int retVal = 0;

  int tmp = LoadFile( o->_fileName, &o->_fileData );
  if ( tmp != 0 )
  {
    retVal = tmp;
  }

  return retVal;
}



int
CommandLineParse( int argc, const char* const* argv, Options* o )
{
  const int MAX_OPTIONS = sizeof(validOptions)/sizeof(validOptions[0]);

  int retVal = 0;

  for ( int i = 1; i < argc; ++i )
  {
    const char* currArg = argv[i];
    int foundOption = -1;

    if ( currArg[0] == '-' )
    {
      if ( currArg[1] == '-' )
      {
        // Long option
        for ( int opt = 0; opt < MAX_OPTIONS; ++opt )
        {
          const ArgumentOption* currOpt = validOptions + opt;
          if ( strcmp( currArg + 2, currOpt->_optionLong ) == 0 )
          {
            foundOption = opt;
            break;
          }
        }
      
      }
      else
      {
        for ( int opt = 0; opt < MAX_OPTIONS; ++opt )
        {
          const ArgumentOption* currOpt = validOptions + opt;
          if ( currOpt->_optionShort == currArg[1] )
          {
            foundOption = opt;
            break;
          }
        }
      }
    }


    if ( foundOption == -1 )
    {
      printf( "Unknown argmument '%s'; aborting compilation.\n", currArg );
      retVal = -1;
    }
    else
    {
      int skip = (*validOptions[foundOption]._readArgFunc)( validOptions[foundOption]._userData, validOptions[foundOption]._internalData, i, argc, argv, o );

      if ( skip < 0 )
      {
        printf( "Something went wrong when interpreting 'argument %s'.\n", currArg );
        retVal = -1;
      }
      else
      {
        i += skip;
      }
    }
  }

  return retVal;
}

