#include <Compiler.h>
#include <Utilities/Numbers.h> // TEMP!!!
#include <Utilities/Output.h>
#include <Utilities/Strings.h>

static size_t lines = 0;
static size_t column = 0;

static compiler_token_type_t lastToken = UNKNOWN_TOKEN;

#define NOT_WHITESPACE(character)                                                   \
    (character != '\n' && character != ' ' && character != '\t')
#define NOT_BLOCK_BEGIN(character)                                                  \
    (character != '{' && character != '(' && character != '<')
#define NOT_BLOCK_END(character)                                                    \
    (character != '}' && character != ')' && character != '>' &&                    \
     character != ';' && character != ',')

static void stripWhitespace(char **contents)
{
    while (!NOT_WHITESPACE(**contents))
        (*contents)++;
}

static compiler_token_type_t resolveToken(const char *const token, size_t length)
{
    switch (*token)
    {
        case 0:
            return EOF_TOKEN;
        case ';':
            return EOS_TOKEN;
        case '"':
            return STRING_TOKEN;
        case '(':
            return SSTART_TOKEN;
        case ')':
            return SEND_TOKEN;
        case '<':
            return ASTART_TOKEN;
        case '>':
            return AEND_TOKEN;
        case '{':
            return BSTART_TOKEN;
        case '}':
            return BEND_TOKEN;
        case '=':
            return EQUALS_TOKEN;
        case ',':
            return COMMA_TOKEN;
    }

    if (*token == 'a' && *(token + 1) == 's')
        return ALIAS_TOKEN;
    else if (strings_compareN(token, "import", length - 1))
        return IMPORT_TOKEN;
    else if (strings_compareN(token, "function", length - 1))
        return FUNCTION_TOKEN;
    else if (strings_compareN(token, "enumerator", length - 1))
        return ENUMERATOR_TOKEN;
    else if (strings_compareN(token, "structure", length - 1))
        return STRUCTURE_TOKEN;
    else if (strings_compareN(token, "type", length - 1))
        return TYPE_TOKEN;
    else if (strings_compareN(token, "known", length - 1))
        return KNOWN_TOKEN;

    switch (lastToken)
    {
        case ASTART_TOKEN:
            return AID_TOKEN;
        case IMPORT_TOKEN:
            return IID_TOKEN;
        case FUNCTION_TOKEN:
            return TID_TOKEN;
        case TID_TOKEN:
            return ID_TOKEN;
        case EQUALS_TOKEN:
            [[fallthrough]];
        case SSTART_TOKEN:
            return VALUE_TOKEN;
        default:
            size_t lastTypeDigits = numbers_countDigits(lastToken);
            char lastTypeString[lastTypeDigits];
            numbers_toString(lastToken, lastTypeDigits, lastTypeString);

            size_t messageLength = 53 + length + lastTypeDigits;
            char message[messageLength];
            strings_concatenateN(
                message, "Found unknown token '$'. The last token was of type $.\n",
                token, length, lastTypeString, lastTypeDigits);
            output_string(message, messageLength, true);
            return UNKNOWN_TOKEN;
    }
}

// make multiple EOS in a row some kind of warning
// notify user with unfinished scope/block (EOF) and unknown scope/block (end token)
static bool validateToken(compiler_token_type_t current)
{
    bool valid = false;
    switch (current)
    {
        case EOF_TOKEN:
            if (lastToken == EOS_TOKEN || lastToken == BEND_TOKEN)
                valid = true;
            break;
        default:
            break;
    }

    lastToken = current;
    return valid;
}

bool compiler_getToken(char **contents, compiler_token_t *token)
{
    stripWhitespace(contents);

    // Remove comments.
    while (**contents == '/' && *(*contents + 1) == '/')
    {
        while (**contents != '\n' && **contents != 0)
            (*contents)++;
        stripWhitespace(contents);
    }

    token->token = *contents;
    while (NOT_WHITESPACE(**contents) && NOT_BLOCK_BEGIN(**contents) &&
           NOT_BLOCK_END(**contents) && **contents != 0)
        (*contents)++;
    if (!NOT_BLOCK_BEGIN(*token->token) || !NOT_BLOCK_END(*token->token))
        (*contents)++;
    token->length = *contents - token->token;

    if (**contents == '\n')
    {
        lines++;
        column = 0;
    }
    else
        column += token->length;

    token->line = lines;
    token->column = column;

    token->type = resolveToken(token->token, token->length);
    if (token->type == UNKNOWN_TOKEN)
        return false;

    if (token->type == STRING_TOKEN)
    {
        token->token++;
        while (**contents != 0 && **contents != '"')
            (*contents)++;
        token->length = *contents - token->token;
        if (**contents == '"')
            (*contents)++;
    }

    validateToken(token->type);
    // return validateToken(token->type);
    return true;
}
