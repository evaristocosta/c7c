#include "c7c.hpp"

// Definição do autômato
DFA<char> afd(0, false);

lexical::lexical(string nomeDoArquivo, string opcao1, string opcao2) {
	original = nomeDoArquivo;
	
	if(!(original.size() > 1)) {
		cout << "Forneça um nome de arquivo válido" << endl;
		exit(0);
	}
	
	
	tamanhoDoArquivo = GetFileSize(original);
	cout << "Tamanho do arquivo: " << tamanhoDoArquivo << endl;
	
	cout << "**********\nInicio da análise léxica..." << endl;
	
	linha = 0;
	coluna = 0;
	posicao = 20000000;
	
	arquivo = original+"_";
	
	// Realiza análise de alfabeto e remove comentários
	analiseCaracteres();
	
	// Construção do AFD
	constroeAFD();
	
	// Realiza a tokenização
	todosTokens();
	
	//enxerta notas
	enxertarNotas();
	
	// Gera arquivo de tokens e tabela de símbolos
	if(!opcao1.compare("-tk") && !(opcao2.size() > 0))
		geraArquivoToken(true, false, false, true);
	else if(!opcao1.compare("-tb") && !(opcao2.size() > 0))
		geraArquivoTabelaDeSimbolos(true);
	else if(!opcao1.compare("-tk") && !opcao2.compare("-tb")) {
		geraArquivoToken(true, false, false, false);
		geraArquivoTabelaDeSimbolos(true);
	} else if(opcao1.size() > 0) {
		cout << "Opcao(oes) invalida(s)" << endl;
		exit(0);
	}
		
	// Print de todos tokens
	printTokens(false);
	// Print da tabela de simbolos
	printTabelaDeSimbolos(false);
	
	
	const char *c = arquivo.c_str();
	remove(c);
	
	//cout << "Qtde de linhas: " << linha << endl;
	cout << "\nFim da análise léxica, não foram encontrados erros.\n**********" << endl;
}


// =============== REALIZA ANÁLISE DE ALFABETO E REMOÇÃO DE CONMENTÁRIOS ===============
void lexical::analiseCaracteres() {
	regex alfabeto("[\\-*(a-z)*(A-Z)*(0-9)*\\s*\\t*\\,*\\.*\\#*\\;*\\\"*\\:*\\|*\\\\*\\+*\\/*\\=*\\~*]*");
	int linhaAnalise = 1, marcador = 1;
	string umCaracter;
	size_t pos;
	erro = false;
	
	// abre o arquivo com código fonte
	sourceCode.open(original);
	
	if(!sourceCode.is_open()) {
		cout << "Impossível abrir o arquivo." << endl;
		exit(1);
	}
	
	newSource.open(arquivo);
	
	
	//cout << "Inicio - análise do alfabeto" << endl;
	
	// Pega linha e verifica caracter a caracter
	while(getline(sourceCode, linhaInteira)) {
		for(pos = 0; pos < linhaInteira.length(); ++pos) {
			umCaracter = linhaInteira.at(pos);
			
			if(!regex_match(umCaracter, alfabeto)) {
				cout << "Caracter não reconhecido na linha " << linhaAnalise;
				cout << " (" << umCaracter << ")" << endl;
				erro = true;
			}
			
			if(!umCaracter.compare("~")) {
				++marcador;
				umCaracter = 0x20;
			}
			
			if(marcador % 2) 
				newSource << umCaracter;
			
		}
		newSource << "\n";
		linhaAnalise++;		
	}
	
	//cout << "Fim - análise do alfabeto\n" << endl;
	
	if(erro) {
		cout << "Erro(s) encontrado(s) na análise do alfabeto." << endl;
		exit(0);
	}
	
	// fecha arquivo e termina parte de análise do alfabeto
	sourceCode.close();
	newSource.close();
}

// =============== CONSTRUÇÃO DO AFD ===============
void lexical::constroeAFD() {
	afd.add_state(0, false);

	// Estados finais
	afd.add_state(TK_NUMBER, 	true);
	afd.add_state(TK_IDENTIFIER,true);
	
	afd.add_state(TK_DPIPE, 	true);
	afd.add_state(TK_SPIPE, 	true);
	afd.add_state(TK_COMMA, 	true);
	afd.add_state(TK_DOT, 		true);
	afd.add_state(TK_PIPEDDOT, 	true);
	afd.add_state(TK_DDOTPIPE, 	true);
	
	afd.add_state(TK_EQUAL, 	true);
	afd.add_state(TK_DDOT, 		true);
	afd.add_state(TK_DQUOTE, 	true);
	
	afd.add_state(TK_SLASH, 	true);
	afd.add_state(TK_SUM, 		true);
	afd.add_state(TK_COMPASSUM, true);
	afd.add_state(TK_SUB, 		true);
	
	afd.add_state(TK_DCOMMA,	true);
	afd.add_state(TK_INVERTSLASH,true);
	afd.add_state(TK_SHARP,		true);
	
	
	// Transições de pontuação
	afd.add_transition(0, 		'|', 	TK_SPIPE);
	afd.add_transition(TK_SPIPE,'|', 	TK_DPIPE);
	afd.add_transition(TK_SPIPE,':', 	TK_PIPEDDOT);
		
	afd.add_transition(0, 		',', 	TK_COMMA);
	afd.add_transition(0, 		'.', 	TK_DOT);
		
	afd.add_transition(0, 		'=', 	TK_EQUAL);
		
	afd.add_transition(0, 		':', 	TK_DDOT);
	afd.add_transition(TK_DDOT, '|', 	TK_DDOTPIPE);
		
	afd.add_transition(0, 		'\"',	TK_DQUOTE);
		
	afd.add_transition(0, 		'/', 	TK_SLASH);
	afd.add_transition(0, 		'+', 	TK_SUM);
	afd.add_transition(TK_SUM, 	'|', 	TK_COMPASSUM);
	afd.add_transition(0, 		'-', 	TK_SUB);
	
	afd.add_transition(0, 		';', 	TK_DCOMMA);
	afd.add_transition(0, 		'\\', 	TK_INVERTSLASH);
	afd.add_transition(0, 		'#', 	TK_SHARP);
	
	
	// Transições de indentificador
	for(char c='a'; c<='z'; c++) {
		afd.add_transition(0, c, TK_IDENTIFIER);
		afd.add_transition(TK_IDENTIFIER, c, TK_IDENTIFIER);
	}
	for(char c='A'; c<='Z'; c++) {
		afd.add_transition(0, c, TK_IDENTIFIER);
		afd.add_transition(TK_IDENTIFIER, c, TK_IDENTIFIER);
	}
	
	// Transições de identificador e numeral
	for(char c='0'; c<='9'; c++) {
		afd.add_transition(TK_IDENTIFIER, c, TK_IDENTIFIER);
		
		afd.add_transition(0, c, TK_NUMBER);
		afd.add_transition(TK_NUMBER, c, TK_NUMBER);
	}
}

// =============== GERENCIAMENTO DE ADIÇÃO DE TOKENS ===============
void lexical::todosTokens(){
	token Token;
	int tamanhoString;
	
	sourceCode.open(arquivo);
	
	//cout << "Início - tokenização" << endl;
	
	// Pega linha por linha
	while(getline(sourceCode, linhaInteira)) {
		coluna = 0;
		++linha;
		++posicao;
		
		tamanhoString = linhaInteira.length();
		
		// Não considera nova linha com \s ou \t
		if(tamanhoString > 0) {
			stringstream strs(linhaInteira);
			
			// Pega palavra por palavra
			while(strs >> palavra) {
				coluna += palavra.length();
				posicao += coluna;
				
				Token = proximoToken();
				tabelaDeSimbolos.push_back(Token);
			}
		}
	}
	
	
	
	// insert de notas
	//exertarNotas();
	
	sourceCode.close();
	
	if(erro) {
		cout << "Erro(s) encontrado(s) na tokenização." << endl;
		exit(0);
	}
		
	//cout << "Fim - tokenização" << endl;
	
	
}

token lexical::proximoToken() {
	token trabalhado;
	
	/* ======== AREA DE VERIFICAÇÃO DE PIPES (RETURN DIRETO) ============= */
	//Verifica se é início ou fim de ritornelo
	if(!palavra.compare("|:"))
		return trabalhado = {TK_PIPEDDOT, "|:", linha, coluna, posicao};
	else if(!palavra.compare(":|"))
		return trabalhado = {TK_DDOTPIPE, ":|", linha, coluna, posicao};

	// ====================================================


	// =============== VERIFICAÇÕES SIMPLES ==============
	// Verificação de escopo: 
	if(palavra.at(0) == '\\') 
		return reconheceEscopo();
	
	// Verificação de tipo de reservado seguido de espaço
	regex tiposReservadas("(violin|viola|cello|numInt|numFrac){1}");
	if(regex_match(palavra, tiposReservadas))
		return reconheceTipoReservado();

	// Verificação de tipo reservado com dois pontos
	size_t onde = palavra.find(":");
	if(onde!=string::npos) {
		string reservada = palavra.substr(0, onde);
		
		trabalhado = reconheceReservadoComDoisPontos(reservada);
		tabelaDeSimbolos.push_back(trabalhado);

		// Continua verificação na falta de espaço após palavra reservada
		palavra = palavra.substr(onde);
	}
	// ===================================================
	
	
	// =============== ATUAÇÃO DO AUTÔMATO ===============
	afd.reset();
	string lexema;
	palavra += '$';
	//coluna = 0;

	for(auto in = palavra.begin(); in < palavra.end(); in++) {
		lexema += *in;
		afd.input(*in);
		char prox = *(in+1);
		
		++coluna;
		++posicao;
		
		if(afd.is_accepting() && !afd.is_accepting(prox)) {
			trabalhado = {(tiposToken)afd.state(), lexema, linha, coluna, posicao};
			tabelaDeSimbolos.push_back(trabalhado);
			lexema.clear();
			afd.reset();
		}
	}
	
	if(!lexema.compare("$"))
		return trabalhado = {TK_EOF, "\\n", linha, coluna, posicao};
	else {
		// No caso de haver algo na string, significa que não foi reconhecido
		palavra.pop_back();		
		cout << "Token não reconhecido - final (" << linha << "): " << palavra << endl;
		return trabalhado = {TK_EMPTY, "UNRECOGNIZED ("+palavra+")", linha, coluna, posicao};
	}
}

// =============== RECONHECIMENTO DE ESCOPO ===============
token lexical::reconheceEscopo() {
	if(!palavra.compare("\\instruments")) 
		return reconhecido = {TK_INSTRUMENTS, "\\instruments", linha, coluna, posicao};
	else if(!palavra.compare("\\setup")) 
		return reconhecido = {TK_SETUP, "\\setup", linha, coluna, posicao};
	else if(!palavra.compare("\\author")) 
		return reconhecido = {TK_AUTHOR, "\\author", linha, coluna, posicao};
	else if(!palavra.compare("\\sheetmusic")) 
		return reconhecido = {TK_SHEETMUSIC, "\\sheetmusic", linha, coluna, posicao};
	else {
		// Recuperação de erros - escopo
		if(palavra.at(1) == 'i') {
			cout << "Token não reconhecido (" << linha << "): " << palavra << endl;
			cout << "Tente \\instruments" << endl;
		} else if(palavra.at(1) == 'a') {
			cout << "Token não reconhecido (" << linha << "): " << palavra << endl;
			cout << "Tente \\author" << endl;
		} else if(palavra.at(1) == 's') {
			cout << "Token não reconhecido (" << linha << "): " << palavra << endl;
			cout << "Tente \\setup ou \\sheetmusic" << endl;
		}
		return reconhecido = {TK_EMPTY, "UNRECOGNIZED ("+palavra+")", linha, coluna, posicao};
	}
}

// =============== RECONHECIMENTO DE TIPO RESERVADO SIMPLES ===============
token lexical::reconheceTipoReservado() {
	if(!palavra.compare("violin"))
		return reconhecido = {TK_S_VIOLIN , "violin", linha, coluna, posicao};
	else if(!palavra.compare("viola"))
		return reconhecido = {TK_S_VIOLA , "viola", linha, coluna, posicao};
	else if(!palavra.compare("cello"))
		return reconhecido = {TK_S_CELLO , "cello", linha, coluna, posicao};
	else if(!palavra.compare("numInt"))
		return reconhecido = {TK_T_INT , "numInt", linha, coluna, posicao};
	else if(!palavra.compare("numFrac"))
		return reconhecido = {TK_T_FRAC , "numFrac", linha, coluna, posicao};
	else {
		cout << "Token não reconhecido (" << linha << "): " << palavra << endl;
		return reconhecido = {TK_EMPTY, "UNRECOGNIZED ("+palavra+")", linha, coluna, posicao};
	}
}

// =============== RECONHECIMENTO DE TIPO RESERVADO COM DOIS PONTOS ===============
token lexical::reconheceReservadoComDoisPontos(string reservada) {
	if(!reservada.compare("key"))
		return reconhecido = {TK_KEY, "key", linha, coluna, posicao};
	else if(!reservada.compare("time"))
		return reconhecido = {TK_TIME, "time", linha, coluna, posicao};
	else if(!reservada.compare("bpm"))
		return reconhecido = {TK_BPM, "bpm", linha, coluna, posicao};
	else if(!reservada.compare("title"))
		return reconhecido = {TK_TITLE, "title", linha, coluna, posicao};
	else if(!reservada.compare("subtitle"))
		return reconhecido = {TK_SUBTITLE, "subtitle", linha, coluna, posicao};
	else if(!reservada.compare("compositor"))
		return reconhecido = {TK_COMPOSITOR, "compositor", linha, coluna, posicao};
	else if(!reservada.compare("copyright"))
		return reconhecido = {TK_COPYRIGHT, "copyright", linha, coluna, posicao};
	else {
		// Recuperação de erros - tipo reservado com dois pontos
		regex tilte("[ilte]{4,}");
		if(regex_match(reservada, tilte)) {
			cout << "Provável erro de escrita em: "+reservada+" (" << linha << "). Reconhecendo como: title" << endl;
			return reconhecido = {TK_TITLE, "title", linha, coluna, posicao};
		}
		
		regex subtilte("[iltesub]{7,}");
		if(regex_match(reservada, subtilte)) {
			cout << "Provável erro de escrita em: "+reservada+" (" << linha << "). Reconhecendo como: subtitle" << endl;
			return reconhecido = {TK_SUBTITLE, "subtitle", linha, coluna, posicao};
		}
		
		regex compoistor("[compsitr]{8,}");
		if(regex_match(reservada, compoistor)) {
			cout << "Provável erro de escrita em: "+reservada+" (" << linha << "). Reconhecendo como: compositor" << endl;
			return reconhecido = {TK_COMPOSITOR, "compositor", linha, coluna, posicao};
		}
		
		regex copyrigth("[copyrigth]{8,}");
		if(regex_match(reservada, copyrigth)) {
			cout << "Provável erro de escrita em: "+reservada+" (" << linha << "). Reconhecendo como: copyright" << endl;
			return reconhecido = {TK_COPYRIGHT, "copyright", linha, coluna, posicao};
		}
		
		cout << "Token não reconhecido (" << linha << "): " << palavra << endl;
		return reconhecido = {TK_EMPTY, "UNRECOGNIZED ("+palavra+")", linha, coluna, posicao};
	}
}

// =============== GERA ARQUIVO COM TOKENS (APENAS PARA ANÁLISE) ===============
void lexical::geraArquivoToken(bool nomes, bool valores, bool colunas, bool posicoes) {
	
	ofstream arquivoToken;
	arquivoToken.open(original+"TK.txt");
	
	// Print de todos tokens
	int controlaLinha = 1;
	vector<token>::iterator receptor;
	
	for(receptor = tabelaDeSimbolos.begin(); receptor != tabelaDeSimbolos.end(); ++receptor) {
	
		if(controlaLinha != receptor->linha) {
			arquivoToken << "\n";
			controlaLinha = receptor->linha;
		}
		
		if(receptor->tipo == TK_EMPTY)
			arquivoToken << receptor->valor << "\t";
		else if(receptor->tipo != TK_EOF) {
			if(nomes)
				arquivoToken << nomeToken(receptor->tipo) << "\t";
			
			if(valores)
				arquivoToken << receptor->valor << "\t";
			
			if(colunas)
				arquivoToken << receptor->coluna << "\t";
			
			if(posicoes)
				arquivoToken << receptor->posicao << "\t";
		}
	}
	
	arquivoToken.close();
}

void lexical::geraArquivoTabelaDeSimbolos(bool imprime) {
	if(imprime) {
		ofstream arquivoTabelaDeSimbolos;
		arquivoTabelaDeSimbolos.open(original+"TABLE.html");
		
		vector<token>::iterator receptor;
		 arquivoTabelaDeSimbolos << "<!DOCTYPE html>\n"
				"<html>\n"
				"<body>\n" 
				"<h2>TABELA DE SIMBOLOS</h2>"
				"<table style=\"width:100%\">\n"
					"<tr>\n"
						"<th style=\"text-align: left;\">VALOR</th>\n"
						"<th style=\"text-align: left;\">TOKEN</th>\n"
						"<th style=\"text-align: left;\">LINHA</th>\n"
						"<th style=\"text-align: left;\">COLUNA</th>\n"
						"<th style=\"text-align: left;\">POSICAO</th>\n"
					"</tr>\n" << endl;
					
		for(receptor = tabelaDeSimbolos.begin(); receptor != tabelaDeSimbolos.end(); ++receptor) {
			 arquivoTabelaDeSimbolos << "<tr>\n"
				<< "<td>\'"<< receptor->valor << "\'</td>\n" 
				<< "<td>"<< nomeToken(receptor->tipo) << "</td>\n" 
				<< "<td>"<< receptor->linha << "</td>\n" 
				<< "<td>"<< receptor->coluna << "</td>\n" 
				<< "<td>"<< receptor->posicao << "</td>\n" 
				<< "</tr>\n";
		}
		
		arquivoTabelaDeSimbolos << "</table>\n"
				"</body>\n"
				"</html>\n";
	}
}

void lexical::printTokens(bool imprime){
	if(imprime) {
		int controlaLinha = 0;
		vector<token>::iterator receptor;
		for(receptor = tabelaDeSimbolos.begin(); receptor != tabelaDeSimbolos.end(); ++receptor) {
			if(controlaLinha != receptor->linha) {
				cout << "\n";
				controlaLinha = receptor->linha;
			}
			
			string nomeTipo = nomeToken(receptor->tipo);

			if(receptor->tipo == TK_EMPTY) {
				erro = true;
				cout << receptor->valor << " ";
			}
			else if(receptor->tipo != TK_EOF)
				cout << nomeTipo << " ";

		}
		cout << "\n";
	}
}

void lexical::printTabelaDeSimbolos(bool imprime) {
	if(imprime) {
		vector<token>::iterator receptor;
		cout << " ===== TABELA DE SIMBOLOS =====" << endl;
		cout << "VALOR:\t\tTOKEN:\t\tLINHA:\t\tCOLUNA:" << endl;
		for(receptor = tabelaDeSimbolos.begin(); receptor != tabelaDeSimbolos.end(); ++receptor) {
			cout << receptor->valor << "\t\t" 
				<< nomeToken(receptor->tipo) << "\t\t" 
				<< receptor->linha << "\t\t" 
				<< receptor->coluna << endl;
		}
	}
}


void lexical::enxertarNotas() {
	token tipo = {TK_T_INT, "numInt", 0, 0, posicao+1};
	
	token varA = {TK_IDENTIFIER, "A", 0, 0, posicao+2},
		varB = {TK_IDENTIFIER, "B", 0, 0, posicao+7},
		varC = {TK_IDENTIFIER, "C", 0, 0, posicao+12},
		varD = {TK_IDENTIFIER, "D", 0, 0, posicao+17},
		varE = {TK_IDENTIFIER, "E", 0, 0, posicao+22},
		varF = {TK_IDENTIFIER, "F", 0, 0, posicao+27},
		varG = {TK_IDENTIFIER, "G", 0, 0, posicao+32},
		varP = {TK_IDENTIFIER, "P", 0, 0, posicao+37};
		
	token atrib = {TK_EQUAL, "=", 0, 0, posicao+3};

	token A = {TK_NUMBER, "1", 0, 0, posicao+4},
		B = {TK_NUMBER, "3", 0, 0, posicao+9},
		C = {TK_NUMBER, "4", 0, 0, posicao+14},
		D = {TK_NUMBER, "6", 0, 0, posicao+19},
		E = {TK_NUMBER, "8", 0, 0, posicao+24},
		F = {TK_NUMBER, "9", 0, 0, posicao+29},
		G = {TK_NUMBER, "11", 0, 0, posicao+34},
		P = {TK_NUMBER, "0", 0, 0, posicao+39};
		
	token spipe = {TK_SPIPE, "|", 0, 0, posicao+5};
	
	size_t i = 0;
	
	for(i = 0; tabelaDeSimbolos.at(i).tipo != TK_INSTRUMENTS && i < tabelaDeSimbolos.size(); i++) {}
	if(i == tabelaDeSimbolos.size()) {
		cout << "Falta bloco de \\instruments" << endl;
		exit(0);
	}

	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, spipe);
	spipe.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, A);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, atrib);
	atrib.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, varA);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, tipo);
	tipo.posicao += 5;
	
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, spipe);
	spipe.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, B);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, atrib);
	atrib.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, varB);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, tipo);
	tipo.posicao += 5;
	
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, spipe);
	spipe.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, C);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, atrib);
	atrib.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, varC);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, tipo);
	tipo.posicao += 5;
	
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, spipe);
	spipe.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, D);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, atrib);
	atrib.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, varD);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, tipo);
	tipo.posicao += 5;
	
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, spipe);
	spipe.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, E);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, atrib);
	atrib.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, varE);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, tipo);
	tipo.posicao += 5;
	
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, spipe);
	spipe.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, F);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, atrib);
	atrib.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, varF);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, tipo);
	tipo.posicao += 5;
	
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, spipe);
	spipe.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, G);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, atrib);
	atrib.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, varG);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, tipo);
	tipo.posicao += 5;
	
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, spipe);
	spipe.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, P);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, atrib);
	atrib.posicao += 5;
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, varP);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, tipo);
	tipo.posicao += 5;
}


// =============== CALCULA TAMANHO DO ARQUIVO ===============
//https://codereview.stackexchange.com/a/1407
long lexical::GetFileSize(string filename) {
	ifstream is;
	is.open(filename.c_str(), ios::binary);
	is.seekg(0, ios::end);
	return is.tellg();
}