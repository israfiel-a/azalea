#include <Compiler.h>
#include <Utilities/Output.h>  // TEMP!
#include <Utilities/Strings.h>

static compiler_ast_node_t astArena[COMPILER_AST_ARENA_SIZE] = {0};

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
    if (strings_compareN(attribute, "staticstd", length - 1))
        return STATICSTD_FILE_ATTRIBUTE;
    else if (strings_compareN(attribute, "noimplementation", length - 1))
        return NOIMPLEMENTATION_FILE_ATTRIBUTE;
    else if (strings_compareN(attribute, "interface", length - 1))
        return INTERFACE_FILE_ATTRIBUTE;

    output_string("Found unknown attribute '", 25, false);
    output_string(attribute, length, false);
    output_string("'.", 2, true);
    return UNKNOWN_FILE_ATTRIBUTE;
}

void compiler_generateAST(const char *const contents,
                          compiler_ast_node_t **head) {
    *head = getNewNode();
    compiler_ast_node_t *cursor = *head;

    char *contentsPointer = (char *)contents;
    compiler_token_t token = {0};
    compiler_getToken(&contentsPointer, &token);

    while (token.type != EOF_TOKEN) {
        switch (token.type) {
            case EOS_TOKEN: [[fallthrough]]; case AEND_TOKEN: advanceTree(&cursor); break;
            case IMPORT_TOKEN:
                cursor->operation = IMPORT_OPERATION;
            break;
            case ASTART_TOKEN:
                cursor->operation = FILE_ATTRIBUTE_OPERATION;
                break;
            case ALIAS_TOKEN:
                if (cursor->operation != IMPORT_OPERATION) {
                    output_string("Found disconnected alias '", 26, false);
                    output_string(token.token, token.length, false);
                    output_string("'.", 2, true);
                    break;
                }
                cursor->contents.import.alias = token.token;
                cursor->contents.import.aliasLength = token.length;
                break;
            case UNKNOWN_TOKEN:
                switch (cursor->operation) {
                    case FILE_ATTRIBUTE_OPERATION:
                        cursor->contents.fileAttribute.attribute =
                            getFileAttribute(token.token, token.length);
                        break;
                    case IMPORT_OPERATION:
                        cursor->contents.import.interface = token.token;
                        cursor->contents.import.interfaceLength = token.length;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;  // TEMP!!!!!
        }

        compiler_getToken(&contentsPointer, &token);
    }
}
