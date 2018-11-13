#include "c7c.hpp"

using namespace NGraph;

void parser::sintatico(vector<token> tabelaDeSimbolos) {
	
	if(!(tabelaDeSimbolos.size() > 0)) {
		cout << "Código-fonte vazio" << endl;
		exit(0);
	}
	
	
	
	
	Graph arvore;
	
	bool erro = true;
	
	token dePesquisa = {TK_EOF, "EOF", 0, 0},
			dpipe = {TK_DPIPE, "||", 0, 0};
			
	size_t tamanhoTabela = tabelaDeSimbolos.size(),
			iterador = 0, limite = 0;
	
	// limpa dos EOF
	while(iterador < tamanhoTabela) {
		if(tabelaDeSimbolos.at(iterador) == dePesquisa) {
			tabelaDeSimbolos.erase(tabelaDeSimbolos.begin()+iterador);
			tamanhoTabela = tabelaDeSimbolos.size();
		}
		
		++iterador;
	}
	iterador = 0;
	
	// Pesquisa por \author
	dePesquisa = {TK_AUTHOR, "\\author", 0, 0};
	if(tabelaDeSimbolos.at(iterador) == dePesquisa) {
		cout << "Achou \\author" << endl;
		
		arvore.insert_edge(NTS_PROG, NTS_SAUTH);
		arvore.insert_edge(NTS_SAUTH, TK_AUTHOR);
		
	} else 
		cout << "Falta bloco de \\author" << endl;
		
		
	dePesquisa = {TK_INSTRUMENTS, "\\instruments", 0, 0};
	for( ; iterador < tamanhoTabela; iterador++) {
		if(tabelaDeSimbolos.at(iterador) == dePesquisa) {
			cout << "Achou \\instruments" << endl;
			
			erro = false;
			
			arvore.insert_edge(NTS_PROG, NTS_SINSTR);
			arvore.insert_edge(NTS_SINSTR, TK_INSTRUMENTS);
			
			limite = iterador;
			break;
		} 
	}
	if(erro) { cout << "Falta bloco de \\instruments" << endl; exit(0);}
	
	
	for(iterador = 1; iterador <= limite; iterador++) {
		if(tabelaDeSimbolos.at(iterador) == dpipe) {
			cout << "Achou || do \\author" << endl;
			
			arvore.insert_edge(NTS_SAUTH, TK_DPIPE);
			
			limite = iterador;
			
			break;
		} else if (iterador == limite) {
			cout << "Falta || do \\author" << endl;
			exit(0);
		}
	}
	
	// \author está corretamente fechado, então faz derivações dentro de autoria
	// entraAutoria(tabelaDeSimbolos);
	
	// verifica se tem algo entre o ultimo dpipe e instruments
	++limite;
	if(!(tabelaDeSimbolos.at(limite) == dePesquisa)) {
		cout << "Existem declarações fora de bloco" << endl;
	}
	iterador = limite;
	
	// pesquisa por setup
	dePesquisa = {TK_SETUP, "\\setup", 0, 0};
	for( ; iterador < tamanhoTabela; iterador++) {
		if(tabelaDeSimbolos.at(iterador) == dePesquisa) {
			cout << "Achou \\setup" << endl;
			
			erro = false;
			
			arvore.insert_edge(NTS_PROG, NTS_SSETUP);
			arvore.insert_edge(NTS_SSETUP, TK_SETUP);
			
			limite = iterador;
			break;
		} 
	}
	if(erro) { cout << "Falta bloco de \\instruments" << endl; exit(0);}
	
}
