#include <math.h>
//#include <fcntl.h> // for open
//#include <unistd.h> // for close
#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

void formatowave (FILE *archivo, wave *w);
void imprimirFormato (wave *wav);
void escribirArchivo(FILE *archivoEscritura, wave *header);
void Multiplicacion (FILE *archivo, wave *in1, wave *in2);
void Escalamiento(double *w, wave *entrada);


int main(int argc, char **argv){
	FILE *archivoEntrada1, *archivoEntrada2, *archivoEscritura;
	wave wav1, wav2;
	wave *conv;	
	//archivoEntrada1=fopen("sen.wav","rb");
	//archivoEntrada2=fopen("step.wav","rb");
	archivoEntrada1=fopen(argv[1],"rb");
	archivoEntrada2=fopen(argv[2],"rb");
	archivoEscritura=fopen(argv[3],"wb");	

	if (archivoEntrada1!=NULL && archivoEntrada2!=NULL && archivoEscritura!=NULL) 
 	{
		formatowave(archivoEntrada1, &wav1);
		formatowave(archivoEntrada2, &wav2);
		//imprimirFormato(&wav2);
		
		Multiplicacion(archivoEscritura,&wav1,&wav2);
		//escribirArchivo(archivoEscritura, conv);
		//imprimirFormato(conv);

		fclose(archivoEntrada1);
		fclose(archivoEntrada2);
		fclose(archivoEscritura);
	}else
 	printf("No se pudo generar el archivo\n");

}

void escribirArchivo(FILE *archivoEscritura, wave *header){
	fwrite(header->riff, sizeof(header->riff),1,archivoEscritura);
	fwrite(header->overall_sizeb, sizeof(header->overall_sizeb),1,archivoEscritura);
	fwrite(header->wave, sizeof(header->wave),1,archivoEscritura);
	fwrite(header->fmt_chunk_marker, sizeof(header->fmt_chunk_marker),1,archivoEscritura);
	fwrite(header->length_of_fmtb, sizeof(header->length_of_fmtb),1,archivoEscritura);
	fwrite(header->format_typeb, sizeof(header->format_typeb),1,archivoEscritura);
	fwrite(header->channelsb, sizeof(header->channelsb),1,archivoEscritura);
	fwrite(header->sample_rateb, sizeof(header->sample_rateb),1,archivoEscritura);
	fwrite(header->byterateb, sizeof(header->byterateb),1,archivoEscritura);
	fwrite(header->block_alignb, sizeof(header->block_alignb),1,archivoEscritura);
	fwrite(header->bits_per_sampleb, sizeof(header->bits_per_sampleb),1,archivoEscritura);
	fwrite(header->data_chunk_header, sizeof(header->data_chunk_header),1,archivoEscritura);
	fwrite(header->data_sizeb, sizeof(header->data_sizeb),1,archivoEscritura);
	fwrite(header->muestras, sizeof(short)*header->num_samples,1,archivoEscritura);
	//fwrite(header->muestras, sizeof(header->muestras),1,archivoEscritura);
	fwrite(header->end, sizeof(header->end),1,archivoEscritura);
}

void imprimirFormato (wave *wav){
	int i;
	printf("(%c)(%c)(%c)(%c):\n", wav->riff[0], wav->riff[1], wav->riff[2], wav->riff[3]);
	printf("(%u):\n", wav->overall_size);
	printf("(%c)(%c)(%c)(%c):\n", wav->wave[0], wav->wave[1], wav->wave[2], wav->wave[3]);
	printf("(%c)(%c)(%c):\n", wav->fmt_chunk_marker[0], wav->fmt_chunk_marker[1], wav->fmt_chunk_marker[2]);
	printf("length_of_fmt(%u):\n", wav->length_of_fmt);
	printf("format_type(%u):\n", wav->format_type);
	printf("channels(%u):\n", wav->channels);
	printf("sample rate:(%u):\n", wav->sample_rate);
	printf("byterate(%u):\n", wav->byterate);
	printf("block_align(%u):\n", wav->block_align);
	printf("bits_per_sample(%u):\n", wav->bits_per_sample);
	printf("(%c)(%c)(%c)(%c):\n", wav->data_chunk_header[0], wav->data_chunk_header[1], wav->data_chunk_header[2], wav->data_chunk_header[3]);
	printf("data_size(%d bytes):\n", wav->data_size);
	printf("num_samples(%lu):\n", wav->num_samples);
	printf("size(%ld):\n", wav->size_of_each_sample);
	for(i=0; i<wav->num_samples; i++){
		printf("(%d:%04x)\n",i, wav->muestras[i]);
		}
	for(i=0; i<74; i++){
		//printf("|%d:%02x", i, wav->end[i]);
	}//printf("|\n");
}

void formatowave (FILE *archivoEntrada, wave *header){
	unsigned char buffer4[4];
 	unsigned char buffer2[2];
 	int i;
	if (archivoEntrada!=NULL) 
 	{
 		fread(header->riff, sizeof(header->riff), 1, archivoEntrada);
 		fread(header->overall_sizeb, sizeof(header->overall_sizeb), 1, archivoEntrada);
 		 header->overall_size  = header->overall_sizeb[0] | 
						(header->overall_sizeb[1]<<8) | 
						(header->overall_sizeb[2]<<16) | 
						(header->overall_sizeb[3]<<24);
 		fread(header->wave, sizeof(header->wave), 1, archivoEntrada);
 		fread(header->fmt_chunk_marker, sizeof(header->fmt_chunk_marker), 1, archivoEntrada);
 		fread(header->length_of_fmtb, sizeof(header->length_of_fmtb), 1, archivoEntrada);
 		 header->length_of_fmt = header->length_of_fmtb[0] |
							(header->length_of_fmtb[1] << 8) |
							(header->length_of_fmtb[2] << 16) |
							(header->length_of_fmtb[3] << 24);
 		fread(header->format_typeb, sizeof(header->format_typeb), 1, archivoEntrada);
 		 header->format_type = header->format_typeb[0] | (header->format_typeb[1] << 8);
 		fread(header->channelsb, sizeof(header->channelsb), 1, archivoEntrada);
 		 header->channels = header->channelsb[0] | (header->channelsb[1] << 8);
 		fread(header->sample_rateb, sizeof(header->sample_rateb), 1, archivoEntrada);
 		 header->sample_rate = header->sample_rateb[0] |
						(header->sample_rateb[1] << 8) |
						(header->sample_rateb[2] << 16) |
						(header->sample_rateb[3] << 24);
 		fread(header->byterateb, sizeof(header->byterateb), 1, archivoEntrada); 
 		 header->byterate  = header->byterateb[0] |
						(header->byterateb[1] << 8) |
						(header->byterateb[2] << 16) |
						(header->byterateb[3] << 24);
 		fread(header->block_alignb, sizeof(header->block_alignb), 1, archivoEntrada);
 		 header->block_align = header->block_alignb[0] |
					(header->block_alignb[1] << 8);
 		fread(header->bits_per_sampleb, sizeof(header->bits_per_sampleb), 1, archivoEntrada);
 		 header->bits_per_sample = header->bits_per_sampleb[0] |
					(header->bits_per_sampleb[1] << 8); 
 		fread(header->data_chunk_header, sizeof(header->data_chunk_header), 1, archivoEntrada);
 		fread(header->data_sizeb, sizeof(header->data_sizeb), 1, archivoEntrada);
		  header->data_size = header->data_sizeb[0] |
				(header->data_sizeb[1] << 8) |
				(header->data_sizeb[2] << 16) | 
				(header->data_sizeb[3] << 24 );
 		header->num_samples = (8 * header->data_size) / (header->channels * header->bits_per_sample);
 		header->size_of_each_sample = (header->channels * header->bits_per_sample) / 8;
 		header->muestras = malloc (sizeof(short)*header->num_samples);
 		for(i=0; i<header->num_samples; i++){
	 		fread(&header->muestras[i], sizeof(short), 1, archivoEntrada);
			//printf("(%x):\n", header->muestras[i]);
		}
		for(i=0; i<74; i++){
			fread(&header->end[i], sizeof(char), 1, archivoEntrada);
			//printf("%x\n", header->end[i]);
		}
	}else{
		printf("No se pudo abrir el archivo\n");
	}
}

void Multiplicacion (FILE *archivo, wave *in1, wave *in2){
	int tam,i;
	wave *mayor, *menor;

	if(in1->num_samples >= in2->num_samples){
		mayor = in1;
		menor = in2;
	}
	else{
		mayor = in2;
		menor = in1;
	}

	double *aux = malloc(sizeof(double)*mayor->num_samples);
	for(i=0; i < mayor->num_samples; i++){
		aux[i] = mayor->muestras[i] * menor->muestras[i];
	}

  	Escalamiento(aux, mayor);
  	escribirArchivo(archivo, mayor);
}


double BuscaMuestraMayor(double *w, wave *signa){
	int i;
	double max = 1;
	for(i=0; i < signa->num_samples; i++)
		if(fabs(w[i]) > max)	
			max = w[i];
	return max/32767;
}

void Escalamiento(double *w, wave *entrada){
	double max = BuscaMuestraMayor(w, entrada);
	printf("\n\n\n MAx: %lf\n\n\n", max);
   	int i;
   	//printf("Samples: %lu\n",entrada->num_samples);
   	for(i=0; i < entrada->num_samples; i++){
  		entrada->muestras[i] = w[i]/max;
  		//printf("%u\n", sample[i]);
  		//if(entrada->muestras[i] > 32767)
  		//	entrada->muestras[i] = 32767;
  		//else if(entrada->muestras[i] < -32767)
  		//	entrada->muestras[i] = -32767;
  		//printf("%04x\n", entrada->muestras[i]);
   	}
}

//PASAR ENTRADA A LAS FUNCIONES ES SOLO PARA CONOCER NUM_SAMPLES