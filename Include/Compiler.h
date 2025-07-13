#ifndef COMPILER_H
#define COMPILER_H

#include <Utilities.h>

struct compiler_ast_node_inner {
    enum compiler_ast_node_type {
        UNKNOWN_TOKEN,
        VARIABLE_TYPE,
        RETURN_TYPE,
        FUNCTION_DECLARATION,
        ATTRIBUTE_DECLARATION,
        BLOCK_START,
        BLOCK_END,
        VARIABLE_VALUE,
        STRUCTURE_DECLARATION,
        ENUMERATOR_DECLARATION,
        TYPE_DECLARATION,
        STRING_BEGIN,
        STRING_END
    } type;
    size_t position;
    void *contents;
};

struct compiler_ast_node {
    struct compiler_ast_node_inner inner;
    struct compiler_ast_node_inner* next;
};

typedef struct compiler_ast {
    struct compiler_ast_node *treeStart;
    struct compiler_ast_node *unknownStart; 
} compiler_ast_t;

bool compiler_digestArguments(int argc, const char *const *const argv, size_t* inputCount, char** inputNames);

bool compiler_validateInput(compiler_ast_t *contents);

void compiler_constructAST(const char* const *const contents, size_t lines, compiler_ast_t *storage);

// contents is changed, see utilities/stringsplitlines
bool compiler_compileFile(const char* const name, char *contents);

#endif // COMPILER_H
