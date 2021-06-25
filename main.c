#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include "kksystem.h"

#define LS 5863588
#define CD 5863276
#define MKDIR 210720772860
#define RMDIR 210726774339
#define TOUCH 210729236360
#define RM 5863780
#define EXIT 6385204799

const unsigned long hash(const char *str) {
    unsigned long hash = 5381;  
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
	printf("Hash: %lu\n", hash);
    return hash;
}
void listEnv(KK failsisten32);
int kkcwd(KK failsisten32);
void changeDirectory(KK *failsisten32, char* name);
int touchWC(KK failsisten32, char* path);
int rmWC(KK failsisten32, char* path);
int mvWC(KK failsisten32, char* path);
int modWC(KK failsisten32, char* path);
NODO* validatePath(KK failsisten32, char* path);
int mover(KK failsisten32, char*path, char* file);


int main(){
	NODO head = { .name="root", .type=1, .path=".", .child = NULL, .next = NULL };
	KK   table = { .root=&head, .curr=&head };

	char *option = (char*)malloc(10);
	char *pathito = (char*)malloc(10);
	printf("Bienvenidos al failsisten32: \n");
	
	while(1){

		printf(">> ");
		scanf("%s", option);

		if (hash(option) == LS) {
		  listEnv(table);
		  continue;
		} else if (hash(option) == EXIT) {
		  return 1;
		}
		scanf("%s", pathito);
		
		switch(hash(option)) {
		case CD:
			printf("Running cd...\n");
			changeDirectory(&table, pathito);
			break;
		case MKDIR:
			printf("Running mkdir...\n");
			mKKdir( table, pathito);
			break;
		case RMDIR:
			printf("Running rmdir...\n");
			rmKKdir( table, pathito);
			break;
		case TOUCH:
			printf("Running touch...\n");
			touchWC( table, pathito);
			break;
		case RM:
			printf("Running rm...\n");
			rmWC( table, pathito);
			break;
		default:
			printf("[ERROR] '%s' is not a valid command.\n", option);
			mover( table, pathito, "aaa" );
			return 1;
		}

	}
	/*
	if (strcmp(option, "mkdir") == 0) {
		printf("hola\n");
	} else if (strcmp(option, "rmdir") == 0) {
		printf("chao\n");
	} else {
		printf("xdd\n");
	}*/

	/*
	mKKdir( table, "hijo");
	mKKdir( table, "nietito");
	mKKdir( table, "renietito");
	touchWC( table, "elnico");

	printf("[+] %s %s %s %s\n", table.root->name, table.root->child->name, table.root->child->next->name, table.root->child->next->next->name);
	rmKKdir( table, "nietito");
	printf("[+] %s %s %s \n", table.root->name, table.root->child->name, table.root->child->next->name );
	rmKKdir( table, "hijo");
	printf("[+] %s %s \n", table.root->name, table.root->child->name );
	*/
}


int mKKdir(KK failsisten32, char* name){
	/*	Inserta un directorio en el nodo actual de la tabla, 
	si ya existe un directorio lo coloca en la lista enlazada */

	char *new_path = (char *)malloc(sizeof(char)*1024);
	memcpy(new_path, failsisten32.curr->path, sizeof(failsisten32.curr->path));
	strcat(new_path, "/");
	strcat(new_path,name);

	printf("Creando: %s\n", new_path);


	int res = mkdir(new_path, 0777);
	if(res < 0){
		printf("No se ha podido crear el directorio. \n");
		return -1;
	}
	
	if( failsisten32.curr->child == NULL ) {

		failsisten32.curr->child = (NODO *)malloc(sizeof(NODO));
		failsisten32.curr->child->name  = name;
		failsisten32.curr->child->type  = 1;
		failsisten32.curr->child->path  = new_path;
		failsisten32.curr->child->child = NULL;
		failsisten32.curr->child->next  = NULL;
	
	} else {

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
		printf("No existe el directorio. \n");
		return -1;

	} else if( failsisten32.curr->child->name == name && failsisten32.curr->child->type == 1 ){
		// es el primer dir
		int res = rmdir( failsisten32.curr->child->path );
		if(res < 0){
		  printf("No se pudo borrar el directorio\n");
		  return -1;
		}
		// Manejar archivos
		NODO* temp = failsisten32.curr->child->next;
		printf("Removiendo: %s\n", failsisten32.curr->child->path);
		free( failsisten32.curr->child );	
		failsisten32.curr->child = temp;
	}else{
		// es otro dir
		int res = rmdir( failsisten32.curr->child->path );

		printf("Removiendo: %s\n", failsisten32.curr->child->path );
		if(res < 0){
			printf("No se ha podido borrar el directorio.\n");
			return -1;
		}
		// Manejar archivos

		NODO* temp = failsisten32.curr->child;
		while(temp->next != NULL){
			if( temp->next->name == name && temp->child->type == 1){
				NODO* new = temp->next->next;
				free( temp->next );	
				temp->next = new;
				return 0;
			}
		}
		return -1;
	}

}

int touchWC(KK failsisten32, char* name){
	/* Inserta un archivo en el nodo actual de la tabla, 
	si ya existe un archivo lo coloca en la lista enlazada */

	char *new_path = (char *)malloc(sizeof(char)*1024);
	memcpy(new_path, failsisten32.curr->path, sizeof(failsisten32.curr->path));
	strcat(new_path, "/");
	strcat(new_path,name);

	printf("Creando: %s\n", new_path);

	int res = creat(new_path, 0777);
	if(res < 0){
		printf("No se ha podido crear el directorio. \n");
		return -1;
	}
	
	if( failsisten32.curr->child == NULL ) {

		failsisten32.curr->child = (NODO *)malloc(sizeof(NODO));
		failsisten32.curr->child->name  = name;
		failsisten32.curr->child->type  = 1;
		failsisten32.curr->child->path  = new_path;
		failsisten32.curr->child->child = NULL;
		failsisten32.curr->child->next  = NULL;
	
	} else {

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

int rmWC(KK failsisten32, char* name){
	/* Remueve un archivo en el nodo actual de la tabla, 
	si no se encuentra el archivo recibimos -1 */	

	if( failsisten32.curr->child == NULL ){
		// no hay nodos
		printf("No existe el directorio. \n");
		return -1;

	} else if( failsisten32.curr->child->name == name && failsisten32.curr->child->type == 0 ){
		// es el primer nodo
		int res = unlink( failsisten32.curr->child->path );
		if(res < 0){
		  printf("No se pudo borrar el directorio\n");
		  return -1;
		}
		// Manejar archivos
		NODO* temp = failsisten32.curr->child->next;
		printf("Removiendo: %s\n", failsisten32.curr->child->path);
		free( failsisten32.curr->child );	
		failsisten32.curr->child = temp;
	}else{
		// es otro dir
		int res = unlink( failsisten32.curr->child->path );

		printf("Removiendo: %s\n", failsisten32.curr->child->path );
		if(res < 0){
			printf("No se ha podido borrar el directorio.\n");
			return -1;
		}
		// Manejar archivos

		NODO* temp = failsisten32.curr->child;
		while(temp->next != NULL){
			if( temp->next->name == name && temp->child->type == 0){
				NODO* new = temp->next->next;
				free( temp->next );	
				temp->next = new;
				return 0;
			}
		}
		return -1;
	}

}

void listEnv(KK failsisten32) {

	printf("Estas en el path: %s\n", failsisten32.curr->path);
	NODO* temp = failsisten32.curr->child;
	
	while (temp != NULL ) {
		printf("%s ", temp->path);
		temp = temp->next;
	}
	printf("\n");
}

void changeDirectory(KK *failsisten32, char* path) {
  if( strcmp(path,"..") == 0){
    failsisten32->curr = failsisten32->root;
    return;
  }
	char delim[] = "/";

	char *ptr = strtok(path, delim);

	char *new_path = (char *)malloc(sizeof(char)*1024);
	memcpy(new_path, failsisten32->curr->path, sizeof(failsisten32->curr->path));
	
	NODO *temp = failsisten32->curr->child;

	strcat(new_path, "/");
	strcat(new_path, ptr);
		
	while( temp != NULL ){
		if (strcmp(temp->path, new_path) == 0) {
			failsisten32->curr = temp;
			printf("Tu path actual es %s, comparado con %s\n", failsisten32->curr->path, new_path);

			ptr = strtok(NULL, delim);
			if (ptr != NULL) {
				strcat(new_path, "/");
				strcat(new_path, ptr);
				temp = failsisten32->curr->child;
			} else {
				return;
			}

		} else {
			temp = temp->next;
		}
	}

}

int mover( KK failsisten32, char* path, char* file){
  // encontrar puntero a file
  NODO* archivo = failsisten32.curr->child;
  NODO mover;
  if( archivo->name != file ){
    while( archivo->next != NULL ){
      if( archivo->name == file ){
	mover = *archivo;
        rmWC( failsisten32, file );
	break;
      }
    }
  }
  
  changeDirectory(&failsisten32, path);
  if( failsisten32.curr->child == NULL ){
    failsisten32.curr->child = (NODO *)malloc(sizeof(NODO));
    failsisten32.curr->child->name = mover.name;
    failsisten32.curr->child->path;
    failsisten32.curr->child->next = NULL;
    failsisten32.curr->child->child = NULL;
  }
  // agrega archivo

  return 1;
}
