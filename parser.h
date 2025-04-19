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

#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include<string> 
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <stack>


using namespace std ; 





enum class NodeType {
    OPERATOR,  // 表示運算符節點
    VARIABLE, 
    NUMBER,
    STRING , 
    BOOL ,
    FLOAT,
    DOUBLE, 
    INT,
    CONSTANT, 
    FUNCTION,  
    STATEMENTS ,
    CALL_FUNCTION , 
    DECLARATION , 
    ASSIGNMENT , 
    STATEMENT ,
    TABLE  , 
    UNARY , 
    BINARY  , 
    INDEX, 
    POST_PROCESSING,
    INITIALIZE,
    CHILDREN,
    IF , 
    EMPTY , 
    FOR , 
    WHILE , 
    ARGUMENT, 
    BLOCK ,
    DELIMITER ,
    CHAR , 
    DO,
    CONDITION_DO , 
    UNKNOWN , 

};
class Node {
    public:
        NodeType type ; 
        std::string value;               // 儲存節點的值
        std::vector<std::shared_ptr<Node>> children; 
  
        // 構造函數，初始化節點值   
        Node(NodeType n , const std::string& v) : type(n) , value(v) {}
    
        Node(const Node& other) = default; 
       

        void addChild(std::shared_ptr<Node> child) {
            children.push_back(child);
        }


        void print(int depth = 0 , int last=false) const {
            

            for (int i = 0; i < depth; ++i) {
                if (i<depth-1)
                {
                std::cout <<  "|   ";  // 用縮排來顯示層級
                }
                else
                {
                    if (!last)
                    {
                        std::cout << "|----";
                    }else
                    {
                        std::cout << "|----" ;             
                    }
                }
            }
            std::cout << value << std::endl;
            for (int i = 0 ; i < children.size() ; i++) {
                if (i < children.size()-1)
                {
                children[i]->print(depth + 1 , false )  ; 
                }
                else
                {
                    children[i]->print(depth + 1 , true) ;
                }
            }
         
          
    
            // 遞迴打印子節點
         
        }
        
        

      
};
    

class Parser {
    private:
       
    public:
        bool PARSING_ERROR  = false ; 
        int ERROR_POSITION = 0 ; 
        std::vector<Token> tokens;
        size_t currentIndex;
        shared_ptr<Node> ParseForUpdate() ; 
        shared_ptr<Node> ParseShift() ; 
        shared_ptr<Node> ParseCompare() ; 
        shared_ptr<Node> ParseTable() ; 

        shared_ptr<Node> ParseLogic_OR();
        shared_ptr<Node> ParseLogic_AND();
        shared_ptr<Node> ParseLogic_XOR();
        shared_ptr<Node> ParseBitwise_OR();
        shared_ptr<Node> ParseBitwise_AND();
        shared_ptr<Node> ParseBitwise_XOR();
        shared_ptr<Node> ParseIs_Equal() ; 
        shared_ptr<Node> ParseIf () ; 
        shared_ptr<Node> ParsElse() ; 
        shared_ptr<Node> ParseCondition_Do () ; 
        shared_ptr<Node> ParseBlock () ; // { print ("hello world")} <-- parse 框框這裡面的指令
        shared_ptr<Node> ParseLoop_Branch_Statemente() ; // 處理 for while if  
        shared_ptr<Node> ParseStatement();
        shared_ptr<Node> ParseExpression() ; 
        shared_ptr<Node> ParseType() ; 
        shared_ptr<Node> ParseTerm ();
        shared_ptr<Node> ParseDots_Or_NonNumericValue () ; 
        shared_ptr<Node> ParseObject () ; 
        shared_ptr<Node> ParseLoop () ; 
        shared_ptr<Node> ParseWhile() ; 
        shared_ptr<Node> ParseInit() ;
        shared_ptr<Node> ParsePostProcessing() ; 
        shared_ptr<Node> ParseArguments()  ;
        shared_ptr<Node> ParseAssign_Only () ; 
        shared_ptr<Node> ParseForCondition() ; 
        shared_ptr<Node> ParseFor () ; 
        shared_ptr<Node> ParseIndexing() ; 
        shared_ptr<Node> ParseCallFunction_Or_Variable_Or_Indexing() ; //
        shared_ptr<Node> ParseCreateFunction() ; 
        shared_ptr<Node> ParseCreateVariable() ; 
        shared_ptr<Node> ParseArguments_Create() ; 
        shared_ptr<Node> ParseAssign_Or_FunctionAsStatemente () ; 
        shared_ptr<Node> ParseFactor () ; 
        shared_ptr<Node> ParseFloat_Double () ; 
        shared_ptr<Node> ParseFunction() ; 
        std::vector<bool> tokens_parsed ; 
      
        void ERROR ()
        {
            PARSING_ERROR = true; 
            ERROR_POSITION = currentIndex ; 
    
        }
        Parser(const std::vector<Token>& tokens) : tokens(tokens), currentIndex(0) {}
    
        void advance() {
            if (currentIndex < tokens.size())
                currentIndex++;
            else
                std::runtime_error("PARSING OUT OF RANGE") ; 
        }
        void previous()
        {
            if (currentIndex !=0 ) 
            {
                currentIndex-- ; 
            }
        }
        void Flag ()
        {
            tokens_parsed[currentIndex] = true ; 
        }

        bool Match(TokenType Match_Token_Type)
        {
            Token Now_Token = currentToken() ; 
            if(!isEnd() && Match_Token_Type == Now_Token.GetType())
            {
                return true ; 
            }
            return false ; 
        }


        bool isEnd()
        {
            return currentIndex-1>=tokens.size() ; //最後有放入End Token  
        }
        bool isStart()
        {
            
            return currentIndex==0 ; 
        }
    
        Token getToken (int position )
        {
            if (position < tokens.size())
            return tokens[position];
            else 
            std::runtime_error("GET TOKEN OUT OF BOUND") ; 
            std::cout<< "GET TOKEN OUT OF BOUND" << endl ; 
            return tokens[tokens.size() - 1 ] ;  
        }
        Token previousToken()
        {
            return tokens[(currentIndex-1)<currentIndex?currentIndex-1 : currentIndex] ; 
        }
        Token nextToken ()
        {
            if (currentIndex+1 >=tokens.size() ) 
            {
                std::cout<< "GET TOKEN OUT OF BOUND" << endl ;
                return tokens[tokens.size()-1] ; 
            }
            return tokens[currentIndex+1] ; 
        }
        Token currentToken()
        {
            if (currentIndex >=tokens.size() ) 
            {
                std::cout<< "GET TOKEN OUT OF BOUND" << endl ;
                return tokens[tokens.size()-1] ; 
            }
            return tokens[currentIndex] ; 
        }
        shared_ptr<Node> Parsing() ; 
};


#endif 