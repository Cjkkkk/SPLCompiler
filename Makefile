CC = gcc
CXX = g++

ifeq ($(OS), Windows_NT)
	EXE = spl.exe
else
	EXE = spl.out
endif

CDEBUG = -g -Wall
CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++14

CFLAGS = -Wno-deprecated-register -O3 $(CDEBUG) $(CSTD) 
CXXFLAGS = -Wno-deprecated-register -O3 $(CXXDEBUG) $(CXXSTD)

DIR_SRC = src
DIR_OBJ = build
DIR_EXE = build

CPPOBJ = main spl_compiler spl_driver spl_symtab spl_ast spl_exception spl_IR spl_SSA

FILES = $(addprefix $(DIR_SRC)\, $(addsuffix .cpp, $(CPPOBJ)))
OBJS  = $(addprefix $(DIR_OBJ)\, $(addsuffix .o, $(CPPOBJ)))

CLEANLIST =	$(OBJS) \
			$(DIR_SRC)\spl_parser.tab.cc \
			$(DIR_SRC)\spl_parser.tab.hh \
			$(DIR_SRC)\location.hh \
			$(DIR_SRC)\position.hh \
			$(DIR_SRC)\stack.hh \
			$(DIR_SRC)\spl_parser.output \
			$(DIR_SRC)\spl_lexer.yy.cc \
			$(DIR_OBJ)\parser.o \
			$(DIR_OBJ)\lexer.o \
			$(DIR_OBJ)\spl_ast.o \
			$(DIR_EXE)\$(EXE) \

.PHONY: all
all: parser lexer objects link

parser: $(DIR_SRC)\spl_parser.y
	cd $(DIR_SRC) && bison -d -v spl_parser.y
	cd ..
	$(CXX) $(CXXFLAGS) -c $(DIR_SRC)\spl_parser.tab.cc -o $(DIR_OBJ)\parser.o

lexer: $(DIR_SRC)\spl_lexer.l
	flex --outfile=$(DIR_SRC)\spl_lexer.yy.cc $(DIR_SRC)\spl_lexer.l
	$(CXX) $(CXXFLAGS) -c $(DIR_SRC)\spl_lexer.yy.cc -o $(DIR_OBJ)\lexer.o

objects: $(FILES)
	$(CXX) $(CXXFLAGS) -c $(DIR_SRC)\main.cpp -o $(DIR_OBJ)\main.o
	$(CXX) $(CXXFLAGS) -c $(DIR_SRC)\spl_compiler.cpp -o $(DIR_OBJ)\spl_compiler.o
	$(CXX) $(CXXFLAGS) -c $(DIR_SRC)\spl_driver.cpp -o $(DIR_OBJ)\spl_driver.o
	$(CXX) $(CXXFLAGS) -c $(DIR_SRC)\spl_symtab.cpp -o $(DIR_OBJ)\spl_symtab.o
	$(CXX) $(CXXFLAGS) -c $(DIR_SRC)\spl_ast.cpp -o $(DIR_OBJ)\spl_ast.o
	$(CXX) $(CXXFLAGS) -c $(DIR_SRC)\spl_exception.cpp -o $(DIR_OBJ)\spl_exception.o
	$(CXX) $(CXXFLAGS) -c $(DIR_SRC)\spl_IR.cpp -o $(DIR_OBJ)\spl_IR.o
	$(CXX) $(CXXFLAGS) -c $(DIR_SRC)\spl_SSA.cpp -o $(DIR_OBJ)\spl_SSA.o
	

link: $(OBJS) $(DIR_OBJ)\parser.o $(DIR_OBJ)\lexer.o $(DIR_OBJ)\spl_ast.o
	$(CXX) $(CXXFLAGS) $(OBJS) $(DIR_OBJ)\parser.o $(DIR_OBJ)\lexer.o $(LIBS) -o $(DIR_OBJ)\$(EXE)

.PHONY: clean
clean:
ifeq ($(OS), Windows_NT)
	del /f /q $(CLEANLIST)
else
	rm -rf $(CLEANLIST)
endif