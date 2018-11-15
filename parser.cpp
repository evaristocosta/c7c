#include "c7c.hpp"

parser::parser(vector<token> tabelaDeSimbolos) {
	// vai funcionar como uma pilha
	copiaTabela = tabelaDeSimbolos;
	
	sintatico();
}

void parser::sintatico() {	
	// Se existe a raíz
	if(!(copiaTabela.size() > 0)) {
		cout << "Código-fonte vazio" << endl;
		exit(0);
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
		
		// Pesquisa por \author
		if(*tipo == TK_AUTHOR) {
			// insere na arvore
			arvore.insert_edge(NTS_PROG, NTS_SAUTH);
			arvore.insert_edge(NTS_SAUTH, *posicao);
			arvore.insert_edge(NTS_SAUTH, NTS_AUTH);
			
			// remove da pilha
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 1;
		
		// deriva bloco de autoria
		autoria();
		
		// procura || do final
		if(*tipo == TK_DPIPE) {
			arvore.insert_edge(NTS_SAUTH, *posicao);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 11;
		/* ======================================== */
		
		
		/* ================ SEÇÃO INSTRUMENTOS ================ */
		// Pesquisa \instruments
		if(*tipo == TK_INSTRUMENTS) {
			arvore.insert_edge(NTS_PROG, NTS_SINSTR);
			arvore.insert_edge(NTS_SINSTR, *posicao);
			arvore.insert_edge(NTS_SINSTR, NTS_INSTRUMENTS);
			
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			throw 2;
			
		// deriva bloco de instruments
		instrumentos();
		
		// procura || do instruments
		if(*tipo == TK_DPIPE) {
			arvore.insert_edge(NTS_SINSTR, *posicao);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 12;
		/* ======================================== */
		
		/* ================ SEÇÃO CONFIGURACAO ================ */
		// Pesquisa \setup
		if(*tipo == TK_SETUP) {
			arvore.insert_edge(NTS_PROG, NTS_SSETUP);
			arvore.insert_edge(NTS_SSETUP, *posicao);
			arvore.insert_edge(NTS_SSETUP, NTS_SETUP);
			
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			throw 3;
			
		// deriva bloco de setup
		configuracao();
		
		// procura || do setup
		if(*tipo == TK_DPIPE) {
			arvore.insert_edge(NTS_SSETUP, *posicao);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 13;
		/* ======================================== */
		
		/* ================ SEÇÃO PARTITURA ================ */
		// Pesquisa \sheetmusic
		if(*tipo == TK_SHEETMUSIC) {
			arvore.insert_edge(NTS_PROG, NTS_SSHEET);
			arvore.insert_edge(NTS_SSHEET, *posicao);
			arvore.insert_edge(NTS_SSHEET, NTS_SHEET);
			
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			throw 4;
			
		// deriva bloco de setup
		partitura();
		
		// procura || do setup
		if(*tipo == TK_DPIPE) {
			arvore.insert_edge(NTS_SSHEET, *posicao);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 14;
		/* ======================================== */
		
		/* ================ FIM DAS SEÇÕES ================ */
		// Se ainda houver algo no vetor, erro
		
	} catch(int erroBloco) {
		switch(erroBloco) {
			case 1:
				cout << "Falta bloco de \\author" << endl;
			break;
			case 11:
				cout << "Falta || de \\author" << endl;
			break;
			case 2:
				cout << "Falta bloco de \\instruments" << endl;
			break;
			case 12:
				cout << "Falta || de \\instruments" << endl;
			break;
			case 3:
				cout << "Falta bloco de \\setup" << endl;
			break;
			case 13:
				cout << "Falta || de \\setup" << endl;
			break;
			case 4:
				cout << "Falta bloco de \\sheetmusic" << endl;
			break;
			case 14:
				cout << "Falta || de \\sheetmusic" << endl;
			break;
		}
		cout << "Existe(m) erro(s) na sintática do código-fonte" << endl;
		exit(0);
	}
	
	
}

/* ================== ABRE AUTORIA ================== */
void parser::autoria() {
	erro = false;
	
	// caso nao tenha nada definido
	if(*tipo == (TK_DPIPE|TK_INSTRUMENTS))
		return;
	
	// enquanto não acha fim do bloco	
	while(*tipo != TK_DPIPE && !erro) {
		// pesquisa declaracao
		switch(*tipo) {
			case TK_TITLE:
				qualNo = NTS_TITLE;
				arvore.insert_edge(NTS_AUTH, qualNo);
				arvore.insert_edge(qualNo, *posicao);
			break;
			case TK_SUBTITLE:
				qualNo = NTS_SUBTITLE;
				arvore.insert_edge(NTS_AUTH, qualNo);
				arvore.insert_edge(qualNo, *posicao);
			break;
			case TK_COMPOSITOR:
				qualNo = NTS_COMPOSITOR;
				arvore.insert_edge(NTS_AUTH, qualNo);
				arvore.insert_edge(qualNo, *posicao);
			break;
			case TK_COPYRIGHT:
				qualNo = NTS_COPYRIGHT;
				arvore.insert_edge(NTS_AUTH, qualNo);
				arvore.insert_edge(qualNo, *posicao);
			break;
			default:
				cout << "Falta declaracao de tipo" << endl;
				localizaErro();
		}
		copiaTabela.erase(copiaTabela.begin());
		
		// pesquisa dois pontos
		if(*tipo == TK_DDOT) {
			arvore.insert_edge(qualNo, *posicao);
		} else {
			cout << "Falta dois pontos depois da declaração" << endl;
			localizaErro();
		}
		copiaTabela.erase(copiaTabela.begin());
			
		// define string
		if(*tipo == TK_DQUOTE) {
			arvore.insert_edge(qualNo, NTS_STRING*10+contadorString);
			arvore.insert_edge(NTS_STRING*10+contadorString, *posicao);
			stringC7();
		} else {
			cout << "Falta inicio de string";
			localizaErro();
		}
		copiaTabela.erase(copiaTabela.begin());
		
		// procura |
		if(*tipo == TK_SPIPE) {
			arvore.insert_edge(qualNo, *posicao);
		} else {
			cout << "Falta delimitador de linha" << endl;
			localizaErro();
		}
		copiaTabela.erase(copiaTabela.begin());
		
		// caso especial
		if(*tipo == TK_INSTRUMENTS) 
			erro = true;
	}
}

/* ================== ABRE INSTRUMENTOS ================== */
void parser::instrumentos() {
	bool tipoDeIdent, fracional;
	
	while(*tipo != TK_DPIPE && !erro) {
		
		++contadorInstrumento;
		int pos = (int)NTS_INSTRUMENT*100+contadorInstrumento;
		arvore.insert_edge(NTS_INSTRUMENTS, pos);
		
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
		}else {
			cout << "Tipo não reconhecido" << endl;
			localizaErro();
		}
		
		
		arvore.insert_edge(pos, *posicao);
		
		copiaTabela.erase(copiaTabela.begin());
		
		if(*tipo == TK_IDENTIFIER) {
			arvore.insert_edge(pos, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		} else {
			cout << "Esperava-se um identificador" << endl;
			localizaErro();
		}
			
		if(tipoDeIdent) {
			if(*tipo == TK_EQUAL) {
				arvore.insert_edge(pos, *posicao);
				copiaTabela.erase(copiaTabela.begin());
			} else {
				cout << "Falta atribuicao" << endl;
				localizaErro();
			}
			
			arvore.insert_edge(pos, NTS_NUMBER);
			
			if(numeros() && !fracional) {
				cout << "Declaracao de inteiro e atribuicao de fracional" << endl;
				localizaErro();
			}
		}
		
		if(*tipo == TK_SPIPE) {
			arvore.insert_edge(pos, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		} else {
			cout << "Falta fim de linha" << endl;
			localizaErro();
		}
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
				arvore.insert_edge(NTS_SETUP, qualNo);
				arvore.insert_edge(qualNo, *posicao);
			break;
			case TK_TIME:
				time = true;
				qualNo = NTS_TIME;
				arvore.insert_edge(NTS_SETUP, qualNo);
				arvore.insert_edge(qualNo, *posicao);
			break;
			case TK_BPM:
				qualNo = NTS_BPM;
				arvore.insert_edge(NTS_SETUP, qualNo);
				arvore.insert_edge(qualNo, *posicao);
			break;
			default:
				cout << "Especificador nao reconhecido" << endl;
				localizaErro();
		}
		copiaTabela.erase(copiaTabela.begin());
		
		// pesquisa dois pontos
		if(*tipo == TK_DDOT) {
			arvore.insert_edge(qualNo, *posicao);
		} else {
			cout << "Falta dois pontos depois da declaração" << endl;
			localizaErro();
		}
		copiaTabela.erase(copiaTabela.begin());
		
		if(key && !time) {
			if(*tipo == TK_IDENTIFIER) {
				arvore.insert_edge(qualNo, *posicao);
				copiaTabela.erase(copiaTabela.begin());
			} else {
				cout << "Nota nao reconhecida" << endl;
				localizaErro();
			}
			
			if(*tipo == TK_SHARP || !copiaTabela.front().valor.compare("b")) {
				arvore.insert_edge(qualNo, *posicao);
				copiaTabela.erase(copiaTabela.begin());
			} 
			
			key = false;
			
		} else if(time && !key) {
			arvore.insert_edge(qualNo, NTS_NUMBER*1000+contadorNumero+1);
			
			if(numeros()) {
				cout << "Fracionais não sao aceitos na formula de compasso" << endl;
				localizaErro();
			}
			
			if(*tipo == TK_SLASH) {
				arvore.insert_edge(qualNo, *posicao);
			} else {
				cout << "Falta / da formula" << endl;
				localizaErro();
			}
			copiaTabela.erase(copiaTabela.begin());
			
			if(numeros()) {
				cout << "Fracionais não sao aceitos na formula de compasso" << endl;
				localizaErro();
			}
			
			time = false;
			
		} else {
			arvore.insert_edge(qualNo, NTS_NUMBER*1000+contadorNumero+1);
			numeros();
		}

		// procura |
		if(*tipo == TK_SPIPE) {
			arvore.insert_edge(qualNo, *posicao);
		} else {
			cout << "Falta delimitador de linha" << endl;
			localizaErro();
		}
		copiaTabela.erase(copiaTabela.begin());
	}
}

/* ================== ABRE PARTITURA ================== */
void parser::partitura() {
	bool ritornelo = false;
	
	while(*tipo != TK_DPIPE){
		if(*tipo == TK_PIPEDDOT) {
			arvore.insert_edge(NTS_SHEET, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
			ritornelo = true;
			
		} else if(*tipo != TK_IDENTIFIER) {
			cout << "Esperava-se um identificador" << endl;
			localizaErro();
		}
		
		linhasMusicais();
		
		while(ritornelo) {
			if(*tipo == TK_NUMBER && copiaTabela.at(1).tipo == TK_DOT) {
				
				arvore.insert_edge(NTS_SHEET, *posicao);
				copiaTabela.erase(copiaTabela.begin());
				
				arvore.insert_edge(NTS_SHEET, *posicao);
				copiaTabela.erase(copiaTabela.begin());
				
			} else if(*tipo == TK_DDOTPIPE) {
				arvore.insert_edge(NTS_SHEET, *posicao);
				copiaTabela.erase(copiaTabela.begin());
				
				ritornelo = false;
			} else {
				cout << "Erro em repeticao" << endl;
				localizaErro();
			}
			
			linhasMusicais();
		}
	}
}


void parser::linhasMusicais() {
	++contadorCompasso;
	int compasso = NTS_COMPASS*20+contadorCompasso;
	
	while(*tipo == TK_IDENTIFIER) {
		arvore.insert_edge(NTS_SHEET, compasso);
		// verifica tudo de uma vez
		if(copiaTabela.at(0).tipo == TK_IDENTIFIER 
			&& copiaTabela.at(1).tipo == TK_DOT
			&& copiaTabela.at(2).tipo == TK_NUMBER
			&& copiaTabela.at(3).tipo == TK_EQUAL) {
				
			
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
		} else {
			cout << "Atribuicao de notas mal declarada" << endl;
			localizaErro();
		}
		
		// opcional
		if(*tipo == TK_SUM) {
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		}
		
		//adicao de notas
		adicionaNotas(compasso);
		
		
		while(*tipo == TK_COMPASSUM) {
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
			adicionaNotas(compasso);
		}
		
		if(*tipo == TK_SPIPE
			|| *tipo == TK_SUM) {
				
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
		} else {
			cout << "Compasso mal terminado" << endl;
			localizaErro();
		}
	}
}


void parser::adicionaNotas(int compasso) {
	if(*tipo != TK_NUMBER) {
		cout << "Altura precisa ser definida por um número inteiro" << endl;
		localizaErro();
	}
	
	while(*tipo == TK_NUMBER) {
		arvore.insert_edge(compasso, *posicao);
		copiaTabela.erase(copiaTabela.begin());
		
		if(*tipo == TK_DOT) {
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		} else {
			cout << "Falta de pontuacao" << endl;
			localizaErro();
		}
			
		
		if(*tipo == TK_IDENTIFIER) {
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		} else {
			cout << "Nota nao reconhecida" << endl;
			localizaErro();
		}
			
		//opcional # b bequadro
		if(*tipo == TK_SHARP 
			|| !copiaTabela.front().valor.compare("b")) {
			//|| copiaTabela.front().valor.compare(bequadro)
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		}
		
		if(*tipo == TK_DOT) {
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
		} else {
			cout << "Falta de pontuacao" << endl;
			localizaErro();
		}
		
		if(*tipo == TK_IDENTIFIER) {
			arvore.insert_edge(compasso, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
		} else {
			arvore.insert_edge(compasso, NTS_NUMBER*1000+contadorNumero-1);
			numeros();
		}
	}
}

void parser::stringC7() {
	copiaTabela.erase(copiaTabela.begin());
	++contadorString;
	regex simbolos("[\\-*\\,*\\.*\\#*\\;*\\\"*\\:*\\|*\\\\*\\+*\\/*\\=*]*");
	
	while(*tipo != TK_DQUOTE) {
		if(*tipo == TK_IDENTIFIER || *tipo == TK_NUMBER || regex_match(copiaTabela.front().valor, simbolos)) {
			arvore.insert_edge(NTS_STRING*10+contadorString, *posicao);
			copiaTabela.erase(copiaTabela.begin());
			
		} else {
			cout << "Erro em string" << endl;
			exit(0);
		}
	}
	
	// se sai do while, entao achou "
	arvore.insert_edge(NTS_STRING*10+contadorString, *posicao);
}

bool parser::numeros() {
	++contadorNumero;
	
	if(*tipo == TK_NUMBER) {
		arvore.insert_edge(NTS_NUMBER*1000+contadorNumero, *posicao);
		copiaTabela.erase(copiaTabela.begin());
	} else {
		cout << "Atribuicao invalida" << endl;
		localizaErro();
	}
	
	// verdadeiro se for frac
	if(*tipo == TK_DOT && copiaTabela.at(1).tipo == TK_NUMBER) {
		arvore.insert_edge(NTS_NUMBER*1000+contadorNumero, *posicao);
		copiaTabela.erase(copiaTabela.begin());
		
		arvore.insert_edge(NTS_NUMBER*1000+contadorNumero, *posicao);
		copiaTabela.erase(copiaTabela.begin());
		return true;
	} else
		return false;
}


void parser::localizaErro() {
	cout << "Perto de: " << copiaTabela.front().valor 
		<< ". Linha: " << copiaTabela.front().linha 
		<< " e coluna: " << copiaTabela.front().coluna << endl;
		
	exit(0);
}

