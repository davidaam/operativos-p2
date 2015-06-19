#ifndef FREE_H
#define FREE_H

#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

void freeStats(struct stat*);
void freeDirents(struct dirent**, int);
#endif