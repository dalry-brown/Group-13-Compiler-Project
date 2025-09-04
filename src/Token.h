// Token.h - Token definitions and types
#pragma once
#include <string>
#include <unordered_map>

enum class TokenType {
    // Literals
    NUMBER,
    IDENTIFIER,
    
    // Keywords
    VAR,
    FUNCTION,
    IF,
    ELSE,
    WHILE,
    RETURN,
    TRUE,
    FALSE,
    
    // Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    
    // Comparison
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_EQUAL,
    GREATER_THAN,
    GREATER_EQUAL,
    
    // Logical
    LOGICAL_AND,
    LOGICAL_OR,
    LOGICAL_NOT,
    
    // Delimiters
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    SEMICOLON,
    
    // Special
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    
    Token(TokenType t, const std::string& v, int l, int c)
        : type(t), value(v), line(l), column(c) {}
};