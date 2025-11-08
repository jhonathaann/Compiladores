#include "parser.h"

// criando o objeto scanner (da analise lexica)
Parser::Parser(string input){
    currentST = globalST = new SymbolTable();
    initSimbolTable(); // inicializa a tabela de simbolos com as palavras reservadas
    scanner = new Scanner(input, globalST); // passa a tabela de simbolos global ao scanner
}

void
Parser::advance(){
    // Se houver tokens armazenados no buffer de lookahead, usa o primeiro
    if (!laBuffer.empty()){
        lToken = laBuffer.front();
        laBuffer.pop_front();
    }else{
        lToken = scanner->nextToken();
    }
}

// garante que existam pelo menos n tokens no buffer de lookahead
void
Parser::ensureLA(int n){
    while((int)laBuffer.size() < n){
        Token* t = scanner->nextToken();
        laBuffer.push_back(t);
    }
}

// retorna o token que esta na posicao n do buffer, sem connsumir
Token*
Parser::nTokenBuffer(int n){
    if(n <= 0){
      return nullptr;  
    } 
    ensureLA(n);
    return laBuffer[n-1];
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

    // verifica se ja chegou ao final do arquivo
    if(lToken->name != END_OF_FILE){
        error("esperado fim de arquivo, mas encontrou tokens apos o final do programa");
    }

    cout << "compilacao encerrada com sucesso!\n";
}

// tabela de simbolos. essa tabela armazena todas as palavras reservadas da linguagem
void
Parser::initSimbolTable(){
    Token* t;
    
    t = new Token(CLASS, "class");
    globalST->add(new STEntry(t, true));
    
    t = new Token(INT, "int");
    globalST->add(new STEntry(t, true));
    
    t = new Token(FLOAT, "float");
    globalST->add(new STEntry(t, true));
    
    t = new Token(STRING, "string");
    globalST->add(new STEntry(t, true));
    
    t = new Token(IF, "if");
    globalST->add(new STEntry(t, true));
    
    t = new Token(ELSE, "else");
    globalST->add(new STEntry(t, true));
    
    t = new Token(WHILE, "while");
    globalST->add(new STEntry(t, true));
    
    t = new Token(DEF, "def");
    globalST->add(new STEntry(t, true));
    
    t = new Token(NEW, "new");
    globalST->add(new STEntry(t, true));
    
    t = new Token(RETURN, "return");
    globalST->add(new STEntry(t, true));
    
    t = new Token(PRINT, "print");
    globalST->add(new STEntry(t, true));
    
    t = new Token(READ, "read");
    globalST->add(new STEntry(t, true));
    
    t = new Token(MAIN, "main");
    globalST->add(new STEntry(t, true));
    
    t = new Token(EXTENDS, "extends");
    globalST->add(new STEntry(t, true));
    
    t = new Token(CONSTRUCTOR, "constructor");
    globalST->add(new STEntry(t, true));
    
    t = new Token(FOR, "for");
    globalST->add(new STEntry(t, true));
    
    t = new Token(BREAK, "break");
    globalST->add(new STEntry(t, true));
    
    t = new Token(SUPER, "super");
    globalST->add(new STEntry(t, true));
}

// 1
void 
Parser::program(){
    
    if(lToken->name == CLASS){
        // chama a producao ClassList
        classList();
    }
    else if(lToken->name == END_OF_FILE){
        // Programa vazio (arquivo vazio) - aceita épsilon
        ;
    }
    else{
        // Token inesperado no início do programa
        // Programa deve começar com 'class' ou estar vazio
        error("esperado 'class' para iniciar declaracao de classe ou arquivo vazio");
    }
}

// 2
void
Parser::classList(){
	do
	{
		classDecl();
	}
	while(lToken->name == CLASS);
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
    // Tenta processar declarações de variáveis
    // Continua enquanto encontrar INT, STRING ou ID
    // MASS: se encontrar um padrao de metodo (Type ID '('), para
    while(lToken->name == INT || lToken->name == STRING || lToken->name == ID){
        // verifica se o padrao atual é de metodo: (Type ID '(') --> temos que olhar dois tokens a frente
        if (lToken->name == INT || lToken->name == STRING || lToken->name == ID){
            Token* t1 = nTokenBuffer(1); // deve ser o identificador (ID)
            Token* t2 = nTokenBuffer(2); // deve ser o separador '('

            // verifica se é um padrao de metodo
            if (t1 != nullptr && t1->name == ID && t2 != nullptr && t2->name == SEP && t2->lexeme == "("){
                break;
            }
        }

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

    varDeclOpt();  // processa declaracoes separadas por virgula

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
            error("esperado identificador apos ',' na declaracao de variavel");
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
    
    // verifica se o proximo token pode inciar uma declaracao de metodo (int, string, ID ou def)
    if(lToken->name == INT || lToken->name == STRING || lToken->name == ID || lToken->name == DEF){
        methodDeclList();
    }else{
        ; // produz a palavra vazia
    }
}

// 14
void
Parser::methodDeclList(){

    // enquanto o proximo token pode iniciar uma declaracao de metodo (int, string, ID ou def)
    while(lToken->name == INT || lToken->name == STRING || lToken->name == ID || lToken->name == DEF){
        methodDecl();
    }
}

// 15
void
Parser::methodDecl(){
    
    // verifica se é um metodo sem tipo de retorno (def)
    if(lToken->name == DEF){
        match(DEF); // consome 'def'
    }
    else{
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
    }

    // espera um ID
    if(lToken->name == ID){
        match(ID); // consome o ID do metodo
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

// 17
void
Parser::ParamListOpt(){

    if(lToken->name == INT || lToken->name == STRING || lToken->name == ID){
        ParamList();
    }else{
        ; // produz a palavra vazia
    }
}

// 18
void
Parser::ParamList(){
    // processa o primeiro parametro
    Param();
    
    // processa os proximos parametros, separados por virgula
    while(lToken->name == SEP && lToken->lexeme == ","){
        match(SEP);  // consome a ","
        Param();     // consome o proximo parametro
    }
}

// 19
void
Parser::Param(){
    type(); // consome o tipo (int, string ou ID)

    // verifica se eh um vetor
    if(lToken->name == SEP && lToken->lexeme == "["){
        match(SEP); // consome o '['

        // verifica se esse colchete eh fechado
        if(lToken->name == SEP && lToken->lexeme == "]"){
            match(SEP);  // consome o ']'
        }else{
            error("esperado o ']' apos o '[' na declaracao do vetor");
        }
    }

    // espera um ID
    if(lToken->name == ID){
        match(ID); // consome o ID
    }else{
        error("esperado um identificador ID de tipo na declaracao da variavel vetor");
    }
}

// 20
void
Parser::StatementsOpt(){
    
    // verifica se o proximo token pode iniciar uma declaracao (statement)
    if(lToken->name == INT || lToken->name == STRING || lToken->name == ID ||
       lToken->name == PRINT || lToken->name == READ || lToken->name == RETURN ||
       lToken->name == SUPER || lToken->name == IF || lToken->name == FOR ||
       lToken->name == BREAK || (lToken->name == SEP && lToken->lexeme == ";")){
        Statements();
    }
    else{
        ; // produz a palavra vazia
    }

}

// 21
void
Parser::Statements(){

    Statement();
    
    // processa as proximas declarações (statement)
    while(lToken->name == INT || lToken->name == STRING || lToken->name == ID ||
          lToken->name == PRINT || lToken->name == READ || lToken->name == RETURN ||
          lToken->name == SUPER || lToken->name == IF || lToken->name == FOR ||
          lToken->name == BREAK || (lToken->name == SEP && lToken->lexeme == ";")){
        Statement();
    }
}

// 22
void
Parser::Statement(){
    // verifica declaracao de variavel com tipo primitivo (int x; ou string y;)
    if(lToken->name == INT || lToken->name == STRING){
        varDecl();  // processa uma declaracao
    }

    // ID pode ser: declaracao de variavel customizada (Pessoa p;) OU atribuicao (x = 10;)
    // precisamos diferenciar pelo contexto
    else if(lToken->name == ID){
        AtribStat();
        
        // AtribStat termina com ;
        if(lToken->name == SEP && lToken->lexeme == ";"){
            match(SEP);
        }else{
            error("esperado ';' apos atribuicao");
        }
    }
    else if(lToken->name == PRINT){
        PrintStat();

        // PrintStat termina com ;
        if(lToken->name == SEP && lToken->lexeme == ";"){
            match(SEP);
        }else{
            error("esperado ';' apos o print");
        }

    }
    else if(lToken->name == READ){
        ReadStat();
        
        // ReadStat termina com ;
        if(lToken->name == SEP && lToken->lexeme == ";"){
            match(SEP);
        }else{
            error("esperado ';' apos read");
        }
    }
    else if(lToken->name == RETURN){
        ReturnStat();
        
        // ReturnStat termina com ;
        if(lToken->name == SEP && lToken->lexeme == ";"){
            match(SEP);
        }else{
            error("esperado ';' apos return");
        }
    }
    else if(lToken->name == SUPER){
        SuperStat();
        
        // SuperStat termina com ;
        if(lToken->name == SEP && lToken->lexeme == ";"){
            match(SEP);
        }else{
            error("esperado ';' apos super");
        }
    }
    else if(lToken->name == IF){
        IfStat();  // IF não termina com ;
    }
    else if(lToken->name == FOR){
        ForStat();  // FOR não termina com ;
    }
    else if(lToken->name == BREAK){
        match(BREAK);
        
        // break termina com ;
        if(lToken->name == SEP && lToken->lexeme == ";"){
            match(SEP);
        }else{
            error("esperado ';' apos break");
        }
    }
    else if(lToken->name == SEP && lToken->lexeme == ";"){
        match(SEP);  // statement vazio (só um ponto e vírgula)
    }
    else{
        error("declaracao (statement) invalida");
    }
}

// 23
void
Parser::AtribStat(){
    LValue();
    
    // espera operador de atribuição '='
    if(lToken->name == OP && lToken->lexeme == "="){
        match(OP);
    }else{
        error("esperado '=' na atribuicao");
    }
    
    // verifica se é AllocExpression (new ID ou Type[)
    if(lToken->name == NEW || lToken->name == INT || lToken->name == STRING){
        AllocExpression();
    }
    else{
        // cc, é apenas uma expressao normal
        Expression();
    }
}

// 24
void
Parser::PrintStat(){
    // nao eh necessario fazer um if pois quem chamou PrintStat ja verificou que eh PRINT
    match(PRINT);
    Expression();
}

// 25
void
Parser::ReadStat(){
    match(READ);
    LValue();
}

// 26
void
Parser::ReturnStat(){
    match(RETURN);
    Expression();
}

// 27
void
Parser::SuperStat(){
    match(SUPER);
    
    if(lToken->name == SEP && lToken->lexeme == "("){
        match(SEP); // consome "("
    }else{
        error("esperado '(' depois de super");
    }
    
    ArgListOpt();
    
    if(lToken->name == SEP && lToken->lexeme == ")"){
        match(SEP); // consome ")"
    }else{
        error("esperado ')' ao final dos argumentos de super");
    }
}

// 28
void
Parser::IfStat(){
    
    match(IF);

    // espera um '('
    if(lToken->name == SEP && lToken->lexeme == "("){
        match(SEP);
    }else{
        error("esperado '(' apos if");
    }

    Expression();

    // espera um ')'
    if(lToken->name == SEP && lToken->lexeme == ")"){
        match(SEP);
    }else{
        error("esperado ')' apos expressao do if");
    }

    // espera um '{'
    if(lToken->name == SEP && lToken->lexeme == "{"){
        match(SEP);
    }else{
        error("esperado '{' para iniciar corpo do if");
    }

    StatementsOpt(); 

    // espera um '}'
    if(lToken->name == SEP && lToken->lexeme == "}"){
        match(SEP);
    }else{
        error("esperado '}' para fechar corpo do if");
    }
    
    // verifica se tem else
    if(lToken->name == ELSE){
        match(ELSE);
        
        // espera um '{'
        if(lToken->name == SEP && lToken->lexeme == "{"){
            match(SEP);
        }else{
            error("esperado '{' para iniciar corpo do else");
        }
        
        StatementsOpt();
        
        // espera um '}'
        if(lToken->name == SEP && lToken->lexeme == "}"){
            match(SEP);
        }else{
            error("esperado '}' para fechar corpo do else");
        }
    }
}

// 29
void
Parser::ForStat(){
   
    match(FOR);
    
    // espera '('
    if(lToken->name == SEP && lToken->lexeme == "("){
        match(SEP);
    }else{
        error("esperado '(' apos for");
    }
    
    AtribStatOpt();
    
    // espera primeiro ';'
    if(lToken->name == SEP && lToken->lexeme == ";"){
        match(SEP);
    }else{
        error("esperado ';' no primeiro campo do for");
    }
    
    ExpressionOpt();
    
    // espera segundo ';'
    if(lToken->name == SEP && lToken->lexeme == ";"){
        match(SEP);
    }else{
        error("esperado ';' no segundo campo do for");
    }
    
    AtribStatOpt();
    
    // espera ')'
    if(lToken->name == SEP && lToken->lexeme == ")"){
        match(SEP);
    }else{
        error("esperado ')' apos cabecalho do for");
    }
    
    // espera '{'
    if(lToken->name == SEP && lToken->lexeme == "{"){
        match(SEP);
    }else{
        error("esperado '{' para iniciar corpo do for");
    }
    
    StatementsOpt();
    
    // espera '}'
    if(lToken->name == SEP && lToken->lexeme == "}"){
        match(SEP);
    }else{
        error("esperado '}' para fechar corpo do for"); 
    }
}

// 30
void
Parser::AtribStatOpt(){
    
    if(lToken->name == INT || lToken->name == STRING){
        // é uma declaracao de variavel com inicializacao
        type(); // consome o tipo (int, string ou ID)

        // verifica se eh um vetor
        if(lToken->name == SEP && lToken->lexeme == "["){
            match(SEP); // consome o "["
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

        // verifica se tem inicialização (= expr)
        if(lToken->name == OP && lToken->lexeme == "="){
            match(OP);
            
            // verifica se é AllocExpression ou Expression
            if(lToken->name == NEW || lToken->name == INT || lToken->name == STRING){
                AllocExpression();
            }
            else{
                Expression();
            }
        }
    }
    else if(lToken->name == ID){
        AtribStat();
    }
    else{
        ; // produz palavra vazia
    }
}

// 31
void
Parser::ExpressionOpt(){
    if(lToken->name == ID || lToken->name == INTEGER_LITERAL || lToken->name == STRING_LITERAL || 
       (lToken->name == OP && (lToken->lexeme == "+" || lToken->lexeme == "-")) ||
       (lToken->name == SEP && lToken->lexeme == "(")){
        Expression();
    }
    else{
        ; // produz palavra vazia 
    }
}

// 32
void
Parser::LValue(){
    match(ID);
    LValueComp();
}

// 33
void
Parser::LValueComp(){

    if(lToken->name == SEP && lToken->lexeme == "."){
        match(SEP); // consome o '.'
        
        if(lToken->name == ID){
            match(ID); // consome o ID
        }else{
            error("esperado identificador apos '.'");
        }
        
        // verifica se eh um vetor
        if(lToken->name == SEP && lToken->lexeme == "["){
            match(SEP); // consome '['
            Expression();
            
            if(lToken->name == SEP && lToken->lexeme == "]"){
                match(SEP); // consome ']'
            }else{
                error("esperado ']' apos a expressao");
            }
        }
        // verifica se é chamada de metodo
        else if(lToken->name == SEP && lToken->lexeme == "("){
            match(SEP); // consome '('
            ArgListOpt();
            
            if(lToken->name == SEP && lToken->lexeme == ")"){
                match(SEP); // consome ')'
            }else{
                error("esperado ')' apos argumentos");
            }
        }
        
        // continua recursivamente
        LValueComp();
    }
    else if(lToken->name == SEP && lToken->lexeme == "["){
        match(SEP); // consome '['
        Expression();
        
        if(lToken->name == SEP && lToken->lexeme == "]"){
            match(SEP); // consome ']'
        }else{
            error("esperado ']' apos expressao do indice");
        }
        
        // continua recursivamente
        LValueComp();
    }
    else{
        ; // produz palavra vazia
    }
}

// 34
void
Parser::Expression(){
    NumExpression();
    
    // verifica se ha um operador relacional
    if(lToken->name == OP && (lToken->lexeme == "<" || lToken->lexeme == ">" || 
                               lToken->lexeme == "<=" || lToken->lexeme == ">=" ||
                               lToken->lexeme == "==" || lToken->lexeme == "!=")){
        match(OP); // consome o operador relacional (terminal)
        NumExpression();
    }
}

// 35
void
Parser::AllocExpression(){
    
    if(lToken->name == NEW){
        match(NEW); // consome 'new'
        
        if(lToken->name == ID){
            match(ID); // consome o ID da classe
        }else{
            error("esperado identificador apos 'new'");
        }
        
        // espera '('
        if(lToken->name == SEP && lToken->lexeme == "("){
            match(SEP);
        }else{
            error("esperado '(' apos identificador em 'new'");
        }
        
        ArgListOpt();
        
        // espera ')'
        if(lToken->name == SEP && lToken->lexeme == ")"){
            match(SEP);
        }else{
            error("esperado ')' ao final dos argumentos de 'new'");
        }
    }
    else if(lToken->name == INT || lToken->name == STRING || lToken->name == ID){
        type(); // consome o tipo
        
        // espera '['
        if(lToken->name == SEP && lToken->lexeme == "["){
            match(SEP);
        }else{
            error("esperado '[' apos tipo na alocacao de array");
        }
        
        Expression();
        
        // espera ']'
        if(lToken->name == SEP && lToken->lexeme == "]"){
            match(SEP);
        }else{
            error("esperado ']' ao final da expressao de alocacao de array");
        }
    }
    else{
        error("esperado 'new' ou tipo na expressao de alocaçao");
    }
}

// 36
void
Parser::NumExpression(){
    Term();
    
    if(lToken->name == OP && (lToken->lexeme == "+" || lToken->lexeme == "-")){
        match(OP); // consome o operador
        Term();
    }
}

// 37
void
Parser::Term(){
    UnaryExpression();
    
    if(lToken->name == OP && (lToken->lexeme == "*" || lToken->lexeme == "/" || lToken->lexeme == "%")){
        match(OP); // consome o operador
        UnaryExpression();
    }
}

// 38
void
Parser::UnaryExpression(){
    
    if(lToken->name == OP && (lToken->lexeme == "+" || lToken->lexeme == "-")){
        match(OP); // consome o operador unario
        UnaryExpression();
    }
    else{
        Factor();
    }
}

// 39
void
Parser::Factor(){
    
    if(lToken->name == INTEGER_LITERAL){
        match(INTEGER_LITERAL);
    }
    else if(lToken->name == STRING_LITERAL){
        match(STRING_LITERAL);
    }
    else if(lToken->name == ID){
        LValue();
        
        if(lToken->name == SEP && lToken->lexeme == "("){
            match(SEP); // consome '('
            ArgListOpt();
            
            if(lToken->name == SEP && lToken->lexeme == ")"){
                match(SEP); // consome ')'
            }else{
                error("esperado ')' ao final dos argumentos");
            }
        }
    }
    else if(lToken->name == SEP && lToken->lexeme == "("){
        match(SEP); // consome '('
        Expression();
        
        if(lToken->name == SEP && lToken->lexeme == ")"){
            match(SEP); // consome ')'
        }else{
            error("expressao");
        }
    }
    else{
        error("fator invalido em expressao");
    }
}

// 40
void
Parser::ArgListOpt(){

    if(lToken->name == ID || lToken->name == INTEGER_LITERAL || lToken->name == STRING_LITERAL ||
       (lToken->name == OP && (lToken->lexeme == "+" || lToken->lexeme == "-")) ||
       (lToken->name == SEP && lToken->lexeme == "(")){
        ArgList();
    }
    else{
        ; // produz palavra vazia
    }
}

// 41
void
Parser::ArgList(){
    Expression();
    
    // processa os proximos, separados por virgula
    while(lToken->name == SEP && lToken->lexeme == ","){
        match(SEP); // consome a ','
        Expression();
    }
}

void
Parser::error(string str)
{
	cout << "linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
