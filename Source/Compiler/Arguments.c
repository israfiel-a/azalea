#include <Compiler.h>
#include <Utilities/Output.h>
#include <Utilities/Strings.h>

bool compiler_getArguments(int argc, const char *const *const argv,
                           compiler_arguments_t *arguments)
{
    if (argc <= 1)
    {
        output_string("The compiler was not provided any arguments.", 44, true);
        return false;
    }

    const char *const helpString =
        "\nUsage: compiler [OPTIONS] [DIRECTORY | FILE]\nOptions:\n\t--help: "
        "Display this message and exit.\n\t--backend [backend]: Specify your "
        "desired backend.\n\t\tSupported backends: " COMPILER_BACKEND_LIST
        "\n\t--interpreted: Run the given target as a script file.\n";

    // This is insurance in case the user only specifies "--help" in arguments, as
    // our system of "always treat last arg as file/directory" will fuck this up.
    if (strings_compare(argv[1], "--help"))
    {
        output_string(helpString, 229, true);
        return false;
    }

    size_t pollType = 0;
    for (size_t i = 1; i < (size_t)argc - 1; ++i)
    {
        const char *const currentArg = argv[i];

        switch (pollType)
        {
            case 1:
                if (strings_compareInsensitive(currentArg, "nasm"))
                {
                    arguments->backend = NASM_BACKEND;
                    pollType = 0;
                    continue;
                }

                output_string("Invalid backend specified.", 26, true);
                return false;
        }

        if (strings_compare(currentArg, "--help"))
        {
            output_string(helpString, 229, true);
            return false;
        }
        else if (strings_compare(currentArg, "--backend"))
            pollType = 1;
        else if (strings_compare(currentArg, "--interpreted"))
            arguments->flags.interpreted = true;
    }

    return true;
}

