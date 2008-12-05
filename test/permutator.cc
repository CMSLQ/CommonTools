#include <stdio.h> 
#include <stdlib.h> 
#include <malloc.h> 

void permutazioni(int*,int,int); 

void permutazioni(int elementi[], int index,int dim) { 
    register int i,j; 
    int app; 

    if(index >= 1) { 
        for(i=index; i >= 0; i--) { 
        app=elementi[i]; 
        elementi[i]=elementi[index]; 
        elementi[index] = app; 
        permutazioni(elementi, index-1, dim); 
        app=elementi[i]; 
        elementi[i]=elementi[index]; 
        elementi[index] = app; 
        } 
    } 
    else { 
        for(j=0; j < dim; j++) 
            printf("%d", elementi[j]); 
        printf("\n"); 
    } 
} 


int main() { 
    int* elementi; 
    int nElementi; 
    register int i; 

    printf("%s", "Inserire numero elementi ==> "); 
    scanf("%d", &nElementi); 
    elementi = (int*) malloc(sizeof(int) * nElementi); 

    for(i=0; i < nElementi; i++) { 
        printf("%s%d\n", "Inserire elemento ", i+1); 
        scanf("%d", &elementi[i]); 
    } 

    permutazioni(elementi, nElementi-1, nElementi); 

    #ifdef __WIN32 
      system("PAUSE");
    #endif 
}
