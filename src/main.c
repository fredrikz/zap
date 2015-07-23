#include "scanner.h"
#include "commandline.h"

int main( int argc, const char* const* argv )
{
	Options o;
	CommandLineInitOption( &o );
	int err;
  err = CommandLineParse( argc, argv, &o );
  if ( err < 0 ) 
    goto quit;
  err = CommandLinePostInitOption( &o );
  if ( err < 0 )
    goto quit;

	Scanner s;
	ScannerInit( &s, &o );
	err = ScannerScan( &s, &o );
  if ( err < 0 )
    goto quit;

quit:
  CommandLineShutdownOption( &o );
}

