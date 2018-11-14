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
		// ponteiro pro comeco da pilha - facilita a escrita
		tipo = &copiaTabela.front().tipo;
		
		// Pesquisa por \author
		if(*tipo == TK_AUTHOR) {
			// insere na arvore
			arvore.insert_edge(NTS_PROG, NTS_SAUTH);
			arvore.insert_edge(NTS_SAUTH, TK_AUTHOR);
			arvore.insert_edge(NTS_SAUTH, NTS_AUTH);
			
			// remove da pilha
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 1;
		
		// deriva bloco de autoria
		autoria();
		
		// procura || do final
		if(*tipo == TK_DPIPE) {
			arvore.insert_edge(NTS_SAUTH, TK_DPIPE);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 11;
		/* ======================================== */
		
		
		/* ================ SEÇÃO INSTRUMENTOS ================ */
		// Pesquisa \instruments
		if(copiaTabela.front().tipo == TK_INSTRUMENTS) {
			arvore.insert_edge(NTS_PROG, NTS_SINSTR);
			arvore.insert_edge(NTS_SINSTR, TK_INSTRUMENTS);
			arvore.insert_edge(NTS_SINSTR, NTS_INSTRUMENTS);
			
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			throw 2;
			
		// deriva bloco de instruments
		instrumentos();
		
		// procura || do instruments
		if(copiaTabela.front().tipo == TK_DPIPE) {
			arvore.insert_edge(NTS_SINSTR, TK_DPIPE);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 12;
		/* ======================================== */
		
		/* ================ SEÇÃO CONFIGURACAO ================ */
		// Pesquisa \setup
		if(copiaTabela.front().tipo == TK_SETUP) {
			arvore.insert_edge(NTS_PROG, NTS_SSETUP);
			arvore.insert_edge(NTS_SSETUP, TK_SETUP);
			arvore.insert_edge(NTS_SSETUP, NTS_SETUP);
			
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			throw 3;
			
		// deriva bloco de setup
		configuracao();
		
		// procura || do setup
		if(copiaTabela.front().tipo == TK_DPIPE) {
			arvore.insert_edge(NTS_SSETUP, TK_DPIPE);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 13;
		/* ======================================== */
		
		
		/* ================ SEÇÃO PARTITURA ================ */
		// Pesquisa \sheetmusic
		if(copiaTabela.front().tipo == TK_SHEETMUSIC) {
			arvore.insert_edge(NTS_PROG, NTS_SSHEET);
			arvore.insert_edge(NTS_SSHEET, TK_SHEETMUSIC);
			arvore.insert_edge(NTS_SSHEET, NTS_SHEET);
			
			copiaTabela.erase(copiaTabela.begin());
			
		} else 
			throw 4;
			
		// deriva bloco de setup
		partitura();
		
		// procura || do setup
		if(copiaTabela.front().tipo == TK_DPIPE) {
			arvore.insert_edge(NTS_SSHEET, TK_DPIPE);
			
			copiaTabela.erase(copiaTabela.begin());
		} else 
			throw 14;
		/* ======================================== */
		
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
		//localizaErro();
		erro = true;
	}
	
	if(erro) {
		cout << "Existem erro(s) na sintática do código-fonte" << endl;
		exit(0);
	}
	
//	cout << arvore;
	
}

/* ================== ABRE AUTORIA ================== */
void parser::autoria() {
	erro = false;
	
	// caso nao tenha nada definido
	if(copiaTabela.front().tipo == (TK_DPIPE|TK_INSTRUMENTS))
		return;
	
	// enquanto não acha fim do bloco	
	while(copiaTabela.front().tipo != TK_DPIPE && !erro) {
		// pesquisa declaracao
		switch(copiaTabela.front().tipo) {
			case TK_TITLE:
				qualNo = NTS_TITLE;
				arvore.insert_edge(NTS_AUTH, qualNo);
				arvore.insert_edge(qualNo, TK_TITLE);
			break;
			case TK_SUBTITLE:
				qualNo = NTS_SUBTITLE;
				arvore.insert_edge(NTS_AUTH, qualNo);
				arvore.insert_edge(qualNo, TK_SUBTITLE);
			break;
			case TK_COMPOSITOR:
				qualNo = NTS_COMPOSITOR;
				arvore.insert_edge(NTS_AUTH, qualNo);
				arvore.insert_edge(qualNo, TK_COMPOSITOR);
			break;
			case TK_COPYRIGHT:
				qualNo = NTS_COPYRIGHT;
				arvore.insert_edge(NTS_AUTH, qualNo);
				arvore.insert_edge(qualNo, TK_COPYRIGHT);
			break;
			default:
				erro = true;
				cout << "Falta declaracao de tipo" << endl;
				localizaErro();
		}
		copiaTabela.erase(copiaTabela.begin());
		
		// pesquisa dois pontos
		if(copiaTabela.front().tipo == TK_DDOT) {
			arvore.insert_edge(qualNo, TK_DDOT);
		} else {
			cout << "Falta dois pontos depois da declaração" << endl;
			localizaErro();
			erro = true;
		}
		copiaTabela.erase(copiaTabela.begin());
			
		// define string
		if(copiaTabela.front().tipo == TK_DQUOTE) {
			arvore.insert_edge(qualNo, NTS_STRING);
			arvore.insert_edge(NTS_STRING, TK_DQUOTE);
			stringC7();
		} else {
			cout << "Falta inicio de string";
			localizaErro();
			erro = true;
		}
		copiaTabela.erase(copiaTabela.begin());
		
		// procura |
		if(copiaTabela.front().tipo == TK_SPIPE) {
			arvore.insert_edge(qualNo, TK_DDOT);
		} else {
			cout << "Falta delimitador de linha" << endl;
			localizaErro();
			erro = true;
		}
		copiaTabela.erase(copiaTabela.begin());
		
		// caso especial
		if(copiaTabela.front().tipo == TK_INSTRUMENTS) 
			erro = true;
	}
}

/* ================== ABRE INSTRUMENTOS ================== */
void parser::instrumentos() {
	bool tipoDeIdent, fracional;
	
	while(copiaTabela.front().tipo != TK_DPIPE && !erro) {
		// sempre comeca com tipo
		if((copiaTabela.front().tipo == TK_S_VIOLIN) 
			|| (copiaTabela.front().tipo == TK_S_VIOLA)
			|| (copiaTabela.front().tipo == TK_S_CELLO)) {
				
			tipoDeIdent = false;
			
			switch(copiaTabela.front().tipo) {
				case TK_S_CELLO:
					arvore.insert_edge(NTS_INSTRUMENTS, TK_S_CELLO);
				break;
				case TK_S_VIOLA:
					arvore.insert_edge(NTS_INSTRUMENTS, TK_S_VIOLA);
				break;
				case TK_S_VIOLIN:
					arvore.insert_edge(NTS_INSTRUMENTS, TK_S_VIOLIN);
				break;
			}
			copiaTabela.erase(copiaTabela.begin());
			
		} else if(copiaTabela.front().tipo == TK_T_FRAC
			|| copiaTabela.front().tipo == TK_T_INT) {
			
			tipoDeIdent = true;
			
			switch(copiaTabela.front().tipo) {
				case TK_T_FRAC:
					fracional = true;
					arvore.insert_edge(NTS_INSTRUMENTS, TK_T_FRAC);
				break;
				case TK_T_INT:
					fracional = false;
					arvore.insert_edge(NTS_INSTRUMENTS, TK_T_INT);
				break;
			}
			copiaTabela.erase(copiaTabela.begin());
			
		} else {
			cout << "Tipo não reconhecido" << endl;
			localizaErro();
		}
		
		if(copiaTabela.front().tipo == TK_IDENTIFIER) {
			arvore.insert_edge(NTS_INSTRUMENTS, TK_IDENTIFIER);
			copiaTabela.erase(copiaTabela.begin());
		} else {
			cout << "Esperava-se um identificador" << endl;
			localizaErro();
		}
			
		if(tipoDeIdent) {
			if(copiaTabela.front().tipo == TK_EQUAL) {
				arvore.insert_edge(NTS_INSTRUMENTS, TK_EQUAL);
				copiaTabela.erase(copiaTabela.begin());
			} else {
				cout << "Falta atribuicao" << endl;
				localizaErro();
			}
			
			arvore.insert_edge(NTS_INSTRUMENTS, NTS_NUMBER);
			
			if(numeros() && !fracional) {
				cout << "Declaracao de inteiro e atribuicao de fracional" << endl;
				localizaErro();
			}
		}
		
		if(copiaTabela.front().tipo == TK_SPIPE) {
			arvore.insert_edge(NTS_INSTRUMENTS, TK_SPIPE);
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
	key = time = false;
	
	while(*tipo != TK_DPIPE && !erro) {
		switch(*tipo) {
			case TK_KEY:
				key = true;
				arvore.insert_edge(NTS_SETUP, TK_KEY);
			break;
			case TK_TIME:
				time = true;
				arvore.insert_edge(NTS_SETUP, TK_TIME);
			break;
			case TK_BPM:
				arvore.insert_edge(NTS_SETUP, TK_BPM);
			break;
			default:
				cout << "Especificador nao reconhecido" << endl;
				localizaErro();
		}
		copiaTabela.erase(copiaTabela.begin());
		
		// pesquisa dois pontos
		if(copiaTabela.front().tipo == TK_DDOT) {
			arvore.insert_edge(NTS_SETUP, TK_DDOT);
		} else {
			cout << "Falta dois pontos depois da declaração" << endl;
			localizaErro();
		}
		copiaTabela.erase(copiaTabela.begin());
		
		if(key && !time) {
			if(*tipo == TK_IDENTIFIER) {
				arvore.insert_edge(NTS_SETUP, TK_IDENTIFIER);
				copiaTabela.erase(copiaTabela.begin());
			} else {
				cout << "Nota nao reconhecida" << endl;
				localizaErro();
			}
			
			// precisa tratar
			if(!copiaTabela.front().valor.compare("#")) {
				
			} else if(!copiaTabela.front().valor.compare("b")) {
				
			}
			
			key = false;
			
		} else if(time && !key) {
			arvore.insert_edge(NTS_SETUP, NTS_NUMBER);
			if(numeros()) {
				cout << "Fracionais não sao aceitos na formula de compasso" << endl;
				localizaErro();
			}
			
			if(*tipo == TK_SLASH) {
				arvore.insert_edge(NTS_SETUP, TK_SLASH);
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
			arvore.insert_edge(NTS_SETUP, NTS_NUMBER);
			numeros();
		}

		// procura |
		if(copiaTabela.front().tipo == TK_SPIPE) {
			arvore.insert_edge(NTS_SETUP, TK_DDOT);
		} else {
			cout << "Falta delimitador de linha" << endl;
			localizaErro();
		}
		copiaTabela.erase(copiaTabela.begin());
	}
}

/* ================== ABRE PARTITURA ================== */
void parser::partitura() {}


// falta simbolos --- regex
// todas strings no mesmo nó ---
void parser::stringC7() {
	copiaTabela.erase(copiaTabela.begin());
	while(copiaTabela.front().tipo != TK_DQUOTE) {
		if(copiaTabela.front().tipo == TK_IDENTIFIER) {
			arvore.insert_edge(NTS_STRING, TK_IDENTIFIER);
			copiaTabela.erase(copiaTabela.begin());
			
		} else if(copiaTabela.front().tipo == TK_NUMBER) {
			arvore.insert_edge(NTS_STRING, TK_NUMBER);
			copiaTabela.erase(copiaTabela.begin());
		
		// } else if() {

		// }	
		
		} else {
			cout << "Erro em string" << endl;
			exit(0);
		}
	}
	
	// se sai do while, entao achou "
	arvore.insert_edge(NTS_STRING, TK_DQUOTE);
}

bool parser::numeros() {
	if(copiaTabela.front().tipo == TK_NUMBER) {
		arvore.insert_edge(NTS_NUMBER, TK_NUMBER);
		copiaTabela.erase(copiaTabela.begin());
	} else {
		cout << "Atribuicao invalida" << endl;
		localizaErro();
	}
	
	// verdadeiro se for frac
	if(copiaTabela.front().tipo == TK_DOT && copiaTabela.at(1).tipo == TK_NUMBER) {
		arvore.insert_edge(NTS_NUMBER, TK_DOT);
		copiaTabela.erase(copiaTabela.begin());
		
		arvore.insert_edge(NTS_NUMBER, TK_NUMBER);
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

