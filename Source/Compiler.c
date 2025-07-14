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

static void functionDeclarationToken(char* declaration, compiler_token_contents_t *contents) {
    declaration += 3;
    contents->function.returnType = declaration;
    while(*declaration != ' ') declaration++;
    *declaration = 0;
    declaration++;

    contents->function.name = declaration;
    while(*declaration != ' ' && *declaration != '(') declaration++;
    
    bool argumentList = false;
    if(*declaration == '(') argumentList = true;
    *declaration = 0;
    declaration++;

    if(!argumentList) {
        contents->function.argumentCount = 0;
        contents->function.argumentString = nullptr;
        contents->function.variadic = false;
    } else {
        contents->function.argumentString = declaration;
        while(*declaration != ')' && (*declaration != '.' && *(declaration + 1) != '.' && *(declaration + 2) != '.')) {
            declaration++;
            contents->function.argumentCount++;
        }
        if(*declaration != ')') {
            contents->function.variadic = true;
            declaration--;
            if(*declaration == ' ') declaration--;
            *declaration = 0;
            while(*declaration != ')') declaration++;
        }
        *declaration = 0;
        declaration++;
    }

    if(*declaration == '-' && *(declaration + 1) == '>') {
        declaration += 2;
        if(*declaration == ' ') declaration++;

        contents->function.defaultReturn = declaration;
        while(*declaration != ';' && *declaration != 0) declaration++;
        *declaration = 0;
        declaration++;
    }
}

static void moveCursorUntilEOS(char **cursor, bool *eol, bool *eos) {
    **cursor = ' ';
    while(**cursor != 0 && **cursor != ';') (*cursor)++;
    
    if(**cursor == 0) *eol = true;
    else if (**cursor == ';') *eos = true;
    **cursor = 0;
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
        }
        else if (token[0] == '{') type = BLOCK_START_TOKEN;
        else if (token[0] == '}') type = BLOCK_END_TOKEN;
        else if(utilities_stringEqual(token, "import")) {
            type = IMPORT_TOKEN;
            if(eos)
                contents.import.interface = nullptr;
            else {
                moveCursorUntilEOS(&cursor, &eol, &eos);
                importToken(token, &contents);
            }
        } 
        else if(utilities_stringEqual(token, "fn")) {
            type = FUNCTION_DECLARATION_TOKEN;
            if(eos) {
                contents.function.name = nullptr;
            } else {
                moveCursorUntilEOS(&cursor, &eol, &eos);
                functionDeclarationToken(token, &contents);
            }
        }
        
        tokens[*tokenCount] = (compiler_token_t){
            .type = type, .contents = contents 
        };
        (*tokenCount)++;

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
}
