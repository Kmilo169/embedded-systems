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

/*
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>


void EscribirSerie(HANDLE idComDev, char *buf);

struct timeval stop,start;
unsigned long tiempo;
unsigned long ingreso=0;
unsigned long clave = 0xF49DC57D;
unsigned long token;
unsigned long tp=30000;

	
int main(){
	
	HANDLE Com;
	char claved[8];
	int clave;
	char act[25]="b";
	COMMTIMEOUTS m_CommTimeouts;
	DCB dcb;
	BOOL fSuccess;
	
	

	Com = CreateFileA("COM3", GENERIC_READ | GENERIC_WRITE,0, NULL, OPEN_EXISTING, 0, NULL);
	if (Com == INVALID_HANDLE_VALUE){
		printf("\nError! - Port can't be opened");
	}else{
		printf("\nPort %s Opened\n");
		
	}	
	
	fSuccess = GetCommState(Com, &dcb);
	
	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fBinary = FALSE;
	dcb.fAbortOnError = TRUE;

	SetCommState(Com, &dcb);
	
	// Leer time-outs actuales  ab se prende b se apaga
	
    GetCommTimeouts(Com, &m_CommTimeouts);

	// Nuevos valores de timeout:
    m_CommTimeouts.ReadIntervalTimeout = 50;
	m_CommTimeouts.ReadTotalTimeoutConstant = 50;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 10;
	m_CommTimeouts.WriteTotalTimeoutConstant = 50;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 10;

	// Establecer timeouts:
	SetCommTimeouts (Com, &m_CommTimeouts);

	//Mandamos la clave
	
	
	printf("Digite su clave para el token\n");
	//EscribirSerie(Com,clave);
	scanf("%s",&act);
	EscribirSerie(Com,act);
	clave=atoi(claved);
	
	//printf("\nLa clave digitada fue %s\n",act);
	
	/*printf("Pulse enter para sincronizar... ");
	getchar();
	EscribirSerie(Com,act);
	gettimeofday(&start,NULL);
	printf("Ingresa el Token:\n");
	while(1){
		gettimeofday(&stop,NULL);
		tiempo=((stop.tv_sec - start.tv_sec)*1000000+stop.tv_usec-start.tv_usec);
		tiempo/=1000.0;
		if(tiempo>=tp){ //si el tiempo es mayor a 30000ms tp cambia su rango
            tp=tp+30000;
		}else{
		    scanf("%lX",&ingreso);
		}
		token = clave^tp;
		if(ingreso==token){
			printf("Acceso Concedido\n");
			printf("Ingresa un nuevo Token:\n");
		}else{
			printf("Acceso Denegado ");
			printf("Ingresa el Token:\n");
		}
	}
		
		
	CloseHandle(Com);
	return 0;
}

void EscribirSerie(HANDLE idComDev, char *buf)
{
    char oBuffer[256];  /* Buffer de salida 
    DWORD iBytesWritten;

    iBytesWritten = 0;
    strcpy(oBuffer, buf);
    WriteFile(idComDev, oBuffer, strlen(oBuffer), &iBytesWritten, NULL);
}*/

