
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
