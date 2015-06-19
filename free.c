#include "free.h"

void freeDirents(struct dirent** d, int n) {
	while(--n) {
		free(d[n]);
	}
	free(d);
}