CC = gcc
CXX = g++

EXE = spl.exe

CDEBUG = -g -Wall
CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++14

CFLAGS = -Wno-deprecated-register -O0 -fpermissive $(CDEBUG) $(CSTD)
CXXFLAGS = -Wno-deprecated-register -O0 -fpermissive $(CXXDEBUG) $(CXXSTD) -I D:\mingw32\include

CPPOBJ = main spl_driver
SOBJ =  parser lexer

FILES = $(addsuffix .cpp, $(CPPOBJ))#加后缀

OBJS  = $(addsuffix .o, $(CPPOBJ))

CLEANLIST =  $(addsuffix .o, $(OBJ)) $(OBJS) \
				 spl_parser.tab.cc spl_parser.tab.hh \
				 location.hh position.hh \
			     stack.hh spl_parser.output parser.o \
				 lexer.o spl_lexer.yy.cc \
				 spl.ilk spl.pdb $(EXE)\

.PHONY: all
all: spl

# %.o : %.cpp
#	$(CC) $(CCFLAGS) -c $< -o $@

spl:$(FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) parser.o lexer.o $(LIBS)

parser: spl_parser.y
	bison -d -v spl_parser.y
	$(CXX) $(CXXFLAGS) -c -o parser.o spl_parser.tab.cc

lexer: spl_lexer.l
	flex --outfile=spl_lexer.yy.cc spl_lexer.l
	$(CXX)  $(CXXFLAGS) -c spl_lexer.yy.cc -o lexer.o

.PHONY: clean
clean:
	del /f /q $(CLEANLIST)