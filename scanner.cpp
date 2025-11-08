#include "scanner.h"    

Scanner::Scanner(string filename, SymbolTable* st)
{
    pos = 0;    // Começa na primeira posição do texto
    line = 1;   // Começa na primeira linha
    symbolTable = st; // armazena a referencia a tabela de simbolos

    
    ifstream inputFile(filename, ios::in);
    string currentLine;

    // verifica se o arquivo foi aberto
    if (inputFile.is_open())
    {
        // le o arquivo linha por linha, preservando as quebras de linha
        while (getline(inputFile, currentLine))
        {
            this->input.append(currentLine + '\n');
        }
        inputFile.close();
    }
    else 
    {
        cout << "erro: ao foi possivel abrir o arquivo '" << filename << "'" << endl;
        exit(EXIT_FAILURE);
    }
}


// verifica se o lexema bate com alguma palavra reservada usando a tabela de símbolos
int Scanner::isKeyword(string lexeme)
{
    // busca o lexema na tabela de simbolos
    STEntry* entry = symbolTable->get(lexeme);
    
    // se encontrou, é uma palavra reservada
    if (entry != 0 && entry->reserved) {
        return entry->token->name;
    }
    
    // cc, é um ID
    return ID;
}


// retorna o numero da linha aonde o scanner esta posicionado
int Scanner::getLine()
{
    return line;
}


// reconhece (e retorna) o proximo token da entrada
Token* Scanner::nextToken()
{
    Token* tok;
    string lexeme;

    // ignora espaços em brancos e comentarios
    while (pos < input.length() && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n')) {
        if (input[pos] == '\n') {
            line++;  // incrementa contador de linhas
        }
        pos++;
    }
    
    // se encontrar "//", ignora tudo ate o final da linha
    if (pos < input.length() - 1 && input[pos] == '/' && input[pos + 1] == '/') {
        // avança ate encontrar uma quebra de linha
        while (pos < input.length() && input[pos] != '\n') {
            pos++;
        }
        // se encontrar quebra de linha, incrementa contador e avança posição
        if (pos < input.length() && input[pos] == '\n') {
            line++;
            pos++;
        }
        // chama recursivamente para continuar analisando apos o comentario
        return nextToken();
    }

    // se encontrar "/*", procura pelo fechamento "*/"
    if (pos < input.length() - 1 && input[pos] == '/' && input[pos + 1] == '*') {
        pos += 2; // Avança sobre "/*"
        bool fechado = false;
        
        // prcura pelo fechamento do comentario
        while (pos < input.length()) {
            if (pos < input.length() - 1 && input[pos] == '*' && input[pos + 1] == '/') {
                fechado = true;
                pos += 2; // Avança sobre "*/"
                break;
            }
            // conta quebras de linha dentro do comentario
            if (input[pos] == '\n') {
                line++;
            }
            pos++;
        }
        
        // verifica se o comentario foi fechado corretamente
        if (!fechado) {
            lexicalError("Comentario de bloco não fechado.");
        }
        
        // chama recursivamente para continuar analisando apos o comentario
        return nextToken();
    }
    
    // verifica se ha espaços em branco apos os comentarios
    while (pos < input.length() && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n')) {
        if (input[pos] == '\n') {
            line++;
        }
        pos++;
    }

    // se a posicao passou o tamanho do input, chegamos no fim
    if (pos >= input.length()) {
        return new Token(END_OF_FILE);
    }

    // obtem o caractere atual para decidir qual tipo de token reconhecer
    char currentChar = input[pos];


    // reconhece os ID
    if (isalpha(currentChar) || currentChar == '_') {
        lexeme = "";
        // constroi o lexema enquanto os caracteres sao validos
        while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_')) {
            lexeme += input[pos];
            pos++;
        }
        // verifica se é palavra-chave ou identificador
        int tokenType = isKeyword(lexeme);
        return new Token(tokenType, lexeme);
    }
    
    // reconhecimento de numeros inteiros
    else if (isdigit(currentChar)) {
        lexeme = "";
        
        // constroi o lexema enquanto os caracteres sao digitos
        while (pos < input.length() && isdigit(input[pos])) {
            lexeme += input[pos];
            pos++;
        }
        return new Token(INTEGER_LITERAL, lexeme);
    }
    

    // reconhecimento de strings --> elas começa e terminam com aspas duplas
    else if (currentChar == '"') {
        lexeme = "";
        pos++; // Pula a aspas inicial
        
        while (pos < input.length() && input[pos] != '"' && input[pos] != '\n') {
            lexeme += input[pos]; 
            pos++; 
        }
        
        // verifica se a string foi fechada corretamente
        if (pos >= input.length() || input[pos] == '\n') {
            lexicalError("string literal nao fechada.");
        }
        
        pos++; // pula a aspas final
        return new Token(STRING_LITERAL, lexeme);
    }
    
    // operadores compostos
    else if (currentChar == '=' && pos + 1 < input.length() && input[pos + 1] == '=') {
        pos += 2;
        return new Token(OP, "=="); 
    }
    else if (currentChar == '!' && pos + 1 < input.length() && input[pos + 1] == '=') {
        pos += 2;
        return new Token(OP, "!="); 
    }
    else if (currentChar == '<' && pos + 1 < input.length() && input[pos + 1] == '=') {
        pos += 2;
        return new Token(OP, "<="); 
    }
    else if (currentChar == '>' && pos + 1 < input.length() && input[pos + 1] == '=') {
        pos += 2;
        return new Token(OP, ">="); 
    }
    

    // operadores aritimeticos simples
    else if (currentChar == '+' || currentChar == '-' || currentChar == '*' || 
             currentChar == '/' || currentChar == '%' || currentChar == '=' ||
             currentChar == '<' || currentChar == '>') {
        lexeme = currentChar;
        pos++;
        return new Token(OP, lexeme);
    }
    
    // separadores
    else if (currentChar == '(' || currentChar == ')' || currentChar == '[' || currentChar == ']' || currentChar == '{' || currentChar == '}' || currentChar == ',' || currentChar == '.' || currentChar == ';') {
        lexeme = currentChar;
        pos++;
        return new Token(SEP, lexeme);
    }
    
    // se chegou ate aqui é porque o caracter nao foi reconhecido como um token valido
    else {
        lexicalError("caractere invalido na entrada.");
        return nullptr; 
    }
}


 // retorna que houve um erro lexico durante a analise
void Scanner::lexicalError(string msg)
{
    cout << "Linha " << line << ": " << msg << endl;
    exit(EXIT_FAILURE);
}
