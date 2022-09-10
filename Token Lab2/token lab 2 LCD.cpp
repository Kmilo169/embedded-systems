#include <stdio.h>
#include <sys/time.h>

struct timeval stop,start; 
unsigned long long tiempo,r=0; 
unsigned long clave = 0x16C09C19/*clave original de inicializacion*/,tokenLocal=0; 
int m=0;
char ok=0;

void imprimeCuadrado();

int main()
{
	printf("Pulse enter para sincronizar...");
	getchar();
	imprimeCuadrado(); //Funcion de cuadrado blanco para activar el reloj en la STM
	while(ok==0)
	{
	gettimeofday(&start,NULL); //Medida de tiempo incial 
	printf("\nINGRESE EL TOKEN>>>");
	scanf("%08X",&tokenLocal); //Lectura del Token ingresado
	gettimeofday(&stop,NULL); //Medida de tiempo final
	tiempo=((stop.tv_sec-start.tv_sec)*1000000+stop.tv_usec-start.tv_usec); //Calculo del tiempo ocupado en micro segundos para ingresar el TOken 
	tiempo/=1000.0; //Escalamiento del tiempo a milisegundos
	r=r+tiempo;
	//printf("Tiempo %lu ms\n",r); // impresión del tiempo ocupado
	//printf("TOKEN ORIGINAL %08X \n", clave); //impresion del token original
	//printf("Token ingresado: %08X \n",tokenLocal); //impresion del token ingresado
	m=r/30000; //calculo de la cantidad de veces que han pasado 30 seg 
	//printf("Clave %08X \n",clave^(m*30000)); //impresion del token resultante del ciclo del 30 seg que este pasando
	if(clave==(tokenLocal^(m*30000))) //Comparacion del token ingresado luego de hacerle la operacion XOR con el original
	{
		printf("\nTiempo %lu ms\n",r); // impresión del tiempo ocupado
		printf("\ntoken aceptado \n"); //Resultado del token ingresado correcto
		ok=1;
	}
	else
	{
		printf("\ntoken invalido \n"); // Resultado del token ingresado incorrecto
		ok=0;
	}
	}
}
	
void imprimeCuadrado() //Funcion de impresion de un cuadrado blanco en la pantalla
{
	unsigned char dato=219; // caracter de cuadrado blanco 
	int i=0;
	for(i=1; i<51; i++) //iteración de impresion del cuadrado en la plantalla para aumentar su tamaño
	{
		printf("%c",dato); //impresion del caracter del cuadrado blanco 
		if(0==(i%10))printf("\n"); //salto de linea para delimitar el ancho del cuadro
	}
}