#include "misc.h"
#include <stdio.h>
#define MAX (((x) > (y)) ? (x) : (y))
#define MIN (((x) < (y)) ? (x) : (y))
/*
Hace un shuffle del arreglo de enteros pasado usando el Algoritmo de Fisher-Yates
	@param int* arr : arreglo a mezclar
	@return int* : arreglo mezclado
*/

void shuffle(int* arr, int size) {

	srand(time(NULL)); // set seed del generador
	int i,r,tmp;
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