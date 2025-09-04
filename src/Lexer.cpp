// Lexer.cpp - Implementation
#include "Lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& input) 
    : input(input), current(0), line(1), column(1) {
    initKeywords();
}

void Lexer::initKeywords() {
    keywords = {
        {"var", TokenType::VAR},
        {"function", TokenType::FUNCTION},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"while", TokenType::WHILE},
        {"return", TokenType::RETURN},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE}
    };
}

char Lexer::peek(int offset) {
    size_t pos = current + offset;
    if (pos >= input.length()) return '\0';
    return input[pos];
}

char Lexer::advance() {
    if (isAtEnd()) return '\0';
    
    char c = input[current++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        } else {
            break;
        }
    }
}

void Lexer::skipComment() {
    if (peek() == '/' && peek(1) == '/') {
        // Skip until end of line
        while (peek() != '\n' && !isAtEnd()) {
            advance();
        }
    }
}

Token Lexer::makeNumber() {
    int startLine = line;
    int startColumn = column;
    std::string value;
    
    while (std::isdigit(peek())) {
        value += advance();
    }
    
    return Token(TokenType::NUMBER, value, startLine, startColumn);
}

Token Lexer::makeIdentifier() {
    int startLine = line;
    int startColumn = column;
    std::string value;
    
    while (std::isalnum(peek()) || peek() == '_') {
        value += advance();
    }
    
    // Check if it's a keyword
    TokenType type = TokenType::IDENTIFIER;
    auto it = keywords.find(value);
    if (it != keywords.end()) {
        type = it->second;
    }
    
    return Token(type, value, startLine, startColumn);
}

Token Lexer::nextToken() {
    skipWhitespace();
    skipComment();
    skipWhitespace(); // Skip whitespace after comments too
    
    if (isAtEnd()) {
        return Token(TokenType::END_OF_FILE, "", line, column);
    }
    
    int startLine = line;
    int startColumn = column;
    char c = advance();
    
    switch (c) {
        case '+': return Token(TokenType::PLUS, "+", startLine, startColumn);
        case '-': return Token(TokenType::MINUS, "-", startLine, startColumn);
        case '*': return Token(TokenType::MULTIPLY, "*", startLine, startColumn);
        case '/': return Token(TokenType::DIVIDE, "/", startLine, startColumn);
        case '(': return Token(TokenType::LEFT_PAREN, "(", startLine, startColumn);
        case ')': return Token(TokenType::RIGHT_PAREN, ")", startLine, startColumn);
        case '{': return Token(TokenType::LEFT_BRACE, "{", startLine, startColumn);
        case '}': return Token(TokenType::RIGHT_BRACE, "}", startLine, startColumn);
        case ',': return Token(TokenType::COMMA, ",", startLine, startColumn);
        case ';': return Token(TokenType::SEMICOLON, ";", startLine, startColumn);
        case '!':
            if (peek() == '=') {
                advance();
                return Token(TokenType::NOT_EQUAL, "!=", startLine, startColumn);
            }
            return Token(TokenType::LOGICAL_NOT, "!", startLine, startColumn);
        case '=':
            if (peek() == '=') {
                advance();
                return Token(TokenType::EQUAL, "==", startLine, startColumn);
            }
            return Token(TokenType::ASSIGN, "=", startLine, startColumn);
        case '<':
            if (peek() == '=') {
                advance();
                return Token(TokenType::LESS_EQUAL, "<=", startLine, startColumn);
            }
            return Token(TokenType::LESS_THAN, "<", startLine, startColumn);
        case '>':
            if (peek() == '=') {
                advance();
                return Token(TokenType::GREATER_EQUAL, ">=", startLine, startColumn);
            }
            return Token(TokenType::GREATER_THAN, ">", startLine, startColumn);
        case '&':
            if (peek() == '&') {
                advance();
                return Token(TokenType::LOGICAL_AND, "&&", startLine, startColumn);
            }
            break;
        case '|':
            if (peek() == '|') {
                advance();
                return Token(TokenType::LOGICAL_OR, "||", startLine, startColumn);
            }
            break;
        default:
            if (std::isdigit(c)) {
                current--; column--; // Back up to re-read the digit
                return makeNumber();
            }
            if (std::isalpha(c) || c == '_') {
                current--; column--; // Back up to re-read the character
                return makeIdentifier();
            }
            break;
    }
    
    return Token(TokenType::UNKNOWN, std::string(1, c), startLine, startColumn);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token = nextToken();
    
    while (token.type != TokenType::END_OF_FILE) {
        tokens.push_back(token);
        token = nextToken();
    }
    
    tokens.push_back(token); // Add EOF token
    return tokens;
}

bool Lexer::isAtEnd() {
    return current >= input.length();
}