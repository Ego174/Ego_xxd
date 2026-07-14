/*
main.c - главный модуль программы. 

Хаиров Егор Вадимович
МК-101
*/

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "getopt.h"
#include "xxd.h"

#define printXxd(path) xxd(offset, readLen, biteLen, biteAmount, (path), format);

// Построение пути к файлу file в директории dir
unsigned char* buildFullPath(unsigned char* dir, unsigned char* file) {
	unsigned char* fullPath = malloc(strlen(dir) + strlen(file) + 2);
	if(!fullPath) {
		printError("Не удалось выделить буфер под полный путь!\n");
	}
	strcpy(fullPath, dir);
	#ifdef _WIN32
	strcat(fullPath, "\\");
	#else
	strcat(fullPath, "/");
	#endif
	unsigned char ind = strlen(fullPath);
	snprintf(fullPath + ind, 256 - ind, "%s", file);
	return fullPath;
}

int main(int argc, char *argv[], char *envp[]) {

	setbuf(stdout, NULL);

	// Переменные
	size_t offset = 0, readLen = 0, biteLen = 1, biteAmount = 16;
	unsigned char *filePath = 0, *dirPath = 0, *format = 0;
	// -----------------------------------------------

	// Чтение флагов
	int c;
	while ((c = getopt(argc, argv, "hi:o:l:g:n:d:f:")) != -1) {
		switch (c) {
			case 'h':
				printf("Usage: %s -i -o -l -g -n -d -f\n", argv[0]);
				exit(0);
				break;

			case 'i':
				filePath = malloc(strlen(optarg) + 1);
				strcpy(filePath, optarg);
				break;

			case 'o':
				sscanf(optarg, "%llu", &offset);
				break;

			case 'l':
				sscanf(optarg, "%llu", &readLen);
				if(readLen == 0) exit(0);
				break;

			case 'g':
				sscanf(optarg, "%llu", &biteLen);
				if(biteLen != 1 && biteLen != 2 && biteLen != 4) {
					printError("Wrong bite size!\n");
				}
				break;

			case 'n':
				sscanf(optarg, "%llu", &biteAmount);
				break;

			case 'd':
				dirPath = malloc(strlen(optarg) + 1);
				strcpy(dirPath, optarg);
				break;

			case 'f':
				format = malloc(strlen(optarg) + 1);
				strcpy(format, optarg);
				break;
		}
	}

	// Обход директории
	if(dirPath) {

        #ifdef _WIN32
            char searchPath[512];
            snprintf(searchPath, sizeof(searchPath), "%s\\*", dirPath);
            WIN32_FIND_DATAA findData;
            HANDLE hFind = FindFirstFileA(searchPath, &findData);
            if(hFind == INVALID_HANDLE_VALUE) {
                printError(("Can't open directory: %s!\n", dirPath));
            } else {
                while(FindNextFileA(hFind, &findData) != 0) {
                    if(strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
                        continue;

                    unsigned char* fullPath = buildFullPath(dirPath, findData.cFileName);
                    struct stat path_stat;
                    if(stat(fullPath, &path_stat) == 0) {
                        if(S_ISREG(path_stat.st_mode)) {
                            printXxd(fullPath);
                        }
                    }
                    free(fullPath);
                }
                FindClose(hFind);
            }
        #else
            DIR* dir = opendir(dirPath);
            if(!dir) {
                printError(("Can't open directory: %s!\n", dirPath));
            }
            struct dirent* de;
            struct stat path_stat;
            while((de = readdir(dir)) != NULL) {
                unsigned char* fullPath = buildFullPath(dirPath, de->d_name);
                stat(fullPath, &path_stat);
                if(S_ISREG(path_stat.st_mode)) {
                    printXxd(fullPath);
                }
                free(fullPath);
            }
            closedir(dir);
        #endif

		free(dirPath);

	}

	// Вывод одного файла
	if(filePath) {
		printXxd(filePath);
		free(filePath);
	}

	if(format) free(format);

}