#ifndef PARSER_H_
#define PARSER_H_

typedef struct Parser__
{
  int _dummy;
} Parser;

struct Options;

void      ParserCreate( Parser* p, struct Options* o );
int       ParserParse( Parser* p, struct Options* o );
void      ParserDestroy( Parser* p );

#endif // PARSER_H_

