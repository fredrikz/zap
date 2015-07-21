#include "scanner.h"
#include "commandline.h"

int main( int argc, const char* const* argv )
{
	Options o;
	CommandLineInitOption( &o );
	CommandLineParse( argc, argv, &o );

	Scanner s;
	ScannerInit( &s );
	ScannerScan( &s );
}

