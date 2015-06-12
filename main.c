#include "misc.h"
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

int main(int argc, char* argv[]) {
	char* dir;
	char* out;
	int n,m,i,j;
	FILE* fout;
	// Parse parametros
	if (strcmp(argv[1],"-d") == 0) {
		// De una vez hago cd al directorio
		chdir(argv[2]);
		n = atoi(argv[3]);
		m = atoi(argv[3]);
		out = argv[5];
	}
	else {
		n = atoi(argv[1]);
		m = atoi(argv[2]);
		out = argv[3];
	}
	// Creo un arreglo de 10 elementos (por las 10 carpetas)
	int* seqFolders = makeArray(10,1);
	// Mezclo el arreglo
	shuffleInt(seqFolders,10);
	// Inicializo arreglo de pipes
	int** pipes = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++) {
		pipes[i] = (int*)malloc(sizeof(int)*2);
	}
	// Inicializo arreglo de pids de mis hijos y arreglo donde recibiré
	// el número de textos leidos por cada hijo
	pid_t* child_pids = (pid_t*)malloc(sizeof(pid_t)*n);
	int* child_ntexts = (int*)malloc(sizeof(int)*n);

	for (i = 0; i < n; i++) {
		int folder = seqFolders[i];
		pipe(pipes[i]);
		if ((child_pids[i] = fork()) == 0) {
			// Cierro el input del pipe
			close(pipes[i][0]);
			// Convierto el numero de carpeta en string
			char folderName[3];
			sprintf(folderName,"%d",folder);
			// Hago cd a ese directorio
			chdir(folderName);
			// Obtengo los archivos del directorio y filtro para dejar solo los archivos
			struct dirent** list;
			int nFiles = scandir(".", &list, isFile, NULL);
			int textSize = 0;
			
			// Si hay mas archivos que los pedidos, permuto el orden
			if (nFiles > m)
				shuffleDirent(list,nFiles);

			// Si hay menos archivos que m tomo todos, en caso contrario m,
			// o lo que es lo mismo, el minimo entre m y nFiles
			m = MIN(m,nFiles);
			
			// Obtengo el largo del texto completo
			struct stat* stats = (struct stat*)malloc(sizeof(struct stat)*m);
			for (j = 0; j < m; j++) {
				stat(list[j]->d_name,&stats[j]);
				textSize += stats[j].st_size;
			}
			char* content = (char*)malloc(sizeof(content)*textSize);
			memset(content, 0, textSize);
			char* buffer = (char*)malloc(sizeof(content)*textSize);
			int fSize = 0;
			FILE* fp;
			// Leo cada archivo y meto su contenido en content
			for (j = 0; j < m; j++) {
				// Limpio el buffer
				memset(buffer, 0, textSize);
				fp = fopen(list[j]->d_name,"r");
				fSize += fread(buffer, 1, stats[j].st_size, fp);
				if (j == 0)
					strncpy(content,buffer,stats[j].st_size);
				else
					strncat(content,buffer,stats[j].st_size);
				fclose(fp);
			}
			write(pipes[i][1], content, strlen(content)+1);
			exit(m);
		}
		else if (child_pids[i] > 0) {
			// Cierro el output
			close(pipes[i][1]);
			// Creo un buffer para leer del pipe
			int buffer_size = 1024;
			char* buffer = (char*)malloc(sizeof(char)*buffer_size);
			fout = fopen(out,"w");
			int bytes_pipe;
			// Mientras queden datos en el pipe leo y voy escribiendo en el archivo de salida
			do {
				// Limpio el buffer
				memset(buffer, 0, strlen(buffer));
				bytes_pipe = read(pipes[i][0], buffer, buffer_size);
				printf("Bytes read: %d\n",bytes_pipe);
				printf("%s",buffer);
				fwrite(buffer, 1, buffer_size,fout);
			} while (bytes_pipe > 0);
		}
		else {
			// Error
		}
	}
	for (i = 0; i < n; i++) {
		wait(&child_ntexts[i]);
		// WHY 256 ? 
		// printf("Child: %d\n",child_ntexts[i] % 256);
	}
	fwrite("\0",1,1,fout);
	fclose(fout);
	return 0;
}