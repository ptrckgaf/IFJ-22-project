/*
    - Scanner implementation for ifj22 compiler
    - Author: Anton Miklis(xmikli05)
*/

#include "scanner.h"
#include "ctype.h"

FsmState FsmNext(FsmState current, char input){
    switch (current) {
        case START:
            if (input == ' ' || input == '\t' || input == '\n' || input == EOF){return START;}
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
            if (input =='/'){return DIVISION;}
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
            if (isalpha(input)){return TYPE_IDENTIFIER;}
            if (input == '>'){return END;}
            return ERROR;

        case TYPE_IDENTIFIER:
            if (isalpha(input)){return TYPE_IDENTIFIER;}
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

        case STRING_START:
            if (input == '"'){return STRING_END;}
            if (input == '\\'){return STRING_BACKSLASH;}
            if (input > 31 && input != '$'){return STRING;}
            if (input == '\n'){return STRING_MULTILINE;}
            return ERROR;

        case STRING:
            if (input == '"'){return STRING_END;}
            if (input == '\\'){return STRING_BACKSLASH;}
            if (input > 31 && input != '$'){return STRING;}
            if (input == '\n'){return STRING_MULTILINE;}
            return ERROR;

        case STRING_MULTILINE:
            //this state is needed to ignore new line symbols in strings
            if (input == '"'){return STRING_END;}
            if (input == '\\'){return STRING_BACKSLASH;}
            if (input > 31 && input != '$'){return STRING;}
            if (input == '\n'){return STRING_MULTILINE;}
            return ERROR;

        case STRING_END:
            return ERROR;

        case VARIABLE_DOLLAR:
            if (isalpha(input) || input == '_'){return VARIABLE_ID;}
            return ERROR;

        case VARIABLE_ID:
            if (isdigit(input) || isalpha(input) || input == '_'){return VARIABLE_ID;}
            return ERROR;

        case DIVISION:
            if (input == '*'){return COMMENT_BLOCK;}
            if (input == '/'){return COMMENT;}
            return ERROR;

        case COMMENT:
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

bool isStateFinal(FsmState state){
    switch (state) {
        case VARIABLE_ID:
        case STRING_END:
        case IDENTIFIER:
        case TYPE_IDENTIFIER:
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

bool isRead(FsmState state){
    switch (state) {
        case STRING:
        case IDENTIFIER:
        case TYPE_IDENTIFIER:
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

TokenType getTokenType(FsmState state){
    switch (state) {
        case IDENTIFIER:        return TOKEN_ID;
        case TYPE_IDENTIFIER:   return TOKEN_TYPE_ID;
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
        case DOT:               return TOKEN_CONCATENATE;
        case NUMBER:            return TOKEN_INT;
        case NUMBER_EXPONENTIAL:
        case NUMBER_DECIMAL:    return TOKEN_FLOAT;
        case STRING_END:        return TOKEN_STRING;
        case PROLOG_3:          return TOKEN_PROLOG;
        case END:               return TOKEN_END;
        default:
            return 0;
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
    if (DynamicStringCompare(identifier, "reads")){return TOKEN_READS;}
    if (DynamicStringCompare(identifier, "readi")){return TOKEN_READI;}
    if (DynamicStringCompare(identifier, "readf")){return TOKEN_READF;}
    if (DynamicStringCompare(identifier, "floatval")){return TOKEN_FLOATVAL;}
    if (DynamicStringCompare(identifier, "intval")){return TOKEN_INTVAL;}
    if (DynamicStringCompare(identifier, "write")){return TOKEN_WRITE;}
    if (DynamicStringCompare(identifier, "strlen")){return TOKEN_STRLEN;}
    if (DynamicStringCompare(identifier, "substring")){return TOKEN_SUBSTRING;}
    if (DynamicStringCompare(identifier, "chr")){return TOKEN_CHR;}
    if (DynamicStringCompare(identifier, "ord")){return TOKEN_ORD;}

    return TOKEN_ID;
}

TokenType processOptionalType(DynamicString *id){
    if (DynamicStringCompare(id, "int")){return TOKEN_KEYWORD_OPT_INT;}
    if (DynamicStringCompare(id, "float")){return TOKEN_KEYWORD_OPT_FLOAT;}
    if (DynamicStringCompare(id, "string")){return TOKEN_KEYWORD_OPT_STRING;}
    error_exit(LEX_ERR, "Only int,float and string keywords are allowed after \"?\"")
}

void processHexSequence(FILE *source, char *input, DynamicString *bufferPtr){
    char hexString[3];
    hexString[0] = fgetc(source);
    hexString[1] = fgetc(source);
    hexString[2] = '\0';
    char *endPtr;
    int hex = strtol(hexString, &endPtr, 16);
    if (*endPtr == '\0' && hex >= 1){
        //remove /x from buffer
        DynamicStringRemoveChar(bufferPtr);
        DynamicStringRemoveChar(bufferPtr);
        //add number in decimal format to buffer
        formatEscapeSequence(bufferPtr, hex);
        //set input to last read character
        *input = hexString[1];
    } else{
        //if /xAA is not valid hex escape sequence then remove x from buffer, unget xAA and put \ to buffer
        ungetc(hexString[1], source);
        ungetc(hexString[0], source);
        ungetc('x', source);
        DynamicStringAddChar(bufferPtr, '\\');
    }
}

void processOctSequence(FILE *source, char *input, DynamicString *bufferPtr){
    char octString[4];
    octString[0] = *input;
    octString[1] = fgetc(source);
    octString[2] = fgetc(source);
    octString[3] = '\0';

    char *endPtr;
    int oct = strtol(octString, &endPtr, 8);

    if (*endPtr == '\0' && oct > 00 && oct <= 0377){
        //remove / and digit after it from buffer
        DynamicStringRemoveChar(bufferPtr);
        DynamicStringRemoveChar(bufferPtr);
        //add character to buffer
        formatEscapeSequence(bufferPtr, oct);
        //set input to last read character
        *input = octString[2];
    } else{
        //if /AAA is not valid octal escape sequence then remove first digit from buffer, unget AAA and put \ to buffer
        ungetc(octString[2], source);
        ungetc(octString[1], source);
        ungetc(octString[0], source);
        DynamicStringAddChar(bufferPtr, '\\');
    }
}
void formatEscapeSequence(DynamicString *buffer, int num){
    DynamicStringAddChar(buffer, '\\');
    char num_as_string[4];
    itoac(num, num_as_string);
    if (num < 100){
        DynamicStringAddChar(buffer, '0');
    }
    if (num < 10){
        DynamicStringAddChar(buffer, '0');
    }

    int i = 0;
    char ch = num_as_string[i];
    while(ch != '\0'){
        DynamicStringAddChar(buffer, ch);
        ch = num_as_string[++i];
    }
}

Stack *scanner(FILE *source){
    //resources initialization
    Stack *stackPtr = StackInit();

    DynamicString *bufferPtr = DynamicStringInit();

    FsmState current = START;
    FsmState next;

    DynamicString *tokenValuePtr;
    TokenType tokenType;
    bool isEnd = 0;     // 1 - end token(?>) was read

    char input = fgetc(source);
    ungetc(input, source);

    //checking source file to start with <
    if (input != '<'){
        fprintf(stderr, "File should start with <?php");
        return NULL;
    }

    //scanner main loop
    while (true){
        input = fgetc(source);

        if (isEnd && input != EOF){
            //return error when there are symbols after end token >?
            fprintf(stderr, "No symbols are allowed after ?>");
            return NULL;
        }

        next = FsmNext(current, input);

        //process escape sequence
        if (next == STRING_BACKSLASH){
            input = fgetc(source);
            processEscSequence(source, bufferPtr, &input);
            //change current state to state before STRING_BACKSLASH
            current = STRING;
            continue;
        }

        //adding chars to buffer
        if (isRead(current) || isRead(next)){
            DynamicStringAddChar(bufferPtr, input);
        }

        if (next == ERROR){
            if (isStateFinal(current)){
                //remove last symbol from buffer
                ungetc(input, source);
                DynamicStringRemoveChar(bufferPtr);
                tokenType = getTokenType(current);
                //token ?> was met,
                if (tokenType == TOKEN_END){
                    isEnd = 1;
                }
                //process keywords and built-in functions
                if (tokenType == TOKEN_ID){
                    tokenType = processIdentifier(bufferPtr);
                }
                //process ?int, ?float, ?string
                if (tokenType == TOKEN_TYPE_ID){
                    tokenType = processOptionalType(bufferPtr);
                }

                //initialize dynamic string in new memory location and copy bufferPtr content
                tokenValuePtr = DynamicStringInit();
                DynamicStringCopy(bufferPtr, tokenValuePtr);

                //push new token on top of stack and clear buffer
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
        if (input == EOF){
            break;
        }
    }

    if (isEnd){
        //remove ?> from stack
        StackPop(stackPtr);
    }
    StackFlip(stackPtr);

    //checking that the first token is prolog
    if (StackTop(stackPtr)->type != TOKEN_PROLOG){
        fprintf(stderr, "File should start with <?php");
        return NULL;
    }

    DynamicStringFree(bufferPtr);
    return stackPtr;
}

void processEscSequence(FILE *source, DynamicString *bufferPtr, char *input) {
    switch ((*input)) {
        case 'n':
            DynamicStringAddChar(bufferPtr, '\\');
            DynamicStringAddChar(bufferPtr, '0');
            DynamicStringAddChar(bufferPtr, '1');
            DynamicStringAddChar(bufferPtr, '0');
            break;
        case 't':
            DynamicStringAddChar(bufferPtr, '\\');
            DynamicStringAddChar(bufferPtr, '0');
            DynamicStringAddChar(bufferPtr, '0');
            DynamicStringAddChar(bufferPtr, '9');
            break;
        case '\\':
            DynamicStringAddChar(bufferPtr, '\\');
            DynamicStringAddChar(bufferPtr, '0');
            DynamicStringAddChar(bufferPtr, '9');
            DynamicStringAddChar(bufferPtr, '2');
            break;
        case '"':
            DynamicStringAddChar(bufferPtr, '\\');
            DynamicStringAddChar(bufferPtr, '0');
            DynamicStringAddChar(bufferPtr, '3');
            DynamicStringAddChar(bufferPtr, '4');
            break;
        case '$':
            DynamicStringAddChar(bufferPtr, '\\');
            DynamicStringAddChar(bufferPtr, '0');
            DynamicStringAddChar(bufferPtr, '3');
            DynamicStringAddChar(bufferPtr, '6');
            break;
        case 'x':
            processHexSequence(source, input, bufferPtr);
            break;
        case '0':case '1':case '2':case '3':
            processOctSequence(source, input, bufferPtr);
            break;
        default:
            //given sequence is not valid escape sequence, that's why we should put backslash to buffer
            DynamicStringAddChar(bufferPtr, '\\');
            ungetc((*input), source);
            break;
    }
}
