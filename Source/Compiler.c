#include <Compiler.h>

bool compiler_digestArguments(int argc, const char *const *const argv, size_t* inputCount, char** inputNames) {
    for(size_t i = 1; i < (size_t)argc; i++) {
        inputNames[*inputCount] = (char*)argv[i];
        (*inputCount)++;
    }

    return true;
}

bool compiler_validateInput(const char *const *const contents) {
    (void)contents;
    return true;
}

void compiler_constructAST(const char* const *const contents, size_t lines, compiler_ast_t *storage) {
    for(size_t i = 0; i < lines; i++) {
        char* currentLetter = (char*)contents[i];
        char* currentTokenBegin = currentLetter;
        while(*currentLetter != 0 && *currentLetter != ' ') {
            if(*currentLetter == '/' && *(currentLetter + 1) == '/') { currentLetter--; break; }
            *currentLetter++;
        }

        
    }
}

bool compiler_compileFile(const char *const name, char *contents) {
    size_t lineCount = utilities_stringCountCharacters(contents, '\n');
    char *lines[lineCount];
    utilities_stringSplitLines(contents, lines, &lineCount);

    if(!compiler_validateInput((const char**)lines)) return false;

    (void)name;    

    return true;
}

