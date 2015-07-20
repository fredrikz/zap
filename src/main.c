#include "scanner.h"

int main( int argc, const char* const* argv )
{
	Scanner s;
	ScannerInit( &s );
	ScannerReadCommandLine( &s, argc, argv );
	ScannerScan( &s );
}

