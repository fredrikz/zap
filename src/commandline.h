#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include "array.h"
#include <stdint.h>

ARRAYTYPE(uint8_t)

enum ELexExtra
{
  LEX_COMMENTS      = 1<<0,
  LEX_WHITESPACE    = 1<<1,
  LEX_EVERYTHING    = 0x7fffffff
};

typedef struct Options
{
  const char*     _currentWorkingDir;
  const char*     _fileName;
  Arrayuint8_t    _fileData;

  const char*     _outputFileName;
  uint32_t        _lexerOptions;
} Options;


void  CommandLineOptionCreate( Options* o );
void  CommandLineOptionDestroy( Options* o );
int   CommandLinePostInitOption( Options* o );
int   CommandLineParse( int argc, const char* const* argv, Options* o );


#endif // COMMANDLINE_H_

