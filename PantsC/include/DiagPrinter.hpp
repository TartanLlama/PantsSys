#pragma once
#include <vector>
#include <iostream>

namespace pants {
	class Diagnostic;

	void PrintDiags(std::vector<Diagnostic> diags, std::istream& file);
}
