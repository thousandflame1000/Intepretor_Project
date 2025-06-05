

#include <string>
#include <memory>
#include <vector>
#include <variant>
#include <regex>
#include <cctype>
#include <iostream>
#include <unordered_map>
#include "lexer.h"
using namespace std ; 
Lexer::Lexer(const std::string& input) : input(input), index(0) {}
struct TokenRule {
    TokenType type;
    std::regex pattern;
};
std::unordered_map<std::string, TokenType> key_words = { 
    {"for", TokenType::FOR},
    {"while", TokenType::WHILE},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"elif", TokenType::ELIF},
    {"break", TokenType::BREAK},
    {"continue", TokenType::CONTINUE},
    {"return", TokenType::RETURN},
    {"{" , TokenType::BRACE_OPEN},
    {"}" , TokenType::BRACE_CLOSE},
    {"[" , TokenType::BRACKET_OPEN},
    {"]" , TokenType::BRACKET_CLOSE},
    {"(" , TokenType::PAREN_OPEN},
    {")" , TokenType::PAREN_CLOSE},
    {"." , TokenType::DOT},
    {"+" , TokenType::ADD},
    {"-" , TokenType::SUB},
    {"!" , TokenType::NOT},
    {"/" , TokenType::DIV},
    {"*" , TokenType::MUL},
    {"%" , TokenType::MOD},
    {"<<" , TokenType::SHIFT_LEFT},
    {">>" , TokenType::SHIFT_RIGHT},
    {">" , TokenType::GREATER_THAN},
    {"<" , TokenType::LESS_THAN},
    {"<=" , TokenType::NOT_GREATER_THAN},
    {">=" , TokenType::NOT_LESS_THAN},
    {"=" , TokenType::ASSIGN},
    {"==" , TokenType::EQUAL},
    {"!=" , TokenType::NOT_EQUAL},
    {"^" , TokenType::BITWISE_XOR},
    {"&" , TokenType::BITWISE_AND},
    {"|" , TokenType::BITWISE_OR},
    {"and" , TokenType::AND},
    {"or" , TokenType::OR},
    {"not" , TokenType::NOT},
    {"," , TokenType::COMMA},
    {";" , TokenType::SEMICOLON},
    {"true" , TokenType::BOOL},
    {"false" , TokenType::BOOL},
    {".." , TokenType::DOTDOT},
    {"class" , TokenType::CLASS},
    {"function" , TokenType::FUNCTION},
    {"var" , TokenType::VAR},
    {"ll" , TokenType::LL}
};

std::unordered_map<TokenType, std::string> token_values = {
    {TokenType::FOR, "FOR"},
    {TokenType::WHILE, "WHILE"},
    {TokenType::IF, "IF"},
    {TokenType::ELSE, "ELSE"},
    {TokenType::ELIF, "ELIF"},
    {TokenType::BREAK, "BREAK"},
    {TokenType::CONTINUE, "CONTINUE"},
    {TokenType::RETURN, "RETURN"},
    {TokenType::BRACE_OPEN, "BRACE_OPEN"},
    {TokenType::BRACE_CLOSE, "BRACE_CLOSE"},
    {TokenType::BRACKET_OPEN, "BRACKET_OPEN"},
    {TokenType::BRACKET_CLOSE, "BRACKET_CLOSE"},
    {TokenType::PAREN_OPEN, "PAREN_OPEN"},
    {TokenType::PAREN_CLOSE, "PAREN_CLOSE"},
    {TokenType::DOT, "DOT"},
    {TokenType::ADD, "ADD"},
    {TokenType::SUB, "SUB"},
    {TokenType::NOT, "NOT"},
    {TokenType::DIV, "DIV"},
    {TokenType::MUL, "MUL"},
    {TokenType::MOD, "MOD"},
    {TokenType::SHIFT_LEFT, "SHIFT_LEFT"},
    {TokenType::SHIFT_RIGHT, "SHIFT_RIGHT"},
    {TokenType::GREATER_THAN, "GREATER_THAN"},
    {TokenType::LESS_THAN, "LESS_THAN"},
    {TokenType::NOT_GREATER_THAN, "NOT_GREATER_THAN"},
    {TokenType::NOT_LESS_THAN, "NOT_LESS_THAN"},
    {TokenType::ASSIGN, "ASSIGN"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::NOT_EQUAL, "NOT_EQUAL"},
    {TokenType::BITWISE_XOR, "BITWISE_XOR"},
    {TokenType::BITWISE_AND, "BITWISE_AND"},
    {TokenType::BITWISE_OR, "BITWISE_OR"},
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::DOTDOT, "DOTDOT"},
    {TokenType::CLASS, "CLASS"},
    {TokenType::FUNCTION, "FUNCTION"},
    {TokenType::VAR, "VAR"},
    {TokenType::LL, "LL"},
    {TokenType::END, "END"},
    {TokenType::WHITESPACE, "WHITESPACE"}
};

std::vector<TokenRule> tokenRules = {
        
    {TokenType::STRING,     std::regex(R"("[^"]*")")},          // 字串
    {TokenType::ADD,        std::regex(R"(\+)")},               // 加號
    {TokenType::SUB,        std::regex(R"(-)")},                // 減號
    {TokenType::ASSIGN,     std::regex(R"(=)")},                // 賦值
    {TokenType::PAREN_OPEN, std::regex(R"(\()")},               // 左括號
    {TokenType::PAREN_CLOSE,std::regex(R"(\))")},               // 右括號
    {TokenType::WHITESPACE,std::regex(R"(\s+)")},               // 空白字符
    {TokenType::MUL,        std::regex(R"(\*)")},               // 乘號
    {TokenType::DIV,        std::regex(R"(/)")},                // 除號
    {TokenType::MOD,        std::regex(R"(\%)")},               // 模除號
    {TokenType::SHIFT_LEFT, std::regex(R"(\<<)")},             // 左位移
    {TokenType::SHIFT_RIGHT,std::regex(R"(\>>)")},             // 右位移
    {TokenType::GREATER_THAN, std::regex(R"(>)")},             // 大於
    {TokenType::LESS_THAN,   std::regex(R"(<)")},              // 小於
    {TokenType::NOT_GREATER_THAN, std::regex(R"(<=)")},        // 小於等於
    {TokenType::NOT_LESS_THAN,    std::regex(R"(>=)")},        // 大於等於
    {TokenType::EQUAL,       std::regex(R"(==)")},             // 等於
    {TokenType::NOT_EQUAL,   std::regex(R"(!=)")},             // 不等於
    {TokenType::BITWISE_XOR, std::regex(R"(\^)")},             // 位運算 XOR
    {TokenType::BITWISE_AND, std::regex(R"(&)")},              // 位運算 AND
    {TokenType::BITWISE_OR,  std::regex(R"(\|)")},             // 位運算 OR
    {TokenType::AND,         std::regex(R"(\band\b)")},        // 邏輯 AND
    {TokenType::OR,          std::regex(R"(\bor\b)")},         // 邏輯 OR
    {TokenType::NOT,         std::regex(R"(\bnot\b)")},        // 邏輯 NOT
    {TokenType::BRACE_OPEN , std::regex(R"(\{)")},             // 左大括號
    {TokenType::BRACE_CLOSE ,std::regex(R"(\})")},            // 右大括號
    {TokenType::COMMA,       std::regex(R"(,)")},              // 逗號
    {TokenType::SEMICOLON,   std::regex(R"(;)")},              // 分號
    {TokenType::BOOL,        std::regex(R"(true|false)")},     // 布林值
    {TokenType::DOTDOT,      std::regex(R"(\.\.)")},           // 兩個點（..）
    {TokenType::CLASS,       std::regex(R"(\bclass\b)")},       // 類別關鍵字
    {TokenType::FUNCTION,    std::regex(R"(\bfunction\b)")},    // 函數關鍵字
    {TokenType::VAR,         std::regex(R"(\bvar\b)")},        // 變數關鍵字 
    {TokenType::NUMBER,     std::regex(R"(\d+)")},
    {TokenType::IDENTIFIER, std::regex(R"([a-zA-Z_]\w*)")}        // 自定義關鍵字 "ll"
};

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    size_t pos = 0;

    while (pos < input.length()) {
        bool matched = false;

        for (const auto& rule : tokenRules) {
            std::smatch match;
            std::string remaining = input.substr(pos);

            if (std::regex_search(remaining, match, rule.pattern, std::regex_constants::match_continuous)) {
                std::string value = match.str();

                // 忽略空白
                if (rule.type == TokenType::WHITESPACE) {
                    pos += value.length();
                    matched = true;
                    break;
                }

                TokenType type = rule.type;

                // 若是 identifier，檢查是不是關鍵字
                if (type == TokenType::IDENTIFIER) {
                    auto it = key_words.find(value);
                    if (it != key_words.end()) {
                        type = it->second;  // 替換為關鍵字 token
                    }
                }

                // 將 TokenType 替換為對應的映射值
                auto token_value_it = token_values.find(type);
                if (token_value_it != token_values.end()) {
                    value = token_value_it->second;  // 使用映射的字串值
                }

                tokens.push_back({type, value});
                pos += value.length();
                matched = true;
                break;
            }
        }

        if (!matched) {
            // std::cerr << "Unknown token at position: " << pos << " (" << input[pos] << ")" << std::endl;
            ++pos;  // 跳過錯誤字元
        }
    }
    tokens.push_back(Token(TokenType::END, "END"));
    return tokens;
}
