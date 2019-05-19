#include<iostream>
#include<time.h>
#include<windows.h>
#include<cstdlib>
#include<locale>
#include<ctype.h>
#include<stdlib.h>

using namespace std;

#define COLUMNAS 15
#define FILAS 15
#define SIN_BOMBA -2
#define BOMBA -1
#define SIN_JUGAR -3

#define ESTADO_PERDEDOR 0
#define ESTADO_GANADOR  1
#define ESTADO_EN_JUEGO 2

int campo[FILAS][COLUMNAS];
bool jugadas[FILAS][COLUMNAS];

int njugadas;
int estado;

float version = 0.1;
int build = 4;
char jugador[30];


void iniciaArr(){
    int i,j;
    for( i = 0; i < FILAS; i++){
        for(j = 0; j < COLUMNAS; j++){
            campo[i][j] = SIN_BOMBA;
            jugadas[i][j] = false; //No se ha jugado en casilla
        }
    }
}

void agregaBombas(int nbombas){
    int i,x,y;// contador de bombas
    i = 1;
    while( i <= nbombas){
        x = rand()%FILAS;
        y = rand()%COLUMNAS;
        campo[x][y] = BOMBA;
        i++;
    }
}

void print(){ //muestra la tabla
    int i,j;
    system("cls");
    cout << "  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14" << endl;
    cout << "---------------------------------------------------------------"<< endl;
    for( i = 0; i < FILAS; i++){//coloca los numeros de la tabla de forma vertical
        
        for(j = 0; j < COLUMNAS; j++){
            if( jugadas[i][j]==1 ){
                if( campo[i][j] == BOMBA){
                    cout << "  X ";
                }else{
                    cout << "  " << campo[i][j] << " ";
                }
            }else{
                cout << "  ? ";
            }
        }
        cout<< " | "<<i<< endl;
    }
}

int contar2(int x, int y){ 
    int nbombas = 0;
    for(int i = x-1; i < x+2; i++){
        for(int j = y-1; j < y+2; j++){
            if( i >= 0 && i < FILAS && j >= 0 && j < COLUMNAS ){
                if( campo[i][j] == BOMBA){ //Existe bomba
                    nbombas++;
                }
            }
        }
    }
    return nbombas;
}

void establecerNumeros(){ //declaracion de los numeros 1,2 y 3 del juego
    int i,j, numero;
    for( i = 0; i < FILAS; i++){
        for(j = 0; j < COLUMNAS; j++){
            if( campo[i][j] == SIN_BOMBA){ //no tiene bomba
                numero = contar2(i,j);
                campo[i][j] = numero;
            }
        }
    }
}

void printAll(){ //muestra la solucion
    for(int i = 0; i < FILAS; i++){
        for(int j = 0; j < COLUMNAS; j++){
            if( campo[i][j] == BOMBA){
                cout << " X ";
            }else{
                cout << " " << campo[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void jugada(int x, int y){//determina si las casillas estan ocupadas
    if( x >= 0 && x < FILAS && y >= 0 && y < COLUMNAS ){
        jugadas[x][y] = true;
        if( campo[x][y] == BOMBA){
            estado = ESTADO_PERDEDOR;
        }
    }else{
        cout << "Imposible jugada" << endl; //casilla bloqueada
        system("pause");
    }
}

int contarJugadas(){
    int numero = 0;
    for(int i = 0; i < FILAS; i++){
        for(int j = 0; j < COLUMNAS; j++){
            if ( jugadas[i][j]){
                numero++;
            }
        }
    }
    return numero;
}

void jugadaEnGrupo(int x, int y){ //calcula las aproximaciones de la bomba
    if( x >= 0 && x < FILAS && y >= 0 && y < COLUMNAS ){
        jugadas[x][y] = true;
        if( campo[x][y] == BOMBA){
            estado = ESTADO_PERDEDOR;
        }else if( campo[x][y] == 0){//Cero bombas alrededor
            for(int i = x-1; i < x+2; i++){
                for(int j = y-1; j < y+2; j++){
                    if( i >= 0 && i < FILAS && j >= 0 && j < COLUMNAS ){
                        if( campo[i][j] == 0 && !jugadas[i][j]){ //Existe bomba
                            jugadaEnGrupo(i,j);
                        }else{
                            jugadas[i][j] = true;
                        }
                    }
                }
            }
        }
    }
}

int getMenu(){//menu principal
	system("cls");
	system("color 07");
	cout << "Bienvenido "<<jugador<<" seleciona la Opcion que Gustes.\n"<<endl;
    cout << "1. Jugar Buscaminas." << endl;
    cout << "2. Instrucciones de Juego." << endl;
    cout << "3. Salir." << endl;
    cout << "Opcion: ";
    int opt;
    cin >> opt;
    return opt;
}

void instrucciones(){//intrucciones del juego
	system("cls");
    cout << "Muy bien "<<jugador<<" te voy a explicar en que consiste el juego."<< endl;
    cout <<"Es una Matriz de tamaño 15x15 y el total de bombas tu mismo lo determinas."<<endl;
    cout <<"Tu único objetivo es tratar de que no te salgan las bombas en alguna de las posiciones,"<<endl;
    cout <<"ya que cuando veas un 3 es que esta cerca una bomba y la probabilidad de que ganes es poca,"<<endl;
    cout <<"también cuando veas 0's en la tabla son casillas que ya no puedes selecionar(quedan en blanco),"<<endl;
    cout <<"y en el ingreso de la bombas no debe ser menor a 0 o mayor que 40. El propósito del juego "<<endl;
	cout <<"es que con ayuda de la materia de Álgebra Lineal hagamos la utilización de Matrices."<<endl;
	cout <<"Demuestra tus habilidades "<<jugador<<" pero lo mas importante DIVIERTETE :D ."<<endl;
	
}

void iniciarJuego(){ //el ingreso del numero de bombas
    int nbombas = -1;
    while( nbombas < 0 || nbombas > 40 ){
        cout << "Numero de bombas: ";
        cin >> nbombas;
    }

    int x,y;
    char salida;
    estado = ESTADO_EN_JUEGO;
    njugadas = 0;
    iniciaArr();

    //PID es un identificador de procesos, cada vez
    // que un nuevo proceso se ejectua se le asigna un PID
    // diferente - Se necesita libreria - unistd.h
    //srand( getpid() );
    srand ( time(NULL) );
    agregaBombas( nbombas );
    establecerNumeros();

    while( estado == ESTADO_EN_JUEGO){ //estado en juego(ingresa de datos de la tabla)
        print();
        cout << "\nFila: ";
        cin >> x;
        cout << "\nColumna: ";
        cin >> y;
        jugadaEnGrupo(x,y);
        system("cls");
        njugadas = contarJugadas();
        if( njugadas == ( FILAS*COLUMNAS - nbombas) ){
            estado = ESTADO_GANADOR;//si es ganador
        }
    }

    if( estado == ESTADO_PERDEDOR){//si es perdedor
        print();
        system("color 47");
        cout << "\n:( Lo siento "<<jugador<< " Perdiste, el juego ha terminado\n" << endl;
        system("C:\\Users\\Migue\\Music\\Mariolose.wav");//musica de lose
        cout << "\nSolucion:\n" << endl;
        printAll();
    }else{
    	system("color 27");
        cout <<"\n:D MUCHAS FELICIDADES "<< jugador<<" , HAS GANADO LA PARTIDA\n"<< endl;
        system("C:\\Users\\Migue\\Music\\Mariowin.wav");//musica de win
        printAll();
    }
}

int main(int argc, char** argv) { //declaracion del menu 
    setlocale(LC_CTYPE, "spanish");
    int opt = 0;
    cout<<"\t\t\t\tBienvenido al juego de BUSCAMINAS\n\t\t\t\t  (Proyecto de Algebra Lineal)\n\n"<<endl; //ingreso del nombre del jugador
	cout<<"Ingresa tu nombre de jugador:\n"<<jugador;
	cin>>jugador;
    do{
        opt = getMenu();
        switch(opt){
            case 1: iniciarJuego(); break;
            case 2: instrucciones(); break;
        }
        system("pause");
        system("cls");
    }while( opt != 3);
    return 0;
}
