#include <regex>
#include "scanner.h"

shared_ptr<Token> Scanner::nextToken()
{
	// get next char
	bool flag = nextChar();
	outloop:
	while (flag && (ch == 13 || ch == ' ' || ch == '\n' || ch == '\t' || ch == '/'))
	{
		if(ch == '/') flag = nextChar();
		if (ch == '*')
		{
			char prev = 0;
			while (nextChar())
			{
				if (ch == '/' && prev == '*') break;
				prev = ch;
			}
			goto outloop;
		}

		if (ch == '/')
		{
			int curr = lineno;
			while (nextChar()) if (curr != lineno) break;
			goto outloop;
		}
		flag = nextChar();
	}

	string str = string(1, ch);
	nextChar();
	str += ch;
	if (str == "++") return make_shared<Token>(INC_OP, str);
	if (str == "--") return make_shared<Token>(DEC_OP, str);
	if (str == "->") return make_shared<Token>(PTR_OP, str);
	if (str == "&&") return make_shared<Token>(AND_OP, str);
	if (str == "||") return make_shared<Token>(OR_OP, str);
	if (str == "<=") return make_shared<Token>(LE_OP, str);
	if (str == ">=") return make_shared<Token>(GE_OP, str);
	if (str == "==") return make_shared<Token>(EQ_OP, str);
	if (str == "!=") return make_shared<Token>(NE_OP, str);
	inputFile.unget();
	inputFile.unget();
	nextChar();

	switch (ch) 
	{ // each character represents itself
	case ';': case '{': case '}': case ',': case ':':
	case '(': case ')': case '[': case ']': case '~': case '&':
	case '*': case '%': case '^': case '?': case '/': case '=':
		return shared_ptr<Token>
			(new Token(static_cast<tokenType>(ch), string(1, ch)));
		break;
	}

	// handle numbers:
	if ((ch >= '0' && ch <= '9') || ch == '-' || ch == '+' || ch == '.')
	{
		string num = string(1, ch);
		while (nextChar())
		{
			if ((ch >= '0' && ch <= '9') || ch == 'e' || ch == 'E' || ch == '.')
			{
				num += ch;
			}
			else {
				break;
			}
		}
		inputFile.unget();
		if (num == ".")
		{
			return shared_ptr<Token>
				(new Token(static_cast<tokenType>('.'), num));
		}

		char* check_flag;
		shared_ptr<Token> new_token;
		double d = strtod(num.c_str(), &check_flag);
		if (*check_flag)
		{
			new_token = make_shared<Token>(ERROR, num);
		}
		else {
			new_token = make_shared<Token>(CONSTANT, num);
		}
		
		return new_token;
	}


	// handle words:
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
	{
		string word = string(1, ch); 
		regex reg("^[_a-zA-Z]\\w*$");
		while (nextChar())
		{
			
			if (!regex_match(word + ch, reg))
			{
				shared_ptr<Token> new_token;
				new_token = symTab.lookupToken(word);
				if (new_token == nullptr)
				{
					new_token = make_shared<varToken>(word);
					symTab.insertToken(word, new_token);
				}
				new_token->add_line(lineno);
				
				inputFile.unget();
				return new_token;
			}
			word += ch;	
		}
	}

	if (ch == '\'')
	{
		string c = string(1, ch);
		regex reg("\'[^\']\'");
		while (nextChar())
		{
			c += ch;
			if (ch == '\'') break;
		}
		shared_ptr<Token> new_token;
		if (regex_match(c, reg))
		{
			new_token = make_shared<Token>(CONSTANT, c.substr(1,c.length() - 2));
		}
		else {
			new_token = make_shared<Token>(ERROR, c.substr(1, c.length() - 2));
		}
		return new_token;
	}

	if (ch == '"')
	{
		string str = string(1, ch);
		regex reg("\"[^\"]*\"");
		while (nextChar())
		{
			str += ch;
			if (ch == '"') break;
		}
		
		shared_ptr<Token> new_token;
		if (regex_match(str, reg))
		{
			new_token = make_shared<Token>(STRING_LITERAL, str.substr(1, str.length() - 2));
		}
		else {
			new_token = make_shared<Token>(ERROR, str.substr(1, str.length() - 2));
		}
		return new_token;
	}


	return nullptr;
}
