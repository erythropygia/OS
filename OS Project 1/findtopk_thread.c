#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

int k_sayilari[5];

struct t_args {
    char* dosya;
    int indis;
    int k;
};

void *sirala(void *argumanlar){

    int length = 1,a_length = 10,a_in = 0,fd,i,j,k,key;
    char* buffer = (char*) malloc(sizeof(char));
    char* fnumber = (char*) malloc(12 * sizeof(char));
    int *array = (int*) malloc(a_length * sizeof(int));

    fd = open(((struct t_args*)argumanlar)->dosya,O_RDWR);
    while(length != 0)
    {
	length = read(fd,buffer,sizeof(char));
	if(a_in == a_length - 2)
        {
		a_length += 10;
		array = realloc(array,a_length * sizeof(int));
	}
	if(strcmp(buffer,"\n") != 0)
	{
		strcat(fnumber,buffer);
	}
	else
	{
        	sscanf(fnumber,"%d",array + a_in);
		strcpy(fnumber,"\0");
		a_in++;
	}
   }

//   for(i=0;i<10;i++) printf("%s ",array[i]);
   for(j = 0;j < a_in;j++)
   {
	key = array[j];
	k = j - 1;

	while(k >= 0 && key < array[k])
        {
		array[k + 1] = array[k];
		k--;
	}
	array[k + 1] = key;
   }
//   for(j = 0;j < a_in;j++)
//   {
//	printf("%d\n",array[j]);
//   }
   printf("K. SAYI : %d\n",array[((struct t_args*)argumanlar)->k]);
   k_sayilari[((struct t_args*)argumanlar)->indis] = array[((struct t_args*)argumanlar)->k];
//   free(buffer);
//   free(fnumber);
//   free(array);
   close(fd);
   pthread_exit(NULL);
}

int main(int argc, char *argv[]){

   if(argc < 4){printf("Yeterli parametre girmediniz!\n");exit(1);}

   int i;
   //int dosya_sayisi = argc-2;
   int k = atoi(argv[1]);
   int n = atoi(argv[2]);

   pthread_t threads[n];
   struct t_args *argumanlar = (struct t_args *)malloc(sizeof(struct t_args));

   clock_t timer = clock();

   for(i = 0;i < n;i++){
       //printf("\n%d. Dosya\n",i+1);
       //printf("-------------\n");
       printf("Gonderilen dosya : %s\n",argv[i+3]);
       argumanlar->dosya = argv[i+3];
       argumanlar->indis = i;
       argumanlar->k = k;
       pthread_create(&threads[i],NULL,sirala,(void *)argumanlar);
       pthread_join(threads[i], NULL);
   }

   timer = clock() - timer;
   printf("Threadler bitti! - Gecen zaman : %f\n",((double)timer)/CLOCKS_PER_SEC);

   int fd = open(argv[n+3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0)
  {
     perror("r1");
     exit(1);
  }

  char cbuffer[12];
  int lenght;

  for(i = 0;i < n;i++){

     strcpy(cbuffer,"\0");
     lenght = sprintf(cbuffer,"%d",k_sayilari[i]);
     strcat(cbuffer,"\n");
     write(fd,cbuffer,lenght+1);

  }

close(fd);

}
