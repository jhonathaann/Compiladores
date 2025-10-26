#include <iostream>
#include <ctype.h>
#include <string>

using namespace std;
enum Names 
{
    UNDEF,              // token não definido
    ID,                 // identificadores (nomes de variáveis, classes)
    INTEGER_LITERAL,    // nuemros inteiros literais (ex: 42, 100)
    OP,                 // operadores <, >, <=, >=, +, −, ∗, /, %, =, == e ! =;
    SEP,                // separadores (SEP): (, ), [, ], {, }, ;, . e ,;
    STRING_LITERAL,     // strings literais entre aspas duplas (ex: "texto")

    // palavras reservadas
    CLASS,              // 'class'
    INT,                // 'int'
    FLOAT,              // 'float'
    STRING,             // 'string'
    IF,                 // 'if'
    ELSE,               // 'else'
    WHILE,              // 'while'
    DEF,                // 'def'
    NEW,                // 'new'
    RETURN,             // 'return'
    PRINT,              // 'print'
    READ,               // 'read'
    MAIN,               // 'main'
    EXTENDS,            // 'extends'
    CONSTRUCTOR,        // 'constructor'
    FOR,                // 'for'
    BREAK,              // 'break'
    SUPER,              // 'super'
    END_OF_FILE         // marca o fim do arquivo de entrada
};

inline string tokenNameToString(Names name)
{
    switch (name) {
        case ID: return "ID";
        case INTEGER_LITERAL: return "INTEGER_LITERAL";
        case OP: return "OP";
        case SEP: return "SEP";
        case STRING_LITERAL: return "STRING_LITERAL";
        case CLASS: return "CLASS";
        case INT: return "INT";
        case FLOAT: return "FLOAT";
        case STRING: return "STRING";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case WHILE: return "WHILE";
        case DEF: return "DEF";
        case NEW: return "NEW";
        case RETURN: return "RETURN";
        case PRINT: return "PRINT";
        case READ: return "READ";
        case MAIN: return "MAIN";
        case EXTENDS: return "EXTENDS";
        case CONSTRUCTOR: return "CONSTRUCTOR";
        case FOR: return "FOR";
        case BREAK: return "BREAK";
        case SUPER: return "SUPER";
        case END_OF_FILE: return "END_OF_FILE";
        default: return "UNDEF";
    }
}


class Token 
{
    public: 
        int name;           // tipo do token (valores do enum Names)
        int attribute;      // atributo opcional do token (não utilizado neste projeto)
        string lexeme;      // texto literal que foi reconhecido como token
    

        // construtor para tokens sem lexema especifico
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        // construtor para tokens com lexema
        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        

        // construtor para tokens com atributo especifico
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};
