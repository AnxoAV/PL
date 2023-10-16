#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void seleccion() {
    /*
    Función empleada para seleccionar el modo en el que se desea jugar (introducir una planilla o a tiempo real).
    Se escribirá 1 o 2 según la opción que desee, si se introduce otra cosa salta error.
    */
    int modo;
    scanf("%d",&modo);
    if(modo==1){
        printf("Has seleccionado introducir una planilla\n");
        system("make opcion=1");
    }else if(modo==2){
        printf("Has seleccionado jugar una partida a tiempo real\n");
        system("make opcion=2");
    }else{
        printf("Por favor, elija 1 o 2 según la opción que deseas\n");
        seleccion();
    }
}

void menu (){
    /*
    Función empleada para ir al menú de selección.
    Simplemente imprime las opciones y, después, el usuario podrá seleccionar la que quiere.
    */
    printf("El programa dispone de dos opciones:\n");
    printf("1 - Introducir una planilla\n");
    printf("2 - Jugar una partida a tiempo real\n");
    printf("Por favor, seleccione el modo\n");
    seleccion();
}
int main() {
	menu();
	return 0;
}