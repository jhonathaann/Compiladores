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
    if(lToken->name == SEP && lToken->lexeme == "{"){
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

    // enquanto o proximo token pode iniciar uma declaracao de variavel
    while(lToken->name == ID || lToken->name == STRING || lToken->name == INT){
        varDecl();
    }
}

// 7
void
Parser::varDecl(){
    type(); // consome o tipo (int, string ou ID)

    // verifica se eh um vetor
    if(lToken->name == SEP && lToken->lexeme == "["){
        match(SEP); // consome o "["

        // na sequencia esse cara deve ser fechado
        if(lToken->name == SEP && lToken->lexeme == "]"){
            match(SEP); // consome o "]"
        }else{
            error("esperado o ']' apos o '[' na declaracao do vetor");
        }
    }

    // espera um ID
    if(lToken->name == ID){
        match(ID); // consome o ID
    }else{
        error("esperado um Identificador ID na declaracao da variavel");
    }

    varDeclListOpt();

    // espera um ';'
    if(lToken->name == SEP && lToken->lexeme == ";"){
        match(SEP);  // consome o ;
    }else{
        error("esperado um ';' ao final da declaracao da variavel");
    }
}

// void 8
void
Parser::varDeclOpt(){

    if(lToken->name == SEP && lToken->lexeme == ","){
        match(SEP); // consome a ','

        if(lToken->name == ID){
            match(ID); // consome o ID
            varDeclOpt();  // chama recursivamente
        }else{
            error("esperado identificador após ',' na declaracao de variavel");
        }
    }else{
        ; // produz palavra vazia
    }
}

// 9 type --> INT | STRING | ID
void
Parser::type(){

    if(lToken->name == INT){
        match(INT); // consome o INT
    }
    else if(lToken->name == STRING){
        match(STRING);  // consome a STRING
    }
    else if(lToken->name == ID){
        match(ID);  // consome o ID
    }else{
        error("esperado tipo: INT, ID ou STRING");
    }
}

// 10
void
Parser::constructDeclListOpt(){

    if(lToken->name == CONSTRUCTOR){
        constructDeclList();
    }else{
        ; // produz a palavra vazia
    }
}

// 11
void
Parser::constructDeclList(){
    
    while(lToken->name == CONSTRUCTOR){
        constructDecl();
    }
}

// 12
void
Parser::constructDecl(){
    // constructor MethodBody
    
    match(CONSTRUCTOR);  // consome o token 'constructor'
    methodBody();        // processa o corpo do construtor
}

// 13
void
Parser::methodDeclListOpt(){
    
    // verifica se o proximo token pode inciar uma declaracao de metodo (int, string ou ID)
    if(lToken->name == INT || lToken->name == STRING || lToken->name == ID){
        methodDeclList();
    }else{
        ; // produz a palavra vazia
    }
}

// 14
void
Parser::methodDeclList(){

    // enquanto o proximo token pode iniciar uma declaracao de metodo
    while(lToken->name == INT || lToken->name == STRING || lToken->name == ID){
        methodDecl();
    }
}

// 15
void
Parser::methodDecl(){
    // Type ID MethodBody | Type [] ID MethodBody
    
    // consome o tipo (int, string ou ID)
    type();

    // verifica se eh um vetor
    if(lToken->name == SEP && lToken->lexeme == "["){
        match(SEP); // consome o "["

        // esse colchete deve ser fechado
        if(lToken->name == SEP && lToken->lexeme == "]"){
            match(SEP); // consome o "]"
        }else{
            error("esperado o ']' apos o '[' na declaracao do vetor");
        }
    }

    // espera um ID
    if(lToken->name == ID){
        match(ID); // ✅ CORRIGIDO - agora consome ID corretamente
    }else{
        error("esperado um Identificador ID na declaracao de metodo");
    }

    methodBody();
}

// 16
void
Parser::methodBody(){
    
    // espera-se "("
    if(lToken->name == SEP && lToken->lexeme == "("){
        match(SEP);  // consome o "("
    }else{
        error("esperado '(' para abrir os parametros da funcao");
    }

    ParamListOpt();

    if(lToken->name == SEP && lToken->lexeme == ")"){
        match(SEP); // consome o ")"
    }else{
        error("esperado o ')' para fechar os parametros da funcao");
    }
    
    // espera-se "{"
    if(lToken->name == SEP && lToken->lexeme == "{"){
        match(SEP);  // consome o "{"
    }else{
        error("esperado '{' para abrir o corpo do metodo");
    }
    
    StatementsOpt();
    
    // espera-se "}"
    if(lToken->name == SEP && lToken->lexeme == "}"){
        match(SEP);  // consome o "}"
    }else{
        error("esperado '}' para fechar o corpo do metodo");
    }
}

void
Parser::error(string str)
{
	cout << "linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
