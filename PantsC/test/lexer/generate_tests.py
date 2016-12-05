import os

cases_dir = 'cases'
test_case_start_string = '--test case--\n'
test_case_end_string = '--end test case--\n'

preamble = '''#include "Lexer.hpp"
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
        out_file.write('\tLexer lexer{file};\n')

        assert in_file.readline() == '#{\n'
        assert in_file.readline() == test_case_start_string

        while True:
            line = in_file.readline()
            
            if line == test_case_end_string:
                break

            out_file.write('\tREQUIRE(lexer.Lex().ToString() == "{}");\n'.format(line[:-1]))
        
        out_file.write('}')
