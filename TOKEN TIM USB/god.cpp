#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <sys/time.h>
#define    BUFFERLENGTH 256


struct timeval stop,start; 
unsigned long long tiempo,r=0; 
unsigned long clave = 0x16C09C19/*clave original de inicializacion*/,tokenLocal=0;
int tempE=0,opcion=0,m=0;
double temp=0;
char ok=0,k=0,xx[25]="b",hora[3]="0",tok[4]="0",tb=0;

int main(int argc, char *argv[])
{
	HANDLE hComm;
	DWORD MORO;
	char   *pcCommPort = "COM9";
	BOOL Write_Status;
	DCB dcbSerialParams;					// Initializing DCB structure
	COMMTIMEOUTS timeouts = { 0 };
	BOOL  Read_Status;                      // Status of the various operations 
	DWORD dwEventMask;						// Event mask to trigger
	char  *TempChar;                        // Temperory Character
	char  *SerialBuffer[BUFFERLENGTH+1];               // Buffer Containing Rxed Data
	DWORD NoBytesRead;                     // Bytes read by ReadFile()
	int i = 0;

	hComm = CreateFileA(pcCommPort,
		GENERIC_READ | GENERIC_WRITE,
		0,    // must be opened with exclusive-access
		NULL, // no security attributes
		OPEN_EXISTING, // must use OPEN_EXISTING
		0,    // not overlapped I/O
		NULL  // hTemplate must be NULL for comm devices
	);

	if (hComm == INVALID_HANDLE_VALUE)
	{
		
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			puts("cannot open port!");
			return 0;
		}

		puts("invalid handle value!");
		return 0;
	}
	else
	  //printf("opening serial port successful");

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Write_Status = GetCommState(hComm, &dcbSerialParams);     //retreives  the current settings

	if (Write_Status == FALSE) {
		printf("\n   Error! in GetCommState()");
		CloseHandle(hComm);
		return 1;
	}


	dcbSerialParams.BaudRate = CBR_57600;      // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
											  //dcbSerialParams.
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity = ODDPARITY;      // Setting Parity = None

	Write_Status = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB

	// Set COM port timeout settings
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	if (SetCommTimeouts(hComm, &timeouts) == 0)
	{
		printf("Error setting timeouts\n");
		CloseHandle(hComm);
		return 1;
	}
	
	DWORD  NumWritten;
	DWORD  dNoOFBytestoWrite;              // No of bytes to write into the port
	DWORD  dNoOfBytesWritten = 0;          // No of bytes written to the port
	
	//====================================================================================
	
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
		
		dNoOFBytestoWrite = sizeof(xx); // Calculating the no of bytes to write into the port
		
		if (!WriteFile(hComm, xx, dNoOFBytestoWrite,&dNoOfBytesWritten, NULL))
		{
			printf("Error writing text to %s\n", pcCommPort);
		}		
		
		Read_Status = SetCommMask(hComm, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception
	
		 /*-------------------------- Program will Wait here till a Character is received ------------------------*/
	
		if (Read_Status == FALSE)
		{
			printf("\n    Error! in Setting WaitCommEvent()");
		}
		else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
		{
			printf("Recepcion: ");
		
			if (!ReadFile(hComm, SerialBuffer, BUFFERLENGTH, &NoBytesRead, NULL))
			
				{
					printf("wrong character");
				}
			printf("%s \n", SerialBuffer);
		}
		
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
		
		dNoOFBytestoWrite = sizeof(xx); // Calculating the no of bytes to write into the port
		
		if (!WriteFile(hComm, xx, dNoOFBytestoWrite,&dNoOfBytesWritten, NULL))
		{
			printf("Error writing text to %s\n", pcCommPort);
		}		
		
		Read_Status = SetCommMask(hComm, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception
	
		 /*-------------------------- Program will Wait here till a Character is received ------------------------*/
	
		if (Read_Status == FALSE)
		{
			printf("\n    Error! in Setting WaitCommEvent()");
		}
		else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
		{
			printf("Recepcion: ");
		
			if (!ReadFile(hComm, SerialBuffer, BUFFERLENGTH, &NoBytesRead, NULL))
			
				{
					printf("wrong character");
				}
			printf("%s \n", SerialBuffer);
		}
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

	CloseHandle(hComm);//Closing the Serial Port
	
	return 0;
}