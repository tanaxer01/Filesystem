
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

//kk?
int kkcwd(KK failsisten32);

//Ready
int mKKdir(KK failsisten32, char* name);
int rmKKdir(KK failsisten32, char* path);

//Falta
int touchWC(KK failsisten32, char* path);
int rmWC(KK failsisten32, char* path);
int mvWC(KK failsisten32, char* path);
int modWC(KK failsisten32, char* path);
