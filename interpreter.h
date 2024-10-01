//
//
//

#ifndef ZLANG_INTERPRETER_H
#define ZLANG_INTERPRETER_H

#include "lexer.h"

typedef struct{
    Lexer * lexer;
    Token * current_token;
} Interpreter;

Interpreter * create_interpreter(Lexer * lexer);
void consume_token(Interpreter * interpreter, TokenType tokenType);
int factor(Interpreter * interpreter);
int term(Interpreter * interpreter);
int expr(Interpreter * interpreter);



#endif //ZLANG_INTERPRETER_H
