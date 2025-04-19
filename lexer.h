/*
$$\      $$\           $$\   $$\     $$\        $$$$$$\                                    $$\       $$\                    
$$$\    $$$ |          $$ |  $$ |    \__|      $$  __$$\                                   $$ |      $$ |                   
$$$$\  $$$$ |$$\   $$\ $$ |$$$$$$\   $$\       $$ /  \__| $$$$$$\  $$\   $$\ $$$$$$\$$$$\  $$$$$$$\  $$ | $$$$$$\  $$$$$$$\ 
$$\$$\$$ $$ |$$ |  $$ |$$ |\_$$  _|  $$ |      $$ |      $$  __$$\ $$ |  $$ |$$  _$$  _$$\ $$  __$$\ $$ |$$  __$$\$$  _____|
$$ \$$$  $$ |$$ |  $$ |$$ |  $$ |    $$ |      $$ |      $$ |  \__|$$ |  $$ |$$ / $$ / $$ |$$ |  $$ |$$ |$$$$$$$$ \$$$$$$\  
$$ |\$  /$$ |$$ |  $$ |$$ |  $$ |$$\ $$ |      $$ |  $$\ $$ |      $$ |  $$ |$$ | $$ | $$ |$$ |  $$ |$$ |$$   ____|\____$$\ 
$$ | \_/ $$ |\$$$$$$  |$$ |  \$$$$  |$$ |      \$$$$$$  |$$ |      \$$$$$$  |$$ | $$ | $$ |$$$$$$$  |$$ |\$$$$$$$\$$$$$$$  |
\__|     \__| \______/ \__|   \____/ \__|       \______/ \__|       \______/ \__| \__| \__|\_______/ \__| \_______\_______/            



┌─────────┐                                       
│  LEXER  ┼───┬────Class Token ──┬──TokenType     
└─────────┘   │                  │                            
              |                  └──Value (string)                           
              └────tokenize(string ) 


*/





#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <memory>
#include <vector>
#include <variant>

#include <cctype>
#include <iostream>
using namespace std ; 
enum class TokenType {
    NUMBER,     // 数字，如 123, 456
    BOOL,
    CHAR,
    STRING,
    IDENTIFIER, // 变量名或函数名，如 x, sum
    FOR,        // 关键字 for
    WHILE,      // 关键字 while
    IF,         // 关键字 if
    ELSE,       // 关键字 else
    ELIF,
    BREAK ,    
    BRACE_OPEN , 
    BRACE_CLOSE , 
    CONTINUE , 
    RETURN , 
    BRACKET_OPEN , 
    BRACKET_CLOSE,
    PAREN_OPEN, // 左括号 (
    PAREN_CLOSE,// 右括号 )
    SEMICOLON , 
    END,        // 结束标记
    COMMA , 
    ASSIGN,
    NEG , 
    ADD ,
    SUB , 
    MUL , 
    DIV , 
    MOD , 
    AND ,
    OR,
    XOR , 
    NOT ,
    EQUAL ,  
    TRUE,
    FALSE,
    NOT_EQUAL , 
    LESS_THAN , 
    GREATER_THAN , 
    NOT_LESS_THAN, // 大於等於
    NOT_GREATER_THAN ,  // 小於等於
    SHIFT_LEFT , 
    SHIFT_RIGHT , 
    DOT ,    
    DOTDOT, // 表示雙位符點數
    AREA ,
    FUNCTION, 
    CLASS ,     
    VAR,
    UNKNOWN     // 无法识别的字符
};




class Token {
    private:
        TokenType type;
        std::string value;
    
    public:
        Token(TokenType t, std::string v)
            : type(t), value(v) {}
        
        void ChangeType (TokenType ttyp) { type = ttyp ; }  
        void ChangeValue (std::string g_val) {value = g_val ; }
        TokenType GetType() const { return type; }
        std::string GetValue() const { return value; }
    };



class Lexer {
        public:
            explicit Lexer(const std::string& input);
            //std::vector<std::unique_ptr<Token>> tokenize();  // Tokenize the input string
            vector<Token> tokenize() ; 
            vector<Token> unary_identify (vector<Token> tks ) ; 

            //Token tokenize1() ; 
            // Function to print tokens to stringstream
        
        private:
            std::string input;  // Input source code
            size_t index;       // Current index in input
            bool Token_Invalid = false ; 
            char currentChar(); // Get the current character
            void previous() ; 
            void advance();     // Move to the next character
            bool isEnd();       // Check if the input is finished
        };
#endif 
