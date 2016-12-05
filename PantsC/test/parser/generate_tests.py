import os

cases_dir = 'cases'
test_case_start_string = '--test case--\n'
test_case_end_string = '--end test case--\n'

preamble = '''#include "Lexer.hpp"
#include "Parser.hpp"
#include "ASTPrinter.hpp"
#include "Tokens.hpp"
#include "catch.hpp"
#include <fstream>
using namespace pants;

'''

for file_name in os.listdir(cases_dir):
    file_name = os.path.join(cases_dir, file_name)
    test_name = os.path.basename(file_name)
    test_file_name = 'testcase_' + os.path.splitext(test_name)[0] + '.cpp'
    with open(file_name, 'r') as in_file, open(test_file_name, 'w') as out_file:
        out_file.write(preamble)
        out_file.write('TEST_CASE("{}", "{}") {{\n'.format(test_name, test_name))
        out_file.write('\tstd::ifstream file {{"{}"}};\n'.format(os.path.abspath(file_name)))
        out_file.write('''\tLexer lexer{file};
        Parser parser{lexer};
        auto ast = parser.Parse();
        std::stringstream ss{};
        ASTPrinter printer{ss};
        printer.Visit(ast);
        ''')

        assert in_file.readline() == '#{\n'
        assert in_file.readline() == test_case_start_string

        data = ""

        while True:
            line = in_file.readline()
            
            if line == test_case_end_string:
                break

            data += line

        out_file.write('const char* data = "{}";\n'.format(data.replace('\n','\\n')));
        out_file.write('\tREQUIRE(ss.str() == data);\n')
        
        out_file.write('}')
