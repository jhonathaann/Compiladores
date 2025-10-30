#include "symboltable.h"
#include <fstream>

class Scanner 
{
    private: 
        string input;    ///< Armazena todo o conteúdo do arquivo fonte como string
        int pos;         ///< Posição atual no texto de entrada (índice)
        int line;        ///< Número da linha atual (para relatórios de erro)
        
        /**
         * @brief Verifica se um lexema é uma palavra-chave
         * @param lexeme String a ser verificada
         * @return Código do token da palavra-chave ou ID se não for palavra-chave
         */
        int isKeyword(string lexeme);
    
    public:
        /**
         * @brief Construtor do Scanner
         * @param filename Nome do arquivo fonte a ser analisado
         * 
         * Abre o arquivo especificado, lê todo seu conteúdo para a string 'input'
         * e inicializa as variáveis de controle (pos=0, line=1).
         */
        Scanner(string filename);

        /**
         * @brief Retorna o número da linha atual
         * @return Número da linha onde o scanner está posicionado
         * 
         * Utilizado para relatórios de erro e informações de debug.
         */
        int getLine();
    
        /**
         * @brief Reconhece e retorna o próximo token da entrada
         * @return Ponteiro para o próximo token reconhecido
         * 
         * Este é o método principal do analisador léxico. Implementa um
         * autômato de estados finitos que:
         * 1. Ignora espaços em branco e comentários
         * 2. Reconhece diferentes tipos de tokens
         * 3. Avança a posição no texto de entrada
         * 4. Retorna NULL quando atinge o fim do arquivo
         */
        Token* nextToken();        
    
        /**
         * @brief Trata erros léxicos encontrados durante a análise
         * @param msg Mensagem de erro a ser exibida
         * 
         * Exibe a mensagem de erro junto com o número da linha onde
         * o erro ocorreu e termina a execução do programa.
         */
        void lexicalError(string msg);
};
