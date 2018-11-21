#include "c7c.hpp"

semantic::semantic(vector<token> tabela, NGraph::Graph arv) {
	arvore = arv;
	tabelaDeSim = tabela;
}