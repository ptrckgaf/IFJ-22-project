project(IFJ22)
cmake_minimum_required(VERSION 3.2)
add_executable(IFJ22 src/main.c src/utils.c src/scanner.c src/parser.c src/codegen.c src/semantics.c src/symtable.c)



add_compile_options(-Wextra -Wall -std=11)