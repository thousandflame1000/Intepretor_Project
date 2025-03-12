#include "lexer.h"
#include <unordered_map>
#include <string>
// Constructor for Lexer
Lexer::Lexer(const std::string& input) : input(input), index(0) {}
using namespace std ; 



std::unordered_map<std::string, std::unique_ptr<Token>> key_words = {
    {"for", std::unique_ptr<ForToken>()} , 


};
 


char Lexer::currentChar() {
    if (index < input.size()) {
        return input[index];
    }
    return '\0';  // Return null char if at the end
}

// Function to advance to the next character
void Lexer::advance() {
    if (index < input.size()) {
        ++index;
    }
}

// Check if we have reached the end of the input
bool Lexer::isEnd() {
    return index >= input.size();
}

// Tokenize function to create tokens
std::vector<std::unique_ptr<Token>> Lexer::tokenize() {





    std::vector<std::unique_ptr<Token>> tokens;
    





    while (!isEnd()) {





        char current = currentChar();




        if (current == '\"') 
        {
            string str =""; 
            bool is_hold_slash = false ; 
            while(current != '\"' && !is_hold_slash) 
            {

                if (current == '\n' )
                {
                    continue;
                }


                if (!is_hold_slash)
                {
                    if (current == '\\'){
                        is_hold_slash = true ; 
                        advance() ; 
                        continue; 
                    }
                    str+= current ; 
                    advance();
                    continue;
                }
                if(is_hold_slash)
                {
                    is_hold_slash = false ; 
                    switch (current)
                    {
                    case 'n':
                        str += '\n';  // 处理换行符
                        continue;
                        break;
                
                    case '\\':
                        str += '\\';  // 处理反斜杠
                        continue;
                        break;
                
                    case 't':
                        str += '\t';  // 处理制表符
                        continue;
                        break;
                
                    case 'r':
                        str += '\r';  // 处理回车符
                        continue;
                        break;
                
                    case 'b':
                        str += '\b';  // 处理退格符
                        continue;
                        break;
                
                    case 'f':
                        str += '\f';  // 处理换页符
                        continue;
                        break;
                
                    case '\'':
                        str += '\'';  // 处理单引号
                        continue;
                        break;
                
                    case '\"':
                        str += '\"';  // 处理双引号
                        continue;
                        break;
                
                    case '0':
                        str += '\0';  // 处理空字符（null）
                        continue;
                        break;
                
                 
                
                    default:
                        
                        break;
                    }
                }         
            } 
            tokens.push_back(std::make_unique<StringToken>(str)) ;         
        }
        
       




        if (isdigit(current)) {
            int number = 0;
            while (isdigit(currentChar())) {
                number = number * 10 + (currentChar() - '0');
                advance();
            }
            if (currentChar() == ' '){
            tokens.push_back(std::make_unique<NumberToken>(number));
            }else
            {
                tokens.push_back(std::make_unique<UnknownToken>()) ; /// 出現 10abc 非法的命名操作 
            }
            continue;
        }





        if (isalpha(current)) {
            string str = "" ;
            str+= current ;
            advance();
            while (isalpha(current) || '_' || isdigit(current))
            {
                str +=current ;
                advance ();
            }
            


            
        }



        


    }
    std::make_unique<EndToken>() ; 
    return tokens;
}

int main() {
    // Example input string
    std::string input = "123 456 789";
    
    // Create a Lexer instance
    Lexer lexer(input);

    // Tokenize the input
    auto tokens = lexer.tokenize();

    // Print each token
    for (const auto& token : tokens) {
        token->print();
    }

    return 0;
}
