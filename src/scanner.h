#ifndef SCANNER_H_
#define SCANNER_H_

enum ScannerToken
{
  TKeyword,
  TOperator,
  TName,
  TNumber


};

typedef struct Scanner__
{
  const char*     _filePath;


} Scanner;


void          ScannerInit( Scanner* s );
void          ScannerScan( Scanner* s );


#endif

