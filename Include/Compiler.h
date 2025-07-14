#ifndef COMPILER_H
#define COMPILER_H

#include <Utilities.h>

typedef enum compiler_token_type {
    UNKNOWN_TOKEN,
    ATTRIBUTE_TOKEN,
    IMPORT_TOKEN,
    FUNCTION_DECLARATION_TOKEN,
    BLOCK_START_TOKEN,
    BLOCK_END_TOKEN,
    STRING_BEGIN_TOKEN,
    STRING_END_TOKEN
} compiler_token_type_t;

typedef union compiler_token_contents {
    struct {
        char* raw;
    } unknown;
    struct {
        // only allowed in entrypoint files
        bool program;
        enum {
            UNKNOWN_ATTRIBUTE,
            // program starts at this function, "bare" means no args, "wrapped" means we get cmd args
            ENTRYPOINT_ATTRIBUTE,
            // statically linked std, only allowed as a program flag
            STATIC_STD_ATTRIBUTE
        } type;
        union {
            enum {
                UNKNOWN_ENTRYPOINT,
                WRAPPED_ENTRYPOINT,
                BARE_ENTRYPOINT 
            } entrypoint;
        } args;
    } attribute;
    struct {
        bool cascading;
        char* interface;
        char* alias;
    } import;
    struct {
        char* returnType;
        char* name;
        size_t argumentCount;
        char* argumentString; // split by comma, maybe " "
        bool variadic;
        char* defaultReturn;
    } function;
} compiler_token_contents_t;

typedef struct compiler_token {
    compiler_token_type_t type;
    compiler_token_contents_t contents;
} compiler_token_t;

void compiler_arguments(int argc, char** argv, char **inputNames, size_t *inputCount);

void compiler_tokenize(char *contents, compiler_token_t *tokens, size_t *tokenCount);

#endif // COMPILER_H
