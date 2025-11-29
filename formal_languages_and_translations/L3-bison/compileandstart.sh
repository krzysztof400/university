bison calc.y -d
flex calc.l
gcc lex.yy.c calc.tab.c -o calc
./calc