#ifndef AZ_COMPILER_TYPES_AST_H
#define AZ_COMPILER_TYPES_AST_H

#define __need_size_t
#include <stddef.h>

// You do NOT need more than eight parameters dawg :pray:
#define COMPILER_MAX_FUNCTION_PARAMETERS 8
// probably will be an issue later--but 4kb? that sounds like a lot to me :3
#define COMPILER_AST_ARENA_SIZE 4096

typedef enum compiler_ast_operation {
    UNKNOWN_OPERATION,
    FILE_ATTRIBUTE_OPERATION,
    IMPORT_OPERATION,
    FUNCTION_DECLARATION_OPERATION,
    TYPE_DECLARATION_OPERATION
} compiler_ast_operation_t;

typedef enum compiler_ast_file_attribute_type {
    UNKNOWN_FILE_ATTRIBUTE,
    STATICSTD_FILE_ATTRIBUTE,
    BUILDFILE_FILE_ATTRIBUTE,
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
        struct {
            char* name;
            size_t nameLength;
            size_t parameterCount;
            struct {
                struct compiler_ast_node *type;
                char* name;
            } parameters[COMPILER_MAX_FUNCTION_PARAMETERS];
        } functionDeclaration;
        struct {
            char* name;
            size_t nameLength;
            size_t resolved;
        } typeDeclaration;
    } contents;

    struct compiler_ast_node *up, *left, *right;
} compiler_ast_node_t;

#endif  // AZ_COMPILER_TYPES_AST_H
