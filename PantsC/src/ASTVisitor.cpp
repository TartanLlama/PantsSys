#include "AST.hpp"

namespace pants {
void ASTVisitor::Visit(AST &ast) {
    for (auto node = ast.NodesBegin(), end = ast.NodesEnd(); node != end; ++node) {
        (*node)->Accept(*this);
    }
}
}
