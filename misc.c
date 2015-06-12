#include "misc.h"
#include <stdio.h>


/*
Hace un shuffle del arreglo de enteros pasado usando el Algoritmo de Fisher-Yates
	@param int* arr : arreglo a mezclar
	@return int* : arreglo mezclado
*/

void shuffleInt(int* arr, int size) {

	srand(time(NULL)); // set seed del generador
	int i,r;
	int tmp;
	for (i = 0; i < size-1; i++) {
		r = rand() % (size-i);
		tmp = arr[r];
		arr[r] = arr[size-i-1];
		arr[size-i-1] = tmp;
	}
}

/*
Hace un shuffle del arreglo de dirent* pasado usando el Algoritmo de Fisher-Yates
	@param const struct dirent** arr : arreglo a mezclar
	@return int* : arreglo mezclado
*/

void shuffleDirent(struct dirent** arr, int size) {

	srand(time(NULL)); // set seed del generador
	int i,r;
	struct dirent* tmp;
	for (i = 0; i < size-1; i++) {
		r = rand() % (size-i);
		tmp = arr[r];
		arr[r] = arr[size-i-1];
		arr[size-i-1] = tmp;
	}
}


int printArray(int* arr, int size) {
	int i;
	for (i=0; i < size; i++) {
		printf("%d ",arr[i]);
	}
	printf("\n");
}

int* makeArray(int n, int start) {
	int* arr = (int*)malloc(sizeof(int)*n);
	int i = start;
	for (; i < start+n; i++) {
		arr[i-start] = i; 
	}
	return arr;
}

int isFile(const struct dirent *dent) {
	struct stat st;
	stat(dent->d_name,&st);
	return S_ISREG(st.st_mode);
}