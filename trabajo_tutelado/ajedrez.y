/* Autores: 
    Anxo Abuelo Veira
    Marta Martín de Argenta Hernández 
*/

%{
	
#include <stdlib.h>
#include "actualizar_imprimir.h"
#include "casilla_inicial.h"
#include "comprobaciones.h"
#include "finpartida.h"
#include "jaque_mate.h"
#include "tablas.h"


void leer_tablero ();

bool tablas_activas=false;
bool simbolo_tablas=false;
char ultimos_movimientos[4][10]={"","","",""};
int repetir_movimiento[4]={0,0,0,0};
int num_movB=0;
int num_movN=0;
int mov_sin_comeropeon=0;

int num_jaquesB=0;
int num_jaquesN=0;

int num_puntosB=0;
int num_puntosN=0;

int col_row[4]={0,0,0,0};
char aux[40]="";

bool mate=false;
char quiero='q';
char color='B';

int ganador=0;
bool enroque[4] = {true,true,true,true};


char *peon = "";
char alpaso[3]="";

int yylex();
char error[80]="";
void yyerror (char const *message);
void error_jugada (char *jugada, char *mensaje);

bool llamar_comprobar_comer (char color_contrario, char *movimiento, char color);
void llamar_actualizar_jugada(char *inicia, char *termina, char color);
void llamar_casilla_inicial (char *jugada, int inicia, char color);
bool llamar_comprobar_todos_jaques(char color);

char *casillas[8][8]={
	{"a1","b1","c1","d1","e1","f1","g1","h1"},
	{"a2","b2","c2","d2","e2","f2","g2","h2"},
	{"a3","b3","c3","d3","e3","f3","g3","h3"},
	{"a4","b4","c4","d4","e4","f4","g4","h4"},
	{"a5","b5","c5","d5","e5","f5","g5","h5"},
	{"a6","b6","c6","d6","e6","f6","g6","h6"},
	{"a7","b7","c7","d7","e7","f7","g7","h7"},
	{"a8","b8","c8","d8","e8","f8","g8","h8"},
};
char *tablero[8][8]={
	{"h1","g1","f1","e1","d1","c1","b1","a1"},
	{"h2","g2","f2","e2","d2","c2","b2","a2"},
	{"h3","g3","f3","e3","d3","c3","b3","a3"},
	{"h4","g4","f4","e4","d4","c4","b4","a4"},
	{"h5","g5","f5","e5","d5","c5","b5","a5"},
	{"h6","g6","f6","e6","d6","c6","b6","a6"},
	{"h7","g7","f7","e7","d7","c7","b7","a7"},
	{"h8","g8","f8","e8","d8","c8","b8","a8"},
};
%}

%union{
	char *s; /* cadena de caracteres */
}

%token <s> CABECERA

%token <s> JUGADA
%token <s> ENROQUE
%token <s> CORONACION
%token <s> COMER

%token <s> JAQUE
%token <s> JAQUE_MATE
%token <s> TABLAS

%token <s> VICTORIA
%token <s> EMPATE

%type <s> resultado partida movimientoB movimientoN tipojugadaB tipojugadaN movimientoBT movimientoNT

%start S
%%
S : CABECERA partida {
		if(ganador==0){
			if(!comprobar_piezas_suficientes || tablas_activas || simbolo_tablas){
				printf("Partida correcta\n");
				printf("Ha ocurrido un empate\n");
				anadir_a_archivo();
			}else{
				yyerror("La partida no son tablas como se especifica.");
			}
			
		}else{
			if(!comprobar_piezas_suficientes || tablas_activas || simbolo_tablas){
				yyerror("La partida son tablas y no se especifica.");
			}else{
				if(ganador==1){
					printf("Partida correcta\n");
					printf("Han ganado blancas\n");
					anadir_a_archivo();
					
				}else{
					printf("Partida correcta\n");
					printf("Han ganado negras\n");
					anadir_a_archivo();
				}
				calcularpuntos();
				printf("Blancas ha hecho %d movimientos\n",num_movB);
				printf("Negras ha hecho %d movimientos\n",num_movN);
				printf("Blancas ha hecho %d jaques\n",num_jaquesB);
				printf("Negras ha hecho %d jaques\n",num_jaquesN);
				printf("Blancas ha terminado con %d puntos de material\n",num_puntosB);
				printf("Negras ha terminado con %d puntos de material\n",num_puntosN);
			}
		}
	}
	;
	partida:
	  movimientoBT movimientoNT partida {
			if(strcmp($1,ultimos_movimientos[num_movB%2])==0){
				repetir_movimiento[num_movB%2]++;
				if(repetir_movimiento[num_movB%2]==2){
					tablas_activas=true;
				}
				num_movB++;
			}else{
				//ultimos_movimientos[num_movB%2]="";
				
				strcpy(ultimos_movimientos[num_movB%2],$1);
				
				repetir_movimiento[num_movB%2]=0;
				repetir_movimiento[2+(num_movB%2)]=0;
				num_movB++;
			}
			if(strcmp($2,ultimos_movimientos[2+(num_movB%2)])==0){
				repetir_movimiento[2+(num_movN%2)]++;
				if(repetir_movimiento[2+num_movN%2]==2){
					tablas_activas=true;
				}
				num_movN++;
			}else{
				//ultimos_movimientos[2+num_movB%2]="";
				strcpy(ultimos_movimientos[2+(num_movB%2)],$2);
				repetir_movimiento[2+(num_movN%2)]=0;
				repetir_movimiento[2+num_movN%2]=0;
				num_movN++;
			}
			
	  }
	| movimientoBT resultado {
		if(mate==true && ganador!=1){
			yyerror("No puede hacer jaque mate un jugador y no ganar");
		}
	}
	| resultado {
		if(mate==true && ganador!=2){
			yyerror("No puede hacer jaque mate un jugador y no ganar");
		}
	}
	;
	movimientoBT:
		movimientoB{
			simbolo_tablas=false;
		}
		| movimientoB TABLAS {
			simbolo_tablas=true;
		}
	;
	movimientoNT:
		movimientoN{
			simbolo_tablas=false;
		}
		| movimientoN TABLAS{
			simbolo_tablas=true;
		}
	;
	movimientoB: 
		tipojugadaB JAQUE {	printf("\n\t\t\t\tJUGAMOS %s\n",$1);
						printf("\n");
						quiero='q';
						num_jaquesB++;
						if(!pieza_hace_jaque($1,'B')){
							yyerror("La jugada no es jaque");
						}}
		| tipojugadaB JAQUE_MATE {
			printf("\n\t\t\t\tJAQUE MATE %s\n",$1);
			printf("\n");
			if(comprobar_jaque_mate($1,'N')){
			mate=true;
			}else{
				yyerror("La jugada no es jaque mate");
			}
		}
		| tipojugadaB {
			quiero='q';
			if(pieza_hace_jaque($1,'B') || comprobar_jaque_mate($1,'N')){
				yyerror("La jugada es jaque o mate y no se ha marcado");
			}
		}
		
		| tipojugadaB CORONACION {
			quiero=$2[1];

			printf("\n\t\t\t\tJUGAMOS %s%s\n",$1,$2);
			printf("\n");
			if(pieza_hace_jaque($1,'B')){
					yyerror("La jugada es jaque y no se ha marcado");
			}else{
				char anadir[2];
				anadir[0]=quiero;
				anadir[1]=color;
				tablero[col_row[3]][col_row[2]] = anadir;
				imprimir_tablero('B');
			}
			
		}
	;
	movimientoN: 
		tipojugadaN JAQUE {printf("\n\t\t\t\tJUGAMOS %s\n",$1);
						printf("\n");
						quiero='q';
						num_jaquesN++;
						if(!pieza_hace_jaque($1,'N')){
							yyerror("La jugada no es jaque");
						}}
		
		| tipojugadaN JAQUE_MATE {
			printf("\n\t\t\t\tJAQUE MATE %s\n",$1);
			printf("\n");
			if(comprobar_jaque_mate($1,'B')){
			mate=true;
			}else{
				yyerror("La jugada no es jaque mate");
			}
		}
		| tipojugadaN {
			quiero='q';
			if(pieza_hace_jaque($1,'N') || comprobar_jaque_mate($1,'B')){
				yyerror("La jugada es jaque o mate y no se ha marcado");
			}
		}
		| tipojugadaN CORONACION {
			quiero=$2[1];

			printf("\n\t\t\t\tJUGAMOS %s%s\n",$1,$2);
			printf("\n");
			if(pieza_hace_jaque($1,'N')){
					yyerror("La jugada es jaque y no se ha marcado");
			}else{
				char anadir[2];
				anadir[0]=quiero;
				anadir[1]=color;
				tablero[col_row[3]][col_row[2]] = anadir;
				imprimir_tablero('N');
			}
			}
		
	;
	tipojugadaB:
		JUGADA{
			printf("\n\t\t\t\tJUGAMOS %s\n",$1);
			printf("\n");
			
			
				if(!comprobar_ahogado('B')){
					comprobar_jugada($1, 0);
				}else{
					yyerror("El rey está ahogado");
				}
	
			}
		| COMER JUGADA{
			printf("\n\t\t\t\tJUGAMOS %s%s\n",$1,$2);
			printf("\n");
			int fila;
			int columna;
			char pieza='P';
			int len=strlen($2);
			if(len==3){
				
				pieza=$2[0];
				fila=$2[2]-'1';
				columna='h'-$2[1];
				$2[0]=$2[1];
				$2[1]=$2[2];
				$2[2]='\0';
			}else{
				fila=$2[1]-'1';
				columna='h'-$2[0];
			}
			$1[1]='\0';
			strcat($1,$2);
			if(tablero[fila][columna][0]==pieza){
				comprobar_jugada($1, 0);
			}else if($1[0]>='a' && $1[0]<'i' && tablero[4][columna][0]==pieza){
				comprobar_jugada($1, 0);
			}else{
				yyerror("No existe la pieza que se quiere comer");
			}
		}
		| ENROQUE {
			
			quiero='q';
			if(strcmp($1,"0-0")==0){
				printf("\n\t\t\t\tENROQUE CORTO DE BLANCAS\n");
				printf("\n");
				if(enroque[0]==true){
					if(tablero[0][1][1]=='N' || tablero[0][1][1]=='B' || tablero[0][2][1]=='N' || tablero[0][2][1]=='B'){
						yyerror("No se puede enrocar si hay piezas en medio\n");
					}else{
						tablero[0][0]="a1";
						tablero[0][1]="RB";
						tablero[0][2]="TB";
						tablero[0][3]="e1";
						imprimir_tablero('B');
					}
				}else{
				
					yyerror("No se puede enrocar si has movido la torre o el rey\n");
				}
			}else{
				printf("\n\t\t\t\tENROQUE LARGO DE BLANCAS\n");
				printf("\n");
				if(enroque[1]==true){
					if(tablero[0][6][1]=='N' || tablero[0][6][1]=='B' || tablero[0][5][1]=='N' || tablero[0][5][1]=='B'|| tablero[0][4][1]=='N' || tablero[0][4][1]=='B'){
						yyerror("No se puede enrocar si hay piezas en medio\n");
					}else{
						tablero[0][7]="h1";
						tablero[0][6]="RB";
						tablero[0][5]="TB";
						tablero[0][4]="e1";
						imprimir_tablero('B');
					}
				}else{
				
					yyerror("No se puede enrocar si has movido la torre o el rey, o si ya has enrocado\n");
				}
			}
			enroque[0]=enroque[1]=false;
		}
		;
		tipojugadaN:
		JUGADA{
			printf("\n\t\t\t\tJUGAMOS %s\n",$1);
			printf("\n");
			quiero='q';
			
				if(!comprobar_ahogado('N')){
					
					comprobar_jugada($1, 1);
				}else{
					yyerror("El rey está ahogado");
				}
			}
		| COMER JUGADA{
			printf("\n\t\t\t\tJUGAMOS %s%s\n",$1,$2);
			printf("\n");
			int fila;
			int columna;
			char pieza='P';
			int len=strlen($2);
			if(len==3){
				
				pieza=$2[0];
				fila=$2[2]-'1';
				columna='h'-$2[1];
				$2[0]=$2[1];
				$2[1]=$2[2];
				$2[2]='\0';
			}else{
				fila=$2[1]-'1';
				columna='h'-$2[0];
			}
			$1[1]='\0';
			strcat($1,$2);
			if(tablero[fila][columna][0]==pieza){
				comprobar_jugada($1, 1);
			}else if($1[0]>='a' && $1[0]<'i' && tablero[4][columna][0]==pieza){
				comprobar_jugada($1, 1);
			}else{
				yyerror("No existe la pieza que se quiere comer");
			}
		}
		| ENROQUE {
			quiero='q';
			if(strcmp($1,"0-0")==0){
				printf("\n\t\t\t\tENROQUE CORTO DE NEGRAS\n");
				printf("\n");
				if(enroque[2]==true){
					if(tablero[7][1][1]=='N' || tablero[7][1][1]=='B' || tablero[7][2][1]=='N' || tablero[7][2][1]=='B'){
						yyerror("No se puede enrocar si hay piezas en medio\n");
					}else{
						tablero[7][0]="a8";
						tablero[7][1]="RN";
						tablero[7][2]="TN";
						tablero[7][3]="e8";
						imprimir_tablero('N');
					}
				}else{
				
					yyerror("No se puede enrocar si has movido la torre o el rey, o si ya has enrocado\n");
				}
			}else{
				printf("\n\t\t\t\tENROQUE LARGO DE NEGRAS\n");
				printf("\n");
				if(enroque[3]==true){
					if(tablero[7][6][1]=='N' || tablero[7][6][1]=='B' || tablero[7][5][1]=='N' || tablero[7][5][1]=='B'|| tablero[7][4][1]=='N' || tablero[7][4][1]=='B'){
						yyerror("No se puede enrocar si hay piezas en medio\n");
					}else{
						tablero[7][7]="h8";
						tablero[7][6]="RN";
						tablero[7][5]="TN";
						tablero[7][4]="e8";
						imprimir_tablero('N');
					}
				}else{
				
					yyerror("No se puede enrocar si has movido la torre o el rey, o si ya has enrocado\n");
				}
			}
			enroque[2]=enroque[3]=false;
		} 
	resultado:
		 VICTORIA {
			if(strcmp($1,"1-0")==0){
				ganador=1;
			}else{
				ganador=2;
			}
		}
		| EMPATE {
			ganador=0;
				
			
		}
	;

%%
int main() {
	leer_tablero();
	yyparse();
	return 0;
}

void leer_tablero ()
/*
	Función empleada para almacenar el tablero en la variable.
	Se va leyendo el archivo "tablero.txt" caracter a caracter, almacenando si se encuentra una pieza.
*/
{
  	char *filename = "tablero.txt";
    FILE *fp = fopen(filename, "r");
	int i=0;
	int j=0;
	int k=0;
	char aux_tablero[2]="";
    if (fp == NULL)
    {
        yyerror("Error: no se ha podido abrir el archivo tablero.txt");
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF){
		if(ch!='\n' && ch!=' '){
		aux_tablero[k]=ch;
		if(k==1){
			if(aux_tablero[1]=='B' || aux_tablero[1]=='N'){
				for(int a=0;a<12;a++){
					if(strcmp(piezas[a],aux_tablero)==0){
						tablero[j][i]=piezas[a];
				}}
			}
			
			k=-1;
			i++;
		}
		if(i==8 && k==-1){
			i=0;
			j++;
		}
		k++;
		}
	}
    // close the file
    fclose(fp);
	printf("Tablero inicial:\n");
	for(int a=0;a<8;a++){
		for(int b=0;b<8;b++){
			printf("%s ",tablero[a][b]);
		}
		printf("\n");
	}
}

/*****************************************ERRORES*****************************************/
void yyerror (char const *message) {
	/*
	Función empleada para lanzar un mensaje de error.
	Se manda el error y después se termina el programa.
		* message: Mensaje de error.
	*/
	fprintf(stderr,"ERROR: %s\n", message);
	exit (1);
}
void error_jugada (char *jugada, char *mensaje){
	/*
	Función empleada para lanzar un mensaje de error especificando la jugada.
	Se concatenan los mensajes con la jugada y se envía a la función anterior.
		* jugada: Jugada en la que ha ocurrido el error.
		* mensaje: Mensaje de error.
	*/
    strcpy(error,"Error en la jugada ");
	strcat(error,jugada);
	strcat(error,mensaje);
	yyerror(error);
}

/****************************FUNCIONES PARA LLAMAR A LIBRERÍAS DESDE OTRAS*************************/
void llamar_casilla_inicial (char *jugada, int inicia, char color){
	casilla_inicial(jugada,inicia,color);
}

bool llamar_comprobar_comer (char color_contrario, char *movimiento, char color){
	comprobar_comer (color_contrario, movimiento, color);
}

bool llamar_comprobar_todos_jaques(char color){
	return comprobar_todos_jaques(color);
}

void llamar_actualizar_jugada(char *inicia, char *termina, char color){
	actualizar_jugada(inicia, termina, color);
}

void llamar_filas_y_columnas(char *inicia, char *termina){
    filas_y_columnas(inicia, termina);
}

void llamar_encontrar_posicion_rey(char color){
	encontrar_posicion_rey(color);
}