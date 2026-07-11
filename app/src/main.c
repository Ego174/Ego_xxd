/*
main.c - главный модуль программы. 

Хаиров Егор Вадимович
МК-101
*/

#include <stdio.h>
#include <string.h>
#include "getopt.h"

// Вывод ошибки
#define printError(text); \
printf("%s\n", text); \
exit(1);

int main(int argc, char *argv[], char *envp[]) {

	// Переменные
	size_t offset = 0, readLen = 0, biteLen = 1, biteAmount = 16;
	unsigned char pathInd = 0, dirInd = 0, formatInd = 0;
	// -----------------------------------------------

	// Чтение флагов
	int c;
	while ((c = getopt(argc, argv, "hi:o:l:g:n:d:f:")) != -1) {
		switch (c) {
			case 'h':
				printf("Usage: %s -i -o -l -g -n -d -f\n", argv[0]);
				break;

			case 'i':
				pathInd = optind;
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
				dirInd = optind;
				break;

			case 'f':
				formatInd = optind;
				break;
		}
	}

}