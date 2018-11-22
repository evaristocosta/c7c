#include "c7c.hpp"

semantic::semantic(vector<token> tabela, vector<NGraph::Graph> arvs) {
	cout << "\n**********\nInício da análise semântica..." << endl;
	tabelaDeSim = tabela;
	
	arvAutor = arvs.at(0);
	arvInstr = arvs.at(1);
	arvConfig = arvs.at(2);
	arvPart = arvs.at(3);
	arvTot = arvs.at(4);
	
	// verifica vars de autor
	vAutor();
	
	
	
	cout << "Fim da análise semântica, não foram encontrados erros.\n**********" << endl;
}

void semantic::vAutor() {
	// p no nó SAUTH (não permite +1)
	NGraph::Graph::const_iterator p = arvAutor.begin();
	// p no nó AUTH
	++p;
	// tam = qtde de filhos de AUTH
	//int tam = NGraph::Graph::out_neighbors(p).size();
	int nodo;
	
	// verifica a quantidade de filhos de cada nó de AUTH; deve ser 4, pelas regras
	NGraph::Graph::vertex_set Si = NGraph::Graph::out_neighbors(p);
	for(NGraph::Graph::vertex_set::const_iterator t = Si.begin(); t != Si.end(); t++) {
		nodo = (int)*t;
		if(arvAutor.out_neighbors(nodo).size() > 4)
			cout << "Atribuições repetidas no bloco de autoria (" 
				<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
	}
	
/* funcao de print de tokens
	for(auto acha: tabelaDeSim)
		if(acha.posicao == (int)*t)
			cout << "achou " << nomeToken(acha.tipo) << endl;
	*/
}

void semantic::vInstrumentos() {}

void semantic::vConfig() {}