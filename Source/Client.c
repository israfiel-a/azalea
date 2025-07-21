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

    char *inputNames[argc];
    size_t inputCount = 0;
    compiler_arguments(argc, argv, inputNames, &inputCount);

    for (size_t i = 1; i < inputCount; i++) {
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

        // temp for logging purposes
        [[maybe_unused]]
        static const char *string_tokens[] = {
            "UNKNOWN_TOKEN", "IMPORT_TOKEN", "FUNCTION_TOKEN", "STRING_TOKEN",
            "ASTART_TOKEN",  "AEND_TOKEN",   "BSTART_TOKEN",   "BEND_TOKEN",
            "SSTART_TOKEN",  "SSEND_TOKEN",  "EOF_TOKEN"};

        char *contentsPointer = contents;
        compiler_token_t token = {0};
        while (token.type != EOF_TOKEN) {
            compiler_getToken(&contentsPointer, &token);
            // utilities_outputStringN(token.token, token.length);
            // utilities_outputString("", true);
            utilities_outputString(string_tokens[token.type], true);
        }
    }

    return 0;
}
