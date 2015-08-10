#include "scanner.h"
#include "commandline.h"
#include "generator/generator.h"

int main( int argc, const char* const* argv )
{
  int commandLineCreated = 0;
  int scannerCreated = 0;
  int generatorCreated = 0;

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

  Generator g;
  if ( o._generateGrammar )
  {
    GeneratorCreate( &g, &o );
    generatorCreated = 1;
  
    err = GeneratorGenerate( &g, &o );

    if ( err < 0 )
      goto quit;

    // For now, always quit
    goto quit;
  }

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

  if ( generatorCreated )
  {
    GeneratorDestroy( &g );
  }

  if ( commandLineCreated )
  {
    CommandLineOptionDestroy( &o );
  }
}

