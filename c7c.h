#ifndef C7C_H
#define C7C_H

#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <sstream>

#include "DFA.h"

using namespace std;

/* ================ TOKEN ================= */
/* ======================================== */
enum tiposToken {
	// Iniciadores de escopo:
	TK_INSTRUMENTS = 255,	// \instruments
	TK_SETUP,		// \setup
	TK_AUTHOR,		// \author
	TK_SHEETMUSIC,	// \sheetmusic

	// Tipos reservados:
	TK_S_VIOLIN,	// violin
	TK_S_VIOLA,		// viola
	TK_S_CELLO,		// cello
	TK_T_INT,		// numInt
	TK_T_FRAC,		// numFrac
	
	// ========== AFD ===========
	TK_KEY,			// key
	TK_TIME,		// time
	TK_BPM,			// bpm
	TK_TITLE,		// title
	TK_SUBTITLE,	// subtitle
	TK_COMPOSITOR,	// compositor
	TK_COPYRIGHT,	// copyright
	
	// Identificadores e literais:
	TK_NUMBER,		// numero
	TK_IDENTIFIER,	// identificador normal e com ponto

	// Delimitadores
	TK_DPIPE,		// ||
	TK_SPIPE,		// |
	TK_COMMA, 		// ,
	TK_DOT, 		// .
	TK_PIPEDDOT, 	// |:
	TK_DDOTPIPE,	// :|

	// Atribuicao
	TK_EQUAL,		// =
	TK_DDOT,		// :
	TK_DQUOTE,		// "

	// Operadores
	TK_SLASH,		// /
	TK_SUM,			// +
	TK_COMPASSUM,	// +|
	TK_SUB,			// -
	
	// Comentario
	TK_COMMENT,		// ~
	
	TK_NEWLINE,		// \n
	
	TK_EOF,			// Fim de palavra
	TK_EMPTY		// Token não reconhecido
};

static string nomeToken(tiposToken tk) {
	switch(tk) {
		case TK_INSTRUMENTS:	return "TK_INSTRUMENTS";
		case TK_SETUP:			return "TK_SETUP";
		case TK_AUTHOR:			return "TK_AUTHOR";
		case TK_SHEETMUSIC:		return "TK_SHEETMUSIC";
		case TK_S_VIOLIN:		return "TK_S_VIOLIN";
		case TK_S_VIOLA:		return "TK_S_VIOLA";
		case TK_S_CELLO:		return "TK_S_CELLO";
		case TK_T_INT:			return "TK_T_INT";
		case TK_T_FRAC:			return "TK_T_FRAC";
		case TK_KEY:			return "TK_KEY";
		case TK_TIME:			return "TK_TIME";
		case TK_BPM:			return "TK_BPM";
		case TK_TITLE:			return "TK_TITLE";
		case TK_SUBTITLE:		return "TK_SUBTITLE";
		case TK_COMPOSITOR:		return "TK_COMPOSITOR";
		case TK_COPYRIGHT:		return "TK_COPYRIGHT";
		case TK_NUMBER:			return "TK_NUMBER";
		case TK_IDENTIFIER:		return "TK_IDENTIFIER";
		case TK_DPIPE:			return "TK_DPIPE";
		case TK_SPIPE:			return "TK_SPIPE";
		case TK_COMMA:			return "TK_COMMA";
		case TK_DOT:			return "TK_DOT";
		case TK_PIPEDDOT:		return "TK_PIPEDDOT";
		case TK_DDOTPIPE:		return "TK_DDOTPIPE";
		case TK_EQUAL:			return "TK_EQUAL";
		case TK_DDOT:			return "TK_DDOT";
		case TK_DQUOTE:			return "TK_DQUOTE";
		case TK_SLASH:			return "TK_SLASH";
		case TK_SUM:			return "TK_SUM";
		case TK_COMPASSUM:		return "TK_COMPASSUM";
		case TK_SUB:			return "TK_SUB";
		case TK_COMMENT:		return "TK_COMMENT";
		case TK_NEWLINE:		return "TK_NEWLINE";
		case TK_EOF:			return "TK_EOF";
		case TK_EMPTY:			return "TK_EMPTY";
		default:				return "default";
	}
}

class token {
	// Inicializa novo 'Token'.
public:
	enum tiposToken tipo = TK_EMPTY; // Um 'tipoToken' correspondente ao tipo do recém criado 'token'.
	string valor = ""; // O 'String' valor do token. Os caracteres reais do lexema descritos.
	int linha = 0, // O número da linha em que o token foi encontrado no código-fonte.
		coluna = 0; // O número da coluna em que o token foi encontrado no código-fonte.
};
/* ======================================== */
/* ======================================== */


/* ================ LÉXICO ================ */
/* ======================================== */
class lexical {
public:
	// Construtor
	lexical();
	
	// Construtor do AFD
	void constroeAFD();
	
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
/* ======================================== */
/* ======================================== */


/* ============== SINTÁTICO =============== */
/* ======================================== */
class parser {
public:
	//Construtor
	parser();
	
	void sintatico(list<token>)
};
/* ======================================== */
/* ======================================== */



#endif //C7C_H