#include "Lexer.hpp"
#include "Tokens.hpp"
#include "catch.hpp"
#include <fstream>
using namespace pants;

TEST_CASE("comment.pant", "comment.pant") {
	std::ifstream file {"/home/simon/PantsSys/PantsC/test/lexer/cases/comment.pant"};
	Lexer lexer{file};
	REQUIRE(lexer.Lex().ToString() == "func");
	REQUIRE(lexer.Lex().ToString() == "id(entry)");
	REQUIRE(lexer.Lex().ToString() == "lsquare");
	REQUIRE(lexer.Lex().ToString() == "rsquare");
	REQUIRE(lexer.Lex().ToString() == "arrow");
	REQUIRE(lexer.Lex().ToString() == "u32");
	REQUIRE(lexer.Lex().ToString() == "is");
	REQUIRE(lexer.Lex().ToString() == "int(1)");
	REQUIRE(lexer.Lex().ToString() == "add");
	REQUIRE(lexer.Lex().ToString() == "int(1)");
	REQUIRE(lexer.Lex().ToString() == "semi");
	REQUIRE(lexer.Lex().ToString() == "end");
	REQUIRE(lexer.Lex().ToString() == "eof");
}