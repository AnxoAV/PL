// Funciones que, de manera conjunta, calculan la casilla inicial y si es válida

/*****************************************LIBRERÍAS*****************************************/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
/*************************************FUNCIONES ESXTERNAS*************************************/
extern char aux[40];
extern char *tablero[8][8];
extern void error_jugada (char *jugada, char *mensaje);
/*************************************FUNCIONES ARCHIVO*************************************/
void strfromchar(char dest[],char src);
void anadir_pieza(int numpiezas,int i, int j);
int jugada_directa(char *jugada, char color);
int jugada_colrow(char *jugada,char color);
int jugada_especifica(char *jugada, char color);
void casilla_inicial (char *jugada, int inicia, char color);

/*************************************FUNCIÓN COMÚN*************************************/
void strfromchar(char dest[],char src){
    /*
    Función para convertir un char en un string.
    Se crea un string que tiene de primer elemento el char y de segundo el fin de string.
        * dest: String en el que se almacena.
        * src: Char que se convierte.
    */
	dest[0] = src;
	dest[1] = '\0';
}

/*************************************AÑADIR PIEZA DISPONIBLE*************************************/
void anadir_pieza(int numpiezas,int i, int j){
    /*
    Función que se emplea para añadir una pieza al listado de piezas disponibles.
    Se añaden las coordenadas a la lista de piezas posibles.
        * numpiezas: Número de piezas que se han añadido ya.
        * i: Fila de la matriz del tablero
        * j: Columna de la matriz del tablero.
    */
    char dest[2];
	char dest2[2];
	strfromchar(dest,'h'-j);
	strfromchar(dest2,'1'+i);
	if(numpiezas==0){
		strcpy(aux,dest);
	}else{
		strcat(aux,",");
		strncat(aux,dest,1);
	}				
	strncat(aux,dest2,1);
}

/**********************************POSIBLES CASILLAS INICIALES**********************************/
int jugada_directa(char *jugada, char color){
    /*
    Función que se emplea para calcular las posibles casillas iniciales si no se especifica ni fila ni columna.
    Se realiza un bucle con todas las casillas del tablero en busca de una que coincida.
        * jugada: Jugada que se está comprobando.
        * color: Color de la pieza a comprobar.
        * return: El número de piezas que se han encontrado.
    */
    int numpiezas=0;
    for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
          
			if(tablero[i][j][0]==jugada[0] && tablero[i][j][1]==color){
                  
				anadir_pieza(numpiezas,i,j);
				numpiezas++;
			}
		}
    }
    
    return numpiezas;
}

int jugada_colrow(char *jugada,char color){
    /*
    Función que se emplea para calcular las posibles casillas iniciales si especifica fila o columna.
    Se realiza un bucle con todas las casillas de dicha fila o columna en busca de una que coincida.
        * jugada: Jugada que se está comprobando.
        * color: Color de la pieza a comprobar.
        * return: El número de piezas que se han encontrado.
    */
    int numpiezas=0;
    
    for(int i=0;i<8;i++){
		if(jugada[1]=='a' || jugada[1]=='b' || jugada[1]=='c' || jugada[1]=='d' || jugada[1]=='e' || jugada[1]=='f' || jugada[1]=='g' || jugada[1]=='h'){
            int col=('h'-jugada[1]);
			if(tablero[i][col][0]==jugada[0] && tablero[i][col][1]==color){
				anadir_pieza(numpiezas,i,col);
				numpiezas++;
			}
		}else{
			int row=(int)(jugada[1]-'1');	
			if(tablero[row][i][0]==jugada[0] && tablero[row][i][1]==color){
				anadir_pieza(numpiezas,row,i);
				numpiezas++;
			}
		}
	}
    return numpiezas;
}

int jugada_especifica(char *jugada, char color){
    /*
    Función que comprueba si hay una pieza en esa casilla.
    Se compara que la pieza y el color coincidan con los especificados.
        * jugada: Jugada que se está comprobando.
        * color: Color de la pieza a comprobar.
        * return: El número de piezas que se han encontrado (1 o 0).
    */
    int col=('h'-jugada[1]);
	int row=(jugada[2]-'1');  
	
    if(tablero[row][col][0]==jugada[0] && tablero[row][col][1]==color){
		anadir_pieza(0,row,col);
        return 1;
	}else{
        return 0;
    }
}

void casilla_inicial (char *jugada, int inicia, char color){
    /*
    Función que se emplea para calcular las posibles casillas iniciales.
    Se realiza un bucle con las piezas del mismo tipo en el que comprobaremos si es una pieza válida.
        * jugada: Jugada que se está comprobando.
        * inicia: Si se trata de la jugada sola (0), se especifica fila/columna (1) o ambas (2)
        * color: Color de la pieza a comprobar.
        * return: verdadero si es un movimiento correcto, falso si se encuentra con una pieza.
    */
    int numpiezas=0;

    strcpy(aux,"");
    if(inicia==0){
        
        numpiezas=jugada_directa(jugada,color);
    }else if(inicia==1){
        numpiezas=jugada_colrow(jugada,color);
    }else if(inicia==2){
        numpiezas=jugada_especifica(jugada,color);
    }else{
        error_jugada(jugada,", tiene más caracteres de los permitidos.");
    }
	if(numpiezas==0){
        error_jugada(jugada,", el jugador no posee piezas de ese tipo.");
    }
    
}

