#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
// Nodo de Cumulo de Unidades de Memoria
typedef struct NodoCUM{
  char* name;
  char* path;
  int   type;
  struct NodoCUM *next;
  struct NodoCUM *child;
// puntero file
} NODO;

typedef struct KKsistem{
  NODO *root; 
  NODO *curr;

} KK;


int mKKdir(KK failsisten32, char* name);
int rmKKdir(KK failsisten32, char* path);

int kkcwd(KK failsisten32);

int touchWC(KK failsisten32, char* path);
int rmWC(KK failsisten32, char* path);
int mvWC(KK failsisten32, char* path);
int modWC(KK failsisten32, char* path);

int main(){
	NODO head = { .name="root", .type=1, .path="./", .child = NULL, .next = NULL };
	KK   table = { .root=&head, .curr=&head };

	mKKdir( table, "hijo");
	mKKdir( table, "nietito");
	mKKdir( table, "renietito");

	printf("[+] %s %s %s %s\n", table.root->name, table.root->child->name, table.root->child->next->name, table.root->child->next->next->name);
	rmKKdir( table, "nietito");
	printf("[+] %s %s %s \n", table.root->name, table.root->child->name, table.root->child->next->name );
	rmKKdir( table, "hijo");
	printf("[+] %s %s \n", table.root->name, table.root->child->name );


}


//  TODO ver por que pinches vergas no funcionaba de la otra forma
//  TODO ver pq el strcat funciona bien 
int mKKdir(KK failsisten32, char* name){
	/*	Inserta un directorio en el nodo actual de la tabla, 
	si ya existe un directorio lo coloca en la lista enlazada */


	char *new_path = (char *)malloc(sizeof(char)*1024);
	memcpy(new_path, failsisten32.curr->path, sizeof(failsisten32.curr->path));
	strcat(new_path,"/");
	strcat(new_path,name);



	int res = mkdir(new_path, 0777);
	if(res < 0)
		return -1;
	
	if( failsisten32.curr->child == NULL ){
		failsisten32.curr->child = (NODO *)malloc(sizeof(NODO));
		failsisten32.curr->child->name  = name;
		failsisten32.curr->child->type  = 1;
		failsisten32.curr->child->path  = new_path;
		failsisten32.curr->child->child = NULL;
		failsisten32.curr->child->next  = NULL;
	
	}else{
		NODO* temp = failsisten32.curr->child;
		while( temp->next != NULL )
			temp = temp->next;
		
		temp->next = (NODO *)malloc(sizeof(NODO));
		temp->next->name   = name;
		temp->next->path   = new_path;
		temp->next->type   = 1;
		temp->next->child  = NULL;
		temp->next->next   = NULL;
	}	

	return 0;
}
	
int rmKKdir(KK failsisten32, char* name){
	/*	Remueve un directorio en el nodo actual de la tabla, 
	si no se encuentra el archivo recibimos -1 */
	

	if( failsisten32.curr->child == NULL ){
		// no hay dirs
		return -1;
	}else if( failsisten32.curr->child->name == name){
		// es el primer dir
		int res = rmdir( failsisten32.curr->child->path );
		if(res < 0)
			return -1;

		// Manejar archivos
		NODO* temp = failsisten32.curr->child->next;
		free( failsisten32.curr->child );	
		failsisten32.curr->child = temp;
	}else{
		// es otro dir
		int res = rmdir( failsisten32.curr->child->path );
		if(res < 0)
			return -1;
		// Manejar archivos
		NODO* temp = failsisten32.curr->child;
		while(temp->next != NULL){
			if( temp->next->name == name){
				NODO* new = temp->next->next;
				free( temp->next );	
				temp->next = new;
				return 0;
			}
		}
		return -1;
	}
}
