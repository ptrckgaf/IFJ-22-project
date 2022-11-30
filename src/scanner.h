#include "utils.h"

/*
 * automaton states
 */

typedef enum{
    START,
    IDENTIFIER,
    TYPE_IDENTIFIER,
    QUESTION_MARK,
    LEFT_PARENTHESE,
    RIGHT_PARENTHESE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COMMA,
    SEMICOLON,
    COLON,

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
    STRING_MULTILINE,
    STRING_END,
    STRING_BACKSLASH,


    VARIABLE_DOLLAR,
    VARIABLE_ID,

    //States for processing comments
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
    DOT,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,
    ASSIGN,
    COMPARE1,
    COMPARE2,
    EXCLAMATION_MARK,
    NEG_COMPARE1,
    NEG_COMPARE2,

    //prolog states
    PROLOG_START,
    PROLOG_1,
    PROLOG_2,
    PROLOG_3,

    END
}AutomatonState;

AutomatonState AutomatonNext(AutomatonState current, char input);

bool isStateFinal(AutomatonState state);

/**
 * Returns 1 if we should read chars in state, 0 if shouldn't
 * @param state
 * @return
 */
bool isRead(AutomatonState state);

/**
 * Function gets token type from corresponding automaton state
 * @param state
 * @return
 */
TokenType getToken(AutomatonState state);

/**
 * Function to process /x00 -/xFF sequence
 * @param source
 * @param input
 * @param bufferPtr
 */
void processHexSequence(FILE *source, char *input, DynamicString *bufferPtr);

/**
 * Function to process octal escape sequnce
 * @param source
 * @param input
 * @param bufferPtr
 */
void processOctSequence(FILE *source, char *input, DynamicString *bufferPtr);

Stack *scanner(FILE *source);
