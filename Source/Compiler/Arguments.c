#include <Compiler.h>
#include <Utilities/Output.h>
#include <Utilities/Strings.h>

bool compiler_arguments(int argc, char **argv,
                        compiler_arguments_t *arguments) {
    size_t pollType = 0;
    for (size_t i = 0; i < (size_t)argc - 1; ++i) {
        const char *const currentArg = argv[i];

        switch (pollType) {
            case 1:
                if (strings_compareInsensitive(currentArg, "nasm")) {
                    arguments->backend = NASM_BACKEND;
                    pollType = 0;
                    continue;
                }

                output_string("Invalid backend specified.", 26, true);
                return false;
        }

        if (strings_compare(currentArg, "--help")) {
            output_string(
                "\nUsage: compiler [OPTIONS] [DIRECTORY | "
                "FILE]\nOptions:\n\t--help: "
                "Display this message and exit.\n\t--backend [backend]: "
                "Specify your desired backend.\n\t\tSupported "
                "backends: " COMPILER_BACKEND_LIST
                "\n\t--interpreted: Run the given file as a script. This will "
                "fail if given a directory.\n",
                259, true);
            return false;
        } else if (strings_compare(currentArg, "--backend"))
            pollType = 1;
        else if (strings_compare(currentArg, "--interpreted"))
            arguments->flags.interpreted = true;
    }

    arguments->target = argv[argc - 1];
    return true;
}

