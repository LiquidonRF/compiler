#include "lexer.h"

void Lexer::readText(std::string fileName)
{
	std::ifstream inputStream(fileName);

	m_inputText = std::string(std::istreambuf_iterator<char>(inputStream), std::istreambuf_iterator<char>());
}

void Lexer::initRules()
{
	Rule rule;
	rule.type = TokenClass::Comment;
	rule.regular = "#.*\n";
	m_rules.push_back(rule);

	rule.type = TokenClass::String;
	rule.regular = "\".*?\"";
	m_rules.push_back(rule);

	rule.type = TokenClass::Separator;
	rule.regular = "(\\r\\n|\\r|\\n|\\s|\\t|\\.\\.|\\,)";
	m_rules.push_back(rule);

	rule.type = TokenClass::Bracket;
	rule.regular = "(\\(|\\)|\\[|\\])";
	m_rules.push_back(rule);

	rule.type = TokenClass::FloatLiteral;
	rule.regular = "([0-9]*\\.[0-9]+)";
	m_rules.push_back(rule);

	rule.type = TokenClass::OctLiteral;
	rule.regular = "(0[0-9]+)";
	m_rules.push_back(rule);

	rule.type = TokenClass::HexLiteral;
	rule.regular = "(0x[\\d\\w]+)";
	m_rules.push_back(rule);

	rule.type = TokenClass::IntLiteral;
	rule.regular = "([1-9]+[0-9]*)|(0)";
	m_rules.push_back(rule);

	rule.type = TokenClass::LogicOperator;
	rule.regular = "==|!=|>=|<=|>|<|&&|\\|\\|";
	m_rules.push_back(rule);

	rule.type = TokenClass::UnaryOperator;
	rule.regular = "\\+|-|%|\\/|\\*|=";
	m_rules.push_back(rule);

	rule.type = TokenClass::KeyWord;
	rule.regular = "(while|for|do|break|if|in|else|return|end|then|puts|gets)(?=\\W)";
	m_rules.push_back(rule);

	rule.type = TokenClass::Identifier;
	rule.regular = "[a-zA-Z]+[\\w\\d]*";
	m_rules.push_back(rule);

	rule.type = TokenClass::EndOfFile;
	rule.regular = "$";
	m_rules.push_back(rule);

}

Token Lexer::getNextToken()
{
	std::string::iterator currChar = m_inputText.begin() + m_charId;

	Token token;

	std::smatch resultedmatch;

	for (size_t i = 0; i < m_rules.size(); i++)
	{
		if (std::regex_search(std::string::const_iterator(currChar), 
			m_inputText.cend(), resultedmatch, m_rules[i].regular)
			&& resultedmatch.position(0) == 0)
		{
			std::string resultedStr = resultedmatch.str(0);

			m_charId += resultedStr.length();

			token.tokenClass = m_rules[i].type;

			std::cmatch brokenMatch;

			if (m_rules[i].type == TokenClass::OctLiteral)
			{
				if (std::regex_search(resultedStr.c_str(), brokenMatch,
					(std::regex)"(0[0-7]+)") && resultedmatch.position(0) == 0 &&
					resultedStr.length() == brokenMatch.length())
				{
					token.tokenClass = m_rules[i].type;
				}
				else {
					token.tokenClass = TokenClass::BrokenOct;
				}
			}

			if (m_rules[i].type == TokenClass::HexLiteral)
			{
				if (std::regex_search(resultedStr.c_str(), brokenMatch,
					(std::regex)"(0x[0-9A-F]+)") && resultedmatch.position(0) == 0 &&
					resultedStr.length() == brokenMatch.length())
				{
					token.tokenClass = m_rules[i].type;
				}
				else {
					token.tokenClass = TokenClass::BrokenHex;
				}
			}

			token.lexema = resultedStr;
			token.line = m_lineId;
			token.posInLine = m_posInLine;

			m_posInLine += resultedStr.length();

			for (auto& iter : resultedStr) 
				if (iter == L'\n')
				{
					m_lineId++;
					m_posInLine = 1;
				}

			return token;
		}
	}

	size_t separatorNum = 0;

	std::string unknownStr;

	token.lexema += *currChar;
	m_charId++;
		
	token.tokenClass = TokenClass::Unknown;
	token.line = m_lineId;
	token.posInLine = m_posInLine;

	m_posInLine++;

	return token;
}

std::string Lexer::getTokenType(TokenClass token)
{
	switch (token)
	{
	case TokenClass::Comment:
		return "Comment";
	case TokenClass::String:
		return "String";
	case TokenClass::Separator:
		return "Separator";
	case TokenClass::Bracket:
		return "Bracket";
	case TokenClass::FloatLiteral:
		return "FloatLiteral";
	case TokenClass::OctLiteral:
		return "OctLiteral";
	case TokenClass::HexLiteral:
		return "HexLiteral";
	case TokenClass::IntLiteral:
		return "IntLiteral";
	case TokenClass::LogicOperator:
		return "LogicOperator";
	case TokenClass::UnaryOperator:
		return "UnaryOperator";
	case TokenClass::Assignment:
		return "Assignment";
	case TokenClass::KeyWord:
		return "KeyWord";
	case TokenClass::Identifier:
		return "Identifier";
	case TokenClass::EndOfFile:
		return "EndOfFile";
	case TokenClass::Unknown:
		return "Unknown";
	case TokenClass::BrokenHex:
		return "BrokenHex";
	case TokenClass::BrokenOct:
		return "BrokenOct";
	}

	return "Invalid Token";
}