#ifndef C7C_H
#define C7C_H

#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <regex>
#include <sstream>

#include "DFA.h"
#include "ngraph.hpp"

using namespace std;

/* ================ TOKEN ================= */
/* ======================================== */
enum tiposToken {
	// Iniciadores de escopo:
	TK_INSTRUMENTS = 301,	// \instruments
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
	
	// Outros símbolos aceitos
	TK_DCOMMA,		// ;
	TK_INVERTSLASH,	//  
	TK_SHARP,		// #
	
	
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
		case TK_DCOMMA:			return "TK_DCOMMA";
		case TK_INVERTSLASH:	return "TK_INVERTSLASH";
		case TK_SHARP:			return "TK_SHARP";
		case TK_EOF:			return "TK_EOF";
		case TK_EMPTY:			return "TK_EMPTY";
		default:				return "default";
	}
}

struct token {
	// Inicializa novo 'Token'.
	enum tiposToken tipo; // Um 'tipoToken' correspondente ao tipo do recém criado 'token'.
	string valor; // O 'String' valor do token. Os caracteres reais do lexema descritos.
	int linha, // O número da linha em que o token foi encontrado no código-fonte.
		coluna, // O número da coluna em que o token foi encontrado no código-fonte.
		posicao;
		
	// operator == overload - por ser um tipo especifico (https://en.cppreference.com/w/cpp/language/operators)
	bool operator==(const token& rhs) {
		return (tipo == rhs.tipo);
	}
};
/* ======================================== */
/* ======================================== */


/* ================ LÉXICO ================ */
/* ======================================== */
class lexical {
public:
	// Construtor
	lexical(string,string,string);
	
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
	void geraArquivoToken(bool,bool,bool,bool);
	void geraArquivoTabelaDeSimbolos(bool);
	void exertarNotas();
	void printTokens(bool);
	void printTabelaDeSimbolos(bool);
	
	// Lista com tokens	
	vector<token> tabelaDeSimbolos;
	
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
		coluna,
		posicao;
		
	bool erro;	
	long tamanhoDoArquivo;
};
/* ======================================== */
/* ======================================== */


/* ============== SINTÁTICO =============== */
/* ======================================== */
enum naoTerminais {
	// Principal
	NTS_PROG = 401,		// <programa principal>

	// Seções
	NTS_SAUTH,			// <secao autoria>
	NTS_SINSTR, 		// <secao instrumentos>
	NTS_SSETUP, 		// <secao configuracao>
	NTS_SSHEET, 		// <secao partitura>

	// Seção autoria
	NTS_AUTH, 			// <autoria>
	NTS_TITLE, 			// <title>
	NTS_SUBTITLE, 		// <subtitle>
	NTS_COMPOSITOR, 	// <compositor>
	NTS_COPYRIGHT, 		// <copyright>

	// Seção instrumentos
	NTS_INSTRUMENTS,	// <instrumentos>
	NTS_INSTRUMENT, 	// <instrumento>
	NTS_TYPENUM, 		// <tipo numerico>
	NTS_TYPEINT, 		// <inteiro>
	NTS_TYPEFRAC, 		// <quebrado>

	// Seção configuração
	NTS_SETUP, 			// <configuracao>
	NTS_KEY, 			// <key>
	NTS_TIME, 			// <time>
	NTS_BPM, 			// <bpm>

	// Seção partitura
	NTS_SHEET, 			// <partitura>
	NTS_SHEETSTRUCT, 	// <estrutura>
	// Repetição
	NTS_REP, 			// <repeticao>
	// Linhas musicais
	NTS_MUSLINE, 		// <linha musical>
	NTS_MUSLINES, 		// <linhas musicais>
	// Compasso e notas
	NTS_COMPASS, 		// <compassos>
	NTS_NOTES, 			// <notas>
	NTS_NOTE, 			// <nota>
	NTS_HIGH, 			// <altura>
	NTS_DURATION, 		// <duracao>
	NTS_NOTESTRUCT, 	// <estrutura de nota>
	NTS_FINALNOTE, 		// <nota final>

	// Regras gerais
	NTS_DIGIT, 			// <digito>
	NTS_DIGITS, 		// <digitos>
	NTS_NUMBER, 		// <numero> -- 434
	NTS_FRAC, 			// <fracao>
	NTS_CHAR, 			// <letra>
	NTS_IDEN, 			// <identificador>
	NTS_SYMBOL, 		// <simbolos>
	NTS_STRING, 		// <string>
};

class parser {
public:
	//Construtor
	parser(vector<token>);
	
	
	void sintatico();
	
	
	void autoria();
	void instrumentos();
	void configuracao();
	void partitura();
	
	
	NGraph::Graph stringC7();
	NGraph::Graph numeros();
	void linhasMusicais();
	void adicionaNotas(int);
	
	
	void localizaErro(int);
	
	bool erro;
	NGraph::Graph arvore, Aautor, Ainstrumentos, Aconfig, Apartitura;
	
private:
	vector<token> copiaTabela;
	
	tiposToken *tipo;
	int *posicao, 
		contadorString = 0, 
		contadorInstrumento = 0, 
		contadorNumero = 0,
		contadorCompasso = 0;
	naoTerminais qualNo;
	
	bool numFrac;
	
};
/* ======================================== */
/* ======================================== */

/* ============== SEMÂNTICO =============== */
/* ======================================== */
class semantic {
public:
	semantic(vector<token>, NGraph::Graph);
	
private:
	NGraph::Graph arvore;
	vector<token> tabelaDeSim;
	
};
/* ======================================== */
/* ======================================== */

#endif //C7C_H