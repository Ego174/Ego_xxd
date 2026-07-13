/*
test_app.c - модуль проверки программы. 

Хаиров Егор Вадимович
МК-101
*/

#include "unity.h"
#include "xxd.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void setUp(void) {
}

void tearDown(void) {
}

void test(size_t offset, size_t readLen, size_t biteLen, size_t biteAmount, unsigned char *filePath, unsigned char *format) {
    int stdout_copy = dup(fileno(stdout));
    FILE *f = freopen("output.txt", "w", stdout);
    if (!f) { perror("freopen"); TEST_ASSERT_TRUE(0); exit(0); }
    xxd(offset, readLen, biteLen, biteAmount, filePath, format);
    fflush(stdout);
    fclose(stdout);
    dup2(stdout_copy, fileno(stdout));
    close(stdout_copy);
}

void test1(void) {
    test(0, 0, 1, 16, "file", 0);
    FILE* output = fopen("output.txt", "r");
    unsigned char* buf = malloc(1024);
    fread(buf, 1, 1024, output);
    fclose(output);
    TEST_ASSERT_TRUE(strcmp(buf, "00000000  10 11 12 13 FA FB FC FD 00 20 30 40 41 42 31 32 | ........ 0@AB12\n00000010  33 34                                           | 34") == 0);
    free(buf);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test1);
    // RUN_TEST(test2);
    // RUN_TEST(test3);
    // RUN_TEST(test4);
    // RUN_TEST(test5);
    // RUN_TEST(test6);
    // RUN_TEST(test7);
    // RUN_TEST(test8);
    // RUN_TEST(test9);
    // RUN_TEST(test10);
    return UNITY_END();
}