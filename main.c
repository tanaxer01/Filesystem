#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include "kksystem.h"

#define LS 5863588
#define CD 5863276
#define MV 5863624
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
int movwc(kk failsisten32, char* old, char* new);
int modWC(KK failsisten32, char* name);
NODO* validatePath(KK failsisten32, char* path);
int mover(KK failsisten32, char*path, char* file);


int main(){
  NODO head = { .name="root", .type=1, .path=".", .child = NULL, .next = NULL };
  KK   table = { .root=&head, .curr=&head };

  char *option = (char*)malloc(10);
  char *pathito = (char*)malloc(10);
  char *pathitonew = (char*)malloc(10);

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
    case MV:
      printf("Running rm...\n");
      scanf("%s", pathitonew);
      movwc( table, pathito, pathitonew);
      break;
    default:
      printf("[ERROR] '%s' is not a valid command.\n", option);
      return 1;
    }

  }
}


int mKKdir(KK failsisten32, char* name){
  /* Inserta un directorio en el nodo actual de la tabla, 
     si ya existe un directorio lo coloca en la lista enlazada */
  
  char *new_path = (char *)malloc(sizeof(char)*1024);
  char *new_name = (char *)malloc(sizeof(name));
  memcpy(new_name, name, sizeof(name));
	 
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
    failsisten32.curr->child->name  = new_name;
    failsisten32.curr->child->type  = 1;
    failsisten32.curr->child->path  = new_path;
    failsisten32.curr->child->child = NULL;
    failsisten32.curr->child->next  = NULL;
	
  } else {

    NODO* temp = failsisten32.curr->child;
    while( temp->next != NULL )
      temp = temp->next;
		
    temp->next = (NODO *)malloc(sizeof(NODO));
    temp->next->name   = new_name;
    temp->next->path   = new_path;
    temp->next->type   = 1;
    temp->next->child  = NULL;
    temp->next->next   = NULL;

  }	

  return 0;
}
	
int rmKKdir(KK failsisten32, char* name){
  /* Remueve un directorio en el nodo actual de la tabla, 
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

int movWC(KK failsisten32, char* old, char* new){
  /* Modifica el nombre de un archivo en el nodo actual de 
     la tabla, si no se encuentra el archivo recibimos -1 */	

  if( failsisten32.curr->child == NULL ){
    // no hay nodos
    printf("No existe el archivo. \n");
    return -1;

  } else if( failsisten32.curr->child->name == old && failsisten32.curr->child->type == 0 ){
    // es el primer nodo
    char *new_path = (char *)malloc(sizeof(failsisten32.curr->path)+sizeof(new));
    memcpy(new_path, failsisten32.curr->path, sizeof( failsisten32.curr->path));
    strcat(new_path, new);
    int res = rename( failsisten32.curr->child->path, new_path );
    if(res < 0){
      printf("No se pudo renombrar el archivo\n");
      return -1;
    }
    // Manejar archivos
    NODO* temp = failsisten32.curr->child->next;
    printf("Renombrando: %s\n", failsisten32.curr->child->path);
    failsisten32.curr->child->name = new;
    failsisten32.curr->child->path = new_path;

  }else{
    // es otro dir
    char *new_path = (char *)malloc(sizeof(failsisten32.curr->path)+sizeof(new));
    memcpy(new_path, failsisten32.curr->path, sizeof( failsisten32.curr->path));
    strcat(new_path, new);
    int res = rename( failsisten32.curr->child->path, new_path );

    printf("Renombrando: %s\n", failsisten32.curr->child->path );
    if(res < 0){
      printf("No se ha podido renombrar el archivo.\n");
      return -1;
    }
    // Manejar archivos

    NODO* temp = failsisten32.curr->child;
    while(temp->next != NULL){
      if( temp->next->name == old && temp->child->type == 0){
	temp->next->name = new;
	temp->next->path = new_path;
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
/*
int mover( KK failsisten32, char* path, char* file){
  // encontrar puntero a file
  printf("we moving'\n");
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
  printf("directory errased\n");
  changeDirectory(&failsisten32, path);
  char * new_path = (char *)malloc(sizeof(char)*100);
  memcpy(new_path, failsisten32.curr->path, sizeof(failsisten32.curr->path));
  strcat(new_path, "/");
  strcat(new_path, file);
  if( failsisten32.curr->child == NULL ){
    failsisten32.curr->child = (NODO *)malloc(sizeof(NODO));
    failsisten32.curr->child->name = mover.name;
    failsisten32.curr->child->path = new_path;
    failsisten32.curr->child->next = NULL;
    failsisten32.curr->child->child = NULL;
  }
  // agrega archivo

  return 1;





  NODO* validatePath( KK failsisten32, char* path ){
    printf("starting\n");

    // 1. Es la raiz ?
    if( strcmp(path, "/") == 0 )
      return failsisten32.root;

    // 2. Partimos desde la raiz o el current
    char delim[] = "/";
    char *ptr = strtok(path, delim);

    NODO* curr;
    if( strcmp(ptr,".") == 0 ){
      if( failsisten32.curr->child == NULL)
	return NULL;
      curr = failsisten32.curr->child;
    }else{
      if( failsisten32.root->child == NULL)
	return NULL;
      curr = failsisten32.root->child;
    }

    // necesitamos ir buscando si los directorios existen
    if( strcmp(ptr,".") == 0){
      ptr = strtok(NULL, delim);
    }
  
    int found;
    while( ptr != NULL ){  
      printf("[+] %s \n", ptr);

      found = 0;
      // si el hijo directo es el directorio
      printf("starting with %s %s\n", ptr, curr->name);
      if( strcmp( curr->name, ptr) == 0 && curr->type == 1){
	printf("AAA\n");
	found = 1;

	continue;
      }
    
      while( curr->next != NULL ){
	printf(" comparando %s y %s\n",ptr, curr->name);
	if( strcmp( curr->name, ptr) == 0 && curr->type == 1){
	  // encontramos el directorio
	  ptr = strtok(NULL, delim);
	  found = 1;
	  curr = curr->child;

	  break;
	}
	curr = curr->next;
      }

      // si no se encontro el directorio
      if( found == 0){
      
      }
      // si el hijo es NULL pero aun no se termina el path
      if( curr == NULL ){
	printf("La ruta ingresada no existe\n");
	return NULL;
      }
    
      ptr = strtok(NULL, delim);
    }
    printf("finalll\n");
    return NULL;
  }


  // . - curr path
  // / - root
  */
