#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

void EscribirSerie(HANDLE idComDev, char *buf);

struct timeval stop,start; 
unsigned long long tiempo,r=0; 
unsigned long clave = 0x16C09C19/*clave original de inicializacion*/,tokenLocal=0;
int tempE=0,opcion=0,m=0;
double temp=0;
char ok=0,k=0,xx[25]="b",hora[3]="0",tok[4]="0",tb=0;


int main()
{
	
	HANDLE Com;
	COMMTIMEOUTS m_CommTimeouts;
	DCB dcb;
	BOOL fSuccess;
	
	Com = CreateFileA("COM9", GENERIC_READ | GENERIC_WRITE,0, NULL, OPEN_EXISTING, 0, NULL);
	if (Com == INVALID_HANDLE_VALUE){
		printf("No señor\n");
		ok=1;
	}else{
		printf("Si señor\n");
		ok=0;
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
	
	while(ok==0)
	{
			printf("Digite la opcion que desea realizar \n");
	printf("1. Sincronizar hora \n");
	printf("2. Cambiar clave \n");
	scanf("%i", &opcion);
	switch(opcion)
	{
		case 1: 
		printf("Tu respuesta: %i. Sincronizar hora \n",opcion);
		gettimeofday(&start,NULL); //Medida de tiempo incial 
		tiempo=start.tv_sec-1641013200;
		temp=(double)tiempo/86400;
		tempE=tiempo/86400;
		temp=temp-tempE;
		temp=temp*24;
		tempE=(int)temp;
		hora[0]=tempE;
		temp=temp-tempE;
		temp=temp*60;
		tempE=(int)temp;
		hora[1]=tempE;
		temp=temp-tempE;
		temp=temp*60;
		tempE=(int)temp;
		hora[2]=tempE;
		xx[0]=0x16;
		xx[1]=0x07;
		xx[2]=0x40;
		for(k=0;k<3;k++)
		{
			xx[k+3]=hora[k];
		}			
		xx[6]=0x19;
		EscribirSerie(Com,xx);
		ok=1;
		break;
		case 2:
			printf("Tu repuesta: %i. Cambiar clave \n",opcion);
			printf("Digite la clave >> ");
			scanf("%08X",&clave);
			for(k=0;k<4;k++)
			{
				tok[k]=(clave>>(24-(k*8))&0X000000FF);
			}
			xx[0]=0x16;
			xx[1]=0x08;
			xx[2]=0x80;
			for(k=0;k<4;k++)
			{
				xx[k+3]=tok[k];
			}
				
			EscribirSerie(Com,xx);
			ok=0;
		break;
		default:
			printf("Digite una opcion valida. :/ \n");
			ok=0;
		break;
		}
	}
	ok=0;
	while(ok==0)
	{
	gettimeofday(&start,NULL); //Medida de tiempo incial 
	printf("\nINGRESE EL TOKEN>>>");
	scanf("%08X",&tokenLocal); //Lectura del Token ingresado
	gettimeofday(&stop,NULL); //Medida de tiempo final
	tiempo=((stop.tv_sec-start.tv_sec)*1000000+stop.tv_usec-start.tv_usec); //Calculo del tiempo ocupado en micro segundos para ingresar el TOken 
	tiempo/=1000.0; //Escalamiento del tiempo a milisegundos
	r=r+tiempo;
	m=r/30000; //calculo de la cantidad de veces que han pasado 30 seg 
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

void EscribirSerie(HANDLE idComDev, char *buf)
{
    char oBuffer[256];  /* Buffer de salida */
    DWORD iBytesWritten;

    iBytesWritten = 0;
    strcpy(oBuffer, buf);
    WriteFile(idComDev, oBuffer, strlen(oBuffer), &iBytesWritten, NULL);
}
