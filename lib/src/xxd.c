/*
xxd.c - главный модуль библиотеки.

Хаиров Егор Вадимович
МК-101
*/

#include <stdlib.h>
#include <stdbool.h>
#include "xxd.h"

// Вывести байт в хексе
#define printByte(); \
unsigned char first = buf[bufInd] >> 4, second = buf[bufInd] & 0x0F; \
printf("%c%c", first + (first < 10 ? 0x30 : 0x37), second + (second < 10 ? 0x30 : 0x37));

// Флаг окончаия чтения
static bool doneReading = false;

// Вывод файла в хексе
void xxd(size_t offset, size_t readLen, size_t biteLen, size_t biteAmount, unsigned char *filePath, unsigned char *format) {

    // Открытие файла
    FILE* input = fopen(filePath, "r");
    if(!input) {
        printError("Can't open file!");
    }
    fseeko64(input, offset, SEEK_SET);
    if(readLen) readLen += offset;

    // Инициализация буфера
    size_t totalByte, bufInd, byteRead;
    unsigned char* buf = malloc(biteLen);
    if(!buf) {
        printError("Can't allocate memory for buffer!");
    }

    printf("%s:\n", filePath);

    // Вывод содержимого
    while(1) {

        // Вывод отступа
        printf("%08X  ", offset);

        // Счетчик прочитанных байт
        totalByte = 0;

        // Вывод кусков байт
        for(size_t bite = 1; bite <= biteAmount; ++bite) {

            // Прочитано нужное кол-во байт, заполнение пробелами
            if(doneReading) {
                for(size_t i = 0; i < biteLen; ++i) printf("  ");
                putchar(' ');
            }

            else {

                // Чтение в буфер
                byteRead = fread(buf, 1, biteLen, input);

                // Прочитано нужное кол-во байт, обрезаем прочитанное
                if(readLen && (ftello64(input) > readLen)) {
                    doneReading = true;
                    byteRead -= ftello64(input) - readLen;
                    for(bufInd = byteRead; bufInd < biteLen; ++bufInd) buf[bufInd] = 0;
                }

                // Файл прочитан до конца
                else if(feof(input)) {
                    doneReading = true;
                    for(size_t i = 0; i < biteLen; ++i) printf("  ");
                    putchar(' ');
                    continue;
                }

                // Вывод куска байт
                for(bufInd = biteLen - 1; bufInd < biteLen; --bufInd) {
                    printByte();
                }

                // Подсчёт прочитанных байт
                totalByte += byteRead;

                // Разделитель кусков
                putchar(' ');

            }
            
            if(ftello64(input) == readLen) doneReading = true;

        }

        // Вывод строки если -g 1
        if(biteLen == 1) {

            printf("| ");
            fseeko64(input, offset, SEEK_SET);
            unsigned char tmp;
            for(size_t i = 0; i < totalByte; ++i) {
                fread(&tmp, 1, 1, input);
                printf("%c", tmp < 0x20 || tmp >= 0x80 ? '.' : tmp);
            }

        }

        // Подсчёт отступа
        offset += totalByte;

        putchar('\n');
        if(feof(input)) doneReading = true;
        if(doneReading) break;

    }

    printf("--------\n\n");

}