#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>


char interfilenames[5][16] = {"interfile1.txt","interfile2.txt","interfile3.txt","interfile4.txt","interfile5.txt"};

int main(int argc,char* argv[])
{
	int K;
	sscanf(argv[1],"%d",&K);
	int N; 
	sscanf(argv[2],"%d",&N);
	
	if(N<=0 || N>5)
	{
	printf("Hatali N Girisi\n");
		return 0;
	}
	if(K < 0)
	{
		printf("Hatali K Girisi\n");
		return 0;
	}
	if((argc-3) != N)
	{
		printf("Eksik Dosya Sayisi\n");
		return 0;
	}
	
	
	int length = 1,length2,a_length = 3,a_in = 0,fd,i,j,k,key;
	char* buffer = (char*) malloc(sizeof(char));
	char* fnumber = (char*) malloc(12 * sizeof(char));
	int *array = (int*) malloc(a_length * sizeof(int));
	char cbuffer[12];
	
	clock_t start = clock(),stop;
	for(i = 0;i < N;i++)
	{
		if(fork() == 0)
		{
			fd = open(argv[i + 3],O_RDWR);
			if( fd < 0)
			{
				printf("%d.Dosya Bulunamadi\n",(i + 1));
				exit(0);
			}
			
			while(length != 0)
			{
				length = read(fd,buffer,sizeof(char));
				if(a_in == a_length - 2)
				{
					a_length += 10;
					array = realloc(array,a_length * sizeof(int));
				}
				if(strcmp(buffer," ") != 0)
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
			
			close(fd);
			fd = open(interfilenames[i],O_CREAT | O_EXCL | O_RDWR,S_IRWXU);
			
			length2 = sprintf(cbuffer,"%d",array[K]);
			strcat(cbuffer," ");
			write(fd,cbuffer,length2 + 1);
			
			close(fd);
			
			free(buffer);
			free(fnumber);
			free(array);
			
			exit(0);
		}
	}
	while( wait(NULL) > 0);
	
	if(1)
	{
		
		for(i = 0; i < N;i++)
		{
			fd = open(interfilenames[i],O_RDWR);
			
			length = 1;
			
			while(length != 0)
			{
				length = read(fd,buffer,sizeof(char));
				if(a_in == a_length - 2)
				{
					a_length += 1;
					array = realloc(array,a_length * sizeof(int));
				}
				if(strcmp(buffer," ") != 0)
				{
					strcat(fnumber,buffer);
				}
				else
				{
					sscanf(fnumber,"%d",array + a_in);
					strcpy(fnumber,"\0");
					a_in++;
					break;
				}
			}
			close(fd);
			remove(interfilenames[i]);
		}
		
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
		
		fd = open("output.txt",O_CREAT | O_EXCL | O_RDWR,S_IRWXU);
		
		for(j = 0;j < a_in;j++)
		{
			strcpy(cbuffer,"\0");
			length2 = sprintf(cbuffer,"%d",array[j]);
			strcat(cbuffer,"\n");
			write(fd,cbuffer,length2 + 1);
		}
		
		close(fd);
		stop = clock();
		
		free(buffer);
		free(fnumber);
		free(array);
		
		printf("\nToplam Gecen Sure:%ld ms\n",stop-start);
		
	}

	return 0;
}
