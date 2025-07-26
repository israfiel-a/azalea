#ifndef AZ_COMPILER_TYPES_TOKENS_H
#define AZ_COMPILER_TYPES_TOKENS_H

#define __need_size_t
#include <stddef.h>

typedef enum compiler_token_type
{
    UNKNOWN_TOKEN,
    IMPORT_TOKEN,
    IID_TOKEN, // import id
    ALIAS_TOKEN,
    ANAME_TOKEN, // alias name
    FUNCTION_TOKEN,
    ENUMERATOR_TOKEN,
    STRUCTURE_TOKEN,
    TYPE_TOKEN,
    STBEGIN_TOKEN,   // string
    STLITERAL_TOKEN, // string
    STEND_TOKEN,
    HLITERAL_TOKEN, // lex
    DLITEAL_TOKEN,  // decimal
    FLITERAL_TOKEN, // float
    EQUALS_TOKEN,
    TID_TOKEN, // type id
    ID_TOKEN,
    COMMA_TOKEN,
    VALUE_TOKEN, // a general value (not literal or string but must be value
                 // semantically); must be resolved by the AST generator (usually
                 // enum members)
    KNOWN_TOKEN,
    ASTART_TOKEN, // attribute
    AID_TOKEN,    // attribute id
    AEND_TOKEN,
    BSTART_TOKEN, // block
    BEND_TOKEN,
    SSTART_TOKEN, // scope
    SEND_TOKEN,
    EOS_TOKEN, // end of statement (';')
    EOF_TOKEN,
} compiler_token_type_t;

typedef struct compiler_token
{
    char *token;
    size_t length;
    compiler_token_type_t type;

    size_t line;
    size_t column;
} compiler_token_t;

#endif // AZ_COMPILER_TYPES_TOKENS_H
