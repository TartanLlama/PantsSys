#include <algorithm>
#include <numeric>
#include <tuple>

#include "DiagPrinter.hpp"
#include "Parser.hpp"

namespace pants {
void PrintDiags(std::vector<Diagnostic> diags, std::istream &file) {
    file.seekg(0);

    std::sort(diags.begin(), diags.end(), [](auto &&a, auto &&b) {
        return std::make_tuple(a.row(), a.col()) <
               std::make_tuple(b.row(), b.col());
    });

    std::size_t line = 0;
    std::string last_text{};
    auto diag = diags.begin();

    while (diag != diags.end()) {
        auto diag_line = diag->row();

        std::string text{};
        if (diag_line != 0 && line == diag_line) {
            text = last_text;
        } else {
            while (line < diag_line) {
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                ++line;
            }

            std::getline(file, text);
            last_text = text;
            ++line;
        }

        fmt::print(stderr, "At ({},{}):\n", diag->row(), diag->col());
        fmt::print(stderr, "{}\n", text);

        std::string cursor{};
        for (std::size_t offset = 0; offset < diag->col(); ++offset) {
            cursor += ' ';
        }
        cursor += '^';
        fmt::print(stderr, "{}\n", cursor);
        fmt::print(stderr, "{}\n", diag->text());
        ++diag;
    }
}
}
