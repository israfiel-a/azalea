#include <Compiler.h>

static size_t lines = 0;
static size_t column = 0;

static void stripWhitespace(char** contents) {
    while (**contents == '\t' || **contents == '\n' || **contents == ' ')
        (*contents)++;
}

static void removeEmptyTokens(char** contents) {
    while (**contents == ';' || **contents == '\n' || **contents == ' ')
        (*contents)++;
}

void compiler_getToken(char** contents, compiler_token_t* token) {
    stripWhitespace(contents);
    // Remove comments.
    while (**contents == '/' && *(*contents + 1) == '/') {
        while (**contents != '\n' && **contents != 0) (*contents)++;
        (*contents)++;
        stripWhitespace(contents);
    }

    token->token = *contents;
    while (**contents != 0 && **contents != ';' && **contents != '\n' &&
           **contents != ' ' && **contents != '(' && **contents != ')' &&
           **contents != '<' && **contents != '>')
        (*contents)++;
    token->length = *contents - token->token;

    if (**contents == '\n') {
        lines++;
        column = 0;
    } else
        column += token->length;

    token->line = lines;
    token->column = column;

    // Remove trailing delimiter or signal EOF.
    if (**contents != 0) {
        if (**contents != '(' && **contents != ')' && **contents != '<' &&
            **contents != '>')
            (*contents)++;
    } else {
        token->type = EOF_TOKEN;
        return;
    }

    if(*token->token == '"') {
        token->type = STRING_TOKEN;
        token->token++;
        while(**contents != 0 && **contents != '"') (*contents)++;
        token->length = *contents - token->token;
        if(**contents == '"') (*contents)++;
    }
    else if (*token->token == '(') {
        token->type = SSTART_TOKEN;
        (*contents)++;
    } else if (*token->token == ')') {
        token->type = SEND_TOKEN;
        (*contents)++;
        removeEmptyTokens(contents);
    } else if (*token->token == '<') {
        token->type = ASTART_TOKEN;
        (*contents)++;
    } else if (*token->token == '>') {
        token->type = AEND_TOKEN;
        (*contents)++;
        removeEmptyTokens(contents);
    } else if (*token->token == '{') {
        token->type = BSTART_TOKEN;
    } else if (*token->token == '}') {
        token->type = BEND_TOKEN;
    } else if (utilities_stringEqualN(token->token, "import", token->length - 1)) {
        token->type = IMPORT_TOKEN;
    } else if (utilities_stringEqualN(token->token, "function",
                                      token->length - 1)) {
        token->type = FUNCTION_TOKEN;
    } else
        token->type = UNKNOWN_TOKEN;
}
