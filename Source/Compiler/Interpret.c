#include <Compiler.h>
#include <Utilities/Numbers.h>
#include <Utilities/Output.h>

struct variable_definition {
    char* name;
    size_t nameLength;
    size_t typeID;
};

struct variable_definition_tree {
    struct variable_definition_node {
        struct variable_definition value;
        struct variable_definition_node* next;
    } first;
    struct variable_definition_node* last;
};

struct function_definition {
    enum {
        EMPTY_FUNCTION,    // no function decided
        INVALID_FUNCTION,  // no implementation given
        BEGAN_FUNCTION,    // began implementation
        FINISHED_FUNCTION  // implementation given
    } state;
    size_t nameLength;
    char* name;
    size_t scopeDepth;
    size_t blockDepth;
    struct variable_definition_tree arguments;
};

struct function_definition_tree {
    struct function_definition_node {
        struct function_definition value;
        struct function_definition_node* next;
    } first;
    struct function_definition_node* last;
};

bool compiler_interpret(char** contents) {
    struct function_definition_tree functions = {0};
    functions.last = &functions.first;
    functions.last->value.arguments.last =
        &functions.last->value.arguments.first;

    size_t blockDepth = 0;
    size_t scopeDepth = 0;

    compiler_token_t token = {0};
    while (token.type != EOF_TOKEN) {
        compiler_getToken(contents, &token);

        switch (token.type) {
            case SSTART_TOKEN:
                scopeDepth++;
                break;
            case SEND_TOKEN:
                scopeDepth--;
                break;
            case BSTART_TOKEN:
                blockDepth++;
                if (functions.last->value.state == INVALID_FUNCTION)
                    functions.last->value.state = BEGAN_FUNCTION;
                break;
            case BEND_TOKEN:
                blockDepth--;
                if (blockDepth == functions.last->value.blockDepth &&
                    functions.last->value.state == BEGAN_FUNCTION)
                    functions.last->value.state = FINISHED_FUNCTION;
                break;
            case FUNCTION_TOKEN:
                if (functions.last->value.state != EMPTY_FUNCTION &&
                    functions.last->value.state != FINISHED_FUNCTION) {
                    output_string(
                        "Tried to create a function within a function.", 45, true);
                    return false;
                }

                if (functions.last->value.state != EMPTY_FUNCTION) {
                    struct function_definition_node definition = {0};
                    definition.value.arguments.last =
                        &definition.value.arguments.first;
                    definition.value.arguments.last->value.typeID = (size_t)-1;

                    functions.last->next = &definition;
                    functions.last = functions.last->next;
                }

                functions.last->value.state = INVALID_FUNCTION;
                functions.last->value.scopeDepth = scopeDepth;
                functions.last->value.blockDepth = blockDepth;
                break;
            case UNKNOWN_TOKEN:
                if (functions.last->value.state == INVALID_FUNCTION &&
                    functions.last->value.name == nullptr) {
                    functions.last->value.name = token.token;
                    functions.last->value.nameLength = token.length;
                } else if (functions.last->value.state == INVALID_FUNCTION &&
                           scopeDepth == functions.last->value.scopeDepth + 1) {
                    if (functions.last->value.arguments.last->value.typeID ==
                        (size_t)-1)
                        numbers_fromString(token.token, token.length,
                                           &functions.last->value.arguments
                                                .last->value.typeID);
                    else {
                        functions.last->value.arguments.last->value.name =
                            token.token;
                        functions.last->value.arguments.last->value.nameLength =
                            token.length;

                        struct variable_definition_node definition = {0};
                        functions.last->value.arguments.last->next =
                            &definition;
                        functions.last->value.arguments.last =
                            functions.last->value.arguments.last->next;
                    }
                }
            default:
                break;
        }
    }
    return true;
}

