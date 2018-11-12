#include "c7c.hpp"

void parser::sintatico(list<token> tabelaDeSimbolos) {
	
	if(!(tabelaDeSimbolos.size() > 0)) {
		cout << "Código-fonte vazio" << endl;
		exit(0);
	}
	
	int ultPipe = 0;
	list<token>::iterator itPesquisa;
	token dePesquisa = {TK_AUTHOR, "\author", 0, 0};
	// Pesquisa por \author
	itPesquisa = find(tabelaDeSimbolos.begin(), tabelaDeSimbolos.end(), dePesquisa);
	
	if(itPesquisa != tabelaDeSimbolos.end()) {
		cout << "Achou \\author" << endl;
		ultPipe = distance(tabelaDeSimbolos.begin(), itPesquisa);
		
	} else {
		cout << "Falta bloco de autoria (\\author)" << endl;
		exit(0);
	}
	
		
	
	
		
}