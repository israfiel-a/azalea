#ifndef COMPILER_H
#define COMPILER_H

#define __need_size_t
#include <stddef.h>

#define COMPILER_ENTRY                                                \
    __asm(                                                            \
        ".global _start\n_start:\nmovl (%rsp), %edi\nlea 8(%rsp), "   \
        "%rsi\ncall main\nmovl %eax, %edi\nmovl $60, %eax\nsyscall"); \
    int main(int argc, char **argv)

typedef enum compiler_token_type {
    UNKNOWN_TOKEN,
    IMPORT_TOKEN,
    FUNCTION_TOKEN,
    STRING_TOKEN,
    ASTART_TOKEN,  // attribute
    AEND_TOKEN,
    BSTART_TOKEN,  // block
    BEND_TOKEN,
    SSTART_TOKEN,  // scope
    SEND_TOKEN,
    EOF_TOKEN
} compiler_token_type_t;

typedef struct compiler_token {
    char *token;
    size_t length;
    compiler_token_type_t type;

    size_t line;
    size_t column;
} compiler_token_t;

void compiler_arguments(int argc, char **argv, char **inputNames,
                        size_t *inputCount);

void compiler_getToken(char **contents, compiler_token_t *token);

void compiler_translateIR(char *token, size_t tokenLength, char *error);

#endif  // COMPILER_H
