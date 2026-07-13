/*
formatPrint.c - модуль для вывода форматной строки.

Хаиров Егор Вадимович
МК-101
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "formatPrint.h"

// Вывод форматной строки
void formatPrint(unsigned char* format, unsigned char** bites, size_t offset, size_t idx, size_t biteLen) {

    size_t formatLen = strlen(format);
    for(size_t i = 0; i < formatLen; ++i) {

        if(format[i] == '\\') {

            ++i;
            putchar('\\');
            switch(format[i]) {

            case '\\':
                putchar('\\');
                break;

            case 'n':
                putchar('\n');
                break;

            case 'r':
                putchar('\r');
                break;

            case 't':
                putchar('\t');
                break;
            
            default:
                printf("Wrong format input!\n");
                exit(1);
                break;
            }
        }

        else if(format[i] == '%') {

            ++i;
            if(format[i] == 'i') printf("%llu", idx);
            else if(format[i] == 'n') printf("%08X", offset);
            else if(format[i] >= 0x30 && format[i] <= 0x39) {

                size_t tmp = i, bite;
                while(format[i] >= 0x30 && format[i] <= 0x39) ++i;

                switch(format[i]) {

                case 'x':
                    sscanf(format, "%llux", &bite);
                    printBite(bite);
                    break;

                case 'c':
                    sscanf(format, "%lluc", &bite);
                    for(size_t id = 0; id < biteLen; ++id)
                        printf("%c", bites[bite][id] < 0x20 || bites[bite][id] >= 0x80 ? '.' : bites[bite][id]);
                    break;
                
                default:
                    printf("Wrong format input!\n");
                    exit(1);
                    break;
                }

            }
            else {
                printf("Wrong format input!\n");
                exit(1);
            }

        }

        else putchar(format[i]);

    }

}