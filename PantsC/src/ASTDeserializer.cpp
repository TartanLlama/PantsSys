#include "ASTDeserializer.hpp"


namespace pants {
AST ASTDeserializer::Deserialize() {
    return AST(Read<std::vector<ASTNode>>());
}
}
