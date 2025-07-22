#include <Compiler.h>
#include <Utilities/Strings.h>
#include <Utilities/Output.h> // TEMP!

static compiler_ast_node_t astArena[COMPILER_AST_ARENA_SIZE];

static compiler_ast_node_t *getNewNode(void) {
    static size_t arenaOffset = 0;
    size_t currentOffset = arenaOffset;
    arenaOffset++;
    return &astArena[currentOffset];
}

static void advanceTree(compiler_ast_node_t **cursor) {
    if ((*cursor)->left == nullptr) {
        (*cursor)->left = getNewNode();
        (*cursor)->left->up = *cursor;
        (*cursor) = (*cursor)->left;
    } else {
        (*cursor)->right = getNewNode();
        (*cursor)->right->up = *cursor;
        (*cursor) = (*cursor)->right;
    }
}

static compiler_ast_file_attribute_type_t getFileAttribute(
    const char *const attribute, size_t length) {
    if (strings_compareN(attribute, "staticstd", length))
        return STATICSTD_FILE_ATTRIBUTE;
    else if (strings_compareN(attribute, "noimplementation", length))
        return NOIMPLEMENTATION_FILE_ATTRIBUTE;
    else if (strings_compareN(attribute, "interface", length))
        return INTERFACE_FILE_ATTRIBUTE;
    return UNKNOWN_FILE_ATTRIBUTE;
}

void compiler_generateAST(char **contents, compiler_ast_node_t **head) {
    *head = getNewNode();
    compiler_ast_node_t *cursor = *head;

    compiler_token_t token = {0};
    compiler_getToken(contents, &token);

    while (token.type != EOF_TOKEN) {
        switch (token.type) {
            case ASTART_TOKEN:
                cursor->operation = FILE_ATTRIBUTE_OPERATION;
                break;
            case AEND_TOKEN:
                advanceTree(&cursor);
                break;
            case UNKNOWN_TOKEN:
                if (cursor->operation == FILE_ATTRIBUTE_OPERATION)
                    cursor->contents.fileAttribute.attribute =
                        getFileAttribute(token.token, token.length);
                break;
            default:
                break;  // TEMP!!!!!
        }

        compiler_getToken(contents, &token);
    }
}
