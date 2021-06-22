#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

typedef struct NodoCUM{
  int index;
  int dir;

  char* nombre;
  char* path;
  struct NodoCum *next;
// puntero file
} Nodo;

typedef struct KKsistem{
  //Nodo *root; 
  //Nodo *curr;
  int a;

} KK;

int mKKdir(KK failsisten32,  char* path);
int rmKKdir(KK failsisten32, char* path);

int touchWC(KK failsisten32, char* path);
int rmWC(KK failsisten32, char* path);
int mvWC(KK failsisten32, char* path);
int modWC(KK failsisten32, char* path);

int main(){
		KK sys = { .a= 10 };
		mKKdir( sys, "./hola/asd" );

}


int mKKdir(KK failsisten32, char* path){
	// 1. Creamos el directorio usando el syscall
	//int res = mkdir(path, 777);
	int res = 1;
	if( res < 0 )
			return -1;

	// 2. Actualizamos el KKsystem
	failsisten32.curr = failsisten32.root;

	char *curr_node = strtok(path,"/");			
	while( curr_node != NULL ){
		printf( " %s\n", curr_node );
		
		curr = failsisten32.curr;
		while( curr != NULL ){		//iterar por los nodos hijos
			if( curr.name == curr_node ){	
				

			curr++;
		}
		if( curr == failsisten32.curr ){
			// No se encontro el dir y debe ser creado.

			*curr.append( 
		}

 

		curr_node = strtok(NULL,"/");
	}

	// path --> dirs []
		

	return 1;
}
