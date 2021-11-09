#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct node
{
	int data;
	struct node *leftNode;
	struct node *rightNode;
};

struct node *root = NULL;
pthread_mutex_t mutex;
int terminate = 0;

void insertData(int data);
void* readAndInsert(void *arg);
void writeOutput(struct node *Node,int count,char* filename);
void freeTree(struct node* Node);

int main(int argc,char* argv[])
{
	int i,control;
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
	if((argc-4) != N)
	{
		printf("Eksik Dosya Sayisi\n");
		return 0;
	}
	
	pthread_mutex_init(&mutex,NULL);
	
	pthread_t threadList[N];
	for(i = 0;i < N;i++)
	{
		control = pthread_create((threadList + i),NULL,&readAndInsert,argv[i + 3]);
		if(control)
		{
			printf("%d. thread olusturulamadi",i);
			continue;
		}
	}
	for(i = 0;i < N;i++)
	{
		pthread_join(threadList[i],NULL);
	}
	
	pthread_mutex_destroy(&mutex);
	writeOutput(root,K,argv[N + 3]);
	freeTree(root);
	return 0;
}

void insertData(int data)
{
	struct node *newNode = (struct node*) malloc(sizeof(struct node));
	struct node *childNode;
	struct node *parentNode;
	
	newNode->data = data;
	newNode->leftNode = NULL;
	newNode->rightNode = NULL;
	
	if(root == NULL)
	{
		root = newNode;
	}
	else
	{
		childNode = root;
		parentNode = NULL;
	
		while(1)
		{
			parentNode = childNode;
			
			if(data < parentNode->data)
			{
				childNode = parentNode->leftNode;
				
				if(childNode == NULL)
				{
					parentNode->leftNode = newNode;
					printf("%d, %dnin soluna\n",newNode->data,parentNode->data);
					return;
				}
			}
			else if(data > parentNode->data)
			{
				childNode = parentNode->rightNode;
				
				if(childNode == NULL)
				{
					parentNode->rightNode = newNode;
					printf("%d, %dnin sagina\n",newNode->data,parentNode->data);
					return;
				}
			}
			else
			{
				return;
			}
		}
	}
}

void* readAndInsert(void *arg)
{
	char *filename = (char*) arg;
	int buffer;
	unsigned int control;
	
	FILE* fp;
	fp = fopen(filename,"r");
	
	control = fscanf(fp,"%d\n",&buffer);
	
	while(control != EOF)
	{	
		pthread_mutex_lock(&mutex);
		insertData(buffer);
		pthread_mutex_unlock(&mutex);
		control = fscanf(fp,"%d\n",&buffer);
	}
	fclose(fp);
}
void writeOutput(struct node *Node,int count,char* filename)
{
	if(Node == NULL)
	{
		return;
	}
	
	writeOutput(Node->rightNode,count,filename);
	if(terminate == count)
	{	
		return;
	}
	FILE* fp;
	fp = fopen(filename,"a");
	fprintf(fp,"%d\n",Node->data);
	printf("%d\n",Node->data);
	fclose(fp);
	terminate++;
	writeOutput(Node->leftNode,count,filename);
	
	
}

void freeTree(struct node* Node)
{
	if(Node != NULL)
	{
		freeTree(Node->leftNode);
		freeTree(Node->rightNode);
		free(Node);
	}
}

