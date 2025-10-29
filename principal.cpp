#include "parser.h"


int main(int argc, char* argv[]) 
{
    // Verificação da linha de comando
    // O programa espera receber exatamente um argumento: o nome do arquivo fonte
    if (argc != 2)
    {
        cout << "Uso: ./xpp_compiler nome_arquivo.xpp\n";
        cout << "Exemplo: ./xpp_compiler programa.xpp\n";
        return 1;
    }

    // cria o parser, que por sua vez criar o scanner dentro
    Parser* parser = new Parser(argv[1]);
    
    // inicia a analise sintatica
    parser->run();
    
    delete parser;
    
    return 0; 
}