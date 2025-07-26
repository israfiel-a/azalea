#include <Utilities/Strings.h>
#include <stdarg.h>
#include <stddef.h>

bool strings_compare(const char *const a, const char *const b)
{
    char *av = (char *)a, *bv = (char *)b;
    while (*av == *bv)
    {
        if (*av == 0)
            return true;
        av++;
        bv++;
    }
    return false;
}

bool strings_compareN(const char *const a, const char *const b, size_t number)
{
    char *av = (char *)a, *bv = (char *)b;
    size_t checked = 0;
    while (*av == *bv)
    {
        if (*av == 0 || checked == number)
            return true;
        av++;
        bv++;
        checked++;
    }
    return false;
}

bool strings_compareInsensitive(const char *const a, const char *const b)
{
    char *av = (char *)a, *bv = (char *)b;
    while (strings_toLower(*av) == strings_toLower(*bv))
    {
        if (*av == 0)
            return true;
        av++;
        bv++;
    }
    return false;
}

size_t strings_getLength(const char *const string)
{
    char *stringPointer = (char *)string;
    while (*stringPointer++ != 0)
        __asm("");
    return stringPointer - string;
}

char strings_toLower(char character)
{
    // Credit: https://stackoverflow.com/a/2661917
    return character > 0x40 && character < 0x5B ? character | 0x60 : character;
}

void strings_concatenate(char *storage, const char *const format, ...)
{
    va_list args;
    va_start(args, format);

    char *formatPointer = (char *)format;
    size_t index = 0;
    while (*formatPointer != 0)
    {
        if (*formatPointer != '$')
        {
            storage[index] = *formatPointer;
            index++;
            formatPointer++;
            continue;
        };

        char *nextString = va_arg(args, char *);
        while (*nextString != 0)
        {
            storage[index] = *nextString;
            index++;
            nextString++;
        }
        formatPointer++;
    }

    va_end(args);
}

void strings_concatenateN(char *storage, const char *const format, ...)
{
    va_list args;
    va_start(args, format);

    char *formatPointer = (char *)format;
    size_t index = 0;
    while (*formatPointer != 0)
    {
        if (*formatPointer != '$')
        {
            storage[index] = *formatPointer;
            index++;
            formatPointer++;
            continue;
        };

        char *nextString = va_arg(args, char *);
        size_t nextStringLength = va_arg(args, size_t);
        for(size_t i = 0; i < nextStringLength; ++i)
        {
            storage[index] = *nextString;
            index++;
            nextString++;
        }
        formatPointer++;
    }

    va_end(args);
}

