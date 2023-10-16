// En esta librería se encuentran las funciones empleadas para comprobar movimientos de las piezas

/*****************************************LIBRERÍAS*****************************************/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
/*************************************FUNCIONES ESXTERNAS*************************************/
extern void error_jugada (char *jugada, char *mensaje);
extern void yyerror (char const *message);
extern int col_row[4];
extern char alpaso[3];
extern char *casillas[8][8];
extern char *tablero[8][8];
extern char error[80];
extern char quiero;
extern int mov_sin_comeropeon;
extern bool enroque[4];
extern void llamar_actualizar_jugada(char *inicia, char *termina, char color);
extern void llamar_casilla_inicial (char *jugada, int inicia, char color);
extern void actualizar_jugada(char *inicia, char *termina, char color);
/*************************************FUNCIONES ARCHIVO*************************************/
void cambiar_posicion(int a, int b);
bool pieza_en_casilla(char *jugada,char color);
void filas_y_columnas(char *inicia, char *termina);
bool comprobar_trayectoria_alfil(char *inicia, char *termina);
bool comprobar_alfil(char *inicia, char *termina);
bool comprobar_caballo(char *inicia, char *termina);
bool comprobar_trayectoria_torre(char *inicia, char *termina,bool iscol);
bool comprobar_torre(char *inicia, char *termina);
bool comprobar_dama(char *inicia, char *termina);
bool comprobar_rey_al_lado();
bool comprobar_rey(char *inicia, char *termina);
bool comprobar_peon(char *inicia, char *termina, char color);
bool pieza_a_comprobar(char pieza,char *inicia,char *termina,char color);
void comprobar_movimiento_legal(char *jugada, char *movimiento, char color);
void comprobar_jugada(char *jugada, int color);
bool comprobar_comer (char color_contrario, char *movimiento, char color);



/*****************************************COMÚN*****************************************/
void cambiar_posicion(int a, int b){
	/*
	Función que se emplea para intercambiar los valores de dos enteros
	Se utiliza una variable auxiliar y se cambian dos variables globales.
		* a: Primer entero.
		* b: Segundo entero.
	*/
    int x = col_row[a];
	col_row[a]=col_row[b];
	col_row[b]=x;
}

bool pieza_en_casilla(char *jugada,char color){
	
	int col = (int) ('h'-jugada[strlen(jugada)-2]);
	int row = (int) (jugada[strlen(jugada)-1]-'1');
	if(tablero[row][col][1]==color){
		return true;
	}else{
		return false;
	}
}

void filas_y_columnas(char *inicia, char *termina){
	/*
	Función que, pasadas las posiciones de inicio y fin, actualiza col_row.
	En col_row se almacenarán los datos en formato entero (correspondiente a las situacones de la matriz).
		* inicia: Casilla en la que comienza la jugada.
		* termina: Casilla en la que termina.
	*/
    col_row[0]= (int) ('h'-inicia[0]);
	col_row[1]= (int) (inicia[1]-'1');
	col_row[2]= (int) ('h'-termina[1]);
	col_row[3]= (int) (termina[2]-'1');
}


/*****************************************ALFIL*****************************************/

bool comprobar_trayectoria_alfil(char *inicia, char *termina){
    /*
    Función que se emplea para comprobar que el alfil no se cruce con piezas en paso.
    Se realiza un bucle con el movimiento que se lleva a cabo y se comprueba que nunca haya piezas.
        * inicia: Coordenadas de la casilla en la que se sitúa inicialmente.
        * termina: Pieza y coordenadas de la casilla en la que se sitúa al terminar.
        * return: verdadero si es un movimiento correcto, falso si se encuentra con una pieza.
    */
	filas_y_columnas(inicia,termina);
	bool ctrl = false;
	char posicion[2];
	int signos[2]={1,1};
	if(col_row[3]<col_row[1]){
		signos[0]=-1;
	}
	if(col_row[2]<col_row[0]){
		signos[1]=-1;
	}
	for (int i=1;i<fabs(col_row[2]-col_row[0])-1;i++){
		strcpy(posicion,tablero[col_row[1]+i*signos[0]][col_row[0]+i*signos[1]]);
		if(posicion[1] =='B' || posicion[1]=='N'){
			return false;
		}
	}
	return true;
}

bool comprobar_alfil(char *inicia, char *termina){
    /*
    Función que comprueba que el movimiento del alfil sea correcto.
    El movimiento del alfil en valor absoluto siempre será igual en vertical u horizontal.
        * inicia: Coordenadas de la casilla en la que se sitúa inicialmente.
        * termina: Pieza y coordenadas de la casilla en la que se sitúa al terminar.
        * return: Verdadero si el movimiento es correcto, falso si no lo es.
    */
    filas_y_columnas(inicia,termina);
	
	if(fabs(col_row[2]-col_row[0]) == fabs(col_row[3]-col_row[1])){
		if(comprobar_trayectoria_alfil(inicia,termina)){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

/*****************************************CABALLO*****************************************/

bool comprobar_caballo(char *inicia, char *termina){
    /*
    Función que comprueba que el movimiento del caballo sea correcto.
    El movimiento del caballo será 1 en vertical y 2 en horizontal o viceversa.
        * inicia: Coordenadas de la casilla en la que se sitúa inicialmente.
        * termina: Pieza y coordenadas de la casilla en la que se sitúa al terminar.
        * return: Verdadero si el movimiento es correcto, falso si no lo es.
    */
    filas_y_columnas(inicia,termina);

	if (fabs(col_row[2]-col_row[0])==2 && fabs(col_row[3]-col_row[1])==1){
		return true;
	}else if(fabs(col_row[2]-col_row[0])==1 && fabs(col_row[3]-col_row[1])==2){
		return true;
	}else{
		return false;
	}
}

/*****************************************TORRE*****************************************/
bool comprobar_trayectoria_torre(char *inicia, char *termina,bool iscol){
    /*
    Función que se emplea para comprobar que la torre no se cruce con piezas en paso.
    Se realiza un bucle con el movimiento que se lleva a cabo y se comprueba que nunca haya piezas.
        * inicia: Coordenadas de la casilla en la que se sitúa inicialmente.
        * termina: Pieza y coordenadas de la casilla en la que se sitúa al terminar.
        * iscol: Si se trata de un movimiento vertical (false) u horizontal (true).
        * return: verdadero si es un movimiento correcto, falso si se encuentra con una pieza.
    */
	filas_y_columnas(inicia,termina);
	
	if(!iscol){ 
		int x;
		if(col_row[2]<col_row[0]){
			cambiar_posicion(2,0);
		}
		for(int j = col_row[0]+1;j<col_row[2];j++){
			if(tablero[col_row[1]][j][1]=='N' || tablero[col_row[1]][j][1]=='B'){
				return false;
			}
		}
		return true;
	}else{
		if(col_row[3]<col_row[1]){
			cambiar_posicion(1,3);
		}
		
		
		for(int j = col_row[1]+1;j<col_row[3];j++){
			if(tablero[j][col_row[0]][1]=='N' || tablero[j][col_row[0]][1]=='B'){
				return false;
			}
		}
		return true;
	}
}

bool comprobar_torre(char *inicia, char *termina){
    /*
    Función que comprueba que el movimiento de la torre sea correcto.
    El movimiento de la torre será en vertical u horizontal (cambia fila o columna).
        * inicia: Coordenadas de la casilla en la que se sitúa inicialmente.
        * termina: Pieza y coordenadas de la casilla en la que se sitúa al terminar.
        * return: Verdadero si el movimiento es correcto, falso si no lo es.
    */
	filas_y_columnas(inicia,termina);
	if(col_row[0] != col_row[2] && col_row[1]!=col_row[3]){
		return false;
	}else if(col_row[1] != col_row[3]){
		return comprobar_trayectoria_torre(inicia,termina,true);
	}else if(col_row[0]!=col_row[2]){
		return comprobar_trayectoria_torre(inicia,termina,false);
	}
}

/*****************************************DAMA*****************************************/
bool comprobar_dama(char *inicia, char *termina){
    /*
    Función que comprueba que el movimiento de la dama sea correcto.
    El movimiento de la dama puede ser como un alfil o como una torre.
        * inicia: Coordenadas de la casilla en la que se sitúa inicialmente.
        * termina: Pieza y coordenadas de la casilla en la que se sitúa al terminar.
        * return: Verdadero si el movimiento es correcto, falso si no lo es.
    */
	if(comprobar_alfil(inicia,termina) || comprobar_torre(inicia,termina)){
		return true;
	}else{
		return false;
	}
}

/*****************************************REY*****************************************/
bool comprobar_rey_al_lado(){
	/*
	Función que comprueba que no haya un rey al lado de donde se pretende mover al rey.
		* return: Verdadero si no hay ningún rey al lado, falso en caso contrario.
	*/
	for(int i=-1;i<=1;i++){
		for(int j=-1;j<=1;j++){
			if(tablero[col_row[3]+i][col_row[2]+j][0]=='R'){
				return false;
			}
		}
	}
	return true;
}

bool comprobar_rey(char *inicia, char *termina){
	/*
    Función que comprueba que el movimiento del rey sea correcto.
    El movimiento del rey es un paso en cualquier dirección (el enroque no se contempla en esta función).
        * inicia: Coordenadas de la casilla en la que se sitúa inicialmente.
        * termina: Pieza y coordenadas de la casilla en la que se sitúa al terminar.
        * return: Verdadero si el movimiento es correcto, falso si no lo es.
    */
    filas_y_columnas(inicia,termina);
	if(fabs(col_row[2]-col_row[0])>1 || fabs(col_row[3]-col_row[1])>1){
		return false;
	} else if(fabs(col_row[2]-col_row[0])==0 && fabs(col_row[3]-col_row[1])==0){
		return false;
	} else{
		return comprobar_rey_al_lado;
	}
}

/*****************************************PEON*****************************************/
bool comprobar_peon(char *inicia, char *termina, char color){
    /*
    Función que comprueba que el movimiento del rey sea correcto.
    El movimiento del peón puede ser uno de los siguientes casos:
        - El peón puede mover siempre un paso hacia delante (si no hay otra pieza ahí).
        - El peón puede dar dos pasos si se trata de su primer movimiento.
        - El peón come con un paso en diagonal.
        - El peón puede comer al paso: si el otro jugador movió dos pasos y tú podrías haberle comido si hubiera movido uno; entonces, puedes comerle igualmente
        * inicia: Coordenadas de la casilla en la que se sitúa inicialmente.
        * termina: Pieza y coordenadas de la casilla en la que se sitúa al terminar.
        * color: Color de la pieza a comprobar.
        * return: Verdadero si el movimiento es correcto, falso si no lo es.
    */
	if(strlen(termina)==5){
		termina[1]=termina[3];
		termina[2]=termina[4];
		termina[3]='\0';
		termina[4]='\0';
	}
	if(strlen(termina)==4){
		termina[1]=termina[2];
		termina[2]=termina[3];
		termina[3]='\0';
	}

	filas_y_columnas(inicia,termina);
	// Caso comer al paso
	
	int a=1;
	if(color=='N'){
		a=-1;
	}
	if(alpaso!=""){
    	if(alpaso[0]!=color && alpaso[1]==termina[1] && (alpaso[2]==inicia[1])){
			int fila=alpaso[2]-'1';
			int columna='h'-alpaso[1];
			int columna2=alpaso[1]-'a';
			tablero[fila][columna]=casillas[fila][columna2];
			return true;
		}
	}
    // Caso comer
    if (col_row[0]!=col_row[2]){
        if(color=='B' && (col_row[3]-col_row[1])==1 && tablero[col_row[3]][col_row[2]][1]=='N'){
          return true;  
        }else if(color=='N' && (col_row[1]-col_row[3])==1 && tablero[col_row[3]][col_row[2]][1]=='B'){
            return true;
        }else{
            return false;
        }
    }

    // Casos de movimientos
	if(tablero[col_row[3]][col_row[2]][1]!='N' && tablero[col_row[3]][col_row[2]][1]!='B'){
    if(col_row[0]==col_row[2] && col_row[3]-col_row[1]==1 && color=='B'){
		if(col_row[3]==7 && quiero!='q'){
			return true;
		}else if(col_row[3]!=7 && quiero=='q'){
			return true;
		}else if(col_row[3]==7 && quiero=='q'){
			strcpy(error,"Los peones que llegan a última fila deben coronar.");
		}else if(col_row[3]!=7 && quiero!='q'){
			strcpy(error,"No se puede coronar si no es un peón en última fila.");
		}
        
    }else if(col_row[0]==col_row[2] && col_row[1]-col_row[3]==1 && color=='N'){
        if(col_row[3]==0 && quiero!='q'){
			return true;
		}else if(col_row[3]!=0 && quiero=='q'){
			return true;
		}else if(col_row[3]==0 && quiero=='q'){
			strcpy(error,"Los peones que llegan a última fila deben coronar.");
		}else if(col_row[3]!=0 && quiero!='q'){
			strcpy(error,"No se puede coronar si no es un peón en última fila.");
		}
    }else if(col_row[0]==col_row[2] && col_row[3]-col_row[1]==2 && col_row[1]==1 && color=='B'){
		strcpy(alpaso,termina);
		alpaso[0]=color;
        return true;
    }else if(col_row[0]==col_row[2] && col_row[1]-col_row[3]==2 && col_row[1]==6 && color=='N'){
        strcpy(alpaso,termina);
		alpaso[0]=color;
        return true;
    }else{
        return false;
    }
	}else{
		return false;
	}
}

/*****************************************SWITCH*****************************************/
bool pieza_a_comprobar(char pieza,char *inicia,char *termina,char color){
	/*
    Función que se emplea para elegir las comprobaciones dependiendo del tipo de pieza.
    Se realiza un bucle con las piezas del mismo tipo en el que comprobaremos si es una pieza válida.
        * pieza: Tipo de pieza que se comprueba.
		* inicia: Coordenadas de la casilla en la que se sitúa inicialmente.
        * termina: Pieza y coordenadas de la casilla en la que se sitúa al terminar.
        * color: Color de la pieza a comprobar.
        * return: verdadero si es un movimiento correcto, falso si se encuentra con una pieza.
    */
	switch(pieza)
		{
			case 'A':
				return comprobar_alfil(inicia,termina);
			break;
			case 'C':
				return comprobar_caballo(inicia,termina);
			break;
			case 'D':
				return comprobar_dama(inicia,termina);
			break;
			case 'R':
				return comprobar_rey(inicia,termina);
			break;
			case 'T':
				return comprobar_torre(inicia,termina);
			break;
			default:
				return comprobar_peon(inicia,termina,color);
			break;
		}
}

/*****************************************COMPROBACIÓN*****************************************/
void comprobar_movimiento_legal(char *jugada, char *movimiento, char color){
    /*
    Función que se emplea para elegir las comprobaciones dependiendo del tipo de pieza.
    Se realiza un bucle con las piezas del mismo tipo en el que comprobaremos si es una pieza válida.
        * inicia: Coordenadas de la casilla en la que se sitúa inicialmente.
        * termina: Pieza y coordenadas de la casilla en la que se sitúa al terminar.
        * color: Color de la pieza a comprobar.
        * return: verdadero si es un movimiento correcto, falso si se encuentra con una pieza.
    */
	
	char pieza = jugada[0];
	char *piezas_disponibles = strtok(aux, ",");
	bool pieza_correcta = false;
	int pos=0;
	char pasar[2]="";
	
	pieza_correcta = pieza_en_casilla(movimiento,color);
	if(pieza_correcta){	
            error_jugada(jugada,", la casilla está ocupada por una pieza del propio jugador.");	
			piezas_disponibles = strtok(NULL, ",");	
	}else{
	while( piezas_disponibles != NULL ) {
		
		pieza_correcta = pieza_a_comprobar(pieza,piezas_disponibles,movimiento,color);
		
		if(pieza_correcta){		
			pos++;
			strcpy(pasar,piezas_disponibles);
		}
		piezas_disponibles = strtok(NULL, ",");
		}
	}
	
	if(pos==1){
		
		llamar_actualizar_jugada(pasar,movimiento,color);
	}else if(pos>1) {
		error_jugada(jugada,", está incompleta. No se especifica qué pieza mover ahí.");
	}else{
		if(strcmp(error,"")==0){
        	error_jugada(jugada,", es ilegal. No se puede mover la pieza ahí.");
		}else{
			yyerror(error);
		}
	}
}

void comprobar_jugada(char *jugada, int color){
	/*
	Función que comprueba que una jugada sea correcta.
	Se trata de la función que va llamando a todas las auxiliares.
		* jugada: Jugada a comprobar.
		* color: 0 si es blanco y 1 si es negro.
	*/
	char colores_disponibles[2] = {'B','N'};
	char color_jugada=colores_disponibles[color];
	char *casillas[8]={"","","","","","","",""};;
	char peon[5];
	char movimiento[3];
	if(alpaso[0]==color_jugada){
		strcpy(alpaso,"");
	}
	
	mov_sin_comeropeon++;
	
	//strcpy(ultimos_movimientos[color],jugada);
	switch( jugada[0] )
	{
    case 'A':
		movimiento[0]=jugada[0];
		movimiento[1]=jugada[strlen(jugada)-2];
		movimiento[2]=jugada[strlen(jugada)-1];
		casilla_inicial(jugada,(strlen(jugada)-3),color_jugada);
		comprobar_movimiento_legal(jugada,movimiento,color_jugada);
		break;
    case 'R':
		enroque[2*color]=false;
		enroque[2*color+1]=false;
		movimiento[0]=jugada[0];
		movimiento[1]=jugada[strlen(jugada)-2];
		movimiento[2]=jugada[strlen(jugada)-1];
		casilla_inicial(jugada,(strlen(jugada)-3),color_jugada);
		comprobar_movimiento_legal(jugada,movimiento,color_jugada);
		break;
	case 'D':
	
		movimiento[0]=jugada[0];
		movimiento[1]=jugada[strlen(jugada)-2];
		movimiento[2]=jugada[strlen(jugada)-1];
		casilla_inicial(jugada,(strlen(jugada)-3),color_jugada);
		
		comprobar_movimiento_legal(jugada,movimiento,color_jugada);
		
		break;
    case 'C':	
		movimiento[0]=jugada[0];
		movimiento[1]=jugada[strlen(jugada)-2];
		movimiento[2]=jugada[strlen(jugada)-1];
		casilla_inicial(jugada,(strlen(jugada)-3),color_jugada);
		comprobar_movimiento_legal(jugada,movimiento,color_jugada);
		break;
	case 'T':
		movimiento[0]=jugada[0];
		movimiento[1]=jugada[strlen(jugada)-2];
		movimiento[2]=jugada[strlen(jugada)-1];
		casilla_inicial(jugada,(strlen(jugada)-3),color_jugada);
		comprobar_movimiento_legal(jugada,movimiento,color_jugada);
		break;
	default:
		mov_sin_comeropeon=0;
		strcpy(peon,"P");
		strcat(peon,jugada);
        casilla_inicial(peon,(strlen(peon)-3),color_jugada);
		comprobar_movimiento_legal(jugada,peon,color_jugada);
		break;
	}
}

bool comprobar_comer (char color_contrario, char *movimiento, char color){
	char jugada[2]="";
	bool pieza_correcta;
	char pieza=movimiento[0];
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			if(tablero[i][j][1]==color_contrario){
				movimiento[0]=tablero[i][j][0];
				jugada[0]='h'-j;
				jugada[1]='1'+i;
			switch(tablero[i][j][0])
			{
			case 'A':
			pieza_correcta = comprobar_alfil(jugada,movimiento);
			break;
			case 'C':
			pieza_correcta = comprobar_caballo(jugada,movimiento);
			break;
			case 'D':
			pieza_correcta = comprobar_dama(jugada,movimiento);
			break;
			case 'R':
			
			pieza_correcta = comprobar_rey(jugada,movimiento) && !comprobar_todos_jaques(color_contrario);
			break;
			case 'T':
			pieza_correcta = comprobar_torre(jugada,movimiento);
			break;
			default:
			pieza_correcta = comprobar_peon(jugada,movimiento,color);
			break;
			}
			if(pieza_correcta==true){
				movimiento[0]=pieza;
				return true;
			}
			}
		}
	}
	movimiento[0]=pieza;
	return false;
}
