#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
extern bool tablas_activas;
extern char *tablero[8][8];
extern bool comprobar_todos_jaques(char color);
extern void yyerror (char const *message);
extern char quiero;
extern int mov_sin_comeropeon;
extern char *casillas[8][8];
extern void llamar_filas_y_columnas(char *inicia, char *termina);
extern int col_row[4];
char *piezas[12] = {"PB","TB","CB","AB","DB","RB","PN","TN","CN","AN","DN","RN"};

/*****************************************IMPRIMIR TABLERO*****************************************/
void imprimir_tablero(char color){
    /*
    Función que se emplea para imprimir el tablero.
    Se recorre mediante bucles las casillas del tablero, mostrando como lo vería el jugador que ha movido.
        * color: Color del jugador que ha movido.
    */
	if (color=='N'){
		for(int i=0;i<8;i++){
			for (int j=0;j<8;j++){
				printf("\t|%s|",tablero[i][j]);
			}
			printf("\n");
		}
	}else{
		for(int i=7;i>=0;i--){
			for (int j=7;j>=0;j--){
				printf("\t|%s|",tablero[i][j]);
			}
			printf("\n");
		}
	}
	printf("\n");
}

void anadir_a_archivo(){
	FILE *resultado = fopen("ajedrez_resultado.txt", "a+");
	for (int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			fputs(tablero[i][j],resultado);
			fputs(" ",resultado);
		}
		fputs("\n",resultado);
	}
}


/*****************************************ACTUALIZAR JUGADA*****************************************/
void actualizar_jugada(char *inicia, char *termina, char color){
    /*
    Función que se emplea para actualizar la matriz del tablero con la nueva jugada.
    Se cambian las casillas correspondientes a la jugada y, a continuación, se imprime el tablero.
        * inicia: Coordenadas de la casilla en la que se sitúa inicialmente.
        * termina: Pieza y coordenadas de la casilla en la que se sitúa al terminar.
        * color: Color de la pieza a comprobar.
    */
	
	llamar_filas_y_columnas(inicia,termina);
	char pieza[2];
	pieza[0] = tablero[col_row[1]][col_row[0]][0];
	pieza[1] = color;
	int a;
	
	for(int i=0;i<12;i++){
		if(strcmp(piezas[i],pieza)==0){
			a=i;
		}
	}
	tablero[col_row[3]][col_row[2]]=piezas[a];
	tablero[col_row[1]][col_row[0]]=casillas[(int)inicia[1]-'1'][(int)inicia[0]-'a'];
	if(comprobar_todos_jaques(color)){
		imprimir_tablero(color);
		yyerror("Movimiento ilegal: Tras este movimiento el rey está en jaque");
	}else{
		if(mov_sin_comeropeon==100){
			tablas_activas=true;
		}
		imprimir_tablero(color);
	}
}
