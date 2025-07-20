#include <Compiler.h>

static inline bool stripUseless(char **string) {
    while (**string == ' ' || **string == '\t') (*string)++;
    // Filter out empty lines.
    if (**string == '\n') {
        (*string)++;
        return false;
    }
    if (**string == '/' && *(*string + 1) == '/') {
        while (**string != '\n' && **string != 0) (*string)++;
        (*string)++;
        return false;
    }
    return true;
}

static inline void advanceCursor(char **string) {
    while (**string != ' ' && **string != '\n' && **string != 0 &&
           **string != ';')
        (*string)++;
}

void compiler_tokenize(char *contents, compiler_token_t *tokens,
                       size_t *tokenCount) {
    char *cursor = contents;

    while (*cursor != 0) {
        if (!stripUseless(&cursor)) continue;
        char *token = cursor;
        advanceCursor(&cursor);

        compiler_token_type_t type = UNKNOWN_TOKEN;

        switch (*token) {
            case '<':
                type = ASTART_TOKEN;
                cursor = token;
                break;
            case 'i':
                if (!utilities_stringEqualUntil(token, "import ", ' ')) break;
                type = IMPORT_TOKEN;
                break;
            case '-':
                if (*(token + 1) != '>') break;
                type = DRETURN_TOKEN;
                break;
            case 'a':
                if (*(token + 1) != 's') break;
                type = ALIAS_TOKEN;
                break;
            case '{':
                type = BSTART_TOKEN;
                break;
            case '}':
                type = BEND_TOKEN;
                break;
        }

        if (type == UNKNOWN_TOKEN) {
            if (utilities_stringEqualN(token, "function ", cursor - token)) {
                type = FUNCTION_TOKEN;
            } else if (*(cursor - 1) == '>') {
                *(cursor - 1) = 0;
                tokens[*tokenCount] =
                    (compiler_token_t){.type = type, .string = token};
                (*tokenCount)++;
                type = AEND_TOKEN;
            }
        }

        tokens[*tokenCount] = (compiler_token_t){.type = type, .string = token};
        (*tokenCount)++;

        cursor++;
    }
}

