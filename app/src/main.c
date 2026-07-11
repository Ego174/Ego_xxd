/*
main.c - главный модуль программы. 

Хаиров Егор Вадимович
МК-101
*/

// Ошибка неправильного формата ввода
#define printError(text); \
printf("%s\n", text); \
exit(1);

// Задание аргументов по флагам
#define setNumValue(variable); \
if(cur + 1 < argc) { \
	int len = strlen(argv[cur+1]); \
	for(int i = 0; i < len; ++i) { \
		if(argv[cur+1][i] < 0x30 || argv[cur+1][i] > 0x39) { \
			printError("Аргумент должен быть числом!"); \
		} \
	} \
	sscanf(argv[cur+1], "%llu", &variable); \
} else { \
	printError("Не передан аргумент к флагу!"); \
}

// Проверка повтора флага
#define checkFlag(num); \
if(flags & (1 << num)) { \
	printError("Повторное использование флага!"); \
} else { \
	flags |= (1 << num); \
}

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {

	// Переменные
	size_t offset = 0, readLen = 0, biteLen = 1, biteAmount = 16;
	unsigned char pathInd = 0, dirInd = 0, formatInd = 0, flags = 0;
	// -----------------------------------------------

	// Читаем аргументы
	int cur = 1;
	for(; cur < argc; cur += 2) {

		// Проверка правильности флага
		if(argv[cur][0] != '-' || argv[cur][1] == '\0' || argv[cur][2] != '\0') {
			printError("Не флаг!");
		}

		// Чтение флагов
		switch(argv[cur][1]) {
		case 'i':
			checkFlag(0);
			if(cur + 1 < argc) pathInd = cur + 1;
			else {
				printError("Не передан аргумент к флагу!");
			}
			break;
		case 'o':
			checkFlag(1);
			setNumValue(offset);
			break;
		case 'l':
			checkFlag(2);
			setNumValue(readLen);
			break;
		case 'g':
			checkFlag(3);
			setNumValue(biteLen);
			break;
		case 'n':
			checkFlag(4);
			setNumValue(biteAmount);
			break;
		case 'd':
			checkFlag(5);
			if(cur + 1 < argc) dirInd = cur + 1;
			else {
				printError("Не передан аргумент к флагу!");
			}
			break;
		case 'f':
			checkFlag(6);
			if(cur + 1 < argc) formatInd = cur + 1;
			else {
				printError("Не передан аргумент к флагу!");
			}
			break;
		}
	}

}