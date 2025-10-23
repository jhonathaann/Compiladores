#include "scanner.h"


int main(int argc, char* argv[]) 
{
    // Verificação da linha de comando
    // O programa espera receber exatamente um argumento: o nome do arquivo fonte
    if (argc != 2)
    {
        cout << "Uso: ./compiler nome_arquivo.xpp\n";
        cout << "Exemplo: ./compiler programa.xpp\n";
        return 1;
    }


    // O construtor do Scanner abre o arquivo e lê todo seu conteúdo
    Scanner* scanner = new Scanner(argv[1]);
    
    Token* currentToken;
    

    do
    {
        // extrai o proximo token da entrada
        currentToken = scanner->nextToken();
        
        cout << "Linha: " << scanner->getLine() << ", Tipo: " << tokenNameToString((Names)currentToken->name)
            << ", Lexema: '" << currentToken->lexeme << "'" << endl;
             
    } while (currentToken->name != END_OF_FILE);

    delete scanner;
    
    return 0; 
}