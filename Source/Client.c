#include <Compiler.h>

ENTRY(main)
{
    const char* startupMessage = "Azalea CLI compiler v" VERSION_STRING "\nCopyright (c) 2025 Israfil Argos, GPLv3\n<https://www.gnu.org/licenses/gpl-3.0.txt>\n";
    utilities_outputString(startupMessage, true);

    char cwd[512] = "CWD: ";
    utilities_getCWD(cwd + 5, 507);
    utilities_outputString(cwd, true);

    char *inputNames[argc];
    size_t inputCount = 0;
    compiler_arguments(argc, argv, inputNames, &inputCount);

    for(size_t i = 1; i < inputCount; i++) {
        utilities_outputString("Processing: ", false);
        utilities_outputString(inputNames[i], true);

        size_t size = utilities_getFileSize(inputNames[i]);
        utilities_outputString("Size: ", false);

        size_t sizeLength = utilities_numberLength(size);
        char sizeString[sizeLength + 1];
        utilities_numberToString(size, sizeLength, sizeString);
        utilities_outputString(sizeString, true);

        char contents[size + 1];
        utilities_loadFile(inputNames[i], size, contents);
        
        size_t tokenCount = 0;
        compiler_token_t tokens[4096]; // TODO: This is abyssmal.
        compiler_tokenize(contents, tokens, &tokenCount);

        for(size_t i = 0; i < tokenCount; i++) {
            size_t length = utilities_numberLength(tokens[i].type);
            char num[length];
            utilities_numberToString(tokens[i].type, length, num);
            utilities_outputString(num, true);
        }
    }

    return 0;
}
