/*
main.c - главный модуль программы. 

Хаиров Егор Вадимович
МК-101
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "getopt.h"
#include "xxd.h"

int main(int argc, char *argv[], char *envp[]) {

	setlocale(LC_ALL, "");
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
				break;

			case 'i':
				filePath = malloc(strlen(optarg));
				strcpy(filePath, optarg);
				break;

			case 'o':
				sscanf(optarg, "%llu", &offset);
				break;

			case 'l':
				sscanf(optarg, "%llu", &readLen);
				break;

			case 'g':
				sscanf(optarg, "%llu", &biteLen);
				break;

			case 'n':
				sscanf(optarg, "%llu", &biteAmount);
				break;

			case 'd':
				dirPath = malloc(strlen(optarg));
				strcpy(dirPath, optarg);
				break;

			case 'f':
				format = malloc(strlen(optarg));
				strcpy(format, optarg);
				break;
		}
	}

	xxd(offset, readLen, biteLen, biteAmount, filePath, dirPath, format);

}