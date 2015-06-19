#include "misc.h"

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

/*
Determina si el archivo pasado en forma de dirent es un archivo regular

@param const struct dirent* dent
@return int: 1 si es un archivo regular | 0 si no
*/
int isFile(const struct dirent *dent) {
	struct stat st;
	lstat(dent->d_name,&st);
	return S_ISREG(st.st_mode);
}

/*
Determina si el archivo pasado en forma de dirent es un directorio distinto de "." y ".."

@param const struct dirent* dent
@return int: 1 si es un archivo de tipo directorio distinto de "." y ".." | 0 si no
*/
int isDir(const struct dirent* dent) {
	struct stat st;
	lstat(dent->d_name,&st);
	return S_ISDIR(st.st_mode) && strcmp(dent->d_name,".") != 0 && strcmp(dent->d_name,"..") != 0; 
}

/* Lee m archivos aleatorios de folder (si hay menos de m, lee todos)
y el string resultante lo guarda en la dirección apuntada por p.

@param char** p: apuntador al string donde se guardará el contenido leido
@param char* folder: ruta de la carpeta
@param int m: número deseado de archivos a leer

@return int: número de archivos leídos

*/

int readFiles(char** p, char* folder, int m) {
	int j, nFiles;
	// Hago cd a ese directorio
	chdir(folder);
	// Obtengo los archivos del directorio
	struct dirent** files;
	nFiles = scandir(".", &files, NULL, NULL);
	
	// Si hay mas archivos que los pedidos, permuto el orden
	if (nFiles > m)
		shuffleDirent(files,nFiles);
	else
	// En caso contrario, leeré todos los archivos
		m = nFiles;
	
	// Obtengo el largo del texto completo y hago stat de cada archivo
	int textSize = 0;
	struct stat* stats = (struct stat*)malloc(sizeof(struct stat)*m);
	for (j = 0; j < m; j++) {
		if (!isFile(files[j]))
			continue;
		stat(files[j]->d_name,&stats[j]);
		textSize += stats[j].st_size;
	}
	// Reservo el espacio necesario para el contenido del texto
	// leido por el hijo
	char* content = (char*)calloc(textSize,sizeof(char));
	int fSize = 0;
	int proc = 0;
	FILE* fp;
	// Leo cada archivo y meto su contenido en content
	for (j = 0; j < m; j++) {
		if (!isFile(files[j]))
			continue;
		// Abro el j-esimo archivo del arreglo shuffleado
		fp = fopen(files[j]->d_name,"r");
		// Leo el archivo y guardo su contenido en la direccion de contenido
		// con un desplazamiento de lo ya leido 
		fSize += fread(content+fSize, 1, stats[j].st_size, fp);
		proc++;
		fclose(fp);
	}
	free(stats);
	for (j = 0; j < nFiles; j++) {
		free(files[j]);
	}
	free(files);
	// Hago que p apunte al texto leido
	*p = content;
	return proc;
}


void readChild(int i, pid_t** pipes, FILE* fout) {
	int exit_code, bytes_pipe;
	// Creo un buffer para leer del pipe
	char buffer[BUFFER_SIZE];
	// Cierro el output
	close(pipes[i][1]);
	// Mientras queden datos en el pipe leo y voy escribiendo en el archivo de salida
	do {
		// Limpio el buffer
		memset(buffer, 0, BUFFER_SIZE);
		// Leo del pipe
		bytes_pipe = read(pipes[i][0], buffer, BUFFER_SIZE);
		// Escribo directamente lo leido en el archivo de salida
		fwrite(buffer, sizeof(char), bytes_pipe ,fout);
		fflush(fout);
	} while (bytes_pipe > 0);
}