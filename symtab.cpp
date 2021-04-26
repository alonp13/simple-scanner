#include "symtab.h"

void SymbolTable::initReserved()
{
	string line;
	ifstream file("reserved.txt");
	while (getline(file, line)) {
		if (line != "") {
			string text = line.substr(0, line.find('\t'));
			int type_num = stoi(line.substr(line.find('\t') + 1));
			tokenType type = static_cast<tokenType>(type_num);
			shared_ptr<Token> token = make_shared<Token>(type,text);
			insertToken(text, token);
		}

	}
	file.close();
}

shared_ptr<Token> SymbolTable::lookupToken(string text)
{
	return symMap[text];
}

void SymbolTable::insertToken(string text, shared_ptr<Token> tokenp)
{
	symMap[text] = tokenp;
}

void SymbolTable::xref()
{
	for (auto& symbol : symMap)
	{
		string text = symbol.first;
		shared_ptr<Token> token = symbol.second;
		if (token->getType() == IDENTIFIER)
		{
			cout << text << "\t";
			shared_ptr<set<int>> lines = token->getLines();
			set<int>::iterator it;
			for (it = lines->begin(); it != lines->end(); ++it) {
				cout << *it << " ";
			}
			cout << endl;
		}
	}
}
