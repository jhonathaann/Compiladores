#include "symboltable.h"
#include <fstream>

class Scanner 
{
    private: 
        string input;    // armazena todo o conteudo do arquivo fonte como string
        int pos;         // posição atual do texto de entrada
        int line;        // numero da linha atual
        SymbolTable* symbolTable; // ponteiro para a tabela de simbolos original
        
        // verifica se um lexema é uma palavra chave
        int isKeyword(string lexeme);
    
    public:

        // novo construtor do scanner
        Scanner(string filename, SymbolTable* st);

        // retorna o numero da linha atual
        int getLine();
    
        // reconhece e retorna o proximo token da entrada
        Token* nextToken();        
    
        // exibe a mensagem de erro junto com o numero da linha onde o erro ocorre
        void lexicalError(string msg);
};
