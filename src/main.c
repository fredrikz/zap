#include "parser.h"
#include "scanner.h"
#include "commandline.h"
#include <stdio.h>

int main( int argc, const char* const* argv )
{
  int commandLineCreated = 0;
  int scannerCreated = 0;
  int parserCreated = 0;

	Options o;
	CommandLineOptionCreate( &o );
  commandLineCreated = 1;
	int err;
  err = CommandLineParse( argc, argv, &o );
  if ( err < 0 )
  {
    printf( "ERROR: Invalid command line parameters\n" );
    goto quit;
  }

  err = CommandLinePostInitOption( &o );
  if ( err < 0 )
  {
    printf( "ERROR: Invalid command line parameters\n" );
    goto quit;
  }

	Scanner s;
	ScannerCreate( &s, &o );
  scannerCreated = 1;
	err = ScannerScan( &s, &o );
  if ( err < 0 )
  {
    printf( "ERROR: Scanner failed\n" );
    goto quit;
  }

  Parser p;
  ParserCreate( &p, &o );
  parserCreated = 1;
  p._scanner = &s;
  err = ParserParse( &p, &o );
  if ( err < 0 )
  {
    printf( "ERROR: Parser failed\n" );
    goto quit;
  }

quit:
  if ( parserCreated )
  {
    ParserDestroy( &p );
  }

  if ( scannerCreated )
  {
    ScannerDestroy( &s );
  }

  if ( commandLineCreated )
  {
    CommandLineOptionDestroy( &o );
  }
}

