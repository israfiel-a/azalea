#ifndef COMPILER_H
#define COMPILER_H

#include <Utilities.h>

typedef enum compiler_token_type {
    UNKNOWN_TOKEN,
    STRING_TOKEN,
    FUNCTION_TOKEN,
    ID_TOKEN,
    DRETURN_TOKEN,
    BSTART_TOKEN,
    BEND_TOKEN,
    ASTART_TOKEN,
    AEND_TOKEN,
    IMPORT_TOKEN,
    ALIAS_TOKEN
} compiler_token_type_t;

typedef struct compiler_token {
    compiler_token_type_t type;
    char *string;
} compiler_token_t;

void compiler_arguments(int argc, char **argv, char **inputNames,
                        size_t *inputCount);

void compiler_tokenize(char *contents, compiler_token_t *tokens,
                       size_t *tokenCount);

#endif  // COMPILER_H
