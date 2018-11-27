#include "c7c.hpp"

parser::parser(vector<token> tabelaDeSimbolos) {
	// vai funcionar como uma pilha
	copiaTabela = tabelaDeSimbolos;
	cout << "**********\nInício da análise sintática..." << endl;
	sintatico();
	cout << "Fim da análise sintática, não foram encontrados erros.\n**********" << endl;
}

void parser::sintatico() {	
	// Se existe a raíz
	if(!(copiaTabela.size() > 0)) {
		cout << "ERRO: Código-fonte vazio." << endl;
		exit(EXIT_FAILURE);
	}
			
	// limpa os EOF
	size_t tamanhoTabela = copiaTabela.size(), iterador = 0;
	while(iterador < tamanhoTabela) {
		if(copiaTabela.at(iterador).tipo == TK_EOF) {
			copiaTabela.erase(copiaTabela.begin()+iterador);
			tamanhoTabela = copiaTabela.size();
		}
		++iterador;
	}
	
	try {
		/* ================ SEÇÃO AUTORIA ================ */
		// ponteiros pro comeco da pilha - facilita a escrita
		tipo = &copiaTabela.front().tipo;
		posicao = &copiaTabela.front().posicao;
		// Variável inútil
		string removedorDeWarning2 = nomeToken(copiaTabela.front().tipo);
		
		// Pesquisa por \author
		if(*tipo == TK_AUTHOR) {
			// insere na arvore
			arvore.insert_edge(NTS_PROG, NTS_SAUTH);
			Aautor.insert_edge(NTS_SAUTH, *posicao);
			Aautor.insert_edge(NTS_SAUTH, NTS_AUTH);
			
			// remove da pilha
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 1;
		
		// deriva bloco de autoria
		autoria();
		
		// procura || do final
		if(*tipo == TK_DPIPE) {
			Aautor.insert_edge(NTS_SAUTH, *posicao);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 11;
		/* ======================================== */
		
		
		/* ================ SEÇÃO INSTRUMENTOS ================ */
		// Pesquisa \instruments
		if(*tipo == TK_INSTRUMENTS) {
			arvore.insert_edge(NTS_PROG, NTS_SINSTR);
			Ainstrumentos.insert_edge(NTS_SINSTR, *posicao);
			Ainstrumentos.insert_edge(NTS_SINSTR, NTS_INSTRUMENTS);
			
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			throw 2;
			
		// deriva bloco de instruments
		instrumentos();
		
		// procura || do instruments
		if(*tipo == TK_DPIPE) {
			Ainstrumentos.insert_edge(NTS_SINSTR, *posicao);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 12;
		/* ======================================== */
		
		/* ================ SEÇÃO CONFIGURACAO ================ */
		// Pesquisa \setup
		if(*tipo == TK_SETUP) {
			arvore.insert_edge(NTS_PROG, NTS_SSETUP);
			Aconfig.insert_edge(NTS_SSETUP, *posicao);
			Aconfig.insert_edge(NTS_SSETUP, NTS_SETUP);
			
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			throw 3;
			
		// deriva bloco de setup
		configuracao();
		
		// procura || do setup
		if(*tipo == TK_DPIPE) {
			Aconfig.insert_edge(NTS_SSETUP, *posicao);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 13;
		/* ======================================== */
		
		/* ================ SEÇÃO PARTITURA ================ */
		// Pesquisa \sheetmusic
		if(*tipo == TK_SHEETMUSIC) {
			arvore.insert_edge(NTS_PROG, NTS_SSHEET);
			Apartitura.insert_edge(NTS_SSHEET, *posicao);
			Apartitura.insert_edge(NTS_SSHEET, NTS_SHEET);
			
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			throw 4;
			
		// deriva bloco de setup
		partitura();
		
		// procura || do setup
		if(*tipo == TK_DPIPE) {
			Apartitura.insert_edge(NTS_SSHEET, *posicao);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 14;
		/* ======================================== */
		
		/* ================ FIM DAS SEÇÕES ================ */
		if(copiaTabela.size() > 0) 
			throw 46;
		
		arvore += Aautor;
		arvore += Ainstrumentos;
		arvore += Aconfig;
		arvore += Apartitura;
		
		todasArvores.push_back(Aautor);
		todasArvores.push_back(Ainstrumentos);
		todasArvores.push_back(Aconfig);
		todasArvores.push_back(Apartitura);
		todasArvores.push_back(arvore);
		
	} catch(int erroBloco) {
		localizaErro(erroBloco);
	}
}

/* ================== ABRE AUTORIA ================== */
void parser::autoria() {
	erro = false;
	
	// caso nao tenha nada definido
	if(*tipo == (TK_DPIPE|TK_INSTRUMENTS))
		return;
	
	// enquanto não acha fim do bloco	
	while(*tipo != TK_DPIPE) {
		// pesquisa declaracao
		switch(*tipo) {
			case TK_TITLE:
				qualNo = NTS_TITLE;
				Aautor.insert_edge(NTS_AUTH, qualNo);
				Aautor.insert_edge(qualNo, *posicao);
			break;
			case TK_SUBTITLE:
				qualNo = NTS_SUBTITLE;
				Aautor.insert_edge(NTS_AUTH, qualNo);
				Aautor.insert_edge(qualNo, *posicao);
			break;
			case TK_COMPOSITOR:
				qualNo = NTS_COMPOSITOR;
				Aautor.insert_edge(NTS_AUTH, qualNo);
				Aautor.insert_edge(qualNo, *posicao);
			break;
			case TK_COPYRIGHT:
				qualNo = NTS_COPYRIGHT;
				Aautor.insert_edge(NTS_AUTH, qualNo);
				Aautor.insert_edge(qualNo, *posicao);
			break;
			default:
				localizaErro(21);
		}
		copiaTabela.erase(copiaTabela.begin());
		
		// pesquisa dois pontos
		if(*tipo == TK_DDOT) {
			Aautor.insert_edge(qualNo, *posicao);
		} else 
			localizaErro(30);
		
		copiaTabela.erase(copiaTabela.begin());
			
		// define string
		if(*tipo == TK_DQUOTE) {
			Aautor.insert_edge(qualNo, NTS_STRING*10+contadorString+1);
			//NGraph::Graph AautorS;
			Aautor.insert_edge(NTS_STRING*10+contadorString+1, *posicao);
			Aautor += stringC7();
			//Aautor += AautorS;
		} else 
			localizaErro(22);
		
		copiaTabela.erase(copiaTabela.begin());
		
		// procura |
		if(*tipo == TK_SPIPE) {
			Aautor.insert_edge(qualNo, *posicao);
		} else 
			localizaErro(28);
		
		copiaTabela.erase(copiaTabela.begin());
		
		// caso especial
		if(*tipo == TK_INSTRUMENTS) 
			break;
	}
}

/* ================== ABRE INSTRUMENTOS ================== */
void parser::instrumentos() {
	bool tipoDeIdent, fracional;
	
	while(*tipo != TK_DPIPE) {
		
		++contadorInstrumento;
		int pos = (int)NTS_INSTRUMENT*100+contadorInstrumento;
		Ainstrumentos.insert_edge(NTS_INSTRUMENTS, pos);
		
		// sempre comeca com tipo
		if((*tipo == TK_S_VIOLIN) 
			|| (*tipo == TK_S_VIOLA)
			|| (*tipo == TK_S_CELLO)) {
				
			tipoDeIdent = false;
			
		} else if(*tipo == TK_T_FRAC) {
			fracional = true;
			tipoDeIdent = true;
			
		} else if(*tipo == TK_T_INT) {
			fracional = false;
			tipoDeIdent = true;
		}else 
			localizaErro(24);
		
		
		Ainstrumentos.insert_edge(pos, *posicao);
		
		copiaTabela.erase(copiaTabela.begin());
		
		if(*tipo == TK_IDENTIFIER) {
			Ainstrumentos.insert_edge(pos, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		} else 
			localizaErro(25);
			
		if(tipoDeIdent) {
			if(*tipo == TK_EQUAL) {
				Ainstrumentos.insert_edge(pos, *posicao);
				copiaTabela.erase(copiaTabela.begin());
			} else 
				localizaErro(26);
			
			++contadorNumero;
			NGraph::Graph AinstrumentosN;
			AinstrumentosN.insert_edge(pos, NTS_NUMBER*1000+contadorNumero);
			AinstrumentosN += numeros();
			Ainstrumentos += AinstrumentosN;
			
			if(numFrac && !fracional) 
				localizaErro(27);
		}
		
		if(*tipo == TK_SPIPE) {
			Ainstrumentos.insert_edge(pos, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		} else 
			localizaErro(28);
		
		if(*tipo == TK_SETUP)
			break;
	}
}

/* ================== ABRE CONFIGURACAO ================== */
void parser::configuracao() {
	bool key, time;
	key = false;
	time = false;
	
	while(*tipo != TK_DPIPE) {
		switch(*tipo) {
			case TK_KEY:
				key = true;
				qualNo = NTS_KEY;
				Aconfig.insert_edge(NTS_SETUP, qualNo);
				Aconfig.insert_edge(qualNo, *posicao);
			break;
			case TK_TIME:
				time = true;
				qualNo = NTS_TIME;
				Aconfig.insert_edge(NTS_SETUP, qualNo);
				Aconfig.insert_edge(qualNo, *posicao);
			break;
			case TK_BPM:
				qualNo = NTS_BPM;
				Aconfig.insert_edge(NTS_SETUP, qualNo);
				Aconfig.insert_edge(qualNo, *posicao);
			break;
			default:
				localizaErro(29);
		}
		copiaTabela.erase(copiaTabela.begin());
		
		// pesquisa dois pontos
		if(*tipo == TK_DDOT) {
			Aconfig.insert_edge(qualNo, *posicao);
		} else 
			localizaErro(30);
		
		copiaTabela.erase(copiaTabela.begin());
		
		if(key && !time) {
			if(*tipo == TK_IDENTIFIER) {
				Aconfig.insert_edge(qualNo, *posicao);
				copiaTabela.erase(copiaTabela.begin());
			} else 
				localizaErro(31);
			
			if(*tipo == TK_SHARP || !copiaTabela.front().valor.compare("b")) {
				Aconfig.insert_edge(qualNo, *posicao);
				copiaTabela.erase(copiaTabela.begin());
			} 
			
			key = false;
			
		} else if(time && !key) {
			++contadorNumero;
			Aconfig.insert_edge(qualNo, NTS_NUMBER*1000+contadorNumero);
			
			Aconfig += numeros();
			if(numFrac) 
				localizaErro(32);
			
			if(*tipo == TK_SLASH) {
				Aconfig.insert_edge(qualNo, *posicao);
			} else 
				localizaErro(33);
			
			copiaTabela.erase(copiaTabela.begin());
			
			++contadorNumero;
			Aconfig.insert_edge(qualNo, NTS_NUMBER*1000+contadorNumero);
			
			Aconfig += numeros();
			if(numFrac) 
				localizaErro(32);
			
			time = false;
			
		} else {
			++contadorNumero;
			Aconfig.insert_edge(qualNo, NTS_NUMBER*1000+contadorNumero);
			Aconfig += numeros();
		}

		// procura |
		if(*tipo == TK_SPIPE) {
			Aconfig.insert_edge(qualNo, *posicao);
		} else 
			localizaErro(28);
		
		copiaTabela.erase(copiaTabela.begin());
		
		if(*tipo == TK_SHEETMUSIC)
			break;
	}
}

/* ================== ABRE PARTITURA ================== */
void parser::partitura() {
	bool ritornelo = false;
	
	while(*tipo != TK_DPIPE){
		if(*tipo == TK_PIPEDDOT) {
			Apartitura.insert_edge(NTS_SHEET, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
			ritornelo = true;
			
		} else if(*tipo != TK_IDENTIFIER) 
			localizaErro(36);
		
		linhasMusicais();
		
		while(ritornelo) {
			if(*tipo == TK_NUMBER && copiaTabela.at(1).tipo == TK_DOT) {
				
				Apartitura.insert_edge(NTS_SHEET, *posicao);
				copiaTabela.erase(copiaTabela.begin());
				
				Apartitura.insert_edge(NTS_SHEET, *posicao);
				copiaTabela.erase(copiaTabela.begin());
				
			} else if(*tipo == TK_DDOTPIPE) {
				Apartitura.insert_edge(NTS_SHEET, *posicao);
				copiaTabela.erase(copiaTabela.begin());
				
				ritornelo = false;
			} else 
				localizaErro(37);
			
			linhasMusicais();
		}
	}
}


void parser::linhasMusicais() {
	++contadorCompasso;
	int compasso = NTS_COMPASS*20+contadorCompasso;
	
	while(*tipo == TK_IDENTIFIER) {
		Apartitura.insert_edge(NTS_SHEET, compasso);
		// verifica tudo de uma vez
		if(copiaTabela.at(0).tipo == TK_IDENTIFIER 
			&& copiaTabela.at(1).tipo == TK_DOT
			&& copiaTabela.at(2).tipo == TK_NUMBER
			&& copiaTabela.at(3).tipo == TK_EQUAL) {
				
			
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			localizaErro(45);
		
		// opcional
		if(*tipo == TK_SUM) {
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		}
		
		//adicao de notas
		adicionaNotas(compasso);
		
		
		while(*tipo == TK_COMPASSUM) {
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
			adicionaNotas(compasso);
		}
		
		if(*tipo == TK_SPIPE
			|| *tipo == TK_SUM) {
				
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			localizaErro(39);
	}
}


void parser::adicionaNotas(int compasso) {
	if(*tipo != TK_NUMBER) 
		localizaErro(40);
	
	while(*tipo == TK_NUMBER) {
		Apartitura.insert_edge(compasso, *posicao);
		copiaTabela.erase(copiaTabela.begin());
		
		if(*tipo == TK_DOT) {
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		} else 
			localizaErro(41);
		
		if(*tipo == TK_IDENTIFIER) {
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		} else 
			localizaErro(42);
			
		//opcional # b N
		if(*tipo == TK_SHARP 
			|| !copiaTabela.front().valor.compare("b")
			|| !copiaTabela.front().valor.compare("N")) {
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		}
		
		if(*tipo == TK_DOT) {
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		} else 
			localizaErro(41);
		
		if(*tipo == TK_IDENTIFIER) {
			Apartitura.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
		} else {
			++contadorNumero;
			Apartitura.insert_edge(compasso, NTS_NUMBER*1000+contadorNumero);
			Apartitura += numeros();
		}
	}
}

NGraph::Graph parser::stringC7() {
	NGraph::Graph grafoS;
	copiaTabela.erase(copiaTabela.begin());
	++contadorString;
	regex simbolos("[\\-*\\,*\\.*\\#*\\;*\\\"*\\:*\\|*\\\\*\\+*\\/*\\=*]*");
	
	while(*tipo != TK_DQUOTE) {
		if(*tipo == TK_IDENTIFIER || *tipo == TK_NUMBER || regex_match(copiaTabela.front().valor, simbolos)) {
			grafoS.insert_edge(NTS_STRING*10+contadorString, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			localizaErro(44);
	}
	
	// se sai do while, entao achou "
	grafoS.insert_edge(NTS_STRING*10+contadorString, *posicao);
	
	return grafoS;
}

NGraph::Graph parser::numeros() {
	NGraph::Graph grafoN;
	if(*tipo == TK_NUMBER) {
		grafoN.insert_edge(NTS_NUMBER*1000+contadorNumero, *posicao);
		copiaTabela.erase(copiaTabela.begin());
	} else 
		localizaErro(45);
	
	// verdadeiro se for frac
	if(*tipo == TK_DOT && copiaTabela.at(1).tipo == TK_NUMBER) {
		grafoN.insert_edge(NTS_NUMBER*1000+contadorNumero, *posicao);
		copiaTabela.erase(copiaTabela.begin());
		
		grafoN.insert_edge(NTS_NUMBER*1000+contadorNumero, *posicao);
		copiaTabela.erase(copiaTabela.begin());
		numFrac = true;
	} else
		numFrac = false;
		
	return grafoN;
}


void parser::localizaErro(int valErro) {
	cout << "ERRO (" << valErro << "): ";
	switch(valErro) {
		case 1:
			cout << "Falta bloco de autoria (\\author)" << endl;
		break;
		case 11:
			cout << "Falta final do bloco de autoria (\\author)" << endl;
		break;
		case 2:
			cout << "Falta bloco de instrumentos (\\instruments)" << endl;
		break;
		case 12:
			cout << "Falta final do bloco de instrumentos (\\instruments)" << endl;
		break;
		case 3:
			cout << "Falta bloco de configurações (\\setup)" << endl;
		break;
		case 13:
			cout << "Falta final do bloco de configurações (\\setup)" << endl;
		break;
		case 4:
			cout << "Falta bloco de partitura (\\sheetmusic)" << endl;
		break;
		case 14:
			cout << "Falta final do bloco de (\\sheetmusic)" << endl;
		break;
		case 21:
			cout << "Falta declaração de tipo" << endl;
		break;
		case 22:
			cout << "Falta inicio de string" << endl;
		break;
		case 24:
			cout << "Tipo não reconhecido" << endl;
		break;
		case 25:
			cout << "Esperava-se um identificador" << endl;
		break;
		case 26:
			cout << "Falta atribuição" << endl;
		break;
		case 27:
			cout << "Declaração de inteiro e atribuição de fracional" << endl;
		break;
		case 28:
			cout << "Falta fim de linha (|)" << endl;
		break;
		case 29:
			cout << "Especificador não reconhecido" << endl;
		break;
		case 30:
			cout << "Falta dois pontos depois da declaração" << endl;
		break;
		case 31:
			cout << "Nota não reconhecida" << endl;
		break;
		case 32:
			cout << "Fracionais não são aceitos na formula de compasso" << endl;
		break;
		case 33:
			cout << "Falta divisor (/) da fórmula de compasso" << endl;
		break;
		case 36:
			cout << "Erro na elaboração de compasso" << endl;
		break;
		case 37:
			cout << "Ritornelo mal construído" << endl;
		break;
		case 39:
			cout << "Compasso mal terminado" << endl;
		break;
		case 40:
			cout << "Altura precisa ser definida por um número inteiro" << endl;
		break;
		case 41:
			cout << "Falta de pontuação" << endl;
		break;
		case 42:
			cout << "Nota não reconhecida" << endl;
		break;
		case 44:
			cout << "Erro em string" << endl;
		break;
		case 45:
			cout << "Atribuição invalida" << endl;
		break;
		case 46:
			cout << "Conteúdo desconhecido após fim do bloco de partitura" << endl;
	}
	
	cout << "Próximo de: ";
	
	if(copiaTabela.size() > 5) {
		cout << "\n\t... ";
		vector<token>::iterator print;
		for(print = copiaTabela.begin(); print != copiaTabela.begin()+4; print++)
			cout << print->valor << " ";
	
		cout << " ..." << endl;
	} else {
		cout << copiaTabela.front().valor;
	}

	cout << " na linha: " << copiaTabela.front().linha 
		<< " e coluna: " << copiaTabela.front().coluna << endl;
		
	cout << "Existe(m) erro(s) na sintática do código-fonte" << endl;
	exit(EXIT_FAILURE);
}

