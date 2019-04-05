#include "c7c.hpp"

using namespace NGraph;

semantic::semantic(vector<token> tabela, vector<Graph> arvs) {
	cout << "\n**********\nInício da análise semântica..." << endl;
	tabelaDeSim = tabela;
	
	arvAutor = arvs.at(0);
	arvInstr = arvs.at(1);
	arvConfig = arvs.at(2);
	arvPart = arvs.at(3);
	arvTot = arvs.at(4);
	
	//arvPart.print();
	
	// verifica contrução de autoria
	vAutor();
	// verifica contrução de instrumentos
	vInstrumentos();
	// verifica contrução de configurações
	vConfig();
	// faz todas verificações de partitura
	vPartitura();
	
	cout << "Fim da análise semântica, não foram encontrados erros.\n**********" << endl;
}

void semantic::vAutor() {
	// p no nó SAUTH (não permite +1)
	p = arvAutor.begin();
	// p no nó AUTH
	++p;
	
	// verifica a quantidade de filhos de cada nó de AUTH; deve ser 4, pelas regras
	Si = Graph::out_neighbors(p);
	for(t = Si.begin(); t != Si.end(); t++) {
		nodo = (int)*t;
		if(arvAutor.out_neighbors(nodo).size() > 4) {
			cout << "Atribuições repetidas no bloco de autoria (" 
				<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
			exit(EXIT_FAILURE);
		}
	}
}

void semantic::vInstrumentos() {
	p = arvInstr.begin();
	++p;

	// salva todas variáveis pra posterior comparação
	vector<string> variaveis;

	Si = Graph::out_neighbors(p);
	for(t = Si.begin(); t != Si.end(); t++) {
		nodo = (int)*t;	

		// é numInt ou numFrac
		if(arvInstr.out_neighbors(nodo).size() == 5) {
			int num;
			string tipoNum;
			float valNum;
			
			Sii = arvInstr.out_neighbors(nodo);
			for(w = Sii.begin(); w != Sii.end(); w++) {
				// se for numero
				if((int)(*w/1000) == 434) {
					Siii = arvInstr.out_neighbors((int)*w);
					string valNumTotal;
					
					for(u = Siii.begin(); u != Siii.end(); u++) {
						for(auto acha: tabelaDeSim){
							if(acha.posicao == (int)*u) {						
								valNumTotal += string(acha.valor);
							}
						}						
					}
					valNum = stod(valNumTotal);
				// se não, é outra coisa	
				} else {
					for(auto acha: tabelaDeSim) 
						if(acha.posicao == (int)*w) {
							if(acha.tipo == TK_IDENTIFIER) {
								tipoNum = acha.valor;
							} else if(acha.tipo == TK_T_INT 
									|| acha.tipo == TK_T_FRAC){
								num = (int)acha.tipo;
							}
						}	
				}
			}
			// salva em ambos vetores
			tNumerico.push_back(make_tuple(num, tipoNum, valNum));
			variaveis.push_back(tipoNum);
			
		// é instrumento
		} else if (arvInstr.out_neighbors(nodo).size() == 3) {
			int instr;
			string valInstr;
			
			Sii = arvInstr.out_neighbors(nodo);
			for(w = Sii.begin(); w != Sii.end(); w++) {
				for(auto acha: tabelaDeSim) {
					if(acha.posicao == (int)*w) 
						if(acha.tipo == TK_IDENTIFIER) {
							valInstr = acha.valor;
						} else if(acha.tipo != TK_SPIPE){
							instr = (int)acha.tipo;
						}
				}
			}
							
			tInstr.push_back(make_tuple(instr, valInstr));
			variaveis.push_back(valInstr);
		} 
	}
	
	// simbolos reservados 
	tInstr.push_back(make_tuple((int)TK_EOF, "#"));
	variaveis.push_back("#");
	tInstr.push_back(make_tuple((int)TK_EOF, "b"));
	variaveis.push_back("b");
	tInstr.push_back(make_tuple((int)TK_EOF, "N"));
	variaveis.push_back("N");

	// verifica multiplas declarações
	for(size_t i = 0; i < variaveis.size(); i++) {
		string var1 = variaveis.at(i);
		for(size_t j = i+1; j < variaveis.size(); j++) {
			string var2 = variaveis.at(j);
			if(!var1.compare(var2)) {
				cout << "Erro no bloco de instrumentos: múltiplas variáveis declaradas (" << var1 << ")" << endl;
				erro = true;
			}
		}
	}
	
	if(erro)
		exit(EXIT_FAILURE);
}

void semantic::vConfig() {
	p = arvConfig.begin();
	++p;

	Si = Graph::out_neighbors(p);
	for(t = Si.begin(); t != Si.end(); t++) {
		nodo = (int)*t;
		switch(nodo) {		// FALTA:
			case NTS_KEY: 	// verificar a variável declarada como tonalidade
				if(arvConfig.out_neighbors(nodo).size() > 6) {
					erro = true;
					cout << "Bloco de Configurações: Problema na definição de tonalidade ("
					<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
				}

			break;
			case NTS_TIME:
				if(arvConfig.out_neighbors(nodo).size() > 6) {
					erro = true;
					cout << "Bloco de Configurações: Mais de uma fórmula de compasso definida ("
					<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
				}
				// primeiro filho de NTS_TIME é numerador
				w = arvConfig.out_neighbors(nodo).begin();
				u = arvConfig.out_neighbors((int)*w).begin();
				int numerador;
				for(auto acha: tabelaDeSim)
					if(acha.posicao == (int)*u)
						numerador = stoi(acha.valor);
				if(!numerador) {
					cout << "Bloco de Configurações: Numerador não pode ser zero" << endl;
					erro = true;
				}
				
				// segundo filho é denominador
				++w;
				u = arvConfig.out_neighbors((int)*w).begin();
				int denominador;
				for(auto acha: tabelaDeSim)
					if(acha.posicao == (int)*u)
						denominador = stoi(acha.valor);
				if(!denominador) {
					erro = true;
					cout << "Bloco de Configurações: Denominador não pode ser zero" << endl;
				}

				// http://www.graphics.stanford.edu/~seander/bithacks.html
				if((denominador & (denominador-1)) != 0) {
					erro = true;
					cout << "Bloco de Configurações: Denominador precisa ser potência de 2" << endl;
				}

			break;
			case NTS_BPM:
				if(arvConfig.out_neighbors(nodo).size() > 4) {
					erro = true;
					cout << "Bloco de Configurações: Mais de um batimento definido ("
					<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
				}

				Sii = arvConfig.out_neighbors(nodo);
				for(w = Sii.begin(); w != Sii.end(); w++) {
					if((int)(*w/1000) == 434) {
						Siii = arvConfig.out_neighbors((int)*w);
						u = Siii.begin();

						int bpm;
						for(auto acha: tabelaDeSim) {
							if(acha.posicao == (int)*u)
								bpm = stoi(acha.valor);
						}
						if(!bpm) {
							erro = true;
							cout << "Bloco de Configurações: BPM não pode ser zero" << endl;
							// porém ainda não tem limite máximo
						}
					}
				}
			break;
		}
	}
	if(erro)
		exit(EXIT_FAILURE);
}

void semantic::vPartitura() {
	bool achou = false;
	
	for(p = arvPart.begin(); p != arvPart.end(); p++) {
		// pega compasso
		/*
		if(arvPart.node(p)/20000 == NTS_NOTES) {			
			Graph::vertex_set S = arvPart.out_neighbors(p);
			Graph testeTree = arvPart.subgraph(S);
			cout << "------ Pai:\n" << arvPart.node(p) << "\n------ Filhos:\n" << testeTree << endl;
		}
		*/
		if(arvPart.node(p)/25000 == NTS_NOTE) {			
			Graph::vertex_set S = arvPart.out_neighbors(p);
			
			t = S.begin();
			for(int i = 0; i < 2; i++) 
				t++;
				
			for(auto acha: tabelaDeSim) {
				// verifica se acha na tabela
				if((acha.posicao == (int)*t && acha.tipo == TK_IDENTIFIER)
					|| (acha.posicao == ((int)*t + 1) && acha.tipo == TK_IDENTIFIER)) {
					cout << acha.valor << endl;
					
					/* Procura valor na tupla (não funciona)
					auto achou = find_if(tNumerico.begin(), tNumerico.end(), [](decltype (*begin(tNumerico)) e) {
						return get<1>(e) == acha.valor;
					});
					if (achou != end(tNumerico)) {
						cout << "Found" << endl;
					}
					*/
				
				}
			}
			
			/*
			Graph testeTree = arvPart.subgraph(S);
			cout << "------ Pai:\n" << arvPart.node(p) << "\n------ Filhos:\n" << testeTree << endl;
			*/
		}
	}
	

	/* Ideia de checagem de quantidade dentro do compasso
	 * - Verificar na arvore existência de +|. A cada um desses, deve fechar +4 
	 * dependendo da fórmula de compasso
	 */ 
	
	// para cada nó da árvore
	for(p = arvPart.begin(); p != arvPart.end(); p++) {
		Si = Graph::out_neighbors(p);
		
		// itera sobre os filhos
		for(t = Si.begin(); t != Si.end(); t++) {
			for(auto acha: tabelaDeSim) {
				// verifica se acha na tabela
				if(acha.posicao == (int)*t && acha.tipo == TK_IDENTIFIER) {
					// verifica se foi declarado como instrumento
					for(size_t i = 0; i < tInstr.size(); i++) {
						auto tInstr1 = tInstr.at(i);
						if(!get<1>(tInstr1).compare(acha.valor)) {
							achou = true;
							break;
						}
					}
					// verifica se foi declarado como numeral
					for(size_t i = 0; i < tNumerico.size(); i++) {
						auto tNum1 = tNumerico.at(i);
						if(!get<1>(tNum1).compare(acha.valor)) {
							achou = true;
							break;
						}
					}
					// se não foi
					if(!achou) {
						erro = true;
						cout << "Variável não declarada (" << acha.valor << "). Linha: " << acha.linha << endl;
					}
					
					achou = false;
				}
			}
		}
	}
	if(erro)
		exit(EXIT_FAILURE);
		
	
}