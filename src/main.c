#include "scanner.h"
#include "commandline.h"

int main( int argc, const char* const* argv )
{
  int commandLineCreated = 0;
  int scannerCreated = 0;
	Options o;
	CommandLineOptionCreate( &o );
  commandLineCreated = 1;
	int err;
  err = CommandLineParse( argc, argv, &o );
  if ( err < 0 ) 
    goto quit;
  err = CommandLinePostInitOption( &o );
  if ( err < 0 )
    goto quit;

	Scanner s;
	ScannerCreate( &s, &o );
  scannerCreated = 1;
	err = ScannerScan( &s, &o );
  if ( err < 0 )
    goto quit;

quit:
  if ( scannerCreated )
  {
    ScannerDestroy( &s );
  }

  if ( commandLineCreated )
  {
    CommandLineOptionDestroy( &o );
  }
}

