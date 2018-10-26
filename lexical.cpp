#include <iostream>
#include <regex>
#include <sstream>

using namespace std;

#include "lexical.h"

DFA<char> afd(0, false);

lexical::lexical() {
	cout << "Inicio da análise léxica:" << endl;
	
	linha = 0;
	coluna = 0;
	
	arquivo = "testcode2.txt";
	tamanhoDoArquivo = GetFileSize(arquivo);
	cout << "Tamanho do arquivo: " << tamanhoDoArquivo << endl;
	
	removeComentario();
	
	// Realiza análise de alfabeto
	//analiseCaracteres();
	
	
	// =============== CONSTRUÇÃO DO AFD ===============
	afd.add_state(0, false);

	// Estados finais
	afd.add_state(sNumero, 				true);
	afd.add_state(sIdentificador, 		true);
	
	afd.add_state(fimDeSecao, 			true);
	afd.add_state(fimDeCompasso, 		true);
	afd.add_state(virgula, 				true);
	afd.add_state(ponto, 				true);
	afd.add_state(comecoDeRepeticao, 	true);
	afd.add_state(fimDeRepeticao, 		true);
	
	afd.add_state(igual, 				true);
	afd.add_state(doisPontos, 			true);
	afd.add_state(aspaDupla, 			true);
	
	afd.add_state(divisao, 				true);
	afd.add_state(soma, 				true);
	afd.add_state(somaDeCompasso, 		true);
	afd.add_state(subtracao, 			true);
	
	afd.add_state(comentario, 			true);
	
	// Transições de pontuação
	afd.add_transition(0, 				'|', 	fimDeCompasso);
	afd.add_transition(fimDeCompasso, 	'|', 	fimDeSecao);
	afd.add_transition(fimDeCompasso, 	':', 	comecoDeRepeticao);
		
	afd.add_transition(0, 				',', 	virgula);
	afd.add_transition(0, 				'.', 	ponto);
		
	afd.add_transition(0, 				'=', 	igual);
		
	afd.add_transition(0, 				':', 	doisPontos);
	afd.add_transition(doisPontos, 		'|', 	fimDeRepeticao);
		
	afd.add_transition(0, 				'\"',	aspaDupla);
		
	afd.add_transition(0, 				'/', 	divisao);
	afd.add_transition(0, 				'+', 	soma);
	afd.add_transition(soma, 			'|', 	somaDeCompasso);
	afd.add_transition(0, 				'-', 	subtracao);
	
	afd.add_transition(0, 				'~', 	comentario);
	
	// Transições de indentificador
	for(char c='a'; c<='z'; c++) {
		afd.add_transition(0, c, sIdentificador);
		afd.add_transition(sIdentificador, c, sIdentificador);
	}
	for(char c='A'; c<='Z'; c++) {
		afd.add_transition(0, c, sIdentificador);
		afd.add_transition(sIdentificador, c, sIdentificador);
	}
	
	// Transições de identificador e numeral
	for(char c='0'; c<='9'; c++) {
		afd.add_transition(sIdentificador, c, sIdentificador);
		
		afd.add_transition(0, c, sNumero);
		afd.add_transition(sNumero, c, sNumero);
	}
	// =============================================
	
	
	// Realiza a tokenização
	//todosTokens();
	
	cout << "\nFim da análise léxica:" << endl;
	cout << "Qtde de linhas: " << linha << endl;
}

void lexical::removeComentario() {
	sourceCode.open(arquivo);
	newSource.open("new_"+arquivo);
	
	if(!sourceCode.is_open()) {
		cout << "Impossível abrir o arquivo." << endl;
		exit(1);
	}
	
	size_t posicao;
	char umPorUm;
	int marcador = 1;
	
	while(getline(sourceCode, linhaInteira)) {
		for(posicao = 0; posicao < linhaInteira.length(); ++posicao) {
			umPorUm = linhaInteira.at(posicao);
			
			if(umPorUm == '~')
				++marcador;
			
			if(marcador % 2) 
				newSource << umPorUm;
		}
		newSource << "\n";
	}
	newSource.close();
	sourceCode.close();
}


// =============== REALIZA ANÁLISE DE ALFABETO ===============
void lexical::analiseCaracteres() {
	regex alfabeto("[\\-*(a-z)*(A-Z)*(0-9)*\\s*\\t*\\,*\\.*\\;*\\\"*\\:*\\~*\\|*\\\\*\\+*\\/*\\=*\\~*]*");
	int linhaAnalise = 1;
	string umCaracter;
	size_t posicao;
	
	// abre o arquivo com código fonte
	sourceCode.open(arquivo);
	
	cout << "Inicio - análise do alfabeto" << endl;
	
	// Pega linha e verifica caracter a caracter
	while(getline(sourceCode, linhaInteira)) {
		for(posicao = 0; posicao < linhaInteira.length(); ++posicao) {
			umCaracter = linhaInteira.at(posicao);
			
			if(!regex_match(umCaracter, alfabeto)) {
				cout << "\n" << "========= Caracter não reconhecido na linha " << linhaAnalise;
				cout << " (" << umCaracter << ")" << endl;
			}
			
		}
		linhaAnalise++;		
	}
	
	cout << "Fim - análise do alfabeto\n\n" << endl;
	// fecha arquivo e termina parte de análise do alfabeto
	sourceCode.close();
}

// =============== GERENCIAMENTO DE ADIÇÃO DE TOKENS ===============
void lexical::todosTokens(){
	token Token;
	int tamanhoString;
	
	sourceCode.open(arquivo);
	
	// Pega linha por linha
	while(getline(sourceCode, linhaInteira)) {
		coluna = 0;
		++linha;
		
		tamanhoString = linhaInteira.length();
		
		// Não considera nova linha com \s ou \t
		if(tamanhoString <= 0) {
			Token = {novaLinha, "nova linha", linha, 0};
			tabelaDeSimbolos.push_back(Token);
		} else {
			stringstream strs(linhaInteira);
			
			// Pega palavra por palavra
			while(strs >> palavra) {
				coluna += palavra.length();
				
				Token = proximoToken();
				tabelaDeSimbolos.push_back(Token);
			}
		}
	}
	/*
	// Print de todos tokens
	list<token>::iterator receptor;
	for(receptor = tabelaDeSimbolos.begin(); receptor != tabelaDeSimbolos.end(); ++receptor) 
		cout << receptor->valor << endl;
	*/
	sourceCode.close();
}

token lexical::proximoToken() {
	token trabalhado;
	
	/* ======== AREA DE VERIFICAÇÃO DE PIPES (RETURN DIRETO) =============
	 * Com a implementação de autômatos, é possível remover essa parte. 
	 * No entanto, isso pode acrescer em desempenho.  */
	// Verificação de pipe duplo: ||
	regex doublePipe("\\|{2}");
	if(regex_match(palavra, doublePipe))
		return trabalhado = {fimDeSecao, "fimDeSecao: ||", linha, coluna};

	// Verificação de pipe unico: |
	regex singlePipe("\\|{1}");
	if(regex_match(palavra, singlePipe))
		return trabalhado = {fimDeCompasso, "fimDeCompasso: |", linha, coluna};
		
	//Verifica se é início ou fim de ritornelo
	if(!palavra.compare("|:"))
		return trabalhado = {comecoDeRepeticao, "comecoDeRepeticao: |:", linha, coluna};
	else if(!palavra.compare(":|"))
		return trabalhado = {fimDeRepeticao, "fimDeRepeticao: :|", linha, coluna};
	// ====================================================


	// =============== VERIFICAÇÕES SIMPLES ==============
	// Poderão permanecer, já que necessitam de \s p/ funcionar
	// Verificação de escopo: 
	if(palavra.at(0) == '\\') 
		return reconheceEscopo();
	
	// Verificação de tipo de reservado seguido de espaço
	regex tiposReservadas("(violin|viola|cello|numInt|numFrac|ritornelo){1}");
	if(regex_match(palavra, tiposReservadas))
		return reconheceTipoReservado();

	// Verificação de tipo reservado com dois pontos
	size_t onde = palavra.find(":");
	if(onde!=string::npos) {
		string reservada = palavra.substr(0, onde);
		regex reservadosComDoisPontos("(key|time|bpm|title|subtitle|compositor|copyright){1}");
		
		if(regex_match(reservada, reservadosComDoisPontos)) {
			trabalhado = reconheceReservadoComDoisPontos(reservada);
			tabelaDeSimbolos.push_back(trabalhado);
		} else {
			coluna = onde;
			cout << "Token não reconhecido - Reservado com dois pontos (" << linha << "): " << palavra << endl;
			trabalhado = {vazio, "Token não reconhecido (Reservado com dois pontos): "+reservada, linha, coluna};
			tabelaDeSimbolos.push_back(trabalhado);
		}
		
		// Continua verificação na falta de espaço após palavra reservada
		palavra = palavra.substr(onde);
	}
	// ===================================================
	
	
	// =============== ATUAÇÃO DO AUTÔMATO ===============
	afd.reset();
	string lexema;
	palavra += '$';
	coluna = 0;

	for(auto in = palavra.begin(); in < palavra.end(); in++) {
		lexema += *in;
		afd.input(*in);
		char prox = *(in+1);
		
		++coluna;
		
		if(afd.is_accepting() && !afd.is_accepting(prox)) {
			trabalhado = {(tiposToken)afd.state(), lexema, linha, coluna};
			tabelaDeSimbolos.push_back(trabalhado);
			lexema.clear();
			afd.reset();
		}
	}
	
	if(!lexema.compare("$"))
		return trabalhado = {fimDaLinha, "fimDaLinha", linha, coluna};
	else {
		palavra.pop_back();		
		cout << "Token não reconhecido - final (" << linha << "): " << palavra << endl;
		return trabalhado = {vazio, "erro - final: "+palavra, linha, coluna};
	}
}

// =============== RECONHECIMENTO DE ESCOPO ===============
token lexical::reconheceEscopo() {
	if(!palavra.compare("\\instruments")) 
		return reconhecido = {instrumentos, "\\instruments", linha, coluna};
	else if(!palavra.compare("\\setup")) 
		return reconhecido = {setup, "\\setup", linha, coluna};
	else if(!palavra.compare("\\author")) 
		return reconhecido = {autor, "\\author", linha, coluna};
	else if(!palavra.compare("\\sheetmusic")) 
		return reconhecido = {partitura, "\\sheetmusic", linha, coluna};
	else {
		cout << "Token não reconhecido (" << linha << "): " << palavra << endl;
		return reconhecido = {vazio, "erro - escopo", linha, coluna};
	}
}

// =============== RECONHECIMENTO DE TIPO RESERVADO SIMPLES ===============
token lexical::reconheceTipoReservado() {
	if(!palavra.compare("violin"))
		return reconhecido = {sViolin , "violin", linha, coluna};
	else if(!palavra.compare("viola"))
		return reconhecido = {sViola , "viola", linha, coluna};
	else if(!palavra.compare("cello"))
		return reconhecido = {sCello , "cello", linha, coluna};
	else if(!palavra.compare("numInt"))
		return reconhecido = {sInteiro , "numInt", linha, coluna};
	else if(!palavra.compare("numFrac"))
		return reconhecido = {sFracional , "numFrac", linha, coluna};
	else {
		cout << "Token não reconhecido (" << linha << "): " << palavra << endl;
		return reconhecido = {vazio, "erro - reservado", linha, coluna};
	}
}

// =============== RECONHECIMENTO DE TIPO RESERVADO COM DOIS PONTOS ===============
token lexical::reconheceReservadoComDoisPontos(string reservada) {
	if(!reservada.compare("key"))
		return reconhecido = {chave, "key", linha, coluna};
	else if(!reservada.compare("time"))
		return reconhecido = {tempo, "time", linha, coluna};
	else if(!reservada.compare("bpm"))
		return reconhecido = {sBpm, "bpm", linha, coluna};
	else if(!reservada.compare("title"))
		return reconhecido = {titulo, "title", linha, coluna};
	else if(!reservada.compare("subtitle"))
		return reconhecido = {subtitulo, "subtitle", linha, coluna};
	else if(!reservada.compare("compositor"))
		return reconhecido = {sCompositor, "compositor", linha, coluna};
	else if(!reservada.compare("copyright"))
		return reconhecido = {direitos, "copyright", linha, coluna};
	else {
		cout << "Token não reconhecido (" << linha << "): " << palavra << endl;
		return reconhecido = {vazio, "erro - dois pontos", linha, coluna};
	}
}

// =============== CALCULA TAMANHO DO ARQUIVO ===============
//https://codereview.stackexchange.com/a/1407
long lexical::GetFileSize(string filename) {
	ifstream is;
	is.open(filename.c_str(), ios::binary);
	is.seekg(0, ios::end);
	return is.tellg();    
}