#include "scanner.h"
#include <deque>

class Parser{
    private:
        Scanner* scanner;
        Token* lToken;  // l --> lookahead
        SymbolTable* globalST;
        SymbolTable* currentST;
        std::deque<Token*> laBuffer; // buffer para lookahead adicional 

        void advance();
        void ensureLA(int n); // garante que ha pelo menos n tokens no buffer
        Token* nTokenBuffer(int n);   // retorna o token que esta na posicao n do buffer, sem connsumir
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
        void constructDeclList();
        void constructDecl();

        void methodDeclListOpt();
        void methodDeclList();
        void methodDecl();
        void methodBody();

        void ParamListOpt();
        void ParamList();
        void Param();
        
        void StatementsOpt();
        void Statements();
        void Statement();

        void AtribStatOpt();
        void AtribStat();

        void PrintStat();
        void ReadStat();
        void ReturnStat();
        void SuperStat();
        void IfStat();
        void ForStat();

        void LValueComp();
        void LValue();

        void AllocExpression();
        void NumExpression();
        void ExpressionOpt();
        void Expression();

        void Term();
        void UnaryExpression();
        void Factor();
        
        void ArgListOpt();
        void ArgList();

        void type();

        void error(string);
        void initSimbolTable();

};