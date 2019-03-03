#include <stdio.h>
#include <unistd.h> //Sleep, write, close
#include <fcntl.h> //open
#include <stdlib.h>
#include <string.h> //strlen
#include <time.h>


#define RUTA "/dev/leds"



void contador(int file);
int leds(int file, char *buf);
int menu();
void adivinarSecuencia(int file);



int main(){

	int fin = 0;
	int opcion;

	int file = open(RUTA, O_RDWR);

	if(file < 0) {
		printf("El fichero no ha podido abrirse\n");
		return 1;
	}

	while (fin == 0) {

		opcion = menu();

		//Apaga los leds
		if((leds(file, "")) != 0) {
			printf("Ha habido un problema al apagar los leds\n");
		}


		if (opcion==0) fin = 1;

		else if (opcion == 1) adivinarSecuencia(file);

		else if (opcion == 2) contador(file);			
		
		// Apaga los leds
		if((leds(file, "")) != 0) {
			printf("Ha habido un problema al apagar los leds\n");
		}
	}


	if (close(file) < 0) {
		printf("No se ha podido cerrar el fichero\n");
		return 1;
	} 

	return 0;
}


void contador(int file) {
	char* buff;
	int i;
	
	for (i=0; i <= 7; i++) {
		
		if (i==0) buff = "0";
		else if (i==1) buff = "3"; 
		else if (i==2) buff = "2";
		else if (i==3) buff = "23";
		else if (i==4) buff = "1";
		else if (i==5) buff = "13";
		else if (i==6) buff = "12";
		else if (i==7) buff = "123";
		 
		if((leds(file, buff)) != 0) {
			printf("Ha habido un problema encendiendo los leds\n");
		}
		
		//Medio segundo de espera
		usleep(500000);
	}
}




void adivinarSecuencia(int file){

	int elegido = 0; //Actua como un booleano
	int nivel;
	int numeroLeds = 3; //Con cada acierto se aumenta en 1 el numero de leds a mostrar
	int correcto = 1; //Si correcto vale 0 se termina la ejecucion porque el usuario ha fallado
	printf("\nIntenta adivinar la secuencia mostrada por teclado.\n");

	printf("\nElige un nivel: \n - Nivel 1: 0.5 segundos entre cada led \n - Nivel 2: 0.25 segundos entre cada led \n - Nivel 3: 0.125 segundos entre cada led \n");

	while (elegido == 0) {

		printf("\nNivel: ");
		scanf("%d", &nivel);

		if (nivel >= 1 && nivel <= 3)
			elegido = 1;
		else
			printf("\nEse nivel es inválido, elija otra vez\n");
	}

	int frecLeds;
	if(nivel == 1) frecLeds = 500000; //Medio segundo
	else if(nivel == 2) frecLeds = 250000;
	else frecLeds = 125000;


				
	while(correcto == 1){
		char in[numeroLeds];

		int i;
		printf("\nGenerando secuencia \n");
		srand(time(NULL)); //Crea una semilla para el rand para que no se repitan los numeros con cada ejecucion
		for(i = 0; i < numeroLeds; i++){

			in[i] = (char)((rand()%3+1) + '0');//Genera numeros aleatorios de 1 a 3 y los convierte en char
			//printf("%c", in[i]);
		}
					
		printf("\nAlla va! Atento al teclado\n");
		usleep(4000000); //Espera 4 segundos


		//Mostrar leds en teclado
		for(i = 0; i < numeroLeds; i++){
			usleep(frecLeds);
			write(file, &in[i], 1);
			usleep(frecLeds);
			write(file, "", 1);
		}

		leds(file, "");

		//Comprobar leds
		i = 0;
		printf("\nEscribe ahora los leds en el orden en el que lo has visto:\n");
		printf("\nNumLock = 1, CapsLock = 2, ScrollLock = 3\n");

		int leido;
		while(i < numeroLeds && correcto == 1){
			printf("Led %d: ", i+1);
			scanf("%d", &leido);

			if((char)(leido + '0') != in[i]){
				printf("\nERROR! HAS PERDIDO!\n");
				correcto=0;

				//Empiezan a parpadear los leds indicando que se acabó
				int j = 0;
				char* mostrar = "123";
				while(j < 8){
					leds(file, mostrar);
					usleep(500000); //Medio segundo
					if(j%2 == 0) mostrar = "";
					else mostrar = "123";
					j++;
				}
			}

			i++;
		}

		//Has ganado esta ronda
		if(correcto == 1){
			printf("\nHAS GANADO!\n");
			printf("\nNumero de leds aumentado en 1\n");
			numeroLeds++;
		}
	}

	char* mostrar = "";
	leds(file, mostrar);
}

int leds(int file, char *buf) {

	int len = strlen(buf);
    
    //Write devuelve el número de bytes escritos
	if(write(file, buf, len) < 0) {
		printf("No se puede escribir en el fichero\n");
		return -1;
	}

	return 0;
}



int menu() {
	int opcion = -1;
	int exito = 0;

	printf("\n\n Opciones:\n");
	printf("----------------------------\n");
	printf(" 1.- Memorizar secuencia\n");
	printf(" 2.- Contador Binario\n"); // Chooses one of the previous modes randomly
	printf(" 0.- Salir\n");

	while (!exito) {
		printf("Elige una opcion: ");

		scanf("%d", &opcion);
		if (opcion >= 0 && opcion <= 2)
			exito = 1;
		else
			printf("Esa opción es inválida, elija otra vez\n");
	}

	return opcion;
}


