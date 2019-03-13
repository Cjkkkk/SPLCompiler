ifeq ($(OS), Windows_NT)
	TARGET := spl_parser.exe
else
	TARGET := spl_parser.out
endif

all: spl_lexer.l spl_parser.y
	flex lex.l
	bison -d spl_parser.y
	gcc -o $(TARGET) spl_lexer.yy.c spl_parser.tab.c -lm

.PHONY: clean
clean:
	rm spl_lexer.yy.c spl_parser.tab.c spl_parser.tab.h $(TARGET)