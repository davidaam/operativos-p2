#include "misc.h"

int main(int argc, char* argv[]) {
	char* dir;
	char* out;
	int n,m,i,j;
	char* wDir = ".";
	// Parseo parametros
	if (strcmp(argv[1],"-d") == 0) {
		wDir = argv[2];
		n = atoi(argv[3]);
		m = atoi(argv[3]);
		out = argv[5];
	}
	else {
		n = atoi(argv[1]);
		m = atoi(argv[2]);
		out = argv[3];
	}
	// Abro el archivo de salida
	FILE* fout = fopen(out,"w");
	// Hago cd al directorio
	chdir(wDir);
	// Obtengo los archivos del directorio actual y filtro para dejar solo los directorios
	struct dirent** dirs;
	int nDirs = scandir(".", &dirs, isDir, NULL);
	// Si hay mas directorios que los pedidos, permuto
	if (nDirs > n)
		shuffleDirent(dirs,nDirs);

	// Inicializo arreglo de pipes
	int** pipes = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++) {
		pipes[i] = (int*)malloc(sizeof(int)*2);
	}
	// Inicializo arreglo de pids de mis hijos y arreglo donde recibiré
	// el número de textos leidos por cada hijo
	pid_t* child_pids = (pid_t*)malloc(sizeof(pid_t)*n);
	int asignado = 0;
	char* folder;
	
	for (i = 0; i < n; i++) {	
		// Si ya se crearon suficientes hijos para cubrir todos los directorios,
		// Los restantes no tendrán carpeta asignada
		if (i >= nDirs) {
			asignado = 0;
		}
		
		else {
			// Le asigno al hijo i la i-esima carpeta del arreglo shuffleado
			folder = dirs[i]->d_name;
			asignado = 1;
		}
		
		// Creo el pipe del i-esimo hijo
		pipe(pipes[i]);
		
		// Creo el i-esimo proceso hijo
		if ((child_pids[i] = fork()) == 0) {
			pid_t my_pid = getpid();
			// Cierro el input del pipe
			close(pipes[i][0]);

			if (!asignado) {
				printf("Hijo %ld: no tengo asignada ninguna carpeta\n",my_pid);
				exit(0);
			}
			printf("Hijo %ld | Directorio asignado: %s\n",my_pid,folder);

			char* content;
			// Leo m archivos (de ser posible, si no todos) de folder y guardo
			// el contenido en content
			m = readFiles(&content, folder, m);
			
			// Mando el contenido por el pipe
			write(pipes[i][1], content, strlen(content));

			// Libero content luego de mandarlo por el pipe
			free(content);
			// El hijo hereda variables del padre, las libero en cada hijo
			for (i = 0; i < n; i++) {
				free(pipes[i]);
			}
			free(pipes);
			for (i = 0; i < nDirs; i++) {
				free(dirs[i]);
			}
			free(dirs);
			free(child_pids);
			fclose(fout);
			// Termino y mando por el exit el número de archivos procesados
			exit(m);
		}
		else if (child_pids[i] < 0) {
			// Error haciendo fork (child_pid < 0)
			perror("Error en fork");
			exit(1);
		}
	}
	for (i = 0; i < n; i++) {
		readChild(i,pipes,fout);	
		int exit_code;
		pid_t pid_child = wait(&exit_code);
		// El exit code esta en los 8 bits más significativos, hago shift a la derecha
		exit_code = exit_code >> 8;
		printf("Hijo %ld aportó %d textos\n",pid_child,exit_code);
	}

	// Cierro el archivo
	fclose(fout);
	// Por cada malloc hago free
	for (i = 0; i < n; i++) {
		free(pipes[i]);
	}
	free(pipes);
	for (i = 0; i < nDirs; i++) {
		free(dirs[i]);
	}
	free(dirs);
	free(child_pids);
	return 0;
}