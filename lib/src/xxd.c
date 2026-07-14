/*
xxd.c - главный модуль библиотеки.

Хаиров Егор Вадимович
МК-101
*/

#include <stdlib.h>
#include <stdbool.h>
#include "xxd.h"
#include "formatPrint.h"

// Флаг окончаия чтения
static bool doneReading;

// Вывод файла в хексе
void xxd(size_t offset, size_t readLen, size_t biteLen, size_t biteAmount, unsigned char *filePath, unsigned char *format) {

    doneReading = false;

    // Открытие файла
    FILE* input = fopen(filePath, "r");
    if(!input) {
        printError("Can't open file!\n");
    }

    // Вывод названия файла
    printf("%s:\n", filePath);

    // Проверка отступа
    fseek(input, 0, SEEK_END);
    if(offset >= ftell(input)) {
        printf("File is too small\n--------\n\n");
        return;
    }
    
    // Установка отступа
    fseek(input, offset, SEEK_SET);
    if(readLen) readLen += offset;

    // Инициализация буфера
    size_t totalByte, bufInd, byteRead, strLen = biteAmount * biteLen, idx = 0;
    unsigned char** bites = malloc(biteAmount * sizeof(unsigned char*));
    if(!bites) {
        printError("Can't allocate memory for bites buffer!\n");
    }
    for(size_t i = 0; i < biteAmount; ++i) {
        bites[i] = malloc(biteLen);
        if(!bites[i]) {
            printError("Can't allocate memory for bite buffer!\n");
        }
    }
    if(!format)
        for(size_t i = 1; i < biteAmount; ++i) free(bites[i]);

    // Вывод содержимого
    while(1) {

        // Стандартный вывод
        if(!format) {

            // Вывод отступа
            printf("%08zX  ", offset);

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
                    byteRead = fread(bites[0], 1, biteLen, input);

                    // Прочитано нужное кол-во байт, обрезаем прочитанное
                    if(readLen && (ftell(input) > readLen)) {
                        doneReading = true;
                        byteRead -= ftell(input) - readLen;
                        for(bufInd = byteRead; bufInd < biteLen; ++bufInd) bites[0][bufInd] = 0;
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
                        printBite(0);
                    }

                    // Подсчёт прочитанных байт
                    totalByte += byteRead;

                    // Разделитель кусков
                    putchar(' ');

                }
                
                if(ftell(input) == readLen) doneReading = true;

            }

            // Вывод строки если -g 1
            if(biteLen == 1) {

                printf("| ");
                fseek(input, offset, SEEK_SET);
                unsigned char tmp;
                for(size_t i = 0; i < totalByte; ++i) {
                    fread(&tmp, 1, 1, input);
                    printf("%c", tmp < 0x20 || tmp >= 0x80 ? '.' : tmp);
                }

            }

            // Подсчёт отступа
            offset += strLen;

            putchar('\n');
            if(feof(input)) doneReading = true;
            if(doneReading) {
                free(bites[0]);
                break;
            }
        }

        // Форматный вывод
        else {

            // Счетчик прочитанных байт
            totalByte = 0;

            // Вывод строки
            for(size_t bite = 0; bite < biteAmount; ++bite) {

                // Прочитано нужное кол-во байт, заполнение пробелами
                if(doneReading) {
                    free(bites[bite]);
                    bites[bite] = 0;
                }

                else {

                    // Чтение в буфер
                    byteRead = fread(bites[bite], 1, biteLen, input);

                    // Прочитано нужное кол-во байт, обрезаем прочитанное
                    if(readLen && (ftell(input) > readLen)) {
                        doneReading = true;
                        byteRead -= ftell(input) - readLen;
                        for(bufInd = byteRead; bufInd < biteLen; ++bufInd) bites[bite][bufInd] = 0;
                    }

                    // Файл прочитан до конца
                    else if(feof(input)) {
                        doneReading = true;
                        free(bites[bite]);
                        bites[bite] = 0;
                        continue;
                    }

                }
                
                if(ftell(input) == readLen) doneReading = true;

            }

            formatPrint(format, bites, offset, idx, biteLen);

            // Подсчёт отступа
            offset += strLen;
            
            if(feof(input)) doneReading = true;
            if(doneReading) break;

        }

        ++idx;

    }

    printf("--------\n\n");

    free(bites);

}