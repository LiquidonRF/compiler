#include <iostream>
#include "compiler_parts/lexer/lexer.h"

int main(int argc, char* argv[])
{
	std::string fileName = "../data/NOD.rb";

	Lexer lexer;

	lexer.readText(fileName);

	while (true)
	{
		Token token = lexer.getNextToken();
		if (token.tokenClass == TokenClass::EndOfFile)
			break;

		if (token.tokenClass != TokenClass::Separator)
			std::cout << lexer.getTokenType(token.tokenClass) << "\t" << token.lexema << std::endl;
	}
}