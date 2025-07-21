#include <Compiler.h>
#include <Utilities/Files.h>
#include <Utilities/Numbers.h>
#include <Utilities/Output.h>
#include <Utilities/Strings.h>

COMPILER_ENTRY {
    const char *startupMessage =
        "Azalea CLI compiler v" VERSION_STRING
        "\nCopyright (c) 2025 Israfil Argos, "
        "GPLv3\n<https://www.gnu.org/licenses/gpl-3.0.txt>\n";
    output_string(startupMessage, true);

    output_stringN("CWD: ", 5);
    char cwd[512];
    files_getCWD(512, cwd);
    output_string(cwd, true);
    size_t cwdLength = strings_getLength(cwd);

    char *inputNames[argc];
    size_t inputCount = 0;
    compiler_arguments(argc, argv, inputNames, &inputCount);

    for (size_t i = 1; i < inputCount; i++) {
        size_t pathLength = strings_getLength(inputNames[i]);
        char inputPath[cwdLength + pathLength + 3];
        char outputPath[cwdLength + pathLength + 3];
        for (size_t j = 0; j < cwdLength; j++) {
            inputPath[j] = cwd[j];
            outputPath[j] = cwd[j];
        }
        inputPath[cwdLength - 1] = outputPath[cwdLength - 1] = '/';
        for (size_t j = 0; j < pathLength; j++) {
            inputPath[cwdLength + j] = inputNames[i][j];
            outputPath[cwdLength + j] = inputNames[i][j];
        }
        inputPath[cwdLength + pathLength - 1] =
            outputPath[cwdLength + pathLength - 1] = '.';
        inputPath[cwdLength + pathLength] = 'a';
        outputPath[cwdLength + pathLength] = 'b';
        inputPath[cwdLength + pathLength + 1] =
            outputPath[cwdLength + pathLength + 1] = 'z';
        inputPath[cwdLength + pathLength + 2] =
            outputPath[cwdLength + pathLength + 2] = 0;

        output_stringN("Processing: ", 12);
        output_string(inputPath, true);

        unsigned int inputFile = files_open(inputPath, FILES_READ, 0);
        unsigned int outputFile =
            files_open(outputPath, FILES_WRITE | FILES_CREATE, 0644);

        size_t size = files_size(inputFile);
        output_stringN("Size: ", 6);

        size_t sizeLength = numbers_countDigits(size);
        char sizeString[sizeLength + 1];
        numbers_toString(size, sizeLength, sizeString);
        output_string(sizeString, true);

        char contents[size + 1];
        files_read(inputFile, size, contents);
        files_close(inputFile);

        char *contentsPointer = contents;
        compiler_token_t token = {0};
        while (token.type != EOF_TOKEN) {
            compiler_getToken(&contentsPointer, &token);
            files_write(outputFile, token.length, token.token);
        }
        files_close(outputFile);
    }
}
