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


#include "parser.h" 
#include "lexer.h"
#include<unordered_map> 
#include<string>
#include<stack>



 /*

<function> ::= "function" <identifier> "(" <parameters> ")" "{" <statements> "}"
<parameters> ::= <parameter> | <parameter> "," <parameters> | ε
<parameter> ::= <type> <identifier> 
-------------------^ // 定義型別?? or no  
<statements> ::= <statement> | <statement> <statements>
<statement> ::= <expression> ";" | <return_statement> ";" | <variable_declaration> ";"
<return_statement> ::= "return" <expression>
<expression> ::= <identifier> | <literal> | <function_call>
<function_call> ::= <identifier> "(" <arguments> ")"

<arguments> ::= <expression> | <expression> "," <arguments> | ε


/////
<expression> ::= <bool> | <integer> | <flaot> | <string> | <char> |<table> | 

////

<table> ::= "[" <element> "]"
<element> ::= <bool> | <integer> | <float> | <string> | <char> | <table>
<bool> ::= "true" | "false"


/////////////////////////////////////////////
<double> ::= <number> |".."| <number>                                                           ----->  ParseDouble
<float> ::= <number> |"."| <number>                                                             ----->  ParseFloat
/////////'///////////////////////////////////

<string> :: <char> <string> |   ε
<char> ::= <letter>
<integer> ::= <number> 
<number> ::= <digit> | <number> | εS
<identifier> ::= <letter> <identifier_rest> | <digit> "_"
<identifier_rest> ::= <letter> <identifier_rest> | <digit> <identifier_rest> | ε
<digit> ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
<letter> ::= "a" | "b" | "c" | ... | "z" | "A" | "B" | ... | "Z"






<a> := <a> "+ - * /" <a> | ε




                                                                                                                                                 +
                                                                                                                                               /  \
                                                                                                                                              *     3
                                                                                                                                            /  \
                                                                                                                                           1   2
TODO LIST -> float -> 
*/
unordered_map <TokenType , int > Top_Down_Piority {      //// 這裡是說Top down 執行下的piority 如果操作符的piority 越低這裡 反而越高 如圖-----^

{TokenType::IDENTIFIER ,  0 },
{TokenType::STRING , 0 },
{TokenType::BOOL , 0 } ,
{TokenType::NUMBER , 0},
{TokenType::DOT ,1},
{TokenType::DOTDOT , 1}  , 
{TokenType::MUL, 2},
{TokenType::DIV , 2 },
{TokenType::MOD , 2 } ,
{TokenType::ADD , 2 },
{TokenType::SUB , 2 }, ///!! 注意! 這邊sub還要先判定是否是負號  因為neg 的piority 低於 mul mod div (但高於dot)
} ;





shared_ptr<Node> Parser::ParseBlock()
{
    advance() ; 
    int current_idx = currentIndex ; 
    Node return_block (NodeType::BLOCK , "BLOCK") ; 
    while (!Match(TokenType::BRACKET_CLOSE)) // 
    {

        if (Match(TokenType::END) || Match(TokenType::BRACKET_OPEN))   // { {  <- 對不上是因為子函數沒對到
        {
            std::runtime_error("BLOCK NOT PARSED CORRECTLY") ; 
            break;
        }


        if (currentToken().GetType() == TokenType::DELIMITER)
        {
            if (currentIndex -current_idx ==1 ) 
            {
                advance() ; 
                current_idx =currentIndex   ;
                continue;
            }else
            {
                shared_ptr<Node> s_ptr = ParsParseAssign_Or_FunctionAsStatemente();
                
                return_block.addChild(s_ptr) ; 
            }

        }

        if (currentToken().GetType() == TokenType::FUNCTION)
        {
             
            shared_ptr<Node> f_ptr = ParseCreateFunction() ; 
            
            return_block.addChild(f_ptr);
            advance() ; 
            current_idx = currentIndex  ; 
            continue;  

        }
        if (currentToken().GetType() == TokenType::IF)
        {
            shared_ptr<Node> if_ptr = ParseIf() ; 
            advance() ; 
            current_idx - currentIndex ; 
            continue; 
        }

        if (currentToken().GetType() == TokenType::WHILE)
        {
            shared_ptr<Node> 
        }



        advance() ; 
    }

}


shared_ptr<Node> Parser::ParseIf()
{  


    advance() ; 
    Node if_node (NodeType::IF , "IF") ; 
    shared_ptr<Node> return_pointer ;   
    
    while (Match(TokenType::ELSE) || Match(TokenType::ELIF))
    {
        if (Match(TokenType::ELSE))
        {

            Node base_node (NodeType::CONDITION_DO , "CONDITION_DO") ; 
            Node l  (NodeType::BOOL , "TRUE") ;
            shared_ptr<Node> l_ptr = std::make_shared<Node> (l) ;
            advance() ; 
            shared_ptr<Node> r_ptr = ParseBlock() ; 
            base_node.addChild(l_ptr) ; 
            base_node.addChild(r_ptr) ; 
            break; 
        }
        
        if (Match(TokenType::ELIF)) 
        {
        }
    

        return return_pointer; 


    }   


}
shared_ptr <Node> Parser::ParseCondition_Do ()
{
    Node base_node (NodeType::CONDITION_DO , "CONDITION_DO") ; 
    shared_ptr<Node> return_pointer = std::make_shared<Node> (base_node) ; 
    shared_ptr<Node> l_ptr = ParseExpression() ; 
    shared_ptr<Node> r_ptr = ParseBlock() ; 
    base_node.addChild(l_ptr) ; 
    base_node.addChild(r_ptr) ; 
    return return_pointer ;  
 }







shared_ptr<Node> Parser::ParseCreateFunction()
{
    Node f(NodeType::FUNCTION , "FUNCTION") ; 
    advance() ; 
    shared_ptr<Node> l_ptr  = ParseArguments () ;    //  l ptr as arguments in function    
    advance() ; 
    shared_ptr<Node> r_ptr  = ParseBlock() ; // r_ptr as the block  ; 
    f.addChild(l_ptr) ; 
    f.addChild(r_ptr) ; 
    shared_ptr<Node> f_ptr = std::make_shared<Node> (f) ; 
    return f_ptr ; 
}  




shared_ptr<Node> Parser:: ParsParseAssign_Or_FunctionAsStatemente()
{
    shared_ptr<Node> l_ptr = ParseObject() ;   // 等等統一命名 
    advance() ; 
    while (Match(TokenType::ASSIGN))
    {
        Node operation(NodeType::OPERATOR , "ASSIGN")  ; 
        shared_ptr<Node> r_ptr =  ParseExpression() ; 
        operation.addChild(l_ptr) ; 
        operation.addChild(r_ptr) ; 
        shared_ptr<Node> base_ptr = std::make_shared<Node> (operation) ; 
        l_ptr = base_ptr ; 
    }
    shared_ptr<Node> return_pointer ; 
    return_pointer = l_ptr ; 
    
    return return_pointer ; 
}

shared_ptr<Node> Parser::ParseArguments()   /// ARGUMENTS 入口從 '(' TOKEN 開始
{
    advance();
    shared_ptr<Node> return_pointer = ParseExpression() ; 
    while (Match(TokenType::COMMA))
    {
        Node operation (NodeType::DELIMITER , "COMMA") ;   
        operation.addChild(return_pointer) ; 
        shared_ptr<Node> base_ptr = std::make_shared<Node> (operation) ; 
        return_pointer = base_ptr ; 
    }
    if (Match(TokenType::PAREN_CLOSE)) 
    {
       return return_pointer ; 
    }
    std::runtime_error( "ARGUMENT IS NOT PARSED") ; 
    return return_pointer ; 
}

shared_ptr<Node> Parser::ParseExpression()   // 
{
    shared_ptr<Node> l_ptr = ParseTerm() ; 
    advance() ; 
    shared_ptr<Node> return_pointer  = l_ptr ; 
                                                     // suspecious here  
    while (Match(TokenType::ADD) || Match(TokenType::SUB)) // Match 內建 is end 判斷 
    {
        if (currentToken().GetType() == TokenType::ADD)
        {
            Node operation(NodeType::OPERATOR , "ADD") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseTerm() ;
            operation.addChild(r_ptr) ; 
            return_pointer = std::make_shared<Node> (operation) ; 
        }
        else if (currentToken().GetType() == TokenType::SUB)
        {
            Node operation(NodeType::OPERATOR , "SUB") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseTerm() ;
            operation.addChild(r_ptr) ; 
            return_pointer = std::make_shared<Node> (operation) ; 
        }
    }
    
    return  return_pointer ; 
}

shared_ptr<Node> Parser::ParseTerm()
{
    shared_ptr<Node> l_ptr = ParseFactor() ; 
    advance() ; 
    shared_ptr<Node> return_pointer  = l_ptr ; 
                                                     // suspecious here  
    while (Match(TokenType::MUL) || Match(TokenType::MOD) || Match(TokenType::DIV)) // Match 內建 is end 判斷 
    {
        if (currentToken().GetType() == TokenType::MUL)
        {
            Node operation(NodeType::OPERATOR , "MUL") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseFactor() ;
            operation.addChild(r_ptr) ; 
            return_pointer = std::make_shared<Node> (operation) ; 
        }
        else if (currentToken().GetType() == TokenType::MOD)
        {
            Node operation(NodeType::OPERATOR , "MOD") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseFactor() ;
            operation.addChild(r_ptr) ; 
            return_pointer = std::make_shared<Node> (operation) ; 
        }
        else if (currentToken().GetType() == TokenType::DIV)
        {
            Node operation(NodeType::OPERATOR , "DIV") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseFactor() ;
            operation.addChild(r_ptr) ; 
            return_pointer = std::make_shared<Node> (operation) ; 
        }
    }
    
    return  return_pointer ; 
}

shared_ptr<Node> Parser::ParseFactor()
{
    if (currentToken().GetType() == TokenType::SUB || currentToken().GetType() == TokenType::NOT)
    {
        if (currentToken().GetType() == TokenType::SUB)
        {
            advance() ; 
            Node operation (NodeType::OPERATOR , "NEG") ; //// 這裡被視為 取負 請勿糾正成 "SUB" 
            shared_ptr<Node> l_ptr = ParseDots_Or_NonNumericValue() ; 
            operation.addChild(l_ptr) ;
            shared_ptr<Node> return_pointer  ; 
            return_pointer = std::make_shared<Node> (operation) ; 
            return return_pointer ; 
        }
        else if (currentToken().GetType() == TokenType::NOT)  /// 
        {
            advance() ; 
            Node operation (NodeType::OPERATOR , "NOT_BITWISE") ; //// 這裡被視為 按位取反 請勿糾正成 "NOT" 
            shared_ptr<Node> l_ptr = ParseDots_Or_NonNumericValue() ; 
            operation.addChild(l_ptr) ;
            shared_ptr<Node> return_pointer  ; 
            return_pointer = std::make_shared<Node> (operation) ; 
            return return_pointer ; 
        }
    }
    
    if (currentToken().GetType() == TokenType::PAREN_OPEN)
    {
        advance() ; ///                                         這裡必須往後推否則自己讀自己 導致無窮循環 (待驗證
        shared_ptr<Node> return_pointer =  ParseExpression() ; 
        
        advance() ; 
        if (currentToken().GetType() == TokenType::PAREN_CLOSE) 
        {
                return return_pointer ; 
        }
        else
        {
            std::runtime_error("PAREN IS NOT PARSED  ") ; 
        }
    }



    return ParseDots_Or_NonNumericValue() ; 
}



shared_ptr<Node> Parser::ParseDots_Or_NonNumericValue()
{

    if (currentToken().GetType() == TokenType::NUMBER) 
    {
        return ParseFloat_Double() ;
         
        
    }    
    if (currentToken().GetType() == TokenType::IDENTIFIER)
    {
        ParseObject() ; 
        return ParseObject();
    }
    if(currentToken().GetType() == TokenType::STRING)
    {
        Node const_node (NodeType::STRING , currentToken().GetValue()) ; 
        shared_ptr<Node> return_pointer = std::make_shared<Node> (const_node) ; 
        return return_pointer ; 
    }
    if(currentToken().GetType() == TokenType::CHAR)
    {
        Node const_node (NodeType::CHAR , currentToken().GetValue()) ; 
        shared_ptr<Node> return_pointer = std::make_shared<Node> (const_node) ; 
        return return_pointer ; 
    }
    if(currentToken().GetType() == TokenType::BOOL) /// lexer 尚未寫入boolean 判斷
    {
        Node const_node (NodeType::BOOL , currentToken().GetValue()) ; 
        shared_ptr<Node> return_pointer = std::make_shared<Node> (const_node) ; 
        return return_pointer ; 
    }



    std::runtime_error ("UNKNOWN TYPE") ; 

}


shared_ptr<Node> Parser::ParseFloat_Double()      ///// <float> :: <number> |"."| <number> <double> ::= <number> |".."| <number> 
{
    Node l (NodeType::NUMBER , currentToken().GetValue()) ;  
    shared_ptr<Node> l_ptr = std::make_shared<Node> (l) ; 
    if (nextToken().GetType() == TokenType::DOT || nextToken().GetType() == TokenType::DOTDOT)
    {
        if (nextToken().GetType() == TokenType::DOT)
        {
            advance() ; 
            if(nextToken().GetType() == TokenType::NUMBER)
            {

                Node r  (NodeType::NUMBER , nextToken().GetValue()) ; 
                shared_ptr<Node> r_ptr = std::make_shared<Node> (r) ; 
                Node operation (NodeType::OPERATOR , "DOT") ; 
                operation.addChild(l_ptr) ; 
                operation.addChild(r_ptr) ;
                
                shared_ptr<Node> return_pointer = std::make_shared<Node> (operation) ; 
                return return_pointer ; 
                
            }  
            else{
                std::runtime_error("INVALID TOKEN AFTER \'.\'") ; 
            }      
        }
        else if (nextToken().GetType() == TokenType::DOTDOT) 
        {
            advance() ; 
 
            if(nextToken().GetType() == TokenType::NUMBER)
            {
                Node r  (NodeType::NUMBER , nextToken().GetValue()) ; 
                shared_ptr<Node> r_ptr = std::make_shared<Node> (r) ; 
                Node operation (NodeType::OPERATOR , "DOTDOT") ; 
                operation.addChild(l_ptr) ; 
                operation.addChild(r_ptr) ;
                
                shared_ptr<Node> return_pointer = std::make_shared<Node> (operation) ; 
                return return_pointer ; 
                
            }  
            else{
                std::runtime_error("INVALID TOKEN AFTER \'..\'") ; 
            }


        }
    }
    shared_ptr<Node> return_pointer ; 
    return_pointer = l_ptr ; 
    return return_pointer;
    
}

shared_ptr<Node> Parser::ParseObject()
{
    shared_ptr<Node> l_ptr = ParseCallFunction_Or_Variable() ; 

    if (nextToken().GetType() == TokenType::DOT)
    {

        advance() ; 
        if(nextToken().GetType() == TokenType::IDENTIFIER)
        {
        

            shared_ptr<Node> r_ptr = ParseCallFunction_Or_Variable() ;  
            Node operation (NodeType::OPERATOR , "DOT") ; 
            operation.addChild(l_ptr) ; 
            operation.addChild(r_ptr) ; 
            
            shared_ptr<Node> return_pointer = std::make_shared<Node> (operation) ; 
            return return_pointer ; 
            
        }  
        else{
            std::runtime_error("INVALID TOKEN AFTER \'.\'") ; 
        }      
        
    }



    shared_ptr<Node> return_pointer ; 
    return_pointer = l_ptr ; 
    return return_pointer;
}





shared_ptr<Node> Parser::ParseCallFunction_Or_Variable()
{
    if (nextToken().GetType() == TokenType::PAREN_OPEN) 
    {
        
        shared_ptr<Node> return_pointer;
        return_pointer= ParseArguments() ; 
        advance() ; 
        if (currentToken().GetType() == TokenType::PAREN_CLOSE)
        {
            return return_pointer ; 
        }else
        {
            cout << "PAREN IS NOT PARSED  " << endl ; 
        }
    }

    Node var (NodeType::VARIABLE , currentToken().GetValue()) ;

    shared_ptr<Node> var_ptr = std::make_shared<Node> (var_ptr) ; 
    return var_ptr  ; 
}













    
    
bool Parsing_Finished = false ; 
shared_ptr<Node> Parser::Parsing() {

    shared_ptr<Node> rootPtr = std::make_shared<Node>(root);  // 通过构造函数创建一个新的shared_ptr

    //ParseRoot(rootPtr); // 开始解析
    
    return rootPtr; // 返回语法树根节点
}