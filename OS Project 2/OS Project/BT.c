#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct NODE{
	int x;
	struct NODE * left;
	struct NODE * right;
}NODE;

pthread_mutex_t mutex;

typedef struct{
	NODE * root;
}AGAC;



AGAC * AGAC_OLUSTUR(){
	AGAC * bst;
	bst=(AGAC *)malloc(sizeof(AGAC));
	if(bst){
		bst->root=NULL;
	}
	return bst;
}



NODE * DUGUM_OLUSTUR(int x){
	NODE * node;

	node=(NODE *)malloc(sizeof(NODE));
	if(node){
		node->x=x;
		node->left=NULL;
		node->right=NULL;
	}
	return node;
}



int DUGUM_EKLE(NODE * root, NODE * node){
	if(root==NULL){
		return 1;
	}
	if(root->x<node->x){
		if(root->right==NULL){
			root->right=node;
			return 0;
		}
		return DUGUM_EKLE(root->right,node);
	}
	else if(root->x>node->x){
		
		if(root->left==NULL){
			
			root->left=node;
			return 0;
		}
				
		return DUGUM_EKLE(root->left,node);

	}
	return 1;
}



NODE * ARA(NODE * root,int x){
	if(!root){
		return NULL;
	}
	if(root->x<x){
		return ARA(root->right,x);
	}else if(root->x>x){
		return ARA(root->left,x);
	}
	
	return root;
}


void YAZDIR(NODE * root){
	if(root==NULL){
		return;
	}
	YAZDIR(root->left);
	printf("%d\n\n",root->x);
	YAZDIR(root->right);
}


int main(void){
	NODE * root;
	NODE * node;
	int eklenecek_deger=0;
	int aranacak_deger=0;
	int i=0;
	int menu=0;;
	AGAC * bst;
	bst=AGAC_OLUSTUR();
	

	printf("1-Ekle \n");
	printf("2-Ara  \n");



	while("true"){
		printf("Menu Sec:");
		scanf("%d",&menu);
		fflush(stdin);
		FILE *fp;
		fp=fopen("infile1.txt","r");

		if(menu==1){
			printf("Eklenecek Deger txt'den Cekiliyor:\n");
			
			
			
			while(!feof(fp)){
				
			fscanf(fp,"%d",&eklenecek_deger);
			node=DUGUM_OLUSTUR(eklenecek_deger);
			DUGUM_EKLE(bst->root,node);
			if(bst->root==NULL){
				bst->root=node;
				
			} 	
		}	
			printf("txt Agaca Eklendi\n");
			printf("Agac LNR Biciminde Siralaniyor\n");
			YAZDIR(bst->root);
		}
		else if(menu==2){
			printf("Agacta Aranacak Deger:");
			scanf("%d",&aranacak_deger);
			node=ARA(bst->root,aranacak_deger);
			if(node){
				
				printf("%d Degeri Agacta Bulundu Root:%d \n",aranacak_deger,node);
			}else{
				
				printf("%d Degeri Agacta Bulunamadi Root:%d\n",aranacak_deger,node);
			} 			
		}
		
		
	}	
	return 0;
}
