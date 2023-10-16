// Funciones que, de manera conjunta, calculan la casilla inicial y si es válida

/*****************************************LIBRERÍAS*****************************************/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
/*************************************FUNCIONES EXTERNAS*************************************/
extern char *tablero[8][8];
extern int mov_sin_comeropeon;
extern int repetir_movimiento[4];
extern void yyerror (char const *message);
extern int num_puntosB;
extern int num_puntosN;

/*********************************NO EXISTEN PIEZAS SUFICIENTES*********************************/
bool piezassinmate(){
    /*
    Función que comprueba si hay piezas suficientes para continuar la partida.
    Con solo un caballo o un alfil no se puede realizar jaque mate.
    */
    int caballoB=0,alfilB=0,caballoN=0,alfilN=0,otro=0;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(tablero[i][j][0]=='R'){

            }else if(strcmp(tablero[i][j],"CN")==0){
                caballoN++;
            }else if(strcmp(tablero[i][j],"CB")==0){
                caballoB++;
            }else if(strcmp(tablero[i][j],"AN")==0){
                alfilN++;
            }else if(strcmp(tablero[i][j],"AB")==0){
                alfilB++;
            }else{
                otro++;
            }
        }
    }
    if(otro>0){
        return false;
    }else{
        if(caballoB>1 || alfilB>1 || (caballoB==1 && alfilB==1)){
            return false;
        }else if(caballoN>1 || alfilN>1 || (caballoN==1 && alfilN==1)){
            return false;
        }else{
            return true;
        }

    }
}
/*********************************COMPROBAR TABLAS*********************************/
void comprobacion_fin (){
    /*
    Función que comprueba si hay tablas.
    Hay tablas por repetir 3 movimientos, se llevan 50 movimientos sin comer o mover peón o no hay piezas suficientes para continuar.
    */
    if(tablas_activas){
		yyerror("La partida no debería haber continuado, han sido tablas.");
	}
	if(mov_sin_comeropeon==100){
		yyerror("La partida no debería haber continuado, cada jugador ha realizado 50 movimientos sin que se haya comido o movido un peón.");
	}
    if(piezassinmate()){
        yyerror("La partida no debería haber continuado, cada jugador no dispone de las piezas necesarias para poder hacer jaque mate.");
    }
}

void calcularpuntos(){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(tablero[i][j][1]=='N'){
                switch(tablero[i][j][0])
                {
			case 'A':
				num_puntosN+=3;
			break;
			case 'C':
				num_puntosN+=3;
			break;
			case 'D':
				num_puntosN+=10;
			break;
			case 'R':
			break;
			case 'T':
				num_puntosN+=5;
			break;
			default:
				num_puntosN+=1;
			break;
		    }
            }
            if(tablero[i][j][1]=='B'){
                switch(tablero[i][j][0])
                {
			case 'A':
				num_puntosB+=3;
			break;
			case 'C':
				num_puntosB+=3;
			break;
			case 'D':
				num_puntosB+=10;
			break;
			case 'R':
			break;
			case 'T':
				num_puntosB+=5;
			break;
			default:
				num_puntosB+=1;
			break;
		    }
            }
        }
    }
}