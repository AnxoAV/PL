%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern FILE *yyin;
int yyerror(char* expected);
char* expected;
extern int yylineno;
%}

%union{
	char * tipo_string;
	char*  val;
	char* error;
}
%start statement
%token DECLARATION_OPEN DECLARATION_CLOSE
%token COMMENT_OPEN COMMENT_CLOSE
%token <tipo_string> ETIQ_OPEN ETIQ_CLOSE
%token <tipo_string> NAME TEXT 
%type <tipo_string> statement text entry

%%

statement:
    declaration statement
	comment statement
	| entry statement
	| ;

declaration:
	DECLARATION_OPEN text DECLARATION_CLOSE;

comment:
	COMMENT_OPEN TEXT COMMENT_CLOSE;
	
entry:
	ETIQ_OPEN statement ETIQ_CLOSE {if(strcmp($1+1,$3+2)!=0){yyerror($1+1);}}
	|ETIQ_OPEN TEXT ETIQ_CLOSE {if(strcmp($1+1,$3+2)!=0){yyerror($1+1);}};

text:
	TEXT text 
	|;

%%

int yyerror(char* expected)
{	
	if(strcmp(expected,"syntax error")!=0){
    	printf("Sintaxis XML incorrecta.Error en línea %d:Encontrado \"%s\" y se esperaba \"</%s\".\n",yylineno,yylval.error,expected);
	}else{
		printf("Sintaxis XML incorrecta.Error:%s, en línea %d\n",expected,yylineno);
	}
    exit(1);
}

int yywrap() 
{
    printf("Sintaxis XML correcta.\n");
    exit(1);
}

int main(int argc, char *argv[])
{
	yyparse();
}