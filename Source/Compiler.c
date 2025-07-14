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

static void importToken(char *declaration, compiler_token_contents_t *contents) {
    if(utilities_stringEqualUntil(declaration+7, "cascading ", ' ')) contents->import.cascading = true;
    else contents->import.cascading = false;

    contents->import.interface = declaration + 7;
    if(contents->import.cascading) contents->import.interface = declaration + 17;

    size_t lastSpace = utilities_stringFindCharacter(declaration, ' ', false);
    if(*(declaration + lastSpace - 2) != 'a' && *(declaration + lastSpace - 1) != 's') return;
    *(declaration + lastSpace - 3) = 0;
    contents->import.alias = declaration + lastSpace + 1;
}

static void tokenizeLine(char *line, compiler_token_t *tokens, size_t *tokenCount) {
    char* cursor = line;

    while(*cursor != 0 && !(*cursor == '/' && *(cursor + 1) == '/')) {
        char* token = cursor;
        while(*cursor != 0 && *cursor != ' ' && *cursor != ';') cursor++;
        
        bool eol = false, eos = false;
        if(*cursor == 0) eol = true;
        else if (*cursor == ';') eos = true;
        *cursor = 0;

        utilities_outputString(token, true);
        utilities_outputString(" ", true);

        compiler_token_type_t type = UNKNOWN_TOKEN; 
        compiler_token_contents_t contents = {.unknown = {.raw = token}};
        if(token[0] == '<') {
            type = ATTRIBUTE_TOKEN;
            attributeToken(token, &contents);
        } else if(utilities_stringEqual(token, "import")) {
            type = IMPORT_TOKEN;
            if(eos) {
                contents.import.cascading = false;
                contents.import.interface = nullptr;
                contents.import.alias = nullptr;
            } else {
                // go until semicolon or eol
                *cursor = ' ';
                while(*cursor != 0 && *cursor != ';') cursor++;
                
                if(*cursor == 0) eol = true;
                else if (*cursor == ';') eos = true;
                *cursor = 0;

                importToken(token, &contents);
                
            }
        }
        
        tokens[*tokenCount] = (compiler_token_t){
            .type = type, .contents = contents 
        };

        if(!eol) cursor++;
    }
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
