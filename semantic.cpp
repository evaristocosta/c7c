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
	
	//cout << arvInstr;
	
	// verifica vars de autor
	vAutor();
	
	vInstrumentos();
	
	vConfig();
	
	cout << arvPart;
	
	cout << "Fim da análise semântica, não foram encontrados erros.\n**********" << endl;
}

void semantic::vAutor() {

	// p no nó SAUTH (não permite +1)
	Graph::const_iterator p = arvAutor.begin();
	// p no nó AUTH
	++p;
	
	// verifica a quantidade de filhos de cada nó de AUTH; deve ser 4, pelas regras
	Si = Graph::out_neighbors(p);
	for(t = Si.begin(); t != Si.end(); t++) {
		nodo = (int)*t;
		if(arvAutor.out_neighbors(nodo).size() > 4)
			cout << "Atribuições repetidas no bloco de autoria (" 
				<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
	}
}

void semantic::vInstrumentos() {
	Graph::const_iterator p = arvInstr.begin();
	++p;

	Si = Graph::out_neighbors(p);
	for(t = Si.begin(); t != Si.end(); t++) {
		nodo = (int)*t;	
		if(arvInstr.out_neighbors(nodo).size() == 5) {
			// é numInt ou numFrac
			int num;
			string tipoNum;
			float valNum;
			
			Sii = arvInstr.out_neighbors(nodo);
			for(w = Sii.begin(); w != Sii.end(); w++) {
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
			tNumerico.push_back(make_tuple(num, tipoNum, valNum));
		} else if (arvInstr.out_neighbors(nodo).size() == 3) {
			// é instrumento
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
			
		} 
	}
	
	bool erro = false;
	for(size_t i = 0; i < tNumerico.size(); i++) {
		auto tNum1 = tNumerico.at(i);
		for(size_t j = i+1; j < tNumerico.size(); j++) {
			auto tNum2 = tNumerico.at(j);
			if(get<1>(tNum1) == get<1>(tNum2)) {
				cout << "Erro: multiplas variáveis declaradas (" << get<1>(tNum1) << ")" << endl;
				erro = true;
			}
		}
	}
	
	for(size_t i = 0; i < tInstr.size(); i++) {
		auto tNum1 = tInstr.at(i);
		for(size_t j = i+1; j < tInstr.size(); j++) {
			auto tNum2 = tInstr.at(j);
			if(get<1>(tNum1) == get<1>(tNum2)) {
				cout << "Erro: multiplas variáveis declaradas (" << get<1>(tNum1) << ")" << endl;
				erro = true;
			}
		}
		
	}
	
	if(erro)
		exit(EXIT_FAILURE);
}

void semantic::vConfig() {
	Graph::const_iterator p = arvConfig.begin();
	++p;
	
	Si = Graph::out_neighbors(p);
	for(t = Si.begin(); t != Si.end(); t++) {
		nodo = (int)*t;
		switch(nodo) {		// FALTA:
			case NTS_KEY: // casos especiais, e verificar variável declarada
				if(arvConfig.out_neighbors(nodo).size() > 5)
					cout << "Problema na definição de tonalidade (" 
					<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
			
			break;
			case NTS_TIME: 
				if(arvConfig.out_neighbors(nodo).size() > 6)
					cout << "Mais de uma fórmula de compasso definida (" 
					<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
				
				w = arvConfig.out_neighbors(nodo).begin();
				u = arvConfig.out_neighbors((int)*w).begin();
				int numerador;
				for(auto acha: tabelaDeSim) 
					if(acha.posicao == (int)*u)
						numerador = stoi(acha.valor);
				if(!numerador)
					cout << "Numerador não pode ser zero" << endl;
				
				++w;
				u = arvConfig.out_neighbors((int)*w).begin();
				int denominador;
				for(auto acha: tabelaDeSim) 
					if(acha.posicao == (int)*u)
						denominador = stoi(acha.valor);
				if(!denominador)
					cout << "Denominador não pode ser zero" << endl;
					
				// http://www.graphics.stanford.edu/~seander/bithacks.html	
				if((denominador & (denominador-1)) != 0)
					cout << "Denominador precisa ser potência de 2" << endl;
			
			break;
			case NTS_BPM: 
				if(arvConfig.out_neighbors(nodo).size() > 4)
					cout << "Mais de um batimento definido (" 
					<< nomeNaoTerminal((naoTerminais)nodo) << ")" << endl;
					
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
						if(!bpm)
							cout << "Erro: BPM não pode ser zero" << endl;
					}
				}
			break;		
		}
	}
}