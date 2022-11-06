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
            if (isdigit(input)){return NUMBER;}
            if (input == '!'){return EXCLAMATION_MARK;}
            if (input == '.'){return DOT;}
            if (input == '{'){return LEFT_BRACKET;}
            if (input == '}'){return RIGHT_BRACKET;}
            if (input == ','){return COMMA;}
            if (input == ':'){return COLON;}
            if (input == ';'){return SEMICOLON;}
            return ERROR;

        case IDENTIFIER:
            if (isdigit(input) || isalpha(input) || input == '_'){return IDENTIFIER;}
            return ERROR;

        case QUESTION_MARK:
            if (isdigit(input) || isalpha(input) || input == '_'){return IDENTIFIER;}
            if (input == '>'){return END;}
            return ERROR;

        case STRING_START:
            if (input == '"'){return STRING_END;}
            if (input > 31){return STRING;}
            //TODO escape sequences
            return ERROR;

        case NUMBER:
            if (isdigit(input)){return NUMBER;}
            if (input == '.'){return NUMBER_DOT;}
            if (input == 'e' || input == 'E'){return NUMBER_E;}
            return ERROR;

        case NUMBER_DOT:
            if (isdigit(input)){return NUMBER_DECIMAL;}
            return ERROR;

        case NUMBER_DECIMAL:
            if (isdigit(input)){return NUMBER_DECIMAL;}
            if (input == 'e' || input == 'E'){return NUMBER_E;}
            return ERROR;

        case NUMBER_E:
            if (isdigit(input)){return NUMBER_EXPONENTIAL;}
            if (input == '+' || input == '-'){return NUMBER_E_SIGN;}
            return ERROR;

        case NUMBER_E_SIGN:
            if (isdigit(input)){return NUMBER_EXPONENTIAL;}
            return ERROR;

        case NUMBER_EXPONENTIAL:
            if (isdigit(input)){return NUMBER_EXPONENTIAL;}
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
            if (isdigit(input) || isalpha(input) || input == '_'){return VARIABLE_ID;}
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
            if (input == '*'){return COMMENT_BLOCK_END;}
            return COMMENT_BLOCK_TEXT;

        case COMMENT_BLOCK_TEXT:
            if (input == '*'){return COMMENT_BLOCK_END;}
            return COMMENT_BLOCK_TEXT;

        case COMMENT_BLOCK_END:
            if (input == '/'){return START;}
            return COMMENT_BLOCK_TEXT;

        case LESS:
            if (input == '='){return LESS_EQUAL;}
            if (input == '?'){return PROLOG_START;}
            return ERROR;

        case PROLOG_START:
            if (input == 'p'){return PROLOG_1;}
            return ERROR;

        case PROLOG_1:
            if (input == 'h'){return PROLOG_2;}
            return ERROR;

        case PROLOG_2:
            if (input == 'p'){return PROLOG_3;}
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

bool isStateFinal(AutomatonState state){
    switch (state) {
        case VARIABLE_ID:
        case STRING_END:
        case IDENTIFIER:
        case LEFT_PARENTHESE:
        case RIGHT_PARENTHESE:
        case NUMBER_DECIMAL:
        case NUMBER_EXPONENTIAL:
        case MULTIPLICATION:
        case DIVISION:
        case PLUS:
        case MINUS:
        case GREATER:
        case LESS:
        case GREATER_EQUAL:
        case LESS_EQUAL:
        case ASSIGN:
        case COMPARE2:
        case NEG_COMPARE2:
        case NUMBER:
        case DOT:
        case END:
        case PROLOG_3:
        case LEFT_BRACKET:
        case RIGHT_BRACKET:
        case SEMICOLON:
        case COLON:
        case COMMA:
            return true;
        default:
            return false;
    }
}

bool isRead(AutomatonState state){
    switch (state) {
        case STRING:
        case IDENTIFIER:
        case VARIABLE_ID:
        case NUMBER_EXPONENTIAL:
        case NUMBER_DECIMAL:
        case NUMBER:
        case NUMBER_E:
        case NUMBER_E_SIGN:
        case NUMBER_DOT:
            return 1;
        default:
            return 0;
    }
}

TokenType getToken(AutomatonState state){
    switch (state) {
        case IDENTIFIER:        return TOKEN_ID; //todo identifier processing
        case VARIABLE_ID:       return TOKEN_VAR_ID;
        case LEFT_PARENTHESE:   return TOKEN_L_PAR;
        case RIGHT_PARENTHESE:  return TOKEN_R_PAR;
        case LEFT_BRACKET:      return TOKEN_L_BRACKET;
        case RIGHT_BRACKET:     return TOKEN_R_BRACKET;
        case COMMA:             return TOKEN_COMMA;
        case SEMICOLON:         return TOKEN_SEMICOLON;
        case COLON:             return TOKEN_COLON;
        case MULTIPLICATION:    return TOKEN_MUL;
        case DIVISION:          return TOKEN_DIV;
        case PLUS:              return TOKEN_PLUS;
        case MINUS:             return TOKEN_MINUS;
        case GREATER:           return TOKEN_GREATER;
        case GREATER_EQUAL:     return TOKEN_GREATER_EQ;
        case LESS:              return TOKEN_LESS;
        case LESS_EQUAL:        return TOKEN_LESS_EQ;
        case ASSIGN:            return TOKEN_ASSIGN;
        case COMPARE2:          return TOKEN_COMPARE;
        case NEG_COMPARE2:      return TOKEN_NEG_COMPARE;
        case NUMBER:            return TOKEN_INT;
        case NUMBER_EXPONENTIAL:
        case NUMBER_DECIMAL:    return TOKEN_DOUBLE;
        case STRING_END:        return TOKEN_STRING;
        case PROLOG_3:          return TOKEN_PROLOG;
        case END:               return TOKEN_END;
        default:
            //todo remove debug print
            printf("something went wrong"); 
    }
}

TokenType processIdentifier(DynamicString *identifier){
    if (DynamicStringCompare(identifier, "else")){return TOKEN_KEYWORD_ELSE;}
    if (DynamicStringCompare(identifier, "float")){return TOKEN_KEYWORD_FLOAT;}
    if (DynamicStringCompare(identifier, "function")){return TOKEN_KEYWORD_FUNCTION;}
    if (DynamicStringCompare(identifier, "if")){return TOKEN_KEYWORD_IF;}
    if (DynamicStringCompare(identifier, "int")){return TOKEN_KEYWORD_INT;}
    if (DynamicStringCompare(identifier, "null")){return TOKEN_KEYWORD_NULL;}
    if (DynamicStringCompare(identifier, "return")){return TOKEN_KEYWORD_RETURN;}
    if (DynamicStringCompare(identifier, "string")){return TOKEN_KEYWORD_STRING;}
    if (DynamicStringCompare(identifier, "void")){return TOKEN_KEYWORD_VOID;}
    if (DynamicStringCompare(identifier, "while")){return TOKEN_KEYWORD_WHILE;}
    if (DynamicStringCompare(identifier, "declare")){return TOKEN_DECLARE;}
    if (DynamicStringCompare(identifier, "strict_types")){return TOKEN_STRICT_TYPES;}
    return TOKEN_ID;
}

Stack *scanner(FILE *source){
    //resources initialization
    Stack *stackPtr = StackInit();

    DynamicString *bufferPtr = DynamicStringInit();

    AutomatonState current = START;
    AutomatonState next;

    DynamicString *tokenValuePtr;
    TokenType tokenType;

    char input;

    //scanner main loop
    while (true){
        input = fgetc(source);
        if (input == EOF){
            break;
        }

        next = AutomatonNext(current, input);
        if (isRead(current) || isRead(next)){
            DynamicStringAddChar(bufferPtr, input);
        }

        if (next == ERROR){
            if (isStateFinal(current)){
                ungetc(input, source);
                DynamicStringRemoveChar(bufferPtr);
                tokenType = getToken(current);
                if (tokenType == TOKEN_ID){
                    tokenType = processIdentifier(bufferPtr);
                }

                //Initializes dynamic string in new memory location and copies there bufferPtr content
                tokenValuePtr = DynamicStringInit();
                DynamicStringCopy(bufferPtr, tokenValuePtr);

                StackPush(stackPtr, TokenInit(tokenType, tokenValuePtr));
                DynamicStringClean(bufferPtr);
                current = START;
            } else{
                //if lexical error occurred, clean resources
                StackFree(stackPtr);
                DynamicStringFree(bufferPtr);
                return NULL;
            }
        } else{
            current = next;
        }
    }
    DynamicStringFree(bufferPtr);
    return stackPtr;
}