#include <fstream>

#include "ASTPrinter.hpp"
#include "DiagPrinter.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Tokens.hpp"
#include "ASTSerializer.hpp"
#include "ASTDeserializer.hpp"

#include "fmt/format.h"

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        fmt::print(stderr, "No file specified");
        return -1;
    }

    auto file_name = argv[1];
    std::ifstream file{file_name};
    if (!file) {
        fmt::print(stderr, "No such file: {}", file_name);
        return -1;
    }

    pants::Lexer lexer{file};
    pants::Parser parser{lexer};

    try {
        auto ast = parser.Parse();

        file.close();
        file.open(file_name);

        pants::ASTPrinter printer{std::cout};
        printer.Visit(ast);
        std::fstream out_file {"out"};

        pants::ASTSerializer serializer{out_file};
        serializer.Visit(ast);
        out_file.close();
        out_file.open("out");

        pants::ASTDeserializer deserializer{out_file};
        auto new_ast = deserializer.Deserialize();
        printer.Visit(new_ast);
    }
    catch (pants::Parser::ParseError) {
        pants::PrintDiags(parser.diags(), file);
    }
}
