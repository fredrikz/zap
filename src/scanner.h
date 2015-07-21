#ifndef SCANNER_H_
#define SCANNER_H_

typedef struct Scanner_ 
{
	const char* 		_filePath;


} Scanner;


void					ScannerInit( Scanner* s );
void					ScannerScan( Scanner* s );


#endif

