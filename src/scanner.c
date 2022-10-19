#include "scanner.h"
#include "ctype.h"

AutomatonState AutomatonNext(AutomatonState current, char input){
    switch (current) {
        case START:
            if (input == ' ' || input == '\t' || input == '\n'){return START;}
            if (input == '$'){return VARIABLE_DOLLAR;}
            if (isalpha(input) || input == '_'){return IDENTIFIER;}
            if (input == '?'){return QUESTION_MARK;}
            if (input == '('){return LEFT_PARENTHESE;}
            if (input == ')'){return RIGHT_PARENTHESE;}
            if (input == '*'){return MULTIPLICATION;}
            if (input == '+'){return PLUS;}
            if(input == '-'){return MINUS;}
            if (input == '>'){return GREATER;}
            if (input == '<'){return LESS;}
            if (input =='='){return ASSIGN;}
            if (input =='/'){return SLASH;}
            if (input =='"'){return STRING_START;}
            if (isnumber(input)){return NUMBER;}
            if (input == '!'){return EXCLAMATION_MARK;}

            return ERROR;

        case IDENTIFIER:
            if (isnumber(input) || isalpha(input) || input == '_'){return IDENTIFIER;}
            return ERROR;

        case QUESTION_MARK:
            if (isnumber(input) || isalpha(input) || input == '_'){return IDENTIFIER;}
            return ERROR;

        case STRING_START:
            if (input == '"'){return STRING_END;}
            if (input > 31){return IDENTIFIER;}
            //TODO escaping sequences
            return ERROR;

        case NUMBER:
            if (isnumber(input)){return NUMBER;}
            if (input == '.'){return NUMBER_DOT;}
            if (input == 'e' || input == 'E'){return NUMBER_E;}
            return ERROR;

        case NUMBER_DOT:
            if (isnumber(input)){return NUMBER_DECIMAL;}
            return ERROR;

        case NUMBER_DECIMAL:
            if (isnumber(input)){return NUMBER_DECIMAL;}
            if (input == 'e' || input == 'E'){return NUMBER_E;}
            return ERROR;

        case NUMBER_E:
            if (isnumber(input)){return NUMBER_EXPONENTIAL;}
            if (input == '+' || input == '-'){return NUMBER_E_SIGN;}
            return ERROR;

        case NUMBER_E_SIGN:
            if (isnumber(input)){return NUMBER_EXPONENTIAL;}
            return ERROR;

        case NUMBER_EXPONENTIAL:
            if (isnumber(input)){return NUMBER_EXPONENTIAL;}
            return ERROR;

        case STRING:
            if (input == '"'){return STRING_END;}
            if (input > 31){return STRING;}
            //TODO escape sequence
            return ERROR;

        case STRING_END:
            return ERROR;

//        case STRING_ESC:
//            return ERROR;

        case VARIABLE_DOLLAR:
            if (isalpha(input) || input == '_'){return VARIABLE_ID;}
            return ERROR;

        case VARIABLE_ID:
            if (isnumber(input) || isalpha(input) || input == '_'){return VARIABLE_ID;}
            return ERROR;

        case SLASH:
            if (input == '*'){return COMMENT_BLOCK;}
            if (input == '/'){return COMMENT;}
            return DIVISION;

        case COMMENT:
            //TODO maybe exclude EOL
            return COMMENT_TEXT;

        case COMMENT_TEXT:
            if (input == '\n'){return START;}
            return COMMENT_TEXT;

        case COMMENT_BLOCK:
            if (input == '*'){return COMMENT_BLOCK_TEXT;}
            return ERROR;

        case COMMENT_BLOCK_TEXT:
            if (input == '*'){return COMMENT_BLOCK_END;}
            return COMMENT_BLOCK_TEXT;

        case COMMENT_BLOCK_END:
            if (input == '/'){return START;}
            return COMMENT_BLOCK_TEXT;

        case LESS:
            if (input == '='){return LESS_EQUAL;}
            return ERROR;

        case GREATER:
            if (input == '='){return GREATER_EQUAL;};
            return ERROR;

        case ASSIGN:
            if (input == '='){return COMPARE1;}
            return ERROR;

        case COMPARE1:
            if (input == '='){return COMPARE2;}
            return ERROR;

        case EXCLAMATION_MARK:
            if (input == '='){return NEG_COMPARE1;}
            return NEG_COMPARE1;

        case NEG_COMPARE1:
            if (input == '='){return NEG_COMPARE2;}
            return ERROR;

        default:
            return ERROR;
    }

}


