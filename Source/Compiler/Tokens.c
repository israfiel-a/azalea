#include <Compiler.h>
#include <Utilities/Numbers.h> // TEMP!!!
#include <Utilities/Output.h>
#include <Utilities/Strings.h>

static size_t line = 1;
static size_t column = 1;

static compiler_token_type_t lastToken = UNKNOWN_TOKEN;

#define NOT_WHITESPACE(character)                                                   \
    (character != '\n' && character != ' ' && character != '\t')
#define NOT_BLOCK_BEGIN(character)                                                  \
    (character != '{' && character != '(' && character != '[' && character != '"')
#define NOT_BLOCK_END(character)                                                    \
    (character != '}' && character != ')' && character != ']' &&                    \
     character != ';' && character != ',')

static void stripWhitespace(char **contents)
{
    while (!NOT_WHITESPACE(**contents))
    {
        if (**contents == '\n')
        {
            line++;
            column = 1;
        }
        else
            column++;
        (*contents)++;
    }
}

static compiler_token_type_t resolveToken(const char *const token, size_t length)
{
    if (*token == 0)
        return EOF_TOKEN;

    if ((lastToken == STBEGIN_TOKEN || lastToken == STLITERAL_TOKEN) &&
        *token != '"')
        return STLITERAL_TOKEN;

    switch (*token)
    {
        case ';':
            return EOS_TOKEN;
        case '"':
            if (lastToken == STBEGIN_TOKEN || lastToken == STLITERAL_TOKEN)
                return STEND_TOKEN;
            return STBEGIN_TOKEN;
        case '(':
            return SSTART_TOKEN;
        case ')':
            return SEND_TOKEN;
        case '[':
            return ASTART_TOKEN;
        case ']':
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
        case EQUALS_TOKEN:
            [[fallthrough]];
        case SSTART_TOKEN:
            return VALUE_TOKEN;
        default:
            return ID_TOKEN;
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
        case ASTART_TOKEN:
            if (lastToken == UNKNOWN_TOKEN || lastToken == EOS_TOKEN)
                valid = true;
            break;
        case AEND_TOKEN:
            if (lastToken == AID_TOKEN || lastToken == SEND_TOKEN)
                valid = true;
            break;
        case IMPORT_TOKEN:
            if (lastToken == EOS_TOKEN)
                valid = true;
            break;
        case EOS_TOKEN:
            if (lastToken != EOS_TOKEN)
                valid = true;
            break;
        case FUNCTION_TOKEN:
            if (lastToken == EOS_TOKEN)
                valid = true;
            break;
        case TID_TOKEN:
            if (lastToken == FUNCTION_TOKEN || lastToken == TYPE_TOKEN)
                valid = true;
            break;
        case ID_TOKEN:
            if (lastToken == TID_TOKEN || lastToken == EOS_TOKEN ||
                lastToken == BSTART_TOKEN)
                valid = true;
            break;
        case EQUALS_TOKEN:
            if (lastToken == ID_TOKEN)
                valid = true;
            break;
        case VALUE_TOKEN:
            if (lastToken == EQUALS_TOKEN || lastToken == SSTART_TOKEN)
                valid = true;
            break;
        case SSTART_TOKEN:
            if (lastToken == AID_TOKEN || lastToken == ID_TOKEN)
                valid = true;
            break;
        case SEND_TOKEN:
            if (lastToken == SSTART_TOKEN || lastToken == VALUE_TOKEN ||
                lastToken == STEND_TOKEN)
                valid = true;
            break;
        case BSTART_TOKEN:
            if (lastToken == VALUE_TOKEN || lastToken == ID_TOKEN ||
                lastToken == SEND_TOKEN)
                valid = true;
            break;
        case BEND_TOKEN:
            if (lastToken == EOS_TOKEN)
                valid = true;
            break;
        case STBEGIN_TOKEN:
            if (lastToken == EQUALS_TOKEN || lastToken == SSTART_TOKEN ||
                lastToken == COMMA_TOKEN)
                valid = true;
            break;
        case STEND_TOKEN:
            [[fallthrough]];
        case STLITERAL_TOKEN:
            [[fallthrough]];
        case AID_TOKEN:
            [[fallthrough]];
        case IID_TOKEN:
            // The various special ID tokens are specifically inserted after their
            // requirements. Therefore they will always be valid.
            valid = true;
            break;
        default:
            break;
    }

    if (!valid)
    {
        size_t lastTypeDigits = numbers_countDigits(lastToken);
        char lastTypeString[lastTypeDigits];
        numbers_toString(lastToken, lastTypeDigits, lastTypeString);

        size_t typeDigits = numbers_countDigits(current);
        char typeString[typeDigits];
        numbers_toString(current, typeDigits, typeString);

        size_t columnDigits = numbers_countDigits(column);
        char columnString[columnDigits];
        numbers_toString(column, columnDigits, columnString);

        size_t lineDigits = numbers_countDigits(line);
        char lineString[lineDigits];
        numbers_toString(line, lineDigits, lineString);

        size_t messageLength =
            68 + lastTypeDigits + typeDigits + columnDigits + lineDigits;
        char message[messageLength];
        strings_concatenateN(message,
                             "Found invalid token of type $ at column $ and line $. "
                             "Last token was $.\n",
                             typeString, typeDigits, columnString, columnDigits,
                             lineString, lineDigits, lastTypeString, lastTypeDigits);
        output_string(message, messageLength, false);
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

    token->line = line;
    token->column = column;

    token->type = resolveToken(token->token, token->length);
    if (token->type == UNKNOWN_TOKEN)
        return false;

    bool valid = validateToken(token->type);
    column += token->length;
    return valid;
}
