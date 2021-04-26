#include "token.h"
#include "tokenstrings.h"

using namespace std;
tokenType Token::getType()
{
	return type;
}

string Token::getText()
{
	return text;
}

void Token::printToken()
{
	if (type < 258) {
		char type_to_print = type;
		cout << type_to_print;
	}
	else {
		cout << tokenStrings[type - 258];
	}
	cout << "\t" << text << endl;
}

//varToken

void varToken::add_line(int l)
{
	lines->insert(l);
}

shared_ptr<set<int>> varToken::getLines()
{
	return lines;
}
