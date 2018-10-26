#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

enum tiposToken {
	// Iniciadores de escopo:
	instrumentos = 255,	// \instruments
	setup,				// \setup
	autor,				// \author
	partitura,			// \sheetmusic

	// Tipos reservados:
	sViolin,			// violin
	sViola,				// viola
	sCello,				// cello
	sInteiro,			// numInt
	sFracional,			// numFrac
	
	// ========== AFD ===========
	chave,				// key
	tempo,				// time
	sBpm,				// bpm
	titulo,				// title
	subtitulo,			// subtitle
	sCompositor,		// compositor
	direitos,			// copyright
	
	// Identificadores e literais:
	sNumero,			// numero
	sIdentificador,		// identificador normal e com ponto

	// Delimitadores
	fimDeSecao,			// ||
	fimDeCompasso,		// |
	virgula, 			// ,
	ponto, 				// .
	comecoDeRepeticao, 	// |:
	fimDeRepeticao,		// :|

	// Atribuicao
	igual,				// =
	doisPontos,			// :
	aspaDupla,			// “

	// Operadores
	divisao,			// /
	soma,				// +
	somaDeCompasso,		// +|
	subtracao,			// -
	
	// Comentario
	comentario,			// ~
	
	novaLinha,
	
	
	fimDaLinha,
	fimDaEntrada,
	vazio
};

class token {
	/// Initializes a new 'Token' object.
public:
	enum tiposToken tipo = vazio; // A 'tipoToken' corresponding to the type of the newly created 'Token'.
	string valor = ""; // The 'String' value of the token. The actual characters of the lexeme described.
	int linha = 0, // The line number where the token was encountered in the source code.
		coluna = 0; // The column number where the token was encountered in the source code.
};


#endif //TOKEN_H