#include "c7c.hpp"

int main(int argc, char **argv)
{
	lexical *Lexical = new lexical();
	
	parser *Parser = new parser(Lexical->tabelaDeSimbolos);
	//Parser->sintatico2(Lexical->tabelaDeSimbolos);
	
	return 0;
}
