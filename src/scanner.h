/*
 * automata states
 */
#include "utils.h"
typedef enum{
    START,
    IDENTIFIER,
    QUESTION_MARK,
    LEFT_PARENTHESE,
    RIGHT_PARENTHESE,
    STRING_START,
    NUMBER,
    NUMBER_DOT,
    NUMBER_DECIMAL,
    NUMBER_E,
    NUMBER_E_SIGN,
    NUMBER_EXPONENTIAL,
    STRING,
    STRING_END,
    STRING_ESC,
    VARIABLE_DOLLAR,
    VARIABLE_ID,
    SLASH,
    COMMENT,
    COMMENT_TEXT,
    COMMENT_BLOCK,
    COMMENT_BLOCK_END,
    COMMENT_BLOCK_TEXT
}AutomataState;

typedef enum{
    TOKEN_FUN_ID,
    TOKEN_VAR_ID,

    TOKEN_L_PAR,
    TOKEN_R_PAR,
    TOKEN_L_BRACKET,
    TOKEN_R_BRACKET,
    TOKEN_EOF,
    TOKEN_EOL,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,

    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_GREATER,
    TOKEN_GREATER_EQ,
    TOKEN_LESS,
    TOKEN_LESS_EQ,
    TOKEN_ASSIGN,
    TOKEN_COMPARE,
    TOKEN_NEG_COMPARE,

    TOKEN_INT,
    TOKEN_DOUBLE,
    TOKEN_STRING
}TokenType;

typedef enum{
    VALUE_INT,
    VALUE_STRING,
    VALUE_DOUBLE,
    VALUE_NULL
}TokenValueType;

typedef enum{
    WHILE_KEYWORD,
    IF_KEYWORD
}Keyword;


typedef struct{
    TokenType type;
    TokenValueType valueType;

    union{
        int integer;
        double decimal;
        DynamicString *stringPtr;
    }value;

}Token;

//todo add comments
Token * TokenInit(TokenType tokenType,TokenValueType valueType, DynamicString *value);

void TokenFree(Token *token);