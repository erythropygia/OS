#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "sorting.h"
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#define semName "sem"

int *sayilar;
sem_t bitis;
int aha;

void* create_shared_memory(size_t size) {

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANONYMOUS;

  return mmap(NULL, size, protection, visibility, -1, 0);
}

int main(int argc,char* argv[]){

	int k = atoi(argv[1]);
	int n = atoi(argv[2]);
	int dosya = argc - 3;
    int i,deger,index[1];
    index[0] = 0;
    int *dizi;
    sem_init(&bitis,n,0);
    dizi = (int*)malloc(k*sizeof(int)*n);
    sem_t *sem_id = sem_open(semName, O_CREAT, 0644, 2);
	void *shared_memory = create_shared_memory(k*sizeof(int));
    void *p = create_shared_memory(4);
    memcpy(p,index,4);
	for(i = 0;i < n;i++){

	    int pid = fork();

	    if(pid == 0){

		    int kucuk,buffer,boyut,indis = 0;
		    sayilar = (int*)malloc(k*sizeof(int));
            sayilar[0] = 0;
            sem_t *sem_id = sem_open(semName, 1);
		    printf("%d ---> child process is starting\n",getpid());


		    FILE *fp;

		    if(fp = fopen(argv[3+i],"r"))
		        printf("Dosya okuma basarili\n");
		    else
		        printf("Dosya okunamadi\n");

		    while(fscanf(fp,"%d\n",&buffer) != EOF){

//                printf("i : %d - ",indis);printArray(sayilar,indis);printf("\n");
  			    mergeSort(sayilar,0,indis);
                tersinecevir(sayilar,0,indis);

                kucuk = sayilar[indis];

                if(buffer > kucuk)
                    sayilar[indis] = buffer;

	            if(indis < k)
                    indis++;

	        }

//            printf("En buyuk k: ");
//            printArray(sayilar,indis);
            memcpy(shared_memory+(k*i),sayilar,indis*sizeof(int));
            sem_post(sem_id);
            exit(0);
	    }

        else{
            sem_wait(sem_id);
            wait(NULL);
            memcpy(index,p,sizeof(int));
            memcpy(dizi+index[0]*k,shared_memory+(k*index[0]),(k*n)*sizeof(int));
            index[0]++;
            memcpy(p,index,sizeof(int));
            printf("En son : ");
            printArray(dizi,k*index[0]);

        }
    sem_post(&bitis);
    sem_getvalue(&bitis,&aha);

    if(aha == n){

        mergeSort(dizi,0,(k*index[0])-1);
        tersinecevir(dizi,0,k*index[0]-1);
        //printArray(dizi,k*index[0]);
        FILE *yaz;
        yaz = fopen(argv[n+3],"w");
            for(i = 0;i<k*index[0];i++)
                fprintf(yaz,"%d\n",dizi[i]);
        printf("\nProcessler bitti! parent process siralanmis diziyi %s dosyasina yazdi!\n",argv[n+3]);
        exit(0);
	}


    }

	return 0;

}
