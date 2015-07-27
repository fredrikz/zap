#ifndef GENERATOR_H_
#define GENERATOR_H_


typedef struct Generator
{
  int _dummy;


} Generator;


struct Options;

void            GeneratorCreate( Generator* g, struct Options* o );
void            GeneratorDestroy( Generator* g );
int             GeneratorGenerate( Generator* g, struct Options* o );

#endif // GENERATOR_H_

