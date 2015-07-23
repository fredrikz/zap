#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

typedef struct Options__
{
  const char*     _currentWorkingDir;
  const char*     _fileName;
  const char*     _fileData;
  unsigned int    _fileSize;
} Options;


void  CommandLineInitOption( Options* o );
void  CommandLineShutdownOption( Options* o );
int   CommandLinePostInitOption( Options* o );
int   CommandLineParse( int argc, const char* const* argv, Options* o );


#endif // COMMANDLINE_H_

