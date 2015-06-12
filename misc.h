#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void shuffleInt(int*,int);
void shuffleDirent(struct dirent**,int);
int printArray(int* arr, int size);
int* makeArray(int n, int start);
int isFile(const struct dirent*);

#endif