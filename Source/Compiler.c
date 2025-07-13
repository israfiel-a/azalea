#include <Compiler.h>

void compiler_arguments(int argc, char **argv, char **inputNames, size_t *inputCount)
{
    for(size_t i = 0; i < (size_t)argc; i++) {
        inputNames[*inputCount] = argv[i];
        (*inputCount)++;
    }
}

static void attributeToken(char* token, compiler_token_contents_t *contents) {
    if(utilities_stringEqualUntil(token, "<program:", ':')) contents->attribute.program = true;
    else contents->attribute.program = false;

    char* attributeName = token+1;
    if(contents->attribute.program) attributeName = token + 9;

    contents->attribute.type = UNKNOWN_ATTRIBUTE;
    if(utilities_stringEqual(attributeName, "staticstd>")) contents->attribute.type = STATIC_STD_ATTRIBUTE;
    else if(utilities_stringEqualUntil(attributeName, "entrypoint(", '(')) {
        contents->attribute.type = ENTRYPOINT_ATTRIBUTE;
        contents->attribute.args.entrypoint= UNKNOWN_ENTRYPOINT;
        if(utilities_stringEqual(attributeName+11, "bare)>")) contents->attribute.args.entrypoint= BARE_ENTRYPOINT;
        else if(utilities_stringEqual(attributeName+11, "wrapped)>")) contents->attribute.args.entrypoint= WRAPPED_ENTRYPOINT;
    }
}

static void tokenizeLine(char *line, compiler_token_t *tokens, size_t *tokenCount) {
    char* cursor = line;

    while(*cursor != 0 && !(*cursor == '/' && *(cursor + 1) == '/')) {
        char* token = cursor;
        while(*cursor != 0 && *cursor != ' ' && *cursor != ';') cursor++;
        
        bool eol = false;
        if(*cursor == 0) eol = true;
        *cursor = 0;

        utilities_outputString(token, true);
        utilities_outputString(" ", true);

        compiler_token_type_t type = UNKNOWN_TOKEN; 
        compiler_token_contents_t contents = {.unknown = {.raw = token}};
        if(token[0] == '<') {
            type = ATTRIBUTE_TOKEN;
            attributeToken(token, &contents);
        }
        
        tokens[*tokenCount] = (compiler_token_t){
            .type = type, .contents = contents 
        };

        if(!eol) cursor++;
    }
    
    (void)tokens;
    (void)tokenCount;
}

void compiler_tokenize(char *contents, compiler_token_t *tokens, size_t *tokenCount) 
{
    size_t lineCount = utilities_stringCountCharacters(contents, '\n');
    char *lines[lineCount];
    utilities_stringSplitLines(contents, lines, &lineCount);

    for(size_t i = 0; i < lineCount; i++) {
        tokenizeLine(lines[i], tokens, tokenCount);
    }

    (void)tokenCount;
    (void)tokens;
}
