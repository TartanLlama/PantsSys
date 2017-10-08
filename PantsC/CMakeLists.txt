find_package(LLVM REQUIRED CONFIG)

add_executable(pc
  src/ASTDeserializer.cpp
  src/ASTPrinter.cpp
  src/ASTSerializationIds.hpp
  src/ASTSerializer.cpp
  src/ASTVisitor.cpp
  src/Compiler.cpp
  src/DiagPrinter.cpp
  src/Lexer.cpp
  src/Parser.cpp
  src/CodeGen.cpp
  ${CMAKE_SOURCE_DIR}/Common/src/PantsIsa.cpp)

target_include_directories(pc PUBLIC include ${CMAKE_SOURCE_DIR}/Common/include ${LLVM_INCLUDE_DIRS})
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(pc PRIVATE fmt optional status_value ${llvm_libs})

set_property(TARGET pc PROPERTY CXX_STANDARD 14)
