#include "c7c.hpp"

int main(int argc, char **argv)
{
	
	/* Argumentos:
	 * 1- nome do arquivo a ser lido;
	 * 2- gerar arquivo de tokens;
	 * 3- gerar arquivo da tabela de simbolos;
	 * */
	 /*
	if(!(argc > 1)) {
	 cout << "Não foi possível compilar\n\nArgumento necessário:\n" 
		"1) nome do arquivo a ser lido;\n"
		"Argumentos opcionais:\n"
		"2) -tk: gera arquivo de tokens;\n"
		"3) -tb: gera arquivo da tabela de simbolos;\n"
		"\nExemplo:\n"
		"./C7 nome.txt -tk -tb" << endl;
	 
	 exit(0);
	} 
*/
	string nomeDoArquivo("line.c7"), op1("-tk"), op2("-tb");
	/*
	switch(argc){
		case 4:
			op2 = argv[3];
		case 3:
			op1 = argv[2];
		case 2:
			nomeDoArquivo = argv[1];
		break;
		default:
		 cout << "Erro de entrada" << endl;
		 exit(0);
	}
	*/
	
	
	lexical *Lexical = new lexical(nomeDoArquivo, op1, op2);
	parser *Parser = new parser(Lexical->tabelaDeSimbolos);
	semantic *Semantic = new semantic(Lexical->tabelaDeSimbolos, Parser->todasArvores);
	
	
	
	return Parser->erro;
}
