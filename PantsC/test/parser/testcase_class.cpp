#include "Lexer.hpp"
#include "Parser.hpp"
#include "ASTPrinter.hpp"
#include "Tokens.hpp"
#include "catch.hpp"
#include <fstream>
using namespace pants;

TEST_CASE("class.pant", "class.pant") {
	std::ifstream file {"/home/simon/PantsSys/PantsC/test/parser/cases/class.pant"};
	Lexer lexer{file};
        Parser parser{lexer};
        auto ast = parser.Parse();
        std::stringstream ss{};
        ASTPrinter printer{ss};
        printer.Visit(ast);
        const char* data = "<<ClassDecl vec2>>\n  <<VarDecl>>\n    <<Type u32>>\n    <<Id x>>\n  <<VarDecl>>\n    <<Type u32>>\n    <<Id y>>\n<<FuncDecl>>\n  <<Id entry>>\n  <<Type nil>>\n  <<VarDecl>>\n    <<Type id(vec2)>>\n    <<Id a>>\n";
	REQUIRE(ss.str() == data);
}