#include "c7c.hpp"

// Definição do autômato
DFA<char> afd(0, false);

lexical::lexical() {
	cout << "Inicio da análise léxica:" << endl;
	
	linha = 0;
	coluna = 0;
	posicao = 10000000;
	
	original = "line.txt";
	tamanhoDoArquivo = GetFileSize(original);
	cout << "Tamanho do arquivo: " << tamanhoDoArquivo << endl;
	
	arquivo = "_"+original;
	
	// Realiza análise de alfabeto e remove comentários
	analiseCaracteres();
	
	// Construção do AFD
	constroeAFD();
	
	// Realiza a tokenização
	todosTokens();
	
	cout << "\nFim da análise léxica:" << endl;
	cout << "Qtde de linhas: " << linha << endl;
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
	
	
	cout << "Inicio - análise do alfabeto" << endl;
	
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
	
	cout << "Fim - análise do alfabeto\n" << endl;
	
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
	
	cout << "Início - tokenização" << endl;
	
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
	
	geraArquivoToken();
	
	// insert de notas
	//exertarNotas();
	
	// Print de todos tokens
/*	int controlaLinha = 0;
	vector<token>::iterator receptor;
	for(receptor = tabelaDeSimbolos.begin(); receptor != tabelaDeSimbolos.end(); ++receptor) {
		cout << nomeToken(receptor->tipo) << endl;
		
		
		/*
		if(controlaLinha != receptor->linha) {
			cout << "\n";
			controlaLinha = receptor->linha;
		}
		
		if(receptor->tipo == TK_EMPTY) {
			erro = true;
			cout << receptor->valor << " ";
		}
		else if(receptor->tipo != TK_EOF)
			cout << nomeToken(receptor->tipo) << " ";
	
	}*/
	//cout << "\n";
	
	if(erro) {
		cout << "Erro(s) encontrado(s) na tokenização." << endl;
		exit(0);
	}
		
	cout << "Fim - tokenização" << endl;
	
	sourceCode.close();
}

token lexical::proximoToken() {
	token trabalhado;
	
	/* ======== AREA DE VERIFICAÇÃO DE PIPES (RETURN DIRETO) =============
	 * Com a implementação de autômatos, é possível remover essa parte. 
	 * No entanto, isso pode acrescer em desempenho.  */
	// Verificação de pipe duplo: ||
/*
	regex doublePipe("\\|{2}");
	if(regex_match(palavra, doublePipe))
		return trabalhado = {TK_DPIPE, "fimDeSecao: ||", linha, coluna, posicao};

	// Verificação de pipe unico: |
	regex singlePipe("\\|{1}");
	if(regex_match(palavra, singlePipe))
		return trabalhado = {TK_SPIPE, "fimDeCompasso: |", linha, coluna, posicao};
*/		
	//Verifica se é início ou fim de ritornelo
	if(!palavra.compare("|:"))
		return trabalhado = {TK_PIPEDDOT, "|:", linha, coluna, posicao};
	else if(!palavra.compare(":|"))
		return trabalhado = {TK_DDOTPIPE, ":|", linha, coluna, posicao};

	// ====================================================


	// =============== VERIFICAÇÕES SIMPLES ==============
	// Poderão permanecer, já que necessitam de \s p/ funcionar
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
		return trabalhado = {TK_EOF, "\n", linha, coluna, posicao};
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
void lexical::geraArquivoToken() {
	
	ofstream arquivoToken;
	arquivoToken.open("TK_"+original);
	
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
		else if(receptor->tipo != TK_EOF)
			arquivoToken << nomeToken(receptor->tipo) << "," << receptor->coluna << "\t";
		else
			arquivoToken << receptor->tipo << "\t";
	}
	
	arquivoToken.close();
}
/*
void lexical::exertarNotas() {
	token A = {TK_NUMBER, "1", 0, 0},
		B = {TK_NUMBER, "2", 0, 0},
		C = {TK_NUMBER, "3", 0, 0},
		D = {TK_NUMBER, "4", 0, 0},
		E = {TK_NUMBER, "5", 0, 0},
		F = {TK_NUMBER, "6", 0, 0},
		G = {TK_NUMBER, "7", 0, 0},
		P = {TK_NUMBER, "0", 0, 0};
	
	size_t i = 0;
	
	for(i = 0; tabelaDeSimbolos.at(i).tipo != TK_INSTRUMENTS && i < tabelaDeSimbolos.size(); i++) {}
	if(i == tabelaDeSimbolos.size()) {
		//problema
	}
	
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+1, A);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+2, B);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+3, C);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+4, D);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+5, E);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+6, F);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+7, G);
	tabelaDeSimbolos.insert(tabelaDeSimbolos.begin()+i+8, P);
	
		
}

*/
// =============== CALCULA TAMANHO DO ARQUIVO ===============
//https://codereview.stackexchange.com/a/1407
long lexical::GetFileSize(string filename) {
	ifstream is;
	is.open(filename.c_str(), ios::binary);
	is.seekg(0, ios::end);
	return is.tellg();
}