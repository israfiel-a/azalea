#ifndef AZ_COMPILER_TYPES_AST_H
#define AZ_COMPILER_TYPES_AST_H

#define __need_size_t
#include <stddef.h>

typedef enum compiler_ast_operation {
    UNKNOWN_OPERATION,
    FILE_ATTRIBUTE_OPERATION,
    IMPORT_OPERATION
} compiler_ast_operation_t;

typedef enum compiler_ast_file_attribute_type {
    UNKNOWN_FILE_ATTRIBUTE,
    STATICSTD_FILE_ATTRIBUTE,
    NOIMPLEMENTATION_FILE_ATTRIBUTE,
    INTERFACE_FILE_ATTRIBUTE
} compiler_ast_file_attribute_type_t;

typedef struct compiler_ast_node {
    compiler_ast_operation_t operation;
    union {
        struct {
            char* raw;
        } unknown;
        struct {
            compiler_ast_file_attribute_type_t attribute;
            union {
                struct {
                    char* name;
                } interface;
            } arguments;
        } fileAttribute;
        struct {
            char* interface;
            char* alias;
            size_t interfaceLength;
            size_t aliasLength;
        } import;
    } contents;

    struct compiler_ast_node *up, *left, *right;
} compiler_ast_node_t;

#endif  // AZ_COMPILER_TYPES_AST_H
