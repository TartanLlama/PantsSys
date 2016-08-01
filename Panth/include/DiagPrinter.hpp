#pragma once
#include <vector>
#include <iostream>

namespace panth {
	class Diagnostic;

	void PrintDiags(std::vector<Diagnostic> diags, std::istream& file);
}
