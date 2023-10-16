all: lab1
lab1:
	flex practica2.l
	bison practica2.y -yd
	gcc -o practica2 lex.yy.c y.tab.c -lfl