extern bool llamar_comprobar_todos_jaques(char color);
extern void llamar_encontrar_posicion_rey(char color);

bool comprobar_piezas_suficientes(){
    /*
    Función que se emplea para comprobar si existen piezas para seguir jugando.
   	La partida no puede continuar si cada jugador solo posee un alfil o un caballo.
        * return: true si existen piezas suficientes y false en caso contrario.
    */
    int AN=0;
    int CN=0;
    int AB=0;
    int CB=0;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(tablero[i][j]=="AN"){
                AN++;
            }else if(tablero[i][j]=="CB"){
                CB++;
            }else if(tablero[i][j]=="AB"){
                AB++;
            }else if(tablero[i][j]=="CN"){  
                CN++;  
            }else if(tablero[i][j]=="RN"){
            }else if(tablero[i][j]=="RB"){
            }else{
                return true;
            }
            if(AN+CN>1 || AB+CB>1){
                return true;
            }
        }
    }
    
    return false;
}

bool comprobar_rey_al_lado_tablas(char color){
	/*
	Función que comprueba que no haya un rey al lado de donde se pretende mover al rey.
		* return: Verdadero si no hay ningún rey al lado, falso en caso contrario.
	*/
	for(int i=-1;i<=1;i++){
		for(int j=-1;j<=1;j++){
            if((posicion_rey[1]+i>=0)&&(posicion_rey[1]+i<8)&&(posicion_rey[0]+j>=0)&&(posicion_rey[0]+j<8)){
			if(tablero[posicion_rey[1]+i][posicion_rey[0]+j][0]=='R' && tablero[posicion_rey[1]+i][posicion_rey[0]+j][1]!=color){
                return false;
			}
            }
		}
	}
	return true;
}

bool comprobar_rey_tablas(char color){
    /*
    Función que se emplea para comprobar si un rey no está en jaque y no se puede mover a ninguna casilla.
   	Se comprueba que el rey no esté en jaque pero sí todas las casillas adyacentes.
        * color: Color del rey.
        * return: true si existen piezas suficientes y false en caso contrario.
    */
    char guardar_rey[2];
    guardar_rey[0]='h'-posicion_rey[0];
    guardar_rey[1]=posicion_rey[1]-'1';
    if(llamar_comprobar_todos_jaques(color)==true){
        
        return false;
    }
 
    for (int a=-1;a<2;a++){
        for(int b=-1;b<2;b++){
            
           posicion_rey[0]=guardar_rey[0]+a;
           posicion_rey[1]=guardar_rey[1]+b;
           
        if(posicion_rey[0]>=0 && posicion_rey[0]<8 && posicion_rey[1]>=0 && posicion_rey[1]<8){
            
        if((tablero[posicion_rey[1]][posicion_rey[0]][1]!='B') && (tablero[posicion_rey[1]][posicion_rey[0]][1]!='N')){
            //comprobar jaque
            
            if (comprobar_rey_al_lado_tablas(color)){
                posicion_rey[0]='h'-posicion_rey[0];
            posicion_rey[1]='1'+posicion_rey[1];
            if(!llamar_comprobar_todos_jaques(color)){
                
                posicion_rey[0]='h'-posicion_rey[0];
                posicion_rey[1]=posicion_rey[1]-'1';
                return false;
            }
        }
        }
        }

        }
    }
   
    return true;
}

bool comprobar_peones(int i,int j, char color){
    /*
    Función que se emplea para comprobar que los peones no se pueden mover a ninguna casilla.
   	Se comprueba que el peón no se pueda mover.
        * i: Fila del peón.
        * j: Columna del peón.
        * color: Color del rey.
        * return: true si no se pueden mover y false si se podría.
    */
    // ningun peon puede avanzar o comer
    if(color=='B' && (tablero[i+1][j][1]!='B' && tablero[i+1][j][1]!='N')){
        return false;
    }else if(color=='N' && (tablero[i-1][j][1]!='B' && tablero[i-1][j][1]!='N')){
        return false;
    }else if(color=='B' && tablero[i+1][j+1][1]=='N'){
        return false;
    }else if(color=='N' && tablero[i-1][j-1][1]=='B' ){
        return false;
    }else if(color=='B' && (tablero[i+2][j][1]!='B' && tablero[i+2][j][1]!='N')&& i==1){
        return false;
    }else if(color=='N' && (tablero[i-2][j][1]!='B' && tablero[i-2][j][1]!='N')&& i==6){
        return false;
    }
    return true;
}

bool comprobar_ahogado(char color){
    /*
    Función que se emplea para comprobar que el rey está ahogado.
   	Se comprueba que el el rey no está en jaque pero el jugador no puede mover ni el rey ni los peones.
        * color: Color del rey.
        * return: true si es ahogado y false en caso contrario.
    */
    llamar_encontrar_posicion_rey(color);
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(tablero[i][j][0]=='R' && tablero[i][j][1]==color){
               
                if(!comprobar_rey_tablas(color)){
                    return false;
                }
            }
             
            if(tablero[i][j][0]=='P' && tablero[i][j][1]==color){
                // clavado
                if(!comprobar_peones(i,j,color)){
                    return false;
                }
            }
            if(tablero[i][j][0]!='P' && tablero[i][j][0]!='R' && tablero[i][j][1]==color){
                return false;
            }
        }
    }
    
    
    return true;
}