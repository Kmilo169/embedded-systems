#include <stdio.h>
#include <conio.h>


int main(void)
{
	FILE *xx,*nw;
	char i=0,j=0;
	int opcion=1,ancho,alto,z=0,v=0,n=54,b,g,r,p,k1[9]={1,0,1,0,1,0,1,0,1},k2[9]={1,1,1,1,1,1,1,1,1},k3[9]={-1,-1,-1,-1,0,-1,-1,-1,-1},kp[9],move,tb=0,tg=0,tr=0,m=0,w=1,anchof[4],altof[4];
	
	printf("1. Convertir a escala de grises \n");
	printf("2. Convolucion con Kernels \n");
	scanf("%i", &opcion);
	printf("Tu repuesta: %i. \n",opcion);
	switch(opcion)
	{
		case 1:
			xx=fopen("mario a ver.bmp","rb");
			nw=fopen("marioxxgray.bmp","wb");
			fseek(xx,18,SEEK_SET);
			ancho=getc(xx);
			fseek(xx,0,SEEK_SET);
			while(feof(xx)==0)
			{
				if(v==0)
				{
				for(v;v<54;v++)
				{
					putc(getc(xx),nw);
				}
				}
				v=54;
				b=getc(xx); 
				g=getc(xx); 
				r=getc(xx);
				p=(b+g+r)/3;
				putc(p,nw);
				putc(p,nw);
				putc(p,nw);
				z=z+3;
				if(z==(ancho))
				{
					p=getc(xx);
					putc(p,nw);
					p=getc(xx);
					putc(p,nw);
					z=0;
				}	
			}
		fclose(nw);
		fclose(xx);
		printf("\nTodo listo revisa la carpeta del archivo. :)");
		break;
		case 2:
			printf("Escoge la opcion que desees: \n");
			printf("1) Kernel de prueba 1 (valores en x): \n");
			for(i;i<3;i++)
			{
				for(j;j<3;j++)
				{
					printf("  %d ",k1[(i*3)+j]);
				}
				j=0;
				printf("\n");
			}
			printf("      /9 \n");
			i=0;
			j=0;
			printf("2) Kernel de prueba 2 (promedio): \n");
			for(i;i<3;i++)
			{
				for(j;j<3;j++)
				{
					printf("  %d ",k2[(i*3)+j]);
				}
				j=0;
				printf("\n");
			}
			printf("      /9 \n");
			j=0;
			i=0;
			printf("3) Kernel de prueba 3 (centro cero y el resto negativo): \n");
			for(i;i<3;i++)
			{
				for(j;j<3;j++)
				{
					printf("   %d ",k3[(i*3)+j]);
				}
				j=0;
				printf("\n");
			}
			j=0;
			i=0;
			printf("4) kernel personalizado. \n");
			scanf("%i", &opcion);
			switch(opcion)
			{
				case 1:
					for(i;i<3;i++)
					{
						for(j;j<3;j++)
						{
							kp[(i*3)+j]=k1[(i*3)+j];
						}
						j=0;
					}
				break;
				case 2:
					for(i;i<3;i++)
					{
						for(j;j<3;j++)
						{
							kp[(i*3)+j]=k2[(i*3)+j];
						}
						j=0;
					}
				break;
				case 3:
					for(i;i<3;i++)
					{
						for(j;j<3;j++)
						{
							kp[(i*3)+j]=k3[(i*3)+j];
						}
						j=0;
					}
				break;
				case 4:
					for(i;i<3;i++)
					{
						for(j;j<3;j++)
						{
							printf("digita el numero que estara en la fila %i y columna %i: ",i+1,j+1);
							scanf("%i", &kp[(i*3)+j]);
						}
						j=0;
					}
				break;
				default:
					printf("\nMarque una opcion valida intente de nuevo :/");
				break;
			}
			j=0;
			i=0;
			printf("El kernel que ha seleccionado es: \n");
			for(i;i<3;i++)
			{
				for(j;j<3;j++)
				{
					printf("  %i ",kp[(i*3)+j]);
				}
				j=0;
				printf("\n");
			}
			xx=fopen("mario a ver.bmp","rb");
			nw=fopen("mario convulado.bmp","wb");
			fseek(xx,18,SEEK_SET);
			anchof[0]=getc(xx);
			anchof[1]=getc(xx)<<8;
			anchof[2]=getc(xx)<<16;
			anchof[3]=getc(xx)<<24;
			ancho=anchof[0]|anchof[1]|anchof[2]|anchof[3];
			m=ancho%4;	
			altof[0]=getc(xx);
			altof[1]=getc(xx)<<8;
			altof[2]=getc(xx)<<16;
			altof[3]=getc(xx)<<24;
			alto=altof[0]|altof[1]|altof[2]|altof[3];
			v=0;
			fseek(xx,v,SEEK_SET);
			while(feof(xx)==0)
			{
				if(v==0)
				{
					for(v;v<54+((ancho+1)*3)+m;v++)
					{
						putc(getc(xx),nw);
					}
					n=3;
				}
				v=v+((ancho-1)*3)+m;
				fseek(xx,v,SEEK_SET);
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[0];
				tg=tg+g*kp[0];
				tr=tr+r*kp[0];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[1];
				tg=tg+g*kp[1];
				tr=tr+r*kp[1];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[2];
				tg=tg+g*kp[2];
				tr=tr+r*kp[2];
				//primera fila
				v=v-m-((ancho)*3)-9;
				fseek(xx,v,SEEK_SET);
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[3];
				tg=tg+g*kp[3];
				tr=tr+r*kp[3];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[4];
				tg=tg+g*kp[4];
				tr=tr+r*kp[4];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[5];
				tg=tg+g*kp[5];
				tr=tr+r*kp[5];
				//segunda fila
				v=v-m-((ancho)*3)-9;
				fseek(xx,v,SEEK_SET);
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[6];
				tg=tg+g*kp[6];
				tr=tr+r*kp[6];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[7];
				tg=tg+g*kp[7];
				tr=tr+r*kp[7];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[8];
				tg=tg+g*kp[8];
				tr=tr+r*kp[8];
				v=v+((ancho-1)*3)+m;
				fseek(xx,v,SEEK_SET);
				tb=tb/9;
				tg=tg/9;
				tr=tr/9;
				putc(tb,nw);
				putc(tg,nw);
				putc(tr,nw);
				n=n+3;
				if(n==((ancho-1)*3))
				{
					if(v==54+((alto-1)*((ancho-1)*3))+(m*(alto-1)))
					{
						w=0;
					}
					putc(getc(xx),nw);	
					putc(getc(xx),nw);
					putc(getc(xx),nw);	
					v=v+3;
					switch(m)
					{
						case 0:
						break;
						case 1:
							putc(0,nw);
						break;
						case 2:
							putc(0,nw);
							putc(0,nw);
						break;
						case 3:
							putc(0,nw);
							putc(0,nw);
							putc(0,nw);
						break;
						default:
						break;
					}
					putc(getc(xx),nw);	
					putc(getc(xx),nw);
					putc(getc(xx),nw);
					n=3;			
					v=v+3;
					if(w==0)
					{
						for(w;w<((ancho-1)*3);w++)
						{
							putc(getc(xx),nw);
						}
						switch(m)
						{
							case 0:
							break;
							case 1:
								putc(0,nw);
							break;
							case 2:
								putc(0,nw);
								putc(0,nw);
							break;
							case 3:
								putc(0,nw);
								putc(0,nw);
								putc(0,nw);
							break;
							default:
							break;
						}
					}					
				}											
			}
			fclose(nw);
			fclose(xx);
		break;
	}
	return 0;
}

/*
xx=fopen("mario a ver.bmp","rb");
			nw=fopen("marioxxgray.bmp","wb");
			fseek(xx,18,SEEK_SET);
			ancho=getc(xx);
			fseek(xx,0,SEEK_SET);
			while(feof(xx)==0)
			{
				if(v==0)
				{
				for(v;v<54;v++)
				{
					putc(getc(xx),nw);
				}
				}
				v=54;
				b=getc(xx); 
				g=getc(xx); 
				r=getc(xx);
				p=(b+g+r)/3;
				putc(p,nw);
				putc(p,nw);
				putc(p,nw);
				z=z+3;
				if(z==(ancho))
				{
					p=getc(xx);
					putc(p,nw);
					p=getc(xx);
					putc(p,nw);
					z=0;
				}	
			}
		fclose(nw);
		fclose(xx);
		printf("\nTodo listo revisa la carpeta del archivo. :)");
		
		//
		
			printf("Escoge la opcion que desees: \n");
			printf("1) Kernel de prueba 1 (valores en x): \n");
			for(i;i<3;i++)
			{
				for(j;j<3;j++)
				{
					printf("  %d ",k1[(i*3)+j]);
				}
				j=0;
				printf("\n");
			}
			printf("      /9 \n");
			i=0;
			j=0;
			printf("2) Kernel de prueba 2 (promedio): \n");
			for(i;i<3;i++)
			{
				for(j;j<3;j++)
				{
					printf("  %d ",k2[(i*3)+j]);
				}
				j=0;
				printf("\n");
			}
			printf("      /9 \n");
			j=0;
			i=0;
			printf("3) Kernel de prueba 3 (centro alto y el resto negativo): \n");
			for(i;i<3;i++)
			{
				for(j;j<3;j++)
				{
					printf("   %d ",k3[(i*3)+j]);
				}
				j=0;
				printf("\n");
			}
			j=0;
			i=0;
			printf("4) kernel personalizado. \n");
			scanf("%i", &opcion);
			switch(opcion)
			{
				case 1:
					for(i;i<3;i++)
					{
						for(j;j<3;j++)
						{
							kp[(i*3)+j]=k1[(i*3)+j];
						}
						j=0;
					}
				break;
				case 2:
					for(i;i<3;i++)
					{
						for(j;j<3;j++)
						{
							kp[(i*3)+j]=k2[(i*3)+j];
						}
						j=0;
					}
				break;
				case 3:
					for(i;i<3;i++)
					{
						for(j;j<3;j++)
						{
							kp[(i*3)+j]=k3[(i*3)+j];
						}
						j=0;
					}
				break;
				case 4:
					for(i;i<3;i++)
					{
						for(j;j<3;j++)
						{
							printf("digita el numero que estara en la fila %i y columna %i: ",i+1,j+1);
							scanf("%i", &kp[(i*3)+j]);
						}
						j=0;
					}
				break;
				default:
					printf("\nMarque una opcion valida intente de nuevo :/");
				break;
			}
			j=0;
			i=0;
			printf("El kernel que ha seleccionado es: \n");
			for(i;i<3;i++)
			{
				for(j;j<3;j++)
				{
					printf("  %i ",kp[(i*3)+j]);
				}
				j=0;
				printf("\n");
			}
			xx=fopen("mario a ver.bmp","rb");
			nw=fopen("marioconvulado.bmp","wb");
			fseek(xx,18,SEEK_SET);
			anchof[0]=getc(xx);
			anchof[1]=getc(xx)<<8;
			anchof[2]=getc(xx)<<16;
			anchof[3]=getc(xx)<<24;
			ancho=anchof[0]|anchof[1]|anchof[2]|anchof[3];
			m=ancho%4;	
			altof[0]=getc(xx);
			altof[1]=getc(xx)<<8;
			altof[2]=getc(xx)<<16;
			altof[3]=getc(xx)<<24;
			alto=altof[0]|altof[1]|altof[2]|altof[3];
			v=0;
			fseek(xx,v,SEEK_SET);
			while(feof(xx)==0)
			{
				if(v==0)
				{
					for(v;v<54+((ancho+1)*3)+m;v++)
					{
						putc(getc(xx),nw);
					}
					n=3;
				}
				v=v+((ancho-1)*3)+m;
				fseek(xx,v,SEEK_SET);
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[0];
				tg=tg+g*kp[0];
				tr=tr+r*kp[0];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[1];
				tg=tg+g*kp[1];
				tr=tr+r*kp[1];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[2];
				tg=tg+g*kp[2];
				tr=tr+r*kp[2];
				//primera fila
				v=v-m-((ancho)*3)-9;
				fseek(xx,v,SEEK_SET);
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[3];
				tg=tg+g*kp[3];
				tr=tr+r*kp[3];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[4];
				tg=tg+g*kp[4];
				tr=tr+r*kp[4];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[5];
				tg=tg+g*kp[5];
				tr=tr+r*kp[5];
				//segunda fila
				v=v-m-((ancho)*3)-9;
				fseek(xx,v,SEEK_SET);
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[6];
				tg=tg+g*kp[6];
				tr=tr+r*kp[6];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[7];
				tg=tg+g*kp[7];
				tr=tr+r*kp[7];
				b=(getc(xx));
				g=(getc(xx));
				r=(getc(xx));
				v=v+3;
				tb=tb+b*kp[8];
				tg=tg+g*kp[8];
				tr=tr+r*kp[8];
				v=v+((ancho-1)*3)+m;
				fseek(xx,v,SEEK_SET);
				tb=tb/9;
				tg=tg/9;
				tr=tr/9;
				putc(tb,nw);
				putc(tg,nw);
				putc(tr,nw);
				n=n+3;
				if(n==((ancho-1)*3))
				{
					if(v==54+((alto-1)*((ancho-1)*3))+(m*(alto-1)))
					{
						w=0;
					}
					putc(getc(xx),nw);	
					putc(getc(xx),nw);
					putc(getc(xx),nw);	
					v=v+3;
					switch(m)
					{
						case 0:
						break;
						case 1:
							putc(0,nw);
						break;
						case 2:
							putc(0,nw);
							putc(0,nw);
						break;
						case 3:
							putc(0,nw);
							putc(0,nw);
							putc(0,nw);
						break;
						default:
						break;
					}
					putc(getc(xx),nw);	
					putc(getc(xx),nw);
					putc(getc(xx),nw);
					n=3;			
					v=v+3;
					if(w==0)
					{
						for(w;w<((ancho-1)*3);w++)
						{
							putc(getc(xx),nw);
						}
						switch(m)
						{
							case 0:
							break;
							case 1:
								putc(0,nw);
							break;
							case 2:
								putc(0,nw);
								putc(0,nw);
							break;
							case 3:
								putc(0,nw);
								putc(0,nw);
								putc(0,nw);
							break;
							default:
							break;
						}
					}					
				}											
			}
			fclose(nw);
			fclose(xx);
		break;
		default:
			printf("\nMarque una opcion valida intente de nuevo :/");
*/