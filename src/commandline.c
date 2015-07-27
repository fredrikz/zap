#include "commandline.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*ReadArgFn_t)( int currArg, int argc, const char* const * argv, Options* o );

typedef struct ArgumentOption__
{
  const char*     _optionLong;
  char            _optionShort;
  const char*     _helpText;
  ReadArgFn_t     _readArgFunc;
} ArgumentOption;



  
static int
ReadFile( int currArg, int argc, const char* const* argv, Options* o )
{
  if ( currArg >= argc - 1 )
  {
    printf( "Expected filename after '%s'.\n", argv[currArg] );
    return -1;
  }

  o->_fileName = argv[currArg + 1];

  return 1;
}



static int
ReadGrammarFile( int currArg, int argc, const char* const* argv, Options* o )
{
  if ( currArg >= argc - 1 )
  {
    printf( "Expected filename after '%s'.\n", argv[currArg] );
    return -1;
  }

  o->_grammarFileName = argv[currArg + 1];

  return 1;
}


const static ArgumentOption validOptions[] = 
{
  { "file",       'f',      "<arg>: File to be compiled", &ReadFile },
  { "grammar",    'g',      "<arg>: Grammar file to be compiled", &ReadGrammarFile }
};



void
CommandLineOptionCreate( Options* o )
{
  memset( o, 0, sizeof(Options) );
  ArrayCreateuint8_t( &o->_fileData );
  ArrayCreateuint8_t( &o->_grammarFileData );
}




void
CommandLineOptionDestroy( Options* o )
{
  ArrayDestroyuint8_t( &o->_fileData );
  ArrayDestroyuint8_t( &o->_grammarFileData );
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
  if ( o->_fileName == 0 && o->_grammarFileName == 0 )
  { 
    printf( "Missing '--filename' or '--grammar' argument.\n" );
    return -1;
  }

  int retVal = 0;

  if ( o->_fileName )
  {
    int tmp = LoadFile( o->_fileName, &o->_fileData );
    if ( tmp != 0 )
    {
      retVal = tmp;
    }
  }

  if ( o->_grammarFileName )
  {
    int tmp = LoadFile( o->_grammarFileName, &o->_grammarFileData );
    if ( tmp != 0 )
    {
      retVal = tmp;
    }
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
      int skip = (*validOptions[foundOption]._readArgFunc)( i, argc, argv, o );

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

