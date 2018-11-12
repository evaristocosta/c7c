#include "c7c.hpp"

int main(int argc, char **argv)
{
	lexical *Lexical = new lexical();
	
	parser *Parser = new parser();
	Parser->sintatico(Lexical->tabelaDeSimbolos);
	
	return 0;
}
