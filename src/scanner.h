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
    FUN_ID_TOKEN,
    VAR_ID_TOKEN,
    L_PAR_TOKEN,
    R_PAR_TOKEN,
    STRING_TOKEN,
    EOF_TOKEN,
    EOL_TOKEN,
}TokenType;

union{
    int value_int;
    DynamicString value_str;
}TokenValue;

typedef struct{
    TokenType type;
    TokenValue value;
}Token;
