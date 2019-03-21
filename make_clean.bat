rmdir /s /q build
mkdir build
del /f /s /q test/spl.exe
cd src
del /f /s /q location.hh
del /f /s /q position.hh
del /f /s /q spl_lexer.yy.cc
del /f /s /q spl_parser.output
del /f /s /q spl_parser.tab.cc
del /f /s /q spl_parser.tab.hh
del /f /s /q stack.hh
cd ..