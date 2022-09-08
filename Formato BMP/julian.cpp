#include <stdio.h>
#include <stdlib.h>
#include<stdio.h>

FILE *img, *imgGris, *imgConv;
	int tamImg, offset;
	int j, k;
	unsigned long long i;//como va a recorrer toda la imagen tiene que tomar valores grandes 8bytes+
	unsigned char kernel[9];//9 valores
	
int main(int argc, char *argv[]) {//como usamos una imagen de 512x512<--512 columnas. no hay padding: (512pixelesx3bytes)mod4=0.
	
	img=fopen("mario a ver.bmp","rb");//rb y wb en vez de r y w hacen referencia a valores binarios y no de texto
	imgGris=fopen("pixelGris.bmp","wb");
	imgConv=fopen("pixelConv.bmp","wb");
	
	fseek(img,2,SEEK_SET);//solo nececito sacar unos datos, no voy a crear un typedefstruct
	fread(&tamImg,sizeof(int),1,img);
	fseek(img,10,SEEK_SET);//solo nececito sacar unos datos, no voy a crear un typedefstruct
	fread(&offset,sizeof(int),1,img);
	printf("Tam del archivo: %i\n",tamImg);
	fseek(img,0,SEEK_SET);//vuelvo a apuntar al inicio del archivo, SEEK_SET es respecto al inicio del archivo
	int tamPix=tamImg-offset;
	
	unsigned char data[offset];//
	
	for(i=0;i<offset;i++){
		fread(&data[i],1,1,img);
	}//leo la cabezera
	
	unsigned char pix[tamPix];
	unsigned char pix2[tamPix];
	//unsigned char pix3[tamPix];
	printf("Cantidad bytes de datos: %i\n",tamPix);//en el arreglo pix solo tengo datos de la imagen, no de sus cabezeras
	
	fseek(img,offset,SEEK_SET);
	for(i=0;i<(tamPix);i++){
		fread(&pix[i],1,1,img);
	}
	 fseek(img,offset,SEEK_SET);
	for(i=0;i<(tamPix);i++){
		fread(&pix2[i],1,1,img);
	}
	 
	//-------------------------------------------------------------------------------------------------------------------------
	unsigned int b, g ,r , gg;
	for(i=0;i<(tamPix);i=i+3){
		b=pix[i];
		g=pix[i+1];
		r=pix[i+2];
		gg=(b+g+r)/3;
		pix[i]=gg;
		pix[i+1]=gg;
		pix[i+2]=gg;
	}//escala de grises, y si funciona.
	fwrite(&data,sizeof(data),1,imgGris);
	fwrite(&pix,sizeof(pix),1,imgGris);
	
	//-------------------------------------------------------------------------------------------------------------------------
	unsigned char x;
	printf("A continuacion, digite el kernel: \n");
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
		printf("Fila %d, columna: %d: ",i+1 ,j+1);
		scanf("%d",&x);
		kernel[i*3+j]=x;	
		}
	}
	
	
	for(i=0;i<tamPix;i=i+3){
		b=0; g=0; r=0;
		for(j=0;j<3;j++){
			b=b+pix2[i+1533+j*3]*kernel[j];//512x3=1536
			g=b+pix2[i+1534+j*3]*kernel[j];
			r=b+pix2[i+1535+j*3]*kernel[j];
		}
		for(j=3;j<6;j++){
			b=b+pix2[i-12+j*3]*kernel[j];//
			g=b+pix2[i-11+j*3]*kernel[j];
			r=b+pix2[i-10+j*3]*kernel[j];
		}
		for(j=6;j<9;j++){
			b=b+pix2[i-1557+j*3]*kernel[j];//-1536-3-18
			g=b+pix2[i-1556+j*3]*kernel[j];
			r=b+pix2[i-1555+j*3]*kernel[j];
		}
		b=b/9;
		g=g/9;
		r=r/9;
		pix[i]=b;
		pix[i+1]=g;
		pix[i+2]=r;
	}
	
	
	
	fwrite(&data,sizeof(data),1,imgConv);
	fwrite(&pix,sizeof(pix),1,imgConv);
	//--------------------------------------------------------------------------------------------------------------------------
	
	
	
	
	
	fclose(img);
	fclose(imgGris);
	fclose(imgConv);
	
	return 0;
}