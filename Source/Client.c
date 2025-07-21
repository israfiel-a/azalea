#include <Compiler.h>

ENTRY(main) {
    const char *startupMessage =
        "Azalea CLI compiler v" VERSION_STRING
        "\nCopyright (c) 2025 Israfil Argos, "
        "GPLv3\n<https://www.gnu.org/licenses/gpl-3.0.txt>\n";
    utilities_outputString(startupMessage, true);

    char cwd[512] = "CWD: ";
    utilities_getCWD(cwd + 5, 507);
    utilities_outputString(cwd, true);
    size_t cwdLength = utilities_stringLength(cwd);

    char *inputNames[argc];
    size_t inputCount = 0;
    compiler_arguments(argc, argv, inputNames, &inputCount);

    for (size_t i = 1; i < inputCount; i++) {
        utilities_outputString("Processing: ", false);
        utilities_outputString(inputNames[i], true);

        size_t pathLength = utilities_stringLength(inputNames[i]);
        char inputPath[cwdLength + pathLength - 2];
        char outputPath[cwdLength + pathLength  - 2];
        for(size_t j = 0; j < cwdLength - 5; j++) { inputPath[j] = cwd[j + 5]; outputPath[j] = cwd[j + 5]; }
        inputPath[cwdLength - 6] = outputPath[cwdLength - 6] = '/';
        for(size_t j = 0; j < pathLength; j++) { inputPath[cwdLength - 5 + j] = inputNames[i][j]; outputPath[cwdLength - 5 + j] = inputNames[i][j];}
        inputPath[cwdLength - 6 + pathLength] = outputPath[cwdLength+pathLength - 6] = '.';
        inputPath[cwdLength - 6 + pathLength + 1] = 'a';
        outputPath[cwdLength - 6 + pathLength + 1] = 'b';
        inputPath[cwdLength - 6 + pathLength + 2] = outputPath[cwdLength+pathLength-6+2]= 'z';
        inputPath[cwdLength - 6 + pathLength + 3] = outputPath[cwdLength+pathLength-6+3] = 0;

        size_t size = utilities_getFileSize(inputPath);
        utilities_outputString("Size: ", false);

        size_t sizeLength = utilities_numberLength(size);
        char sizeString[sizeLength + 1];
        utilities_numberToString(size, sizeLength, sizeString);
        utilities_outputString(sizeString, true);

        char contents[size + 1];
        utilities_loadFile(inputPath, size, contents);

        char *contentsPointer = contents;
        compiler_token_t token = {0};
        while (token.type != EOF_TOKEN) {
            compiler_getToken(&contentsPointer, &token);
            utilities_outputStringN(token.token, token.length);
            utilities_writeFile(outputPath, token.token, token.length);
        }
    }
}
