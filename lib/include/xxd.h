/*
xxd.h - заголовочный файл модуля xxd.c.

Хаиров Егор Вадимович
МК-101
*/

#pragma once
#include <stdio.h>

// Вывод ошибки
#define printError(text); \
printf("%s\n", text); \
exit(1);

// Вывод файла в хексе
void xxd(size_t offset, size_t readLen, size_t biteLen, size_t biteAmount, unsigned char *filePath, unsigned char *format);