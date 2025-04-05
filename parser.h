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
    CONSTANT, 
    FUNCTION,  
    ASSIGNMENT , 
    STATEMENT , 
    ROOT,
    DELIMITER ,
    CHAR , 
    UNKNOWN , 

};








class Node {
    public:
        NodeType type ; 
        
        std::string value;               // 儲存節點的值
        std::vector<std::shared_ptr<Node>> children;  // 儲存子節點
    
        // 構造函數，初始化節點值
        Node(NodeType n , const std::string& v) : type(n) , value(v) {}
    
        // 添加子節點
        void addChild(std::shared_ptr<Node> child) {
            children.push_back(child);
        }
    
        // 打印樹（簡單的遞迴打印）
        void print(int depth = 0) const {
            // 顯示當前節點值
            for (int i = 0; i < depth; ++i) {
                std::cout << "  ";  // 用縮排來顯示層級
            }
            std::cout << value << std::endl;
    
            // 遞迴打印子節點
            for (const auto& child : children) {
                child->print(depth + 1);
            }
        }
};
    

class Parser {
    private:
        std::vector<Token> tokens;
        size_t currentIndex;
        shared_ptr<Node> ParseBlock () ; // { print ("hello world")} <-- parse 框框這裡面的指令
        shared_ptr<Node> ParseLoop_Branch_Statemente() ; // 處理 for while if  
        shared_ptr<Node> ParseStatement();
        shared_ptr<Node> ParseExpression() ; 
        shared_ptr<Node> ParseTerm ();
        shared_ptr<Node> ParseDots_Or_NonNumericValue () ; 
        shared_ptr<Node> ParseObject () ; 
        shared_ptr<Node> ParseCallFunction_Or_Variable() ; //
        shared_ptr<Node> ParseCreateFunction() ; 
        shared_ptr<Node> ParseArguments() ; 
        shared_ptr<Node> ParsParseAssign_Or_FunctionAsStatemente () ; 
        shared_ptr<Node> ParseFactor () ; 
        shared_ptr<Node> ParseRoot () ;
        shared_ptr<Node> ParseFloat_Double () ; 
        shared_ptr<Node> ParseFunction() ; 
        std::vector<bool> tokens_parsed ; 
    public:
        Node root =  Node(NodeType::ROOT, "ROOT");  

        Parser(const std::vector<Token>& tokens) : tokens(tokens), currentIndex(0) {}
    
        void advance() {
            if (currentIndex < tokens.size())
                currentIndex++;
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
            return tokens[position];
        }
        Token previousToken()
        {
            return tokens[(currentIndex-1)<currentIndex?currentIndex-1 : currentIndex] ; 
        }
        Token nextToken ()
        {
            return tokens[currentIndex+1] ; 
        }
        Token currentToken()
        {
            return tokens[currentIndex] ; 
        }


        shared_ptr<Node> Parsing() ; 


};

#endif PARSER_H