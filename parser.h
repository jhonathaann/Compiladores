#include "scanner.h"

class Parser{
    private:
        Scanner* scanner;
        Token* lToken;  // l --> lookahead

        void advance();
        void match(int);  // verifica se o token passado como parametro (atual) casa com alguma producao da gramatica

    public:
        Parser(string);  // construtor da classe. recebe apenas o nome do arquivo de entrada
        void run();  // metodo para iniciar o processo de analise sintatica

        // uma metodo para cada nao terminal da gramatica
        void program();  // primeiro nao terminal da gramatica
        void classList();
        void classDecl();
        void classBody();

        void varDeclListOpt();
        void varDeclList();
        void varDecl();
        void varDeclOpt();

        void constructDeclListOpt();
        void methodDeclListOpt();

        void type();


        void error(string);

};