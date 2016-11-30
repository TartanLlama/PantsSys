#pragma once
#include <iostream>
#include <vector>

namespace pants {
class Diagnostic;

void PrintDiags(std::vector<Diagnostic> diags, std::istream &file);
}
