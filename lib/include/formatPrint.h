/*
formatPrint.h - заголовочный файл модуля formatPrint.c.

Хаиров Егор Вадимович
МК-101
*/

#pragma once
#include <stdio.h>

// Вывести байт в хексе
#define printBite(ind) \
for(size_t i = biteLen - 1; i < biteLen; --i) { \
    unsigned char first = bites[ind][i] >> 4, second = bites[ind][i] & 0x0F; \
    printf("%c%c", first + (first < 10 ? 0x30 : 0x37), second + (second < 10 ? 0x30 : 0x37)); \
}

// Вывод форматной строки
void formatPrint(unsigned char* format, unsigned char** bites, size_t offset, size_t idx, size_t biteLen);