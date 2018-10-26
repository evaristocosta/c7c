#ifndef LEXICAL_H
#define LEXICAL_H

#include <fstream>
#include <list>

#include "token.h"
#include "DFA.h"

class lexical {
public:
	// Construtor
	lexical();
	
	// Removedor de comentários
	void removeComentario();
	
	// Análise de alfabeto
	void analiseCaracteres();
	
	// Tokenizadores
	token proximoToken(); 
	void todosTokens();
	
	// Funções de reconhecimento
	token reconheceEscopo();
	token reconheceTipoReservado();
	token reconheceReservadoComDoisPontos(string);
	
	// Tamanho do arquivo
	long GetFileSize(string filename);
	
private:
	// Suporte de arquivo
	ifstream sourceCode;
	ofstream newSource;
 
	// Strings gerais
	string arquivo, 	// Nome do arquivo
		linhaInteira,		// Linha inteira separada de getline
		palavra;			// String de manipulação
		
	// Lista com tokens	
	list<token> tabelaDeSimbolos;
	
	// Token de trabalho nas funções de reconhecimento
	token reconhecido;
	
	// Variáveis de linha e coluna
	int linha,
		coluna;
		
	long tamanhoDoArquivo;
};

#endif //LEXICAL_H