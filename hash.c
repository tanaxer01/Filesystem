#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

int main(){
    char str = "mkdir"; 
    unsigned long hash = 5381;  
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
	printf("Hash: %d\n", hash);
}
