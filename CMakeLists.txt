project(IFJ22)
cmake_minimum_required(VERSION 3.2)
add_executable(ifj22 src/main.c src/utils.c src/scanner.c)



add_compile_options(-Wextra -Wall -std=11)