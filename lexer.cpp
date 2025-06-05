/*
$$\      $$\           $$\   $$\     $$\        $$$$$$\                                    $$\       $$\                    
$$$\    $$$ |          $$ |  $$ |    \__|      $$  __$$\                                   $$ |      $$ |                   
$$$$\  $$$$ |$$\   $$\ $$ |$$$$$$\   $$\       $$ /  \__| $$$$$$\  $$\   $$\ $$$$$$\$$$$\  $$$$$$$\  $$ | $$$$$$\  $$$$$$$\ 
$$\$$\$$ $$ |$$ |  $$ |$$ |\_$$  _|  $$ |      $$ |      $$  __$$\ $$ |  $$ |$$  _$$  _$$\ $$  __$$\ $$ |$$  __$$\$$  _____|
$$ \$$$  $$ |$$ |  $$ |$$ |  $$ |    $$ |      $$ |      $$ |  \__|$$ |  $$ |$$ / $$ / $$ |$$ |  $$ |$$ |$$$$$$$$ \$$$$$$\  
$$ |\$  /$$ |$$ |  $$ |$$ |  $$ |$$\ $$ |      $$ |  $$\ $$ |      $$ |  $$ |$$ | $$ | $$ |$$ |  $$ |$$ |$$   ____|\____$$\ 
$$ | \_/ $$ |\$$$$$$  |$$ |  \$$$$  |$$ |      \$$$$$$  |$$ |      \$$$$$$  |$$ | $$ | $$ |$$$$$$$  |$$ |\$$$$$$$\$$$$$$$  |
\__|     \__| \______/ \__|   \____/ \__|       \______/ \__|       \______/ \__| \__| \__|\_______/ \__| \_______\_______/ 
*/

#include "lexer.h"
#include <unordered_map>
#include<unordered_set>
#include <string>
#include <iostream>

Lexer::Lexer(const std::string& input) : input(input), index(0) {}
using namespace std ; 


//------------------------------------------------------------

int FUNCTION_POINTER = 0 ; 
int VARIABLE_POINTER = 0 ; 

vector<int> syntax={1}; 
//------------------------------------------------------------

std::unordered_map<std::string, TokenType> key_words = { // 關鍵字 
    {"for", TokenType::FOR} ,
    {"while" , TokenType::WHILE} , 
    {"if" , TokenType::IF} , 
    {"else" , TokenType::ELSE} , 
    {"elif" , TokenType::ELIF} ,  
    {"break" , TokenType::BREAK},
    {"continue" ,TokenType::CONTINUE},
    {"return" , TokenType::RETURN},
    {"{" , TokenType::BRACE_OPEN},
    {"}" , TokenType::BRACE_CLOSE},
    {"[" , TokenType::BRACKET_OPEN},
    {"]" , TokenType::BRACKET_CLOSE},
    {"(" , TokenType::PAREN_OPEN},
    {")" , TokenType::PAREN_CLOSE} ,
    {"." , TokenType::DOT} , 
    {"+" , TokenType::ADD} , 
    {"-" , TokenType::SUB} ,
    {"!" , TokenType::NOT} ,
    {"/" , TokenType::DIV} ,
    {"*" , TokenType::MUL} , 
    {"%" , TokenType::MOD} , 
    {"<<" , TokenType::SHIFT_LEFT} , 
    {">>" , TokenType::SHIFT_RIGHT} , 
    {">" , TokenType::GREATER_THAN},
    {"<" , TokenType::LESS_THAN},
    {"<=" , TokenType::NOT_GREATER_THAN},
    {">=" , TokenType::NOT_LESS_THAN},
    {"~" , TokenType::BITWISE_NOT} , 
    {"=" , TokenType::ASSIGN},
    {"==" ,TokenType::EQUAL} , 
    {"nil" ,TokenType::NIL} , 
    {"!=" , TokenType::NOT_EQUAL} , 
    
    {"^" , TokenType::BITWISE_XOR} , 
    {"&" , TokenType::BITWISE_AND} , 
    {"|" ,TokenType::BITWISE_OR} ,
    {"and" , TokenType::AND},
    {"or" , TokenType::OR} ,
    {"not" , TokenType::NOT} , 
    
    {"," , TokenType::COMMA} , 
    {";" , TokenType::SEMICOLON},
    {"true" , TokenType::BOOL},
    {"false" , TokenType::BOOL},
    {".." , TokenType::DOTDOT} ,
    {"function" , TokenType::FUNCTION} , 
    {"var" ,TokenType::VAR}   ,
    {"ll" , TokenType::LL}

};
 


char Lexer::currentChar() {
    if (index < input.size()) {
        return input[index];
    }
    return '\0';  // Return null char if at the end (End Of file 返回 0  ) 
}
// Function to advance to the next character (取得下一位字元)
void Lexer::advance() {
    if (index < input.size()) {
        ++index;
    }else
    {
        cout <<"LEXER OUT OF INDEX (ADVANCE)" ; 
    }
}

// 往回看 (能別用就別用  用了 請在此標註  --->              )
void Lexer::previous()
{
    size_t a = index ; //Size_T is unsigned  
    if(index > a-1)
    {
        --index;
    }else
    {
        cout << "LEXER OUT OF INDEX (PREVIOUS) " ; 
    }
}





// Check if we have reached the end of the input
bool Lexer::isEnd() {  // 判斷是否讀取越界  (eof )
    return index >= input.size();
}



std::vector<Token> Lexer::unary_identify(std::vector<Token> tks) {
    static const std::unordered_set<TokenType> unary_preceding_tokens = {
        TokenType::PAREN_OPEN, TokenType::ASSIGN, TokenType::RETURN,
        TokenType::COMMA,TokenType::SEMICOLON , 
        TokenType::BRACKET_OPEN
    };
    
    for (size_t i = 0; i < tks.size(); ++i) {
        if (tks[i].GetType() == TokenType::SUB) {
            if (i == 0 || unary_preceding_tokens.count(tks[i - 1].GetType())) {
                tks[i].ChangeType(TokenType::NEG);
                tks[i].ChangeValue("NEG");
            }
        }
    }
    return tks;
}

// Tokenize function to create tokens
std::vector<Token> Lexer::tokenize() { 
    std::vector<Token> tokens;

    while (!isEnd()) {   // <-------- 讀完一個字符操作跳到這 直到文本結束    
        
        char current = currentChar();      
        if (current == ' ') {  advance(); continue;}
        if (current == '\0') {break ; }  
        if (current == '\n') {advance() ; continue;} 
        /*if(current == '\'') {  // 处理字符                                     ///// <<<------- 這邊有 EDGE CASE 要處理 
            bool is_char_get = false;  
            bool something_error = false; 

            advance();  // 取下一个字符
            if (isEnd())         ////   -->  ''
            {
                tokens.push_back(Token(TokenType::UNKNOWN ,  "UNKNOWN")) ; 
                break;
            }
            char c = currentChar();  ///// -------> c 
            char get_char = c;     //這是第二個  
            advance(); 
            if (isEnd())         ////   --> 'cc  
            {
                tokens.push_back(Token(TokenType::UNKNOWN ,  "UNKNOWN")) ; 
                break;
            }
            char next = currentChar(); //這是第三個 
            if ( get_char != '\\' && next == '\'' )    ////   --> 'c'   
            {
                tokens.push_back(Token(TokenType::CHAR ,std::string(1, get_char))) ;  advance() ; continue;;
                
            }else if (get_char!='\\' && next !='\'' )
            {
                tokens.push_back(Token(TokenType::UNKNOWN ,  "UNKNOWN")) ;  advance() ; continue;;
            }
            advance();
            if (isEnd())             /////   '\n     <----- 少個 ' 
            {
                tokens.push_back(Token(TokenType::UNKNOWN ,  "UNKNOWN")) ;  break;
            }
            char next_next = currentChar();
            if (next_next != '\'' ) {tokens.push_back(Token(TokenType::UNKNOWN ,  "UNKNOWN")) ;  } 
            // 检查是否符合有效的转义字符
            if (next_next == '\'') {
                advance();   
                if (next_next != '\'') {
                    previous();  // 回退字符流，处理错误情况 ////// previous   HERE 
                    is_char_get = false;
                    something_error = true; 
                }
                is_char_get = true;  // 设置字符获取成功
                switch (next) {          
                    case 'n': get_char = '\n'; break; // 处理换行符
                    case '\\': get_char = '\\'; break;  // 处理反斜杠
                    case 't': get_char = '\t'; break;  // 处理制表符
                    case 'r': get_char = '\r'; break;  // 处理回车符
                    case 'b': get_char = '\b'; break;  // 处理退格符
                    case 'f': get_char = '\f'; break;  // 处理换页符
                    case '\'': get_char = '\''; break;  // 处理单引号
                    case '\"': get_char = '\"'; break;  // 处理双引号
                    case '0': get_char = '\0'; break;  // 处理空字符（null）
                    default:
                        cout << "UNKOWN CHAR ASSIGNED" << endl ; 
                        is_char_get = false;  // 转义字符无效
                        break;
                }
                }
                string str ="" ; 
                if (is_char_get)
                {
                    tokens.push_back(Token(TokenType::CHAR ,std::string(1, get_char))) ; 
                }else
                {
                    tokens.push_back(Token(TokenType::UNKNOWN ,  "UNKNOWN")) ;  ; 
                }
            continue; 
        } */
        if (current == '\"')  ///// 這裡開始處理
        {
            string str =""; 
            bool string_compelete = false ; 
            bool is_hold_slash = false ;
            advance() ;  
            char c = currentChar() ; 
            while(!isEnd()) 
            {
                
                if (!is_hold_slash)
                {
                    if (currentChar()== '\\'){
                        is_hold_slash = true ; 
                        c = currentChar() ; 
                        str += c ; 
                        advance() ; 
                        continue; 
                    }
                    if (currentChar() == '\"')
                    {
                        string_compelete = true ; 
                        break ;
                    }
                    c  = currentChar() ; 
                    str+= c ;
                    advance();
                   continue;
                }
                if(is_hold_slash)
                {
                    c = currentChar() ; 
                    is_hold_slash= false ; 
                    str += c ; 
                    advance() ; 
                    continue;
                }    
            }

            advance() ; 
            //tokens.push_back(std::make_unique<StringToken>(str)) ; 
            if (string_compelete)
            {
                tokens.push_back(Token(TokenType::STRING , str)) ; 
            } else
            {
                tokens.push_back(Token(TokenType::UNKNOWN, "UNKNOWN")) ; 
            }
            continue;
        } //// ///// 這裡結束處理字串
        if (isdigit(current)) { /// 這裡開始處理數字 
            string number ; 
            while (isdigit(currentChar()) ) {
                number +=currentChar() ;
                advance();
                if (isEnd()) break;
            }
            if (isalpha(toupper(currentChar())) != true ){ /// 啊?  為何isalpha 允許判定 大寫 
               // cout << currentChar() << endl ; 
            //tokens.push_back(std::make_unique<NumberToken>(number));
            tokens.push_back(Token(TokenType::NUMBER , number)) ; 
            }else
            {
                //tokens.push_back(std::make_unique<UnknownToken>()) ; /// 出現 如 10abc 非法的命名操作 
                tokens.push_back(Token(TokenType::UNKNOWN ,  "UNKNOWN")) ;  
            }
            continue;
        }


        if (isalpha(current)) { /// 處理 Identifer 以及關鍵字  (開頭不允許digit )
            string str = "" ;
            bool is_push= true ;
            str+= current ;
            advance();
            char c = currentChar() ;
             
            while (isalpha(c) ||  c == '_' || isdigit(c)) // 只允許 a~z A~Z _ 0~9
            {
               
                str +=c ;

                advance ();
                if (isEnd()) break;
                c = currentChar() ;
                
            }
            auto it = key_words.find(str) ; 
            TokenType holds = TokenType::UNKNOWN ; 
            if (it != key_words.end())
            {
                holds = it -> second ; 
            } 
            switch (holds)
            {
            case TokenType::FOR :
                tokens.push_back(Token(TokenType::FOR , "FOR")) ; 
                break;
            case TokenType::WHILE : 
            tokens.push_back(Token(TokenType::WHILE , "WHILE")) ; 
                break ; 
            case TokenType::OR :
            tokens.push_back(Token(TokenType::OR , "OR")) ; 
                break ; 
            case TokenType::AND :
            tokens.push_back(Token(TokenType::AND , "AND")) ; 
                break ; 
            case TokenType::NOT : 
            tokens.push_back(Token(TokenType::NOT , "NOT")) ; 
                break;
            case TokenType::BREAK :
            tokens.push_back(Token(TokenType::BREAK , "BREAK")) ;
                break ; 
            case TokenType::CONTINUE :
            tokens.push_back(Token(TokenType::CONTINUE , "CONTINUE")) ; 
                break  ; 
            case TokenType::RETURN :
            tokens.push_back(Token(TokenType::RETURN , "RETURN")) ; 
                break; 
            case TokenType::IF :
            tokens.push_back(Token(TokenType::IF , "IF")) ; 
                break ; 
            case TokenType::ELSE:
            tokens.push_back(Token(TokenType::ELSE , "ELSE")) ;
                break ; 
            case TokenType::ELIF:
            tokens.push_back(Token(TokenType::ELIF , "ELIF")) ;
                break ; 
            case TokenType::CLASS:
            tokens.push_back(Token(TokenType::CLASS , "CLASS")) ; 
                break;
            case TokenType::BOOL:
             tokens.push_back(Token(TokenType::BOOL ,  str)) ; // 這裡要判定 之後改
                break;
            case TokenType::FUNCTION:
            tokens.push_back(Token(TokenType::FUNCTION , "FUNCTION")) ;     
                break; 
            case TokenType::VAR:
            tokens.push_back(Token(TokenType::VAR , "VAR"))  ; 
                break ; 
            case TokenType::NIL:
            tokens.push_back(Token(TokenType::NIL , "NIL")) ; 
                break; 
            default:
                is_push = false ; 
                break;
            }
            if (!is_push)
            {
            tokens.push_back(Token(TokenType::IDENTIFIER , str)) ;
            }

            continue ; 
        }
  
        string  Special_Character= "" ; 
        Special_Character += current ; 
        if (key_words.find(Special_Character) != key_words.end()) //// 這裡處理關鍵符號如 {}[]() +-*/=!><. etc...
        {
            
            advance() ; 
            char next = currentChar() ; 
            TokenType get_token_type = key_words[Special_Character] ;
            
            switch (get_token_type)
            {
            case TokenType::BRACE_OPEN :
                tokens.push_back(Token(TokenType::BRACE_OPEN , "BRACE_OPEN")) ; 
                break  ; 
            case TokenType::BRACE_CLOSE:
            tokens.push_back(Token(TokenType::BRACE_CLOSE , "BRACE_CLOSE")) ; 
                break ; 
            case TokenType::BRACKET_OPEN:
                 tokens.push_back(Token(TokenType::BRACKET_OPEN , "BRACKET_OPEN")) ; 
                break; 
            case TokenType::BRACKET_CLOSE:
                 tokens.push_back(Token(TokenType::BRACKET_CLOSE , "BRACKET_CLOSE")) ; 
                break ; 
            case TokenType::PAREN_OPEN:
            tokens.push_back(Token(TokenType::PAREN_OPEN , "PAREN_OPEN")) ; 
                break ; 
            case TokenType::PAREN_CLOSE:
            tokens.push_back(Token(TokenType::PAREN_CLOSE , "PAREN_CLOSE")) ;  
                break  ;
            case TokenType::ADD:
            tokens.push_back(Token(TokenType::ADD , "ADD")) ; 
                break; 
            case TokenType::SUB:
            tokens.push_back(Token(TokenType::SUB , "SUB")) ; 
                break;
            case TokenType::DIV:
            tokens.push_back(Token(TokenType::DIV , "DIV")) ; 
                break;
            case TokenType::MOD:
            tokens.push_back(Token(TokenType::MOD , "MOD")) ; 
                break;  
            case TokenType::BITWISE_OR:
            tokens.push_back(Token(TokenType::BITWISE_OR , "BITWISE_OR")) ; 
                break;
            case TokenType::BITWISE_AND:
            tokens.push_back(Token(TokenType::BITWISE_AND , "BITWISE_AND")) ; 
                break; 
            case TokenType::BITWISE_XOR :
            tokens.push_back(Token(TokenType::BITWISE_XOR , "BITWISE_XOR")) ; 
                break ; 
            case TokenType::MUL:

            tokens.push_back(Token(TokenType::MUL , "MUL")) ; 
                break;
            case TokenType::BITWISE_NOT :
            tokens.push_back(Token(TokenType::BITWISE_NOT , "BITWISE_NOT"  ) ) ; 
                break ;  
            
            
            case TokenType::DOT:
                if (!isEnd())
                {
                    Special_Character+= currentChar() ; 
                    if (key_words.find(Special_Character) != key_words.end())
                    {
                        if (key_words[Special_Character] == TokenType::DOTDOT)
                        {
                            tokens.push_back(Token(TokenType::DOTDOT , "DOTDOT")) ; /// 12..33 表示使用DOUBLE
                            advance() ; 
                        }
                        else
                        {
                        
                            tokens.push_back(Token(TokenType::DOT , "DOT")) ; 

                        }
                    }
                    else{
                        tokens.push_back(Token(TokenType::DOT , "DOT")) ; 
                    }
                }else{
                    tokens.push_back(Token(TokenType::DOT , "DOT")) ; 
                }
                break;  
            case TokenType::GREATER_THAN:
                if (!isEnd())
                {
                    Special_Character+= currentChar() ; 
                    if (key_words.find(Special_Character) != key_words.end())
                    {
                        if (key_words[Special_Character] == TokenType::SHIFT_RIGHT)
                        {
                            tokens.push_back(Token(TokenType::SHIFT_RIGHT , "SHIFT_RIGHT")) ;
                            advance() ; 

                        }else if (key_words[Special_Character] == TokenType::NOT_LESS_THAN)
                        {
                            tokens.push_back(Token(TokenType::NOT_LESS_THAN , "NOT_LESS_THAN")) ;
                            advance() ; 
                        }
                        else
                        {
                            tokens.push_back(Token(TokenType::GREATER_THAN , "GREATER_THAN")) ; 
                        }
                    }
                    else{
                        tokens.push_back(Token(TokenType::GREATER_THAN , "GREATER_THAN")) ; 
                    }
                }else{
                    tokens.push_back(Token(TokenType::GREATER_THAN , "GREATER_THAN")) ; 
                }
                break; 
            
                case TokenType::LESS_THAN:
                if (!isEnd())
                {
                    Special_Character+= currentChar() ; 
                    if (key_words.find(Special_Character) != key_words.end())
                    {
                        if (key_words[Special_Character] == TokenType::SHIFT_LEFT)
                        {
                            tokens.push_back(Token(TokenType::SHIFT_LEFT , "SHIFT_LEFT")) ;
                            advance() ; 

                        }else if (key_words[Special_Character] == TokenType::NOT_GREATER_THAN)
                        {
                            tokens.push_back(Token(TokenType::NOT_GREATER_THAN , "NOT_GREATER_THAN")) ;
                            advance() ; 

                        }
                        else
                        {
                            tokens.push_back(Token(TokenType::LESS_THAN , "LESS_THAN")) ;
                        }
                    }
                    else{
                        tokens.push_back(Token(TokenType::LESS_THAN , "LESS_THAN")) ;
                    }
                }else{
                    tokens.push_back(Token(TokenType::LESS_THAN , "LESS_THAN")) ;
                }
                break; 

            
            case TokenType::ASSIGN:
                if (!isEnd())
                {
                    Special_Character+= currentChar() ; 
                    if (key_words.find(Special_Character) != key_words.end())
                    {
                        if (key_words[Special_Character] == TokenType::EQUAL)
                        {
                            tokens.push_back(Token(TokenType::EQUAL , "EQUAL")) ;
                            advance() ; 

                        }else
                        {
                            tokens.push_back(Token(TokenType::ASSIGN , "ASSIGN")) ;
                        }
                    }
                    else{
                        tokens.push_back(Token(TokenType::ASSIGN , "ASSIGN")) ;
                    }
                }else{
                    tokens.push_back(Token(TokenType::ASSIGN , "ASSIGN")) ;
                }
                break; 
            case TokenType::NOT:
                if (!isEnd())
                {
                    Special_Character+= currentChar() ; 
                    if (key_words.find(Special_Character) != key_words.end())
                    {
                        if (key_words[Special_Character] == TokenType::NOT_EQUAL)
                        {
                            tokens.push_back(Token(TokenType::NOT_EQUAL , "NOT_EQUAL")) ; 
                            advance() ; 
                        }else
                        {
                            tokens.push_back(Token(TokenType::NOT , "NOT")) ;
                        }
                    }
                    else{
                        tokens.push_back(Token(TokenType::NOT , "NOT")) ; 
                    }
                }else{
                    tokens.push_back(Token(TokenType::NOT , "NOT")) ; 
                }
                break; 

            case TokenType::COMMA :
            tokens.push_back(Token(TokenType::COMMA , "COMMA")) ; 
                break; 
            case TokenType::SEMICOLON :
            tokens.push_back(Token(TokenType::SEMICOLON , "SEMICOLON")) ; 
                break ; 
          

            
            default :
                //tokens.push_back(std::unique_ptr<>) 
                cout<<"TOKENS NOT IN DICT" <<endl ;  ///// 這裡應該達不到 否則是 case 少打
                break  ; 

            }
            continue;
        }
        else 
        {   
            advance() ; 
            tokens.push_back(Token(TokenType::UNKNOWN ,  "UNKNOWN")) ; 
            continue;
        }
        
       cout << "!_WARNING_! MISSING CASES !" << endl ; 
        advance() ;
    }
  //  std::make_unique<EndToken>() ; 
  tokens.push_back(Token(TokenType::END ,  "END")) ; 
  tokens.push_back(Token(TokenType::END ,  "END_1")) ; 
  tokens.push_back(Token(TokenType::END ,  "END_2")) ; 
  tokens.push_back(Token(TokenType::END ,  "END_2")) ; 
    return tokens;
 }



