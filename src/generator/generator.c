#include "generator.h"
#include "../commandline.h"


void
GeneratorCreate( Generator* g, Options* o )
{
  (void)o;
  memset( g, 0, sizeof(Generator) );
}



void
GeneratorDestroy( Generator* g )
{
  (void)g;
}



int             
GeneratorGenerate( Generator* g, Options* o )
{
  (void)g;
  (void)o;
  return 0;
}

