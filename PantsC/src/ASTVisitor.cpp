#include "AST.hpp"

namespace pants {
void ASTVisitor::Visit(AST &ast) {
    for (auto&& node : ast.GetNodes()) {
        node->Accept(*this);
    }
}
}
