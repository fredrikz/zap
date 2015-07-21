#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_


typedef struct Options__
{
	const char* 		_fileName;
	const char*			_currentWorkingDir;
} Options;


void 	CommandLineInitOption( Options* o );
int 	CommandLineParse( int argc, const char* const* argv, Options* o );


#endif // COMMANDLINE_H_

