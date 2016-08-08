#include <fstream>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Tokens.hpp"
#include "DiagPrinter.hpp"

#include "fmt/format.h"

int main(int argc, const char* argv[]) {
	if (argc < 2) { 
		fmt::print(stderr, "No file specified"); 
		return -1;
	}

	auto file_name = argv[1];
	std::ifstream file{ file_name };
	if (!file) {
		fmt::print(stderr, "No such file: {}", file_name);
		return -1;	
	}
   
	pants::Lexer lexer{ file };
	while (true) {
		auto token = lexer.Lex();
		if (token.status() == pants::LexStatus::End) {
			break;
		}

		if (token.status() == pants::LexStatus::Unrecognised) {
			fmt::print(stderr, "Unrecognised");
			break;
		}
		fmt::print("{}\n", token.value().ToString());
	}
}
