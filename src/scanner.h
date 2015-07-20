#ifndef SCANNER_H_
#define SCANNER_H_

typedef struct Scanner_ 
{
	const char* 		_filePath;


} Scanner;


void					ScannerInit( Scanner* s );
void					ScannerReadCommandLine( Scanner* s, int argc, const char* const* argv );
void					ScannerScan( Scanner* s );


#endif

