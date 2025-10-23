#include "parser.h"

Parser::Parser(string input){

    scanner = new Scanner(input);
}

void
Parser::advance(){
    lToken = scanner->nextToken();
}

void
Parser::match(int t){
    if(lToken->name == t || lToken->attribute == t){
        advance();
    }else{
        error("erro dentro da funcao match!\n");
    }
}

void
Parser::run(){
    advance();

    program();

    cout << "compilacao encerrada com sucesso!\n";
}

// 1
void 
Parser::program(){
    
    if(lToken->name == CLASS){
        // chama a producao ClassList
        classList();
    }else{
        // produz palavra vazia
        ;
    }
}

// 2
void
Parser::classList(){
    // a primeira producao é sempre ClassDecl
    classDecl();

    // verifica se o proximo token tbm inicia uma classe
    while(lToken->name == CLASS){
        classList();
    }
}

// 3
void
Parser::classDecl(){
    // espera-se a palavra reservada "class"
    match(CLASS);

    // espera-se um ID depois de "class"
    match(ID);

    if(lToken->name == EXTENDS){
        match(EXTENDS);
        match(ID);
    }

    classBody();

}

// 4
void
Parser::classBody(){

    // espera-se "{"
    if(lToken->name == SEP && lToken->lexeme != "{"){
        match(SEP); // consome o "{"
    }else{
        error("esperado '{' para iniciar corpo da classe");
    }

    varDeclListOpt();
    constructDeclListOpt();
    methodDeclListOpt();

    // fecha o corpo da classe
    if(lToken->name == SEP && lToken->lexeme == "}"){
        match(SEP); // consome o "}"
    }else{
        error("esperado '}' ao final do corpo da classe!\n");
    }

}

// 5
void
Parser::varDeclListOpt(){
    // se o proximo token pode iniciar uma declaração de variavel
    if(lToken->name == ID || lToken->name == STRING || lToken->name == INT){
        varDeclList();
    }else{
        ; // epsilon
    }
}

// 6
void
Parser::varDeclList(){

    while(lToken->name == ID || lToken->name == STRING || lToken->name == INT){
        varDecl();
    }
}


void
Parser::error(string str)
{
	cout << "linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
