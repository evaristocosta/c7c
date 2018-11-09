#ifndef LEXICAL_H
#define LEXICAL_H

#include <fstream>
//#include <list>

//#include "token.h"
#include "DFA.h"

class lexical {
public:
	// Construtor
	lexical();
	
	// Análise de alfabeto
	void analiseCaracteres();
	
	// Tokenizadores
	token proximoToken(); 
	void todosTokens();
	
	// Funções de reconhecimento
	token reconheceEscopo();
	token reconheceTipoReservado();
	token reconheceReservadoComDoisPontos(string);
	
	// Manipulação de arquivo
	long GetFileSize(string);
	void geraArquivoToken();
	
	// Lista com tokens	
	list<token> tabelaDeSimbolos;
	
private:
	// Suporte de arquivo
	ifstream sourceCode;
	ofstream newSource;
 
	// Strings gerais
	string original, arquivo, 	// Nome dos arquivos
		linhaInteira,		// Linha inteira separada de getline
		palavra;			// String de manipulação
	
	// Token de trabalho nas funções de reconhecimento
	token reconhecido;
	
	// Variáveis de linha e coluna
	int linha,
		coluna;
		
	bool erro;	
	long tamanhoDoArquivo;
};

#endif //LEXICAL_H