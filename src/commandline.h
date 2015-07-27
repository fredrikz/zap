#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include "array.h"
#include <stdint.h>

ARRAYTYPE(uint8_t)

typedef struct Options
{
  const char*     _currentWorkingDir;
  const char*     _fileName;
  Arrayuint8_t    _fileData;

  const char*     _grammarFileName;
  Arrayuint8_t    _grammarFileData;
} Options;


void  CommandLineOptionCreate( Options* o );
void  CommandLineOptionDestroy( Options* o );
int   CommandLinePostInitOption( Options* o );
int   CommandLineParse( int argc, const char* const* argv, Options* o );


#endif // COMMANDLINE_H_

