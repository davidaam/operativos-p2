#include "misc.h"

int main(int argc, char* argv[]) {
	char* dir;
	char* out;
	int n,m,i;
	// Parse parametros
	if (strcmp(argv[1],"-d") == 0) {
		dir = argv[2];
		n = atoi(argv[3]);
		m = atoi(argv[3]);
		out = argv[5];
	}
	else {
		n = atoi(argv[1]);
		m = atoi(argv[2]);
		out = argv[3];
	}
	int* seqFolders = makeArray(10,1);
	shuffle(seqFolders,10);
	int** pipes = (int**)malloc(sizeof(int*)*n);
	pid_t* child_pids = (pid_t*)malloc(sizeof(pid_t)*n);
	int* child_ntexts = (int*)malloc(sizeof(int)*n);
	for (i = 0; i < n; i++) {
		int folder = seqFolders[i];
		if ((child_pids[i] = fork()) == 0) {
			// Hijo
		}
		else if (child_pids[i] > 0) {
			// Padre
		}
		else {
			// Error
		}
	}
	for (i = 0; i < n; i++) {
		wait(&child_ntexts[i]);
		// WHY 256 ? 
		printf("Child: %d\n",child_ntexts[i]/256);
	}
	return 0;
}