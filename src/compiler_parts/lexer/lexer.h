#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <iterator>

enum class TokenClass
{
	Comment,
	String,
	Separator,
	Bracket,
	FloatLiteral,
	OctLiteral,
	HexLiteral,
	IntLiteral,
	LogicOperator,
	UnaryOperator,
	Assignment,
	KeyWord,
	Identifier,
	EndOfFile,
	Unknown,
	BrokenHex,
	BrokenOct
};

struct Token {
	TokenClass tokenClass;
	std::string lexema;
	size_t line;
	size_t posInLine;
};

struct Rule {
	std::regex regular;
	TokenClass type;
};

class Lexer
{
public:
	Token getNextToken();
	void readText(std::string fileName);
	std::string getTokenType(TokenClass token);

	Lexer() { initRules(); }
	~Lexer() { m_rules.clear(); }
private:
	std::vector<Rule> m_rules;
	std::string m_inputText;
	void initRules();
	size_t m_charId = 0;
	size_t m_lineId = 1;
	size_t m_posInLine = 1;
};