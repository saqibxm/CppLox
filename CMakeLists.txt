cmake_minimum_required(VERSION 3.20)

project(Lox)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_SOURCE_DIR ./CppLox)
set(SOURCES
        CppLox/Main.cpp
        CppLox/Lox.cpp
        CppLox/src/Token.cpp
        CppLox/src/Literal.cpp

        CppLox/src/Common/Statement.cpp
        CppLox/src/Common/Visitor.cpp
        CppLox/src/Common/Expression.cpp
        CppLox/src/Common/RPNPrinter.cpp

        CppLox/src/Lexer/Lexer.cpp
        CppLox/src/Parser/Parser.cpp
        CppLox/src/Interpreter/Interpreter.cpp

        CppLox/src/Diagnostics/Diagnostics.cpp

        CppLox/src/Functions/Callable.cpp
        CppLox/src/Functions/LoxFunction.cpp
        CppLox/src/Functions/Clock.cpp
        CppLox/src/Environment/Environment.cpp
        CppLox/include/Functions/Sleep.hpp
)

include_directories("CppLox/include/" "CppLox/")
add_executable(lox ${SOURCES})
