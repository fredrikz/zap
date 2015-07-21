#include "commandline.h"
#include <string.h>
#include <stdio.h>

typedef int (*ReadArgFn_t)( int currArg, int argc, const char* const * argv, Options* o );

typedef struct ArgumentOption__
{
	const char*			_optionLong;
	char						_optionShort;
	const char*			_helpText;
	ReadArgFn_t			_readArgFunc;
} ArgumentOption;



	
int
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



const static ArgumentOption validOptions[] = 
{
	{ "file",		'f',			"File to be compiled", &ReadFile }

};



void
CommandLineInitOption( Options* o )
{
	memset( o, 0, sizeof(Options) );
}



int
CommandLineParse( int argc, const char* const* argv, Options* o )
{
	const int MAX_OPTIONS = sizeof(validOptions)/sizeof(validOptions[0]);

	int retVal = 0;

	for ( int i = 0; i < argc; ++i )
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

