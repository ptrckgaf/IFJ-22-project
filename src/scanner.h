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

    //States for processing numbers
    NUMBER,
    NUMBER_DOT,
    NUMBER_DECIMAL,
    NUMBER_E,
    NUMBER_E_SIGN,
    NUMBER_EXPONENTIAL,

    //States for processing strings
    STRING_START,
    STRING,
    STRING_END,
    STRING_ESC,

    VARIABLE_DOLLAR,
    VARIABLE_ID,

    //States for processing comments
    SLASH,
    COMMENT,
    COMMENT_TEXT,
    COMMENT_BLOCK,
    COMMENT_BLOCK_END,
    COMMENT_BLOCK_TEXT,
    ERROR,

    //States to process operations
    MULTIPLICATION,
    DIVISION,
    PLUS,
    MINUS,
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,
    ASSIGN,
    COMPARE1,
    COMPARE2,
    EXCLAMATION_MARK,
    NEG_COMPARE1,
    NEG_COMPARE2
}AutomatonState;

AutomatonState AutomatonNext(AutomatonState current, char input);

