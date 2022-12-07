/*
    - Implementacia syntaktickej analýzy v rámci projektu z IFJ
    - Autor: Patrik Gáfrik (xgafri00)
*/


#include "utils.h"
#include "parser.h"

Token *token;
ASTstruct *ast;
bool in_func_body = false;

char *displayNodes[] = {"SEQ",
                        "PROLOG",
                        "FUNC_DEF",
                        "PARAMS_RETURNTYPE",
                        "RETURN_TYPE_INT",
                        "RETURN_TYPE_FLOAT",
                        "RETURN_TYPE_STRING",
                        "RETURN_TYPE_VOID",
                        "PARAM_ID_INT",
                        "PARAM_ID_FLOAT",
                        "PARAM_ID_STRING",
                        "RETURN",
                        "INT",
                        "FLOAT",
                        "STRING",
                        "VAR_ID",
                        "IF",
                        "ELSE",
                        "WHILE",
                        "VAR_ASSIGNMENT",
                        "PLUS",
                        "MINUS",
                        "MUL",
                        "DIV",
                        "GREATER",
                        "LESS",
                        "GREATER_EQ",
                        "LESS_EQ",
                        "COMPARE",
                        "NEG_COMPARE",
                        "CONCATENATE",
                        "READS",
                        "READI",
                        "READF",
                        "WRITE",
                        "FLOATVAL",
                        "INTVAL",
                        "STRLEN",
                        "SUBSTRING",
                        "CHR",
                        "ORD",
                        "END",
                        "FUNC_ID",
                        "NULL"};


precedence_table preced_table[] = {
        {TOKEN_ID, -1, NODE_FUNC_ID},
        {TOKEN_VAR_ID, -1, NODE_VAR_ID},
        {TOKEN_TYPE_ID, -1, -1},
        {TOKEN_KEYWORD_ELSE, -1, NODE_ELSE},
        {TOKEN_KEYWORD_FLOAT, -1, RETURN_TYPE_FLOAT},
        {TOKEN_KEYWORD_FUNCTION, -1, NODE_FUNC_DEF},
        {TOKEN_KEYWORD_IF, -1, NODE_IF},
        {TOKEN_KEYWORD_INT, -1, RETURN_TYPE_INT},
        {TOKEN_KEYWORD_NULL, -1, NODE_NULL},
        {TOKEN_KEYWORD_RETURN, -1, NODE_RETURN},
        {TOKEN_KEYWORD_STRING, -1, RETURN_TYPE_STRING},
        {TOKEN_KEYWORD_VOID, -1, RETURN_TYPE_VOID},
        {TOKEN_KEYWORD_WHILE, -1,NODE_WHILE},
        {TOKEN_DECLARE, -1,-1},
        {TOKEN_STRICT_TYPES, -1,-1},
        {TOKEN_L_PAR, -1,-1},
        {TOKEN_R_PAR, -1,-1},
        {TOKEN_L_BRACKET, -1,-1},
        {TOKEN_R_BRACKET, -1,-1},
        {TOKEN_COMMA, -1,-1},
        {TOKEN_SEMICOLON, -1,-1},
        {TOKEN_COLON, -1,-1},
        {TOKEN_MUL, 13,NODE_MUL},
        {TOKEN_DIV,13 ,NODE_DIV},
        {TOKEN_PLUS, 12,NODE_PLUS},
        {TOKEN_MINUS, 12,NODE_MINUS},
        {TOKEN_GREATER, 10,NODE_GREATER},
        {TOKEN_GREATER_EQ, 10,NODE_GREATER_EQ},
        {TOKEN_LESS, 10,NODE_LESS},
        {TOKEN_LESS_EQ, 10,NODE_LESS_EQ},
        {TOKEN_ASSIGN, -1,NODE_VAR_ASSIGNMENT},
        {TOKEN_COMPARE, 9,NODE_COMPARE},
        {TOKEN_NEG_COMPARE, 9,NODE_NEG_COMPARE},
        {TOKEN_CONCATENATE, 12,NODE_CONCATENATE},
        {TOKEN_INT, -1,NODE_INT},
        {TOKEN_FLOAT, -1, NODE_FLOAT},
        {TOKEN_STRING, -1,NODE_STRING},
        {TOKEN_READS, -1,NODE_READS},
        {TOKEN_READI, -1,NODE_READI},
        {TOKEN_READF, -1,NODE_READF},
        {TOKEN_WRITE, -1,NODE_WRITE},
        {TOKEN_FLOATVAL, -1, NODE_FLOATVAL},
        {TOKEN_INTVAL, -1, NODE_INTVAL},
        {TOKEN_STRLEN, -1,NODE_STRLEN},
        {TOKEN_SUBSTRING, -1,NODE_SUBSTRING},
        {TOKEN_CHR, -1,NODE_CHR},
        {TOKEN_ORD, -1,NODE_ORD},
        {TOKEN_PROLOG, -1,-1},
        {TOKEN_END, -1,-1}

};

// TODO literaly mozu byt typu NULL
// TODO term moze byt int, float, string, var_id alebo null


// funkcia vykona syntakticku analyzu tokenov a vytvori AST

int parser(Stack *stack)
{
    ast = parse(stack); 
    Print_tree(ast);

  return 0;
}

// zaciatok syntaktickej analyzy

ASTstruct *parse(Stack *stack)
{
    ASTstruct *root = NULL;
    
    do
    {
        root = createNode(SEQ, NULL, root, prolog(stack));
    } while (!StackIsEmpty(stack));

    return root;
}


ASTstruct *prolog(Stack *stack)
{
    ASTstruct *root = NULL;
    token = loadToken(stack);

    if (token == NULL)
    {
        error_exit(SYN_ERR, "Syntax error! Prolog missing.");
    }

    if (token->type == TOKEN_PROLOG)
    {
        expectToken(TOKEN_DECLARE, stack);
        expectToken(TOKEN_L_PAR, stack);
        expectToken(TOKEN_STRICT_TYPES, stack);
        expectToken(TOKEN_ASSIGN, stack);
        expectToken(TOKEN_INT, stack);
        expectToken(TOKEN_R_PAR, stack);
        expectToken(TOKEN_SEMICOLON, stack);


        root = createNode(SEQ, NULL, program(stack), createNode(NODE_PROLOG, NULL, NULL, NULL));


    }
    else
    {
        error_exit(SYN_ERR, "Syntax error! Prolog missing.");
    }

    return root;
}


ASTstruct *program(Stack *stack)
{
    ASTstruct *root = NULL;

    token = loadToken(stack);
    if (token == NULL)
        return NULL;

    if (token->type == TOKEN_KEYWORD_FUNCTION)
    {
        if (in_func_body)
        {
            error_exit(SYN_ERR, "Syntax error! Nested function definition.");
        }
        root = function_define(stack);
    }
    else
    {
        unloadToken(stack);
        root = stmt(stack);
    }

    return root;
}


ASTstruct *function_define(Stack *stack)
{
    ASTstruct *root = NULL;
    ASTstruct *parameters = NULL;
    ASTstruct *returntype = NULL;
    ASTstruct *params_returntype = NULL;
    ASTstruct *func = NULL;

    token = loadToken(stack);
    if (token == NULL)
        return NULL;

    if (token->type == TOKEN_READS || token->type == TOKEN_READF || token->type == TOKEN_READI
        || token->type == TOKEN_WRITE || token->type == TOKEN_STRLEN || token->type == TOKEN_SUBSTRING
        || token->type == TOKEN_CHR || token->type == TOKEN_ORD)
    {
        error_exit(UNDEF_FUNC_ERR, "Semantic error! Re-definition of built-in function.");
    }


    if (token->type == TOKEN_ID)
    {
        DynamicString *tmpString = DynamicStringInit();
        DynamicStringCopy(token->value.stringPtr, tmpString);
        Token *tmpToken = TokenInit(TOKEN_ID, tmpString);

        expectToken(TOKEN_L_PAR, stack);
        parameters = params(stack);
        expectToken(TOKEN_R_PAR, stack);
        expectToken(TOKEN_COLON, stack);
        returntype = rt(stack);
        expectToken(TOKEN_L_BRACKET, stack);
        in_func_body = true;
        params_returntype = createNode(NODE_PARAMS_RETURNTYPE, NULL, parameters, returntype);

        func = createNode(NODE_FUNC_DEF, tmpToken, params_returntype, stmt(stack));

        expectToken(TOKEN_R_BRACKET, stack);
        in_func_body = false;

        root = createNode(SEQ, NULL, program(stack), func);
    }
    else
    {
        error_exit(SYN_ERR, "Syntax error! Function identifier expected.")
    }

    return root;
}


ASTstruct *params(Stack *stack)
{
    ASTstruct *param = NULL;

    token = loadToken(stack);
    if (token == NULL)
        return NULL;

    switch(token->type)
    {
        case TOKEN_KEYWORD_INT:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                param = createNode(NODE_PARAM_ID_INT, token, NULL, NULL);
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Variable identifier expected!");

        case TOKEN_KEYWORD_FLOAT:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                param = createNode(NODE_PARAM_ID_FLOAT, token, NULL, NULL);
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Variable identifier expected!");

        case TOKEN_KEYWORD_STRING:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                param = createNode(NODE_PARAM_ID_STRING, token, NULL, NULL);
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Variable identifier expected!");

        default:
            unloadToken(stack);
            return NULL;
    }

    // zoznam parametrov funkcii pri definici funkcii oddelenych ciarkov (moze byt aj prazdny)
    token = loadToken(stack);
    // viac parametrov
    if (token->type == TOKEN_COMMA)
    {
        return createNode(SEQ, NULL, params(stack), param);
    }
    // jeden parameter
    else
    {
        unloadToken(stack);
        return createNode(SEQ, NULL, NULL, param);
    }
}


ASTstruct *rt(Stack *stack)
{
    token = loadToken(stack);
    if (token == NULL)
        return NULL;

    switch(token->type)
    {
        case TOKEN_KEYWORD_INT:
            return createNode(RETURN_TYPE_INT, NULL, NULL, NULL);

        case TOKEN_KEYWORD_FLOAT:
            return createNode(RETURN_TYPE_FLOAT, NULL, NULL, NULL);

        case TOKEN_KEYWORD_STRING:
            return createNode(RETURN_TYPE_STRING, NULL, NULL, NULL);

        case TOKEN_KEYWORD_VOID:
            return createNode(RETURN_TYPE_VOID, NULL, NULL, NULL);

        default:
            error_exit(SYN_ERR, "Syntax error! Invalid returntype.");
    }

}


ASTstruct *stmt(Stack *stack)
{
    ASTstruct *root = NULL;
    ASTstruct *if_cond = NULL;
    ASTstruct *if_body = NULL;
    ASTstruct *else_body = NULL;
    ASTstruct *node_if = NULL;
    ASTstruct *node_else = NULL;
    ASTstruct *while_cond = NULL;
    ASTstruct *while_body = NULL;
    ASTstruct *while_node = NULL;
    ASTstruct *identif_node = NULL;
    ASTstruct *node_var_assignment = NULL;
    ASTstruct *func = NULL;
    ASTstruct *built_in_func = NULL;

    token = loadToken(stack);
    if (token == NULL)
        return NULL;

    switch(token->type)
    {
        case TOKEN_KEYWORD_RETURN:
            root = createNode(SEQ, NULL, NULL, createNode(NODE_RETURN, NULL, expr(stack,0), NULL));
            expectToken(TOKEN_SEMICOLON, stack);
            break;

        case TOKEN_KEYWORD_IF:
            expectToken(TOKEN_L_PAR, stack);
            if_cond = createNode(SEQ, NULL, NULL, expr(stack,0));
            if (if_cond->rightNode == NULL)
            {
                error_exit(SYN_ERR, "Syntax error! Condition expected after 'if'.");
            }
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_L_BRACKET, stack);
            if_body = stmt(stack);
            expectToken(TOKEN_R_BRACKET, stack);
            expectToken(TOKEN_KEYWORD_ELSE, stack);
            expectToken(TOKEN_L_BRACKET, stack);
            else_body = stmt(stack);
            expectToken(TOKEN_R_BRACKET, stack);
            node_else = createNode(NODE_ELSE, NULL, else_body, if_body);
            node_if = createNode(NODE_IF, NULL, if_cond, node_else);
            root = createNode(SEQ, NULL, program(stack), node_if);
            break;

        case TOKEN_KEYWORD_WHILE:
            expectToken(TOKEN_L_PAR, stack);
            while_cond = createNode(SEQ, NULL, NULL, expr(stack,0));
            if (while_cond->rightNode == NULL)
            {
                error_exit(SYN_ERR, "Syntax error! Condition expected after 'while'.");
            }
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_L_BRACKET, stack);
            while_body = stmt(stack);
            expectToken(TOKEN_R_BRACKET, stack);
            while_node = createNode(NODE_WHILE, NULL, while_cond, while_body);
            root = createNode(SEQ, NULL, program(stack), while_node);
            break;

        case TOKEN_VAR_ID:
            // fix pre $var;
            if (loadToken(stack)->type == TOKEN_SEMICOLON)
            {
                error_exit(UNDEF_VAR_ERR, "Syntax error! Undefined variable!");
            }
            unloadToken(stack);

            expectToken(TOKEN_ASSIGN, stack);
            identif_node = createNode(NODE_VAR_ID, token, NULL, NULL);
            node_var_assignment = createNode(NODE_VAR_ASSIGNMENT, NULL, identif_node, expr(stack,0));
            if (node_var_assignment->rightNode == NULL)
            {
                error_exit(SYN_ERR, "Syntax error! 'R-value' expected in variable assignment.");
            }
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), node_var_assignment);
            break;

        case TOKEN_ID:
            expectToken(TOKEN_L_PAR, stack);

            DynamicString *tmpString = DynamicStringInit();
            DynamicStringCopy(token->value.stringPtr, tmpString);
            Token *tmpToken = TokenInit(TOKEN_ID, tmpString);
            ASTstruct *args = func_args(stack);
            token = tmpToken;

            func = createNode(NODE_FUNC_ID, token, args, NULL);
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), func);
            break;

        case TOKEN_READS:
            built_in_func = createNode(NODE_READS, NULL, NULL, NULL);
            expectToken(TOKEN_L_PAR, stack);
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), built_in_func);
            break;

        case TOKEN_READI:
            built_in_func = createNode(NODE_READI, NULL, NULL, NULL);
            expectToken(TOKEN_L_PAR, stack);
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), built_in_func);
            break;

        case TOKEN_READF:
            built_in_func = createNode(NODE_READF, NULL, NULL, NULL);
            expectToken(TOKEN_L_PAR, stack);
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), built_in_func);
            break;

        case TOKEN_WRITE:
            expectToken(TOKEN_L_PAR, stack);
            built_in_func = createNode(NODE_WRITE, NULL, func_args(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), built_in_func);
            break;

        case TOKEN_STRLEN:
            expectToken(TOKEN_L_PAR, stack);
            built_in_func = createNode(NODE_STRLEN, NULL, str_arg(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), built_in_func);
            break;

        case TOKEN_SUBSTRING:
            expectToken(TOKEN_L_PAR, stack);
            built_in_func = createNode(NODE_SUBSTRING, NULL, substr_args(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), built_in_func);
            break;

        case TOKEN_ORD:
            expectToken(TOKEN_L_PAR, stack);
            built_in_func = createNode(NODE_ORD, NULL, str_arg(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), built_in_func);
            break;

        case TOKEN_CHR:
            expectToken(TOKEN_L_PAR, stack);
            built_in_func = createNode(NODE_CHR, NULL, int_arg(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), built_in_func);
            break;

        case TOKEN_FLOATVAL:
            expectToken(TOKEN_L_PAR, stack);
            built_in_func = createNode(NODE_FLOATVAL, NULL, func_args(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), built_in_func);
            break;

        case TOKEN_INTVAL:
            expectToken(TOKEN_L_PAR, stack);
            built_in_func = createNode(NODE_INTVAL, NULL, func_args(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, program(stack), built_in_func);
            break;

        default:
            unloadToken(stack);
            return NULL;
            
    }

    return root;
}

ASTstruct *str_arg(Stack *stack)
{
    ASTstruct *root = NULL;
    token = loadToken(stack);

    switch (token->type)
    {
        case TOKEN_VAR_ID:
            root = createNode(NODE_VAR_ID, token, NULL, NULL);
            break;

        case TOKEN_STRING:
            root = createNode(NODE_STRING, token, NULL, NULL);
            break;

        case TOKEN_INT:
        case TOKEN_FLOAT:
            error_exit(PARAMS_ERR, "Semantic error! Function must have string as parameter.");


        default:
            error_exit(SYN_ERR, "Syntax error! Invalid argument(s) in built-in function call.");
    }

    return root;
}

ASTstruct *int_arg(Stack *stack)
{
    ASTstruct *root = NULL;
    token = loadToken(stack);

    switch (token->type)
    {
        case TOKEN_VAR_ID:
            root = createNode(NODE_VAR_ID, token, NULL, NULL);
            break;

        case TOKEN_INT:
            root = createNode(NODE_STRING, token, NULL, NULL);
            break;

        case TOKEN_STRING:
        case TOKEN_FLOAT:
        error_exit(PARAMS_ERR, "Semantic error! Function must have integer as parameter.");


        default:
        error_exit(SYN_ERR, "Syntax error! Invalid argument(s) in built-in function call.");
    }

    return root;
}

ASTstruct *substr_args(Stack *stack)
{
    ASTstruct *root = NULL;
    token = loadToken(stack);

    switch (token->type)
    {
        case TOKEN_VAR_ID:
            root = createNode(SEQ, NULL, createNode(NODE_VAR_ID, token, NULL, NULL), NULL);
            break;

        case TOKEN_STRING:
            root = createNode(SEQ, NULL, createNode(NODE_STRING, token, NULL, NULL), NULL);
            break;

        case TOKEN_INT:
        case TOKEN_FLOAT:
        error_exit(PARAMS_ERR, "Semantic error! 'substr()' must have string as first parameter.");


        default:
        error_exit(SYN_ERR, "Syntax error! Invalid argument(s) in built-in function call.");
    }
    expectToken(TOKEN_COMMA, stack);
    token = loadToken(stack);

    switch (token->type)
    {
        case TOKEN_VAR_ID:
            root->leftNode = createNode(SEQ, NULL, createNode(NODE_VAR_ID, token, NULL, NULL), NULL);
            break;

        case TOKEN_INT:
            root->leftNode = createNode(SEQ, NULL, createNode(NODE_INT, token, NULL, NULL), NULL);
            break;

        case TOKEN_STRING:
        case TOKEN_FLOAT:
        error_exit(PARAMS_ERR, "Semantic error! 'substr()' must have int as second parameter.");


        default:
        error_exit(SYN_ERR, "Syntax error! Invalid argument(s) in built-in function call.");
    }

    expectToken(TOKEN_COMMA, stack);
    token = loadToken(stack);

    switch (token->type)
    {
        case TOKEN_VAR_ID:
            root->leftNode->leftNode = createNode(SEQ, NULL, createNode(NODE_VAR_ID, token, NULL, NULL), NULL);
            break;

        case TOKEN_INT:
            root->leftNode->leftNode = createNode(SEQ, NULL, createNode(NODE_INT, token, NULL, NULL), NULL);
            break;

        case TOKEN_STRING:
        case TOKEN_FLOAT:
        error_exit(PARAMS_ERR, "Semantic error! 'substr()' must have int as third parameter.");


        default:
        error_exit(SYN_ERR, "Syntax error! Invalid argument(s) in built-in function call.");
    }

    return root;
}

ASTstruct *func_args(Stack *stack)
{
    ASTstruct *arg = NULL;
    token = loadToken(stack);
    if (token == NULL)
        return NULL;

    switch(token->type)
    {
        case TOKEN_VAR_ID:
            arg = createNode(NODE_VAR_ID, token, NULL, NULL);
            break;

        case TOKEN_INT:
            arg = createNode(NODE_INT, token, NULL, NULL);
            break;

        case TOKEN_FLOAT:
            arg = createNode(NODE_FLOAT, token, NULL, NULL);
            break;

        case TOKEN_STRING:
            arg = createNode(NODE_STRING, token, NULL, NULL);
            break;

        case TOKEN_KEYWORD_NULL:
            arg = createNode(NODE_NULL, token, NULL, NULL);
            break;

        default:
            unloadToken(stack);
            return NULL;
    }

    token = loadToken(stack);
    if (token->type == TOKEN_COMMA)
    {
        return createNode(SEQ, NULL, func_args(stack), arg);
    }
    else
    {
        unloadToken(stack);
        return createNode(SEQ, NULL, NULL, arg);
    }
}

ASTstruct *expr(Stack *stack, int preced)
{
    ASTstruct *root = NULL;
    ASTstruct *help = NULL;
    Token *value = NULL;
    bool is_loaded = false;

    token = loadToken(stack);
    if (token == NULL) return NULL;

    switch (token->type)
    {
        case TOKEN_INT:
        case TOKEN_FLOAT:
        case TOKEN_STRING:
        case TOKEN_KEYWORD_NULL:
            root = createNode(preced_table[token->type].node_type, token, NULL, NULL);
            token = loadToken(stack);
            is_loaded = true;
            break;

        case TOKEN_MINUS:
        case TOKEN_PLUS:
        case TOKEN_MUL:
        case TOKEN_DIV:
        case TOKEN_LESS:
        case TOKEN_LESS_EQ:
        case TOKEN_GREATER:
        case TOKEN_GREATER_EQ:
        case TOKEN_COMPARE:
        case TOKEN_NEG_COMPARE:
        case TOKEN_CONCATENATE:
            root = expr(stack, preced);
            break;

        case TOKEN_L_PAR:
            root = expr(stack, 0);
            expectToken(TOKEN_R_PAR, stack);
            token = loadToken(stack);
            is_loaded = true;
            break;

        case TOKEN_VAR_ID:
            root = createNode(NODE_VAR_ID, token, NULL, NULL);
            token = loadToken(stack);
            is_loaded = true;
            break;

        case TOKEN_ID:
            expectToken(TOKEN_L_PAR, stack);
            DynamicString *tmpString = DynamicStringInit();
            DynamicStringCopy(token->value.stringPtr, tmpString);
            Token *tmpToken = TokenInit(TOKEN_ID, tmpString);
            ASTstruct *args = func_args(stack);
            token = tmpToken;

            root = createNode(NODE_FUNC_ID, token, args, NULL);
            expectToken(TOKEN_R_PAR, stack);
            break;

        case TOKEN_READS:
        case TOKEN_READI:
        case TOKEN_READF:
            root = createNode(preced_table[token->type].node_type, NULL, NULL, NULL);
            expectToken(TOKEN_L_PAR, stack);
            expectToken(TOKEN_R_PAR, stack);
            break;

        case TOKEN_WRITE:
            error_exit(EXPR_ERR, "Semantic error! write() cannot be a part of expression.");

        case TOKEN_STRLEN:
            expectToken(TOKEN_L_PAR, stack);
            root = createNode(NODE_STRLEN, NULL, str_arg(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            break;

        case TOKEN_SUBSTRING:
            expectToken(TOKEN_L_PAR, stack);
            root = createNode(NODE_SUBSTRING, NULL, substr_args(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            break;

        case TOKEN_ORD:
            expectToken(TOKEN_L_PAR, stack);
            root = createNode(NODE_ORD, NULL, str_arg(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            break;

        case TOKEN_CHR:
            expectToken(TOKEN_L_PAR, stack);
            root = createNode(NODE_CHR, NULL, int_arg(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            break;

        case TOKEN_FLOATVAL:
            expectToken(TOKEN_L_PAR, stack);
            root = createNode(NODE_FLOATVAL, NULL, func_args(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            break;

        case TOKEN_INTVAL:
            expectToken(TOKEN_L_PAR, stack);
            root = createNode(NODE_INTVAL, NULL, func_args(stack), NULL);
            expectToken(TOKEN_R_PAR, stack);
            break;

        default:
            unloadToken(stack);
    }


    // TODO kontrola dvoch po sebe iducich operatorov

    // kontrola precedencie, cyklus while prevzaty z internetu
    while (token && preced_table[token->type].preced_value >= preced)
    {
        int op = token->type;
        help = expr(stack, preced_table[op].preced_value + 1);
        root = createNode(preced_table[op].node_type, NULL, root, help);
        is_loaded = false;
    }
    if (is_loaded)
    {
        unloadToken(stack);
    }

    return root;
}

/**** PRINT AST ******/

void Print_tree2(ASTstruct* TempTree, char* sufix, char fromdir) {
/* vykresli sktrukturu binarniho stromu */
  if (TempTree != NULL) {
    char* suf2 = (char*) malloc(strlen(sufix) + 4);
    strcpy(suf2, sufix);
    
    if (fromdir == 'L') {
      suf2 = strcat(suf2, "  |");
      printf("%s\n", suf2);
    } else
      suf2 = strcat(suf2, "   ");
    
    Print_tree2(TempTree->rightNode, suf2, 'R');
    if (TempTree->value){
        switch (TempTree->value->valueType) {
            case VALUE_STRING:
                printf("%s  +-[ (%d) %s \"%s\" ]\n", sufix, TempTree->type,  displayNodes[TempTree->type], TempTree->value->data.stringPtr->value);
                break;
            case VALUE_INT:
                printf("%s  +-[ (%d) %s \"%d\" ]\n", sufix, TempTree->type,  displayNodes[TempTree->type], TempTree->value->data.integer);
                break;
            case VALUE_DOUBLE:
                printf("%s  +-[ (%d) %s \"%f\" ]\n", sufix, TempTree->type,  displayNodes[TempTree->type], TempTree->value->data.decimal);
                break;
            case VALUE_NULL:
                printf("%s  +-[ (%d) %s \"%d\" ]\n", sufix, TempTree->type,  displayNodes[TempTree->type], -1);
                break;
        }
    }
    else
      printf("%s  +-[ (%d) %s ]\n", sufix, TempTree->type,  displayNodes[TempTree->type]);
    
    strcpy(suf2, sufix);
    
    if (fromdir == 'R')
      suf2 = strcat(suf2, "  |");
    else
      suf2 = strcat(suf2, "   ");

    Print_tree2(TempTree->leftNode, suf2, 'L');

    if (fromdir == 'R')
      printf("%s\n", suf2);

    free(suf2);
  }
}

void Print_tree(ASTstruct* TempTree) {
  printf("#===========================================\n");
  printf("#Struktura binarniho stromu:\n");
  printf("#\n");

  if (TempTree != NULL)
     Print_tree2(TempTree, "#", 'X');
  else
     printf("#Strom je prazdny...\n");

  printf("#\n");
  printf("#===========================================\n");
} 


/**** PRINT AST ******/