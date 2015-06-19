#ifndef MISC_H
#define MISC_H

#define BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void shuffleInt(int*,int);
void shuffleDirent(struct dirent**,int);
void printArray(int* arr, int size);
int* makeArray(int n, int start);
int isFile(const struct dirent*);
int isDir(const struct dirent*);
#endif