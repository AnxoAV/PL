### PL. Fichero makefile basico
# FUENTE: nombre fichero fuente con extension .l
FUENTE = ajedrez
# PRUEBA: nombre fichero de prueba
PRUEBA = ejemplos/ajedrez_enroque.txt
# LIB (libreria flex): lfl (gnu/linux, windows); ll (macos)
LIB = lfl

all: compile run

compile:
	flex $(FUENTE).l
	bison -o $(FUENTE).tab.c $(FUENTE).y -yd
	gcc -o $(FUENTE) lex.yy.c $(FUENTE).tab.c -$(LIB)

run:
ifeq ($(opcion),1)
		./$(FUENTE) < $(PRUEBA)
else
		./$(FUENTE)
endif

clean:
	rm $(FUENTE) lex.yy.c $(FUENTE).tab.c $(FUENTE).tab.h
