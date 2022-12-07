/*
    - Scanner implementation for ifj22 compiler
    - Author: Anton Miklis(xmikli05)
*/

#include "utils.h"

/*
 * Automaton states
 */
typedef enum{
    START,
    IDENTIFIER,
    TYPE_IDENTIFIER,
    QUESTION_MARK,

    //States for processing separators
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

    //States for processing prolog
    PROLOG_START,
    PROLOG_1,
    PROLOG_2,
    PROLOG_3,

    END
}FsmState;
/**
 * Returns next automaton state
 * @param current
 * @param input
 * @return
 */
FsmState FsmNext(FsmState current, char input);
/**
 * Returns 1 if state is final, 0 if not
 * @param state
 * @return
 */
bool isStateFinal(FsmState state);

/**
 * Returns 1 if scanner should add chars in state, 0 if shouldn't
 * @param state
 * @return
 */
bool isRead(FsmState state);

/**
 * Function gets token type from corresponding automaton state
 * @param state
 * @return
 */
TokenType getTokenType(FsmState state);
/**
 * Identifies built-in functions and keywords
 * @param identifier
 * @return
 */
TokenType processIdentifier(DynamicString *identifier);
/**
 * Returns type identifier from string
 * @param id
 * @return
 */
TokenType processOptionalType(DynamicString *id);
/**
 * Converts escape sequences to ascii values
 * @param source
 * @param bufferPtr
 * @param input
 */
void processEscSequence(FILE *source, DynamicString *bufferPtr, char *input);
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
/**
 * Adds int to buffer in the proper format
 * @param buffer
 * @param num
 */
void formatEscapeSequence(DynamicString *buffer, int num);
/**
 * Main scanner function.
 * Returns pointer on tokens stack
 * @param source
 * @return
 */
Stack *scanner(FILE *source);
