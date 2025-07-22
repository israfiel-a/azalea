#ifndef AZ_COMPILER_TYPES_TOKENS_H
#define AZ_COMPILER_TYPES_TOKENS_H

#define __need_size_t
#include <stddef.h>

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

#endif // AZ_COMPILER_TYPES_TOKENS_H
