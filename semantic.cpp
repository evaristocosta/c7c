#include "c7c.hpp"

semantic::semantic(vector<token> tabela, vector<NGraph::Graph> arvs) {
	cout << "\n**********\nInício da análise semântica..." << endl;
	tabelaDeSim = tabela;
	
	arvAutor = arvs.at(0);
	arvInstr = arvs.at(1);
	arvConfig = arvs.at(2);
	arvPart = arvs.at(3);
	arvTot = arvs.at(4);
	
	cout << arvInstr;
	
	// verifica vars de autor
	vAutor();
	
	vConfig();
	
	
	cout << "Fim da análise semântica, não foram encontrados erros.\n**********" << endl;
}

void semantic::vAutor() {

	// p no nó SAUTH (não permite +1)
	NGraph::Graph::const_iterator p = arvAutor.begin();
	// p no nó AUTH
	++p;
	// tam = qtde de filhos de AUTH
	//int tam = NGraph::Graph::out_neighbors(p).size();
	
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

void semantic::vInstrumentos() {

	NGraph::Graph::const_iterator p = arvInstr.begin();
	++p;

	NGraph::Graph::vertex_set Si = NGraph::Graph::out_neighbors(p);
	for(NGraph::Graph::vertex_set::const_iterator t = Si.begin(); t != Si.end(); t++) {
		nodo = (int)*t;	
		if(arvInstr.out_neighbors(nodo).size() == 5) {
			// é numInt ou numFrac
			int num;
			string tipoNum;
			float valNum;
			
			NGraph::Graph::vertex_set Sii = arvInstr.out_neighbors(nodo);
			for(NGraph::Graph::vertex_set::const_iterator w = Si.begin(); w != Si.end(); w++) 
				for(auto acha: tabelaDeSim) 
					if(acha.posicao == (int)*w) 
						if(acha.tipo == TK_IDENTIFIER) {
							tipoNum = acha.valor;
						} else if(acha.tipo == (TK_NUMBER%1000)) {
							
						} else 
							num = (int)acha.tipo;
							
			tNumerico.push_back(make_tuple(num, tipoNum, valNum));
			
		} else if (arvInstr.out_neighbors(nodo).size() == 3) {
			// é instrumento
			int instr;
			string valInstr;
			
			NGraph::Graph::vertex_set Sii = arvInstr.out_neighbors(nodo);
			for(NGraph::Graph::vertex_set::const_iterator w = Si.begin(); w != Si.end(); w++) 
				for(auto acha: tabelaDeSim) 
					if(acha.posicao == (int)*w) 
						if(acha.tipo == TK_IDENTIFIER) {
							valInstr = acha.valor;
						} else {
							instr = (int)acha.tipo;
						}
							
			tInstr.push_back(make_tuple(instr, valInstr));
			
		} 
			
	}
	
	// itera nos dois novos vetores a procura de valores iguais
	
	
}

void semantic::vConfig() {
	// p no nó SSETUP
	NGraph::Graph::const_iterator p = arvConfig.begin();
	// p no nó SETUP
	++p;
	
	NGraph::Graph::vertex_set Si = NGraph::Graph::out_neighbors(p);
	for(NGraph::Graph::vertex_set::const_iterator t = Si.begin(); t != Si.end(); t++) {
		nodo = (int)*t;
		switch(nodo) {		// FALTA:
			case NTS_KEY: // casos especiais, e verificar variável declarada
				if(arvConfig.out_neighbors(nodo).size() > 5)
					cout << "Problema na definição de tonalidade (" 
					<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
			
			break;
			case NTS_TIME: // verificar denominador
				if(arvConfig.out_neighbors(nodo).size() > 6)
					cout << "Mais de uma fórmula de compasso definida (" 
					<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
			
			break;
			case NTS_BPM: // verificar se não é zero
				if(arvConfig.out_neighbors(nodo).size() > 4)
					cout << "Mais de um batimento definido (" 
					<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
					
			break;
			default:
				cout << "Erro na árvore" << endl;
				exit(EXIT_FAILURE);
		}
	}
	
	
}