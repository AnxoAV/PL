
%{
#include <stdlib.h>
#include "ajedrez.tab.h"
#include <string.h>
%}


coordenaday [1-8]
pieza [DARTC]
coordenadax [a-h]
delim [ \t]
jugada {pieza}*{coordenadax}*{coordenaday}*{coordenadax}{coordenaday}
nuevo_movimiento [\n]
linea {nuevo_movimiento}*
coronar "="[DATC]
numeros [0-1]
resultado_victoria {numeros}"-"{numeros}
resultado_empate "1/2-1/2"
jaque "+"
jaque_mate "++"
enroque "0-0"
enroque_largo "0-0-0"
cabecera "Blancas"{delim}+"Negras"

ofrecer_tablas "(=)"
capturar_pieza {pieza}*{coordenadax}*{coordenaday}*"x"

%%
{delim}  ;
{linea}  ;
{coronar} {yylval.s = strdup(yytext); return CORONACION;}
{jugada} {yylval.s = strdup(yytext); return JUGADA;};
{enroque_largo} {yylval.s = strdup(yytext); return ENROQUE;};
{enroque} {yylval.s = strdup(yytext); return ENROQUE;};
{resultado_victoria} {yylval.s = strdup(yytext); return VICTORIA;};
{resultado_empate} {yylval.s = strdup(yytext); return EMPATE;};
{jaque} {yylval.s = strdup(yytext); return  JAQUE;};
{jaque_mate} {yylval.s = strdup(yytext); return  JAQUE_MATE;};;
{ofrecer_tablas} {yylval.s = strdup(yytext); return  TABLAS;}
{cabecera} {yylval.s = strdup(yytext); return CABECERA;}
{capturar_pieza} {yylval.s = strdup(yytext); return COMER;}

. ;
%%