#include <Compiler.h>
#include <Utilities/Output.h>
#include <Utilities/Strings.h>

[[gnu::always_inline]] [[gnu::flatten]] [[nodiscard]] [[gnu::nonnull(1)]]
static inline bool checkInformationRequest(const char *const arg)
{
    if (__builtin_expect(strings_compare(arg, "--help"), 0))
    {
        const char helpString[] =
            "Usage: compiler [OPTIONS] [DIRECTORY | "
            "FILE]\nOptions:\n\tINFORMATION:\n\t--help: Display this message and "
            "exit.\n\t--version: Display version information and "
            "exit.\n\t--license: "
            "Display copyright information and exit.\n\n\tCOMPILATION:\n\t--backend "
            "[backend]: Specify your desired backend.\n\t\tSupported "
            "backends: " COMPILER_BACKEND_LIST
            "\n\t--interpreted: Run the given target as a script file.\n\n";
        output_string(helpString, sizeof(helpString), false);
        return false;
    }
    else if (__builtin_expect(strings_compare(arg, "--version"), 0))
    {
        const char versionString[] =
            "Azalea compiler interface version " VERSION_STRING ".\n\n";
        output_string(versionString, sizeof(versionString), false);
        return false;
    }
    else if (__builtin_expect(strings_compare(arg, "--license"), 0))
    {
        const char licenseString[] =
            "This source code is under the GNU Affero General Public License "
            "version 3.0. "
            "It can be repackaged, redistributed, and modified according to the "
            "terms of "
            "the license. The AGPL can be found at "
            "<https://www.gnu.org/licenses/agpl-3.0.md>.\n\n";
        output_string(licenseString, sizeof(licenseString), false);
        return false;
    }
    return true;
}

[[gnu::always_inline]] [[gnu::flatten]] [[nodiscard]] [[gnu::nonnull(2)]]
static inline bool validateArguments(int argc, const char *const *const argv)
{
    output_string("\n", 1, false);
    if (__builtin_expect(argc <= 1, 0))
    {
        output_string("The compiler was not provided any arguments.\n", 45, false);
        return false;
    }

    // This is insurance in case the user only specifies an info request in
    // arguments, as our system of "always treat last arg as file/directory" will
    // fuck this up.
    return checkInformationRequest(argv[1]);
}

bool compiler_getArguments(int argc, const char *const *const argv,
                           compiler_arguments_t *arguments)
{
    if (__builtin_expect(!validateArguments(argc, argv), 0))
        return false;

    size_t pollType = 0;
    for (size_t i = 1; i < (size_t)argc - 1; ++i)
    {
        const char *const currentArg = argv[i];
        if(!checkInformationRequest(currentArg)) return false;

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

        if (strings_compare(currentArg, "--backend"))
        {
            pollType = 1;
        }
        else if (strings_compare(currentArg, "--interpreted"))
        {
            arguments->flags.interpreted = true;
        }
        else
        {
            size_t argLength = strings_getLength(currentArg);
            size_t messageLength = 38 + argLength;
            char message[messageLength];
            strings_concatenate(message, "Got unknown command line argument '$'.\n",
                                4, currentArg, argLength);
            output_string(message, messageLength, false);
            return false;
        }
    }

    return true;
}

