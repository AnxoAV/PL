%{
#include "y.tab.h"
#include <stdbool.h>

int lines_number = 0;
%}

letter												[A-Za-záéíóú]
space                                               [ ]
point                                               [\.]
digit												[0-9]
special_symbols										"&lt"|"&gt"|"&amp"
all_symbols											[\~\@\#\$\%\^\*\!\"\(\)\+\{\}\"\|\[\]\;\'\\\,\.\/\:]		
%option yylineno			

%%
"<!--" {return COMMENT_OPEN;}
"-->"  {return COMMENT_CLOSE;}
"<?"												{return DECLARATION_OPEN;}
"?>"												{return DECLARATION_CLOSE;}
"<"({letter})({letter}|{digit})*">" {yylval.val = strdup(yytext);return ETIQ_OPEN;}
"</"({letter})({letter}|{digit})*">" {yylval.val = strdup(yytext);return ETIQ_CLOSE;}

({letter}|{digit})({letter}|{digit}|{letter}{point}|{space}{letter}|{point}{digit}|{letter}{all_symbols})*	{return TEXT;}
({letter})({letter}{letter})* {return TEXT;}
({digit})({digit}|{all_symbols}{digit}|{space}{digit})*            {return TEXT;}
({digit})({digit}{all_symbols}|{digit})*        {return TEXT;}
({all_symbols}|{special_symbols}|{digit})* 			{return TEXT;}
"\n"                    							lines_number++;
[ \t]+                  							;
. {yylval.error = strdup(yytext);}
%%