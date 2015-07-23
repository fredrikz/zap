#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include "array.h"

ARRAYTYPE(char)

typedef struct Options__
{
  const char*     _currentWorkingDir;
  const char*     _fileName;
  Arraychar       _fileData;
} Options;


void  CommandLineInitOption( Options* o );
void  CommandLineShutdownOption( Options* o );
int   CommandLinePostInitOption( Options* o );
int   CommandLineParse( int argc, const char* const* argv, Options* o );


#endif // COMMANDLINE_H_

