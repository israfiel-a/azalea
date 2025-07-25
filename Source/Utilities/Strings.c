#include <Utilities/Strings.h>
#include <stdarg.h>

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

void strings_concatenate(char *storage, const char *const format,
                         size_t formatLength, ...)
{
    va_list args;
    va_start(args, formatLength);

    size_t inserted = 0;
    for (size_t i = 0; i < formatLength; ++i)
    {
        if (format[i] != '$')
        {
            storage[i + inserted] = format[i];
            continue;
        }

        const char *const nextString = va_arg(args, const char *const);
        size_t nextStringLength = va_arg(args, size_t);

        for (size_t j = 0; j < nextStringLength; ++j)
        {
            storage[i + inserted] = nextString[j];
            inserted++;
        }
    }

    va_end(args);
}

