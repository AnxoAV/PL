%{
unsigned charCount = 0, wordCount = 0, lineCount = 0,numCount = 0;
unsigned mayusCount = 0, minusCount = 0,numFrase = 0;
%}
digit [0-9]  
carct [a-zA-Z]
mayus [A-Z][A-Z]+
minus [a-záéíóú][a-záéíóú]+
word [^ \t\n\.]+
especial [\,\:\;] 
frase [\.]+
%%
{frase} {numFrase++;charCount+=strlen(yytext);}
{digit}+ {numCount++;charCount+=strlen(yytext);}
{mayus}|{mayus}{especial} {mayusCount++;charCount+=strlen(yytext);}
{minus}|{minus}{especial} {minusCount++;charCount+=strlen(yytext);}
{word} {wordCount++;charCount += strlen(yytext);}
%%
main()
{
yylex();
printf("El número de palabras del documento es %d\n", wordCount+numCount+mayusCount+minusCount);
printf("El número de caracteres del documento es %d\n",charCount);
printf("El número de palabras numéricas del documento es %d\n",numCount);
printf("El número de palabras del documento formada solo por mayus es %d\n",mayusCount);
printf("El número de palabras del documento formada solo por minus es %d\n",minusCount);
printf("El número de frases del documento es %d\n",numFrase);
}