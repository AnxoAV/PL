// Librería de funciones empleadas para comprobar jaques y jaque mates
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
extern bool llamar_comprobar_comer (char color_contrario, char *movimiento, char color);
extern void error_jugada (char *jugada, char *mensaje);
extern bool comprobar_trayectoria_torre(char *inicia, char *termina,bool iscol);
int devuelve[4]={0,0,0,0};
char posicion_rey[2]="";

/*****************************************JAQUE*****************************************/
void cas_jaque(char *posicion){
	/*
    Función que almacena en devuelve las posiciones de la pieza y el rey como enteros.
    Se realiza la conversión de char a int y se almacena en la matriz.
        * posicion: Posición de la pieza.
        * return: Casilla en la que se encuentra dicho rey.
    */
	devuelve[0]='h'-posicion[0];
	devuelve[1]=posicion[1]-'1';
	devuelve[2]='h'-posicion_rey[0];
	devuelve[3]=posicion_rey[1]-'1';
}

void encontrar_posicion_rey(char color){
 	/*
    Función que se emplea para obtener la casilla donde se encuentra el rey correspondiente a un color.
    Se realiza un bucle con las piezas del mismo tipo en el que comprobaremos si es una pieza válida.
        * color: Color del rey a buscar.
        * return: Casilla en la que se encuentra dicho rey.
    */
	
	for(int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			if(tablero[i][j][0]=='R' && tablero[i][j][1]==color){
				posicion_rey[0]='h'-j;
				posicion_rey[1]='1'+i;
			}
		}
	}
	
}

bool jaque_peon(char *posicion,char color){
	/*
    Función que se emplea para comprobar si un peón desde una posición provoca jaque.
   	Un peón realiza jaque si el rey se encuentra en diagonal hacia delante.
		* posicion: Posición del peón.
		* color: Color del peón.
        * return: true si es jaque y false si no lo es.
    */
	cas_jaque(posicion);
	if(color=='B'){
		return (fabs(devuelve[0]-devuelve[2])==1 && devuelve[3]-devuelve[1]==1);	
	}else{
		return (fabs(devuelve[0]-devuelve[2])==1 && devuelve[1]-devuelve[3]==1);
	}
}

bool jaque_alfil(char *posicion){
	/*
    Función que se emplea para comprobar si un alfil desde una posición provoca jaque.
   	Un alfil realiza un jaque si el rey se encuentra en diagonal hacia él.
		* posicion: Posición del alfil.
        * return: true si es jaque y false si no lo es.
    */
	cas_jaque(posicion);
	int inicios[2]={devuelve[0],devuelve[1]};
	if (fabs(devuelve[0]-devuelve[2])==fabs(devuelve[1]-devuelve[3])){
		if(devuelve[3]<devuelve[1]){
			inicios[1]=devuelve[3];
		}
		if(devuelve[2]<devuelve[0]){
			inicios[0]=devuelve[2];
		}
		for(int i=1;i<fabs(devuelve[1]-devuelve[3])-1;i++){
			if(tablero[inicios[0]+i][inicios[1]+i][1]=='N' || tablero[inicios[0]+i][inicios[1]+i][1]=='B'){
				
		
				return false;
			}
		}
		
		return true;
	}else{
		return false;}
}

bool jaque_caballo(char *posicion){
	/*
    Función que se emplea para comprobar si un caballo desde una posición provoca jaque.
   	Un caballo realiza un jaque si el rey se encuentra en L respecto a él.
		* posicion: Posición del caballo.
        * return: true si es jaque y false si no lo es.
    */
	cas_jaque(posicion);
	if (fabs(devuelve[0]-devuelve[2])==2 && fabs(devuelve[1]-devuelve[3])==1){
		return true;
	}else if(fabs(devuelve[0]-devuelve[2])==1 && fabs(devuelve[1]-devuelve[3])==2){
		return true;
	}else{
		return false;
	}
}

bool jaque_torre(char *posicion){
	/*
    Función que se emplea para comprobar si una torre desde una posición provoca jaque.
   	Una torre realiza un jaque si el rey se encuentra en la misma fila o columna.
		* posicion: Posición de la torre.
        * return: true si es jaque y false si no lo es.
    */
	cas_jaque(posicion);
	int inicios[2]={devuelve[0],devuelve[1]};
	if((devuelve[0]==devuelve[2]) && (devuelve[1]!=devuelve[3])){
		if(devuelve[3]<devuelve[1]){
			inicios[1]=devuelve[3];
		}
		for(int i=1;i<fabs(devuelve[1]-devuelve[3])-1;i++){
			if(tablero[inicios[1]+i][inicios[0]][1]=='N' || tablero[inicios[1]+i][inicios[0]][1]=='B'){
				
				return false;
			}
		}
		return true;
	}else if((devuelve[0]!=devuelve[2]) && (devuelve[1]==devuelve[3])){
		if(devuelve[2]<devuelve[0]){
			inicios[0]=devuelve[2];
		}
		for(int i=1;i<fabs(devuelve[0]-devuelve[2])-1;i++){
			if(tablero[inicios[1]][inicios[0]+i][1]=='N' || tablero[inicios[1]][inicios[0]+i][1]=='B'){
				
				return false;
			}
		}
		return true;
	}else{
		return false;
	}
}

bool jaque_dama(char *posicion){
	/*
    Función que se emplea para comprobar si una dama desde una posición provoca jaque.
   	Una dama realiza un jaque si lo haría siendo una torre o un alfil.
		* posicion: Posición de la dama.
        * return: true si es jaque y false si no lo es.
    */
	return (jaque_alfil(posicion) || jaque_torre(posicion));
}

bool comprobar_jaque(char pieza, char color, char *posicion){
	/*
    Función que se emplea para comprobar si una pieza desde una posición provoca jaque a otra.
   	Se calcula la comprobación dependiendo de la pieza a emplear.
		* pieza: Tipo de pieza que realiza el jaque
        * color: Color de la pieza que podría provocar jaque.
		* posicion: Posición en la que se encuentra la pieza.
        * return: true si es jaque y false si no lo es.
    */
	switch( pieza )
	{
		case 'A':
			return jaque_alfil(posicion);
		break;
		case 'C':
			return jaque_caballo(posicion);
		break;
		case 'D':
			return jaque_dama(posicion);
		break;
		case 'T':
			return jaque_torre(posicion);
		break;
		case 'R':
			return false;
		break;
		default:
			return jaque_peon(posicion,color);
		break;
	}
}

/*****************************************HAGO JAQUE*****************************************/
bool pieza_hace_jaque(char *movimiento, char color){
	/*
    Función que se emplea para comprobar si una jugada es un jaque.
   	Se calcula si, una vez encontrado el rey, la pieza le hace jaque.
		* movimiento: Jugada que se plantea si es jaque.
		* color: Color de la pieza.
        * return: true si es jaque y false si no lo es.
    */
	char pieza=movimiento[0];
	char posicion[2];
	char color_contrario='N';
	posicion[0]=movimiento[strlen(movimiento)-2];
	posicion[1]=movimiento[strlen(movimiento)-1];
	if(color=='N'){
		color_contrario='B';
	}
	encontrar_posicion_rey(color_contrario);
	comprobar_jaque(pieza,color,posicion);
}

/*****************************************REY EN JAQUE*****************************************/
bool comprobar_todos_jaques(char color){
	/*
    Función que se emplea para comprobar si un rey se encuentra en jaque.
   	Se calcula si alguna pieza hace jaque al rey.
		* color: Color del rey.
        * return: true si es jaque y false si no lo es.
    */
	char color_contrario='N';
	char posicion[2]="";
	if(color=='N'){
		color_contrario='B';
	}
	//encontrar_posicion_rey(color);
	for(int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			if(tablero[i][j][1]==color_contrario && tablero[i][j][0]!='R'){
				posicion[0]='h'-j;
				posicion[1]='1'+i;
				
				cas_jaque(posicion);
				if(comprobar_jaque(tablero[i][j][0],color_contrario,posicion)){
					return true;
				}
			}
		}
	}
	return false;
}

/*****************************************JAQUE MATE*****************************************/

bool bloqueos_alfil(char *posicion, char color){
	/*
    Función que se emplea para comprobar si una pieza puede bloquear el jaque mate de un alfil.
   	Para cada casilla que se cruza del alfil al rey se calcula si le hace jaque (si se podría mover ahí) alguna piez.
		* posicion: Posición del alfil.
		* color: Color del rey.
        * return: true si alguna pieza puede interceptar y false en caso contrario.
    */
	cas_jaque(posicion);
	char guardar_rey[2];
	guardar_rey[0]=posicion_rey[0];
	guardar_rey[1]=posicion_rey[1];
	int menor1=devuelve[1];
	int menor2=devuelve[0];
	char color_contrario='N';
	if(color=='N'){
		color_contrario='B';
	}
	if(devuelve[3]<devuelve[1]){
		menor1=devuelve[3];
	}
	if(devuelve[2]<devuelve[0]){
		menor1=devuelve[2];
	}
	for(int i=1;i<fabs(devuelve[1]-devuelve[3])-1;i++){
			posicion_rey[1]='1'+menor1+i;
			posicion_rey[0]='h'-menor2-i;
			
			if(comprobar_todos_jaques(color_contrario)){
				
				posicion_rey[1]=guardar_rey[1];
				posicion_rey[0]=guardar_rey[0];
				return true;
			}
	}
	posicion_rey[1]=guardar_rey[1];
	posicion_rey[0]=guardar_rey[0];
	return false;

}

bool bloqueos_torre(char *posicion,char color){
	/*
    Función que se emplea para comprobar si una pieza puede bloquear el jaque mate de una torre.
   	Para cada casilla que se cruza de la torre al rey se calcula si le hace jaque (si se podría mover ahí) alguna piez.
		* posicion: Posición de la torre.
		* color: Color del rey.
        * return: true si alguna pieza puede interceptar y false en caso contrario.
    */
	cas_jaque(posicion);
	char guardar_rey[2];
	guardar_rey[0]=posicion_rey[0];
	guardar_rey[1]=posicion_rey[1];
	char color_contrario='N';
	if(color=='N'){
		color_contrario='B';
	}

	if(devuelve[0]==devuelve[2]){
		int menor=devuelve[1];
		if(devuelve[1]>devuelve[3]){
			menor=devuelve[3];
		}
		for(int i=1;i<fabs(devuelve[1]-devuelve[3])-1;i++){
			posicion_rey[1]='1'+menor+i;
			
			if(comprobar_todos_jaques(color_contrario)){
				posicion_rey[0]=guardar_rey[0];
				posicion_rey[1]=guardar_rey[1];
				return true;
			}
		}
		posicion_rey[0]=guardar_rey[0];
		posicion_rey[1]=guardar_rey[1];
		return false;
	}else{
		int menor=devuelve[0];
		if(devuelve[0]>devuelve[2]){
			menor=devuelve[2];
		}
		for(int i=1;i<(fabs(devuelve[0]-devuelve[2])-1);i++){
			posicion_rey[0]='h'-menor-i;
			if(comprobar_todos_jaques(color_contrario)){
				posicion_rey[0]=guardar_rey[0];
				posicion_rey[1]=guardar_rey[1];
				return true;
			}
			
		}
		posicion_rey[0]=guardar_rey[0];
		posicion_rey[1]=guardar_rey[1];
		return false;
	}
}

bool bloqueos_dama(char *posicion,char color){
	/*
    Función que se emplea para comprobar si una pieza puede bloquear el jaque mate de una dama.
   	Para cada casilla que se cruza de la dama al rey se calcula si le hace jaque (si se podría mover ahí) alguna piez.
		* posicion: Posición de la dama.
		* color: Color del rey.
        * return: true si alguna pieza puede interceptar y false en caso contrario.
    */
	return bloqueos_alfil(posicion,color) || bloqueos_torre(posicion,color);
}

bool comprobar_bloqueos(char *jugada, char color){
	/*
    Función que se emplea para comprobar que no puede ser bloqueado un jaque mate.
   	Se llamará a la función correspondiente según el tipo de pieza.
		* jugada: Jugada realizada.
		* color: Color del rey.
        * return: true si es jaque mate y false en caso contrario.
    */
	char posicion[2];
	posicion[0]=jugada[strlen(jugada)-2];
	posicion[1]=jugada[strlen(jugada)-1];
	
	switch(jugada[0])
	{
		case 'A':
			return bloqueos_alfil(posicion,color);
		break;
		case 'D':
			return bloqueos_dama(posicion,color);
		break;
		case 'T':
			return bloqueos_torre(posicion,color);
		break;
		default:
			yyerror("No es una pieza válida");
		break;
	}
}

bool comprobar_jaque_mate(char *jugada, char color){
	/*
    Función que se emplea para comprobar si se trata de un jaque mate.
   	Comprueba que el rey esté en jaque, no se pueda mover, la pieza no pueda ser comida y que no se pueda interceptar el jaque.
		* jugada: Jugada realizada.
		* color: Color del rey.
        * return: true si es jaque mate y false en caso contrario.
    */
	encontrar_posicion_rey(color);
	char guardar_rey[2];
	guardar_rey[0]=posicion_rey[0];
	guardar_rey[1]=posicion_rey[1];
	char color_contrario='N';
	if (color=='N'){
		color_contrario='B';
	}
	if(comprobar_jaque(jugada[0],color,&jugada[1])==false){
		return false;
	}else{
		for(int i=-1;i<2;i++){
			for(int j=-1;j<2;j++){
				posicion_rey[0]=guardar_rey[0]+i;
				posicion_rey[1]=guardar_rey[1]+j;
				if(posicion_rey[0]>='a' && posicion_rey[0]<='h' && posicion_rey[0]>='1' && posicion_rey[0]<='8'){
				bool retorno=comprobar_todos_jaques(color_contrario);
				if(retorno==false){
					posicion_rey[0]=guardar_rey[0];
					posicion_rey[1]=guardar_rey[1];
					
					return false;
				}
				}
			}
		}
		
		if(llamar_comprobar_comer(color,jugada,color_contrario)){
			return false;
		}
		posicion_rey[0]=guardar_rey[0];
		posicion_rey[1]=guardar_rey[1];
		if(jugada[0]=='A' || jugada[0]=='T' || jugada[0]=='D'){
			return !comprobar_bloqueos(jugada,color);
		}
	}
}