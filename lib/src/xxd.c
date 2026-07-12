/*
xxd.c - главный модуль библиотеки.

Хаиров Егор Вадимович
МК-101
*/

#include <stdlib.h>
#include "xxd.h"

// Вывести байт в хексе
#define printByte(); \
unsigned char first = buf[bufInd] >> 4, second = buf[bufInd] & 0x0F; \
printf("%c%c", first + (first < 10 ? 0x30 : 0x37), second + (second < 10 ? 0x30 : 0x37));

// Вывод файла в хексе
void xxd(size_t offset, size_t readLen, size_t biteLen, size_t biteAmount, unsigned char *filePath, unsigned char *dirPath, unsigned char *format) {

    if(readLen) readLen += offset;

    // Открытие файла
    FILE* input = fopen(filePath, "r");
    if(!input) {
        printError("Can't open file!");
    }
    fseeko64(input, offset, SEEK_SET);

    // Инициализация буфера
    size_t bufSize = biteLen * biteAmount, totalByte = 0, bufInd, byteRead;
    unsigned char* buf = malloc(bufSize);
    if(!buf) {
        printError("Can't allocate memory for buffer!");
    }

    // Вывод содержимого
    while((byteRead = fread(buf, 1, bufSize, input)) > 0) {
        printf("%08X  ", offset);
        if(readLen && ftello64(input) >= readLen) byteRead -= ftello64(input) - readLen;
        for(bufInd = 0;  bufInd < bufSize; ++bufInd) {
            if(bufInd < byteRead) {
                printByte();
            } else printf("  ");
            if((bufInd + 1) % biteLen == 0) putchar(' ');
        }
        if(biteLen == 1) {
            printf("| ");
            for(size_t i = 0; i < byteRead; ++i) putchar(buf[i] < 0x20 || buf[i] >= 0x7F ? '.' : buf[i]);
        }
        putchar('\n');
        if(byteRead != bufSize) break;
        offset += byteRead;
    }

}