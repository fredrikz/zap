CC = clang
LD = $(CC)
OUTPUT_OPTION=-MMD -MP -o $@
CFLAGS = -Wall -Werror -Weverything -pedantic-errors -Wno-padded -std=c11 -g
INCLUDES = 
LDFLAGS = -lm


SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)
DEP = $(SRC:%.c=%.d)
-include $(DEP)


.PHONY: clean


zap: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o zap $(LOADLIBES) $(LDLIBS)

grammar: zap
	./zap -g grammar/zap.grammar

clean:
	rm -f $(OBJ) $(DEP)
	rm -f zap

