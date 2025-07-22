#ifndef AZ_COMPILER_TYPES_ARGUMENTS_H
#define AZ_COMPILER_TYPES_ARGUMENTS_H

typedef enum compiler_backend { NASM_BACKEND } compiler_backend_t;
#define COMPILER_BACKEND_LIST "NASM"

typedef struct compiler_arguments {
    struct {
        bool interpreted : 1;
    } flags;
    compiler_backend_t backend;
    char *target;
} compiler_arguments_t;

#endif // AZ_COMPILER_TYPES_ARGUMENTS_H
