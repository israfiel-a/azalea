#include <Compiler.h>
#include <Utilities/Files.h>
#include <Utilities/Numbers.h>
#include <Utilities/Output.h>
#include <Utilities/Strings.h>

COMPILER_ENTRY {
    const char *startupMessage =
        "\nAzalea CLI compiler v" VERSION_STRING
        "\nCopyright (c) 2025 Israfil Argos, "
        "GPLv3\n<https://www.gnu.org/licenses/gpl-3.0.txt>\n";
    output_string(startupMessage, 113, true);

    output_string("CWD: ", 5, false);
    char cwd[512];
    files_getCWD(512, cwd);
    size_t cwdLength = strings_getLength(cwd);
    output_string(cwd, cwdLength, true);

    compiler_arguments_t arguments = {0};
    if (!compiler_arguments(argc, argv, &arguments)) return -1;

    if (arguments.flags.interpreted) {
        size_t pathLength = strings_getLength(arguments.target);
        char filePath[cwdLength + pathLength];
        for (size_t i = 0; i < cwdLength; ++i) filePath[i] = cwd[i];
        filePath[cwdLength - 1] = '/';
        for (size_t i = 0; i < pathLength; ++i)
            filePath[i + cwdLength] = arguments.target[i];
        filePath[cwdLength + pathLength - 1] = 0;

        output_string("\nInterpreting file '", 19, false);
        output_string(filePath, cwdLength + pathLength - 1, false);
        output_string("'.", 2, true);

        unsigned int file = files_open(filePath, FILES_READ, 0);

        size_t fileSize = files_size(file);
        output_string("File size: ", 11, false);
        size_t fileSizeDigits = numbers_countDigits(fileSize);
        char fileSizeString[fileSizeDigits];
        numbers_toString(fileSize, fileSizeDigits, fileSizeString);
        output_string(fileSizeString, fileSizeDigits, true);

        char contents[fileSize + 1];
        if (!files_read(file, fileSize, contents)) {
            output_string("Failed to read file.", 20, true);
            return -1;
        }
        files_close(file);
        output_string("Read file into memory.\n", 22, true);
        
        compiler_ast_node_t *head;
        compiler_generateAST((char **)&contents, &head);
    }
}
