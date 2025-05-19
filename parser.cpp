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
<expression> ::= <bool> | <integer> |<flaot> | <string> | <char> |<table> | 

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

using namespace std ;
shared_ptr<Node> Parser::ParseIf()
{  
    Node if_node  (NodeType::IF  , "IF") ; 
    if(currentToken().GetType() != TokenType::IF)
    {
        std::runtime_error("IF IS NOT PARSED CORRECTLY ") ; 
        cout << "IF IS NOT PARSED CORRECTLY " << endl ; 
    }
    advance() ;
    if_node.addChild(ParseCondition_Do())  ; 
    advance() ; 
    while (Match(TokenType::ELSE) || Match(TokenType::ELIF))
    {
        if (Match(TokenType::ELSE))
        {
            advance() ;  
            if_node.addChild(ParsElse()) ;
            advance() ;  
            break; 
        }
        if (Match(TokenType::ELIF)) 
        {
            advance() ; 
            if_node.addChild(ParseCondition_Do()) ; 
            advance() ; 
            continue;
        }
    }   
    previous() ; 
    shared_ptr<Node> return_pointer = std::make_shared<Node> (if_node) ;   
    return  return_pointer ; 
}

/*
FOR----
    |---- INITIALIZING 
    |
    |----- CONDITION DO 
                |
                |--------- CONDITION 
                |----------DO
                            |---- BLOCK ...
                            |-- FOR OPERATION like (i++)  




*/

shared_ptr<Node> Parser::ParsePostProcessing()
{
    Node post_processing (NodeType::POST_PROCESSING , "POST_PROCESSING")  ; 
    if (currentToken().GetType() == TokenType::PAREN_CLOSE) // -> ) 停止
    {
        previous() ; // 這邊會對齊(後來會advance 查找 semicolon)
        return std::make_shared<Node> (post_processing) ; 
    }
    shared_ptr<Node> first_post_processing = ParseAssign_Or_FunctionAsStatemente() ; 
    post_processing.addChild(first_post_processing) ; 
    advance() ; 
    while (currentToken().GetType() == TokenType::COMMA) 
    {
        shared_ptr<Node> next_init = ParseAssign_Or_FunctionAsStatemente() ;          
        post_processing.addChild(next_init) ; 
        advance() ; 
    }
    previous(); // 探頭縮回
    return std::make_shared<Node> (post_processing) ; 
}







shared_ptr<Node> Parser::ParseInit()
{
    Node init (NodeType::INITIALIZE  , "INITIALIZE") ; 
    if (currentToken().GetType() == TokenType::SEMICOLON)
    {
        previous() ; // 這邊會對齊(後來會advance 查找 semicolon)
        return std::make_shared<Node> (init) ; 
    }    
    
    shared_ptr<Node> first_init = ParseAssign_Or_FunctionAsStatemente() ; 
    init.addChild(first_init) ; 
   
    advance() ; 
   // cout <<"init reached 1 " << endl ; 
    while (currentToken().GetType() == TokenType::COMMA ) 
    {
        advance() ; 
        shared_ptr<Node> next_init = ParseAssign_Or_FunctionAsStatemente() ;          
        init.addChild(next_init) ; 
        advance() ; 
    }
    
    previous(); // 探頭縮回
    return std::make_shared<Node> (init) ; 
}
shared_ptr<Node> Parser::ParseForCondition()
{
    Node condtion_do (NodeType::CONDITION_DO  , "CONDITION_DO") ; 
    if (currentToken().GetType() == TokenType::SEMICOLON)
    {
        previous()  ; 
        Node true_node (NodeType::BOOL , "true") ; 
        condtion_do.addChild(std::make_shared<Node>(true_node)) ; 
        return std::make_shared<Node> (condtion_do) ; 
    }

    shared_ptr<Node> condition  = ParseAssign_Or_FunctionAsStatemente() ;     
    condtion_do.addChild(condition) ; 
    return std::make_shared<Node> (condtion_do) ; 
}

shared_ptr<Node> Parser::ParseFor()
{
    // 確保遇到 'for' 開頭
    if (currentToken().GetType() != TokenType::FOR)
    {
        std::runtime_error("EXPECTED 'for' TO BEGIN FOR LOOP");
        cout << "EXPECTED 'for' TO BEGIN FOR LOOP" << endl;
    }
    advance(); // 跳過 'for'
    if (currentToken().GetType() != TokenType::PAREN_OPEN)
    {
        std::runtime_error("EXPECTED '(' AFTER 'for'");
        cout << "EXPECTED '(' AFTER 'for'" << endl;
    }
    advance(); // 跳過 '('
    // 解析初始化部分
    shared_ptr<Node> init_node = ParseInit();
    advance() ;   // 初始化通常是語句，如變數初始化 
    if (currentToken().GetType() != TokenType::SEMICOLON)
    {
        std::runtime_error("EXPECTED ';' AFTER INIT STATEMENT");
        cout << "EXPECTED ';' AFTER INIT STATEMENT" << endl;
        cout<< "INIT:" << currentToken().GetValue() <<endl ; 
    }
    advance(); // 跳過 ';'，結束初始化部分

    // 解析條件部分
     
    
    shared_ptr<Node> condition_do_node =ParseForCondition() ; 
    advance();
    if (currentToken().GetType() != TokenType::SEMICOLON)
    {
        std::runtime_error("EXPECTED ';' AFTER CONDITION");
        cout << "EXPECTED ';' AFTER CONDITION" << endl;
    }
    advance(); // 跳過 ';'，結束條件部分

    // 解析後處理部分（如遞增/遞減操作）
    shared_ptr<Node> post_node = ParsePostProcessing() ;  // 後處理通常是語句，如變數遞增或遞減
    advance() ; 
    if (currentToken().GetType() != TokenType::PAREN_CLOSE)
    {
        std::runtime_error("EXPECTED ')' AFTER POST STATEMENT");
        cout << "EXPECTED ')' AFTER POST STATEMENT" << endl;
        cout << currentToken().GetValue() << endl ; 
    }
    advance() ; 
    // 解析迴圈體（block）
    shared_ptr<Node> body_node = ParseBlock();  // 解析迴圈的主體部分（通常是語句區塊）

    // 創建 'for' 迴圈節點並回傳
    shared_ptr<Node> for_node = std::make_shared<Node>(NodeType::FOR, "FOR");
    for_node->addChild(init_node);    // 初始化
    for_node->addChild(condition_do_node); // 條件
    for_node->addChild(post_node);    // 後處理  
    condition_do_node->addChild(body_node) ;  // 迴圈主體
    return for_node;
}





shared_ptr<Node> Parser::ParseWhile() // -->對齊 WHILE
{
    if (currentToken().GetType() != TokenType::WHILE)
    {
        std::runtime_error("WHILE IS NOT PARSED CORRECTLY ") ; 
        cout << "WHILE IS NOT PARSED CORRECTLY " << endl ; 
    }
    Node while_block (NodeType::WHILE , "WHILE") ; 
    advance() ; 
    shared_ptr<Node> l_ptr = ParseCondition_Do() ; 
    while_block.addChild(l_ptr) ; 
    shared_ptr<Node> return_pointer = std::make_shared<Node> (while_block) ;
    return return_pointer ; 
}


shared_ptr<Node> Parser::ParseBlock()
{
    // 確認是否遇到 '{' 開始區塊
    if (currentToken().GetType() != TokenType::BRACE_OPEN)
    {
        std::runtime_error("EXPECTED '{' TO BEGIN BLOCK");
        cout << "EXPECTED '{' TO BEGIN BLOCK" << endl;
    }

    advance(); // 跳過 '{'
    
    shared_ptr<Node> block_node = std::make_shared<Node>(NodeType::BLOCK, "BLOCK");

    while (currentToken().GetType() != TokenType::BRACE_CLOSE)
    {
        // 如果是空語句的分號，直接跳過
        if (Match(TokenType::SEMICOLON))
        {
            advance();  // 跳過 ';'
            continue;
        }
        if (Match(TokenType::END))
        {
            cout<<"EXPECT '}' TO END BLOCK " << endl ;    // 跳過 ';'
            break;
        }
        // 處理每一個語句或結構
        if (currentToken().GetType() == TokenType::IF)
        {
            shared_ptr<Node> if_node = ParseIf();  // 假設有 ParseIf 方法處理 if
            block_node->addChild(if_node);
            advance() ; 
            continue;
        }
        else if (currentToken().GetType() == TokenType::WHILE)
        {
            shared_ptr<Node> while_node = ParseWhile();  // 假設有 ParseWhile 方法處理 while
            block_node->addChild(while_node);
            advance() ; 
            continue;
        }
        else if (currentToken().GetType() == TokenType::FOR)
        {
            shared_ptr<Node> for_node = ParseFor();  // 假設有 ParseFor 方法處理 for
            block_node->addChild(for_node);
            advance() ; 
            continue;
        }
        else if (currentToken().GetType() == TokenType::FUNCTION)
        {
            shared_ptr<Node> func_node = ParseCreateFunction();  // 假設有 ParseFunctionCall 處理函數
            block_node->addChild(func_node);
            advance() ; 
            continue;
        }
        else
        {
            shared_ptr<Node> stmt_node = ParseStatement();  // 假設有 ParseStatement 處理語句
            block_node->addChild(stmt_node);
            advance() ;  
        }

        // 檢查是否有分號結尾
        if (Match(TokenType::SEMICOLON))
        {
            advance();  // 跳過 ';'
        }else 
        {
            cout << "EXPECT ';' AFTER STATEMENT IN BLOCK  " << endl ;      
            ERROR(); 
        }

    }

    // 確保遇到 '}' 結束區塊
    if (currentToken().GetType() != TokenType::BRACE_CLOSE)
    {
        std::runtime_error("EXPECTED '}' TO END BLOCK");
        cout << "EXPECTED '}' TO END BLOCK" << endl;
        cout << currentToken().GetValue() << endl ; 
    }
    return block_node;
}



shared_ptr<Node> Parser::ParseArguments()   /// ARGUMENTS 入口從 '('的後一個 TOKEN 開始
{
    Node arguments( NodeType::ARGUMENT , "ARGUMENT") ; 
    
    if (nextToken().GetType() == TokenType:: PAREN_CLOSE) 
    {
        advance() ; 
        return std::make_shared<Node> (arguments);
    }
    advance() ; 
    shared_ptr<Node> first_argument = ParseAssign_Or_FunctionAsStatemente(); 
    arguments.addChild(first_argument) ;  
    advance() ; 
    while (Match(TokenType::COMMA))
    {
        advance() ; 
        shared_ptr<Node> next_argument = ParseAssign_Or_FunctionAsStatemente() ; 
        arguments.addChild(next_argument) ; 
        advance() ; 
    }
    if (!Match(TokenType::PAREN_CLOSE))
    {
        cout << "ARGUMENT IS NOT PARSED " << endl ; 
        cout << currentToken().GetValue() << endl ; 
    }
    return std::make_shared<Node> (arguments); 
}



shared_ptr <Node> Parser::ParsElse ()
{   
    Node base_node (NodeType::CONDITION_DO , "CONDITION_DO") ;     
    Node true_node (NodeType::BOOL , "true") ; 
    shared_ptr<Node> l_ptr = std::make_shared<Node> (true_node) ;  
    shared_ptr<Node> r_ptr = ParseBlock() ; 
    base_node.addChild(l_ptr) ; 
    base_node.addChild(r_ptr) ; 
    shared_ptr<Node> return_pointer = std::make_shared<Node> (base_node) ; 
    return return_pointer ;  
 }
shared_ptr <Node> Parser::ParseCondition_Do ()
{   
    Node base_node (NodeType::CONDITION_DO , "CONDITION_DO") ;     
    if (currentToken().GetType() != TokenType::PAREN_OPEN)
    {
        cout << "EXPECTED '(' CONDITION_DO"  << endl ; 
    }
    advance();
    if (currentToken().GetType() == TokenType::PAREN_CLOSE)
    {
        cout << "EXPECTED AN EXPRESSION IN CONDITION DO "  << endl ; 
    }
    shared_ptr<Node> l_ptr = ParseAssign_Or_FunctionAsStatemente() ; 
    advance() ; 
    if (currentToken().GetType() != TokenType::PAREN_CLOSE)
    {
        cout << "EXPECTED ')' CONDITION_DO"  << endl ; 
        cout << currentToken().GetValue() << endl ; 
    }
    advance(); 
    shared_ptr<Node> r_ptr = ParseBlock() ; 
    base_node.addChild(l_ptr) ; 
    base_node.addChild(r_ptr) ; 
    shared_ptr<Node> return_pointer = std::make_shared<Node> (base_node) ; 
    return return_pointer ;  
 }


 shared_ptr<Node> Parser::ParseArguments_Create()   /// ARGUMENTS 入口從 '('的後一個 TOKEN 開始
 {
     Node arguments( NodeType::ARGUMENT , "ARGUMENT") ; 
     
     if (nextToken().GetType() == TokenType:: PAREN_CLOSE) 
     {
         advance() ; 
         return std::make_shared<Node> (arguments);
     }
     advance() ; 
     shared_ptr<Node> first_argument = ParseAssign_Only(); // 區別在這 
     arguments.addChild(first_argument) ;  
     advance() ; 
     while (Match(TokenType::COMMA))
     {
         advance() ; 
         shared_ptr<Node> next_argument = ParseAssign_Only() ; 
         arguments.addChild(next_argument) ; 
         advance() ; 
     }
     if (!Match(TokenType::PAREN_CLOSE))
     {
         cout << "ARGUMENT IS NOT PARSED " << endl ; 
     }
     return std::make_shared<Node> (arguments); 
 }
 



shared_ptr<Node> Parser::ParseCreateFunction()
{
    if (currentToken().GetType() != TokenType::FUNCTION) 
    {
        cout << "ERROR WHEN PARSING CREATE FUNCTION " << endl ; 
    }

    Node f(NodeType::FUNCTION , "FUNCTION") ; 
    advance() ; 

    if (currentToken().GetType() != TokenType::IDENTIFIER) 
    {
        cout << "EXPECTED IDENTIFIER WHEN PARSING FUNCTION " << endl ; 
    }

   f.value =currentToken().GetValue() ; 


    advance() ; 
    shared_ptr<Node> l_ptr  = ParseArguments () ;    //  l ptr as arguments in function    
    advance() ; 
    shared_ptr<Node> r_ptr  = ParseBlock() ; // r_ptr as the block  ; 
    f.addChild(l_ptr) ; 
    f.addChild(r_ptr) ; 
    shared_ptr<Node> f_ptr = std::make_shared<Node> (f) ; 
    return f_ptr ; 
}


shared_ptr<Node> Parser::ParseCreateVariable()
{
    Node declare_node (NodeType::DECLARATION , "DECLARATION") ; 
    if (currentToken().GetType() == TokenType::VAR  && nextToken().GetType()==TokenType::IDENTIFIER) //  sus 
    {
        advance();
        Token tks = currentToken()  ; 
        //declare_node.value = tks.GetValue() ;
        Node  variable (NodeType::VARIABLE  , tks.GetValue() ) ; 
        declare_node.addChild(std::make_shared<Node> (variable)) ;   
        shared_ptr<Node> return_pointer = std::make_shared<Node> (declare_node) ; 
        return  return_pointer ;        
    }
    else
    {
        if (currentToken().GetType() != TokenType::VAR)
        {
            cout << "ERROR WHEN NOT FOUND VAR AND TRY TO PARSE IT " << endl ;  
        }
        else
        {
            cout << "ERROR WHEN PARSING CREATE VARIABLE"  << endl ; 
        }
        shared_ptr<Node> return_pointer = std::make_shared<Node> (declare_node) ; 
        return return_pointer ; 

    }
}

shared_ptr<Node> Parser::ParseAssign_Only() // table 用來禁止多重 assign 
{


    //Node l (NodeType::VARIABLE , currentToken().GetValue()) ; // here fucking sus 
    shared_ptr<Node> l_ptr = ParseLogic_OR() ; 
    advance() ; 
    if (Match(TokenType::ASSIGN))
    {
       // cout << "reached5" << endl ; 
        Node operation(NodeType::ASSIGN_KEY , "ASSIGN_KEY")  ; 
        advance() ; 
        shared_ptr<Node> r_ptr =  ParseLogic_OR() ; 
        advance() ; 
        operation.addChild(r_ptr) ; 
        operation.addChild(l_ptr) ; 
        shared_ptr<Node> base_ptr = std::make_shared<Node> (operation) ; 
        l_ptr = base_ptr ;    
    }

    previous() ; 
    return l_ptr ; 
}




shared_ptr<Node> Parser::ParseStatement()
{
    if (currentToken().GetType() == TokenType::CONTINUE)
    {
        Node continue_node(NodeType::CONTINUE , "CONTINUE") ;
        return std::make_shared<Node>(continue_node) ; 
    }   
    if (currentToken().GetType() == TokenType::BREAK)
    {
        Node break_node(NodeType::BREAK , "BREAK") ;
        return std::make_shared<Node>(break_node) ; 
    }
    if (currentToken().GetType() == TokenType::RETURN)
    {
        Node return_node (NodeType::RETURN , "RETURN") ;
        if(nextToken().GetType() == TokenType::SEMICOLON)
        {
            return std::make_shared<Node> (return_node) ; 
        } 
        advance();
        shared_ptr<Node> l_ptr =  ParseAssign_Or_FunctionAsStatemente() ; 
        return_node.addChild(l_ptr) ; 
        return std::make_shared<Node> (return_node) ;
    }
    if (currentToken().GetType() == TokenType::VAR )
    {
        Node declartion_node (NodeType::DECLARATION , "DECLARATION") ; 
        if (nextToken().GetType() == TokenType::IDENTIFIER)
        {
            advance();
            Node l (NodeType::VARIABLE  , currentToken().GetValue()) ; 
            shared_ptr<Node> l_ptr = std::make_shared<Node> (l) ; 
            declartion_node.addChild(l_ptr) ; 
            if (nextToken().GetType() == TokenType::ASSIGN)
            {
             advance() ;  advance() ; 
             shared_ptr<Node> r_ptr = ParseAssign_Or_FunctionAsStatemente() ;   
             declartion_node.addChild(r_ptr); 
            }
            return std::make_shared<Node> (declartion_node) ; 
        }else
        {
            cout << "EXPECTED IDENTIFIER AFTER VAR " << endl ; 
            ERROR();
        }
    
    }
    return ParseAssign_Or_FunctionAsStatemente() ; 
}






shared_ptr<Node> Parser::ParseAssign_Or_FunctionAsStatemente()
{
    shared_ptr<Node> l_ptr;

    // 處理 var 開頭的變數宣告
    l_ptr = ParseLogic_OR();
    
    advance(); // 看看接下來是什麼

    // 檢查是否是賦值（右結合）
    if (Match(TokenType::ASSIGN)) {
        advance(); // 吃掉 '='
        shared_ptr<Node> r_ptr = ParseAssign_Or_FunctionAsStatemente(); // 遞迴，確保右結合

        Node operation(NodeType::BINARY, "ASSIGN");
        operation.addChild(l_ptr);   // 左邊是變數
        operation.addChild(r_ptr);   // 右邊是表達式

        return std::make_shared<Node>(operation);
    }

    previous(); // 沒有 ASSIGN 的話回復狀態
    return l_ptr;
}
//----------------------- Logic



shared_ptr<Node> Parser::ParseLogic_OR()
{
    shared_ptr<Node> l_ptr = ParseLogic_AND() ; 
    advance() ;   
    while (Match(TokenType::OR)) 
    {
         
        if (currentToken().GetType() == TokenType::OR)
        {
            Node operation(NodeType::BINARY , "OR") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseLogic_AND() ;
            advance() ;
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
    }
    previous() ; 
    shared_ptr<Node> return_pointer = l_ptr ; 
    return  return_pointer ; 
}


shared_ptr<Node> Parser::ParseLogic_AND()
{
    shared_ptr<Node> l_ptr = ParseCompare() ; 
    advance() ;   
    while (Match(TokenType::AND)) 
    {
         
        if (currentToken().GetType() == TokenType::AND)
        {
            Node operation(NodeType::BINARY , "AND") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseCompare() ;
            advance() ;
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
    }
    previous() ; 
    shared_ptr<Node> return_pointer = l_ptr ; 
    return  return_pointer ; 
}




shared_ptr<Node> Parser::ParseCompare()
{

    shared_ptr<Node> l_ptr = ParseIs_Equal() ; 
    advance() ;   
    while (Match(TokenType::GREATER_THAN) ||Match(TokenType::NOT_GREATER_THAN) || Match(TokenType::LESS_THAN) ||Match(TokenType::NOT_LESS_THAN) ) 
    {
         
        if (currentToken().GetType() == TokenType::GREATER_THAN)
        {
            Node operation(NodeType::BINARY , "GREATER_THAN") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseIs_Equal() ;
            advance() ;
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
        if (currentToken().GetType() == TokenType::NOT_GREATER_THAN)
        {
            Node operation(NodeType::BINARY , "NOT_GREATER_THAN") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseIs_Equal() ;
            advance() ;
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
        if (currentToken().GetType() == TokenType::LESS_THAN)
        {
            Node operation(NodeType::BINARY , "LESS_THAN") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseIs_Equal() ;
            advance() ;
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
        if (currentToken().GetType() == TokenType::NOT_LESS_THAN)
        {
            Node operation(NodeType::BINARY , "NOT_LESS_THAN") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseIs_Equal() ;
            advance() ;
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
    }
    previous() ; 
    shared_ptr<Node> return_pointer = l_ptr ; 
    return  return_pointer ; 



}

shared_ptr<Node> Parser::ParseIs_Equal()
{
    shared_ptr<Node> l_ptr = ParseBitwise_OR() ; 
    advance() ;   
    while (Match(TokenType::EQUAL) ||Match(TokenType::NOT_EQUAL) ) 
    {
         
        if (currentToken().GetType() == TokenType::EQUAL )
        {
            Node operation(NodeType::BINARY , "EQUAL") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseBitwise_OR() ;
            advance() ;
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
        if (currentToken().GetType() == TokenType::NOT_EQUAL)
        {
            Node operation(NodeType::BINARY , "NOT_EQUAL") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseBitwise_OR() ;
            advance() ;
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
    }
    previous() ; 
    shared_ptr<Node> return_pointer = l_ptr ; 
    return  return_pointer ; 
}






//-----------------------Bitwise


shared_ptr<Node> Parser::ParseBitwise_OR()
{
    shared_ptr<Node> l_ptr = ParseBitwise_XOR() ; 
    advance() ;   
    while (Match(TokenType::BITWISE_OR)) 
    {
         
        if (currentToken().GetType() == TokenType::BITWISE_OR)
        {
            Node operation(NodeType::BINARY , "BITWISE_OR") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseBitwise_XOR() ;
            advance() ;
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
    }
    previous() ; 
    shared_ptr<Node> return_pointer = l_ptr ; 
    return  return_pointer ; 
}

shared_ptr<Node> Parser::ParseBitwise_XOR()
{
    shared_ptr<Node> l_ptr = ParseBitwise_AND() ; 
    advance() ;   
    while (Match(TokenType::BITWISE_XOR)) 
    {
         
        if (currentToken().GetType() == TokenType::BITWISE_XOR)
        {
            Node operation(NodeType::BINARY , "BITWISE_XOR") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseBitwise_AND() ;
            advance() ;
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
    }
    previous() ; 
    shared_ptr<Node> return_pointer = l_ptr ; 
    return  return_pointer ; 
}

shared_ptr<Node> Parser::ParseBitwise_AND()
{
    shared_ptr<Node> l_ptr = ParseShift();  
    advance();

    while (Match(TokenType::BITWISE_AND))
    {
        if (currentToken().GetType() == TokenType::BITWISE_AND)
        {
            Node operation(NodeType::BINARY, "BITWISE_AND");
            operation.addChild(l_ptr);
            advance();
            shared_ptr<Node> r_ptr = ParseShift();  
            advance();
            operation.addChild(r_ptr);
            l_ptr = std::make_shared<Node>(operation);  // 更新左操作數
        }
    }

    previous();  // 回退一次，準備返回父節點
    shared_ptr<Node> return_pointer = l_ptr;
    return return_pointer;
}

shared_ptr<Node> Parser::ParseShift()
{
    // 位移操作符解析
    shared_ptr<Node> l_ptr = ParseExpression();  // 基本運算項

    advance();

    while (Match(TokenType::SHIFT_LEFT) || Match(TokenType::SHIFT_RIGHT))
    {
        if (currentToken().GetType() == TokenType::SHIFT_LEFT)
        {
            Node operation(NodeType::BINARY, "SHIFT_LEFT");
            operation.addChild(l_ptr);
            advance();
            shared_ptr<Node> r_ptr = ParseExpression();  
            advance();
            operation.addChild(r_ptr);
            l_ptr = std::make_shared<Node>(operation);
        }
        else if (currentToken().GetType() == TokenType::SHIFT_RIGHT)
        {
            Node operation(NodeType::BINARY, "SHIFT_RIGHT");
            operation.addChild(l_ptr);
            advance();
            shared_ptr<Node> r_ptr = ParseExpression();  
            advance();
            operation.addChild(r_ptr);
            l_ptr = std::make_shared<Node>(operation);
        }
    }

    previous();  // 回退一次，準備返回父節點
    return l_ptr;
}


shared_ptr<Node> Parser::ParseExpression()  
{
    shared_ptr<Node> l_ptr = ParseTerm() ; 
    advance() ;   /// why it pused to 2 when 1+2 
    
    while (Match(TokenType::ADD) || Match(TokenType::SUB)) // Match 內建 is end 判斷 
    {
         
        if (currentToken().GetType() == TokenType::ADD)
        {
            Node operation(NodeType::BINARY , "ADD") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseTerm() ;
            advance() ;
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
        else if (currentToken().GetType() == TokenType::SUB)
        {
            Node operation(NodeType::BINARY , "SUB") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseTerm() ;
            advance() ; 
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
    }
    
    previous() ; 
    
    shared_ptr<Node> return_pointer = l_ptr ; 
    return  return_pointer ; 
}

shared_ptr<Node> Parser::ParseTerm()
{
 
    shared_ptr<Node> l_ptr = ParseFactor() ; 
    advance() ;  // 這邊操作 增加耦合性 必須修改    
    while (Match(TokenType::MUL) || Match(TokenType::MOD) || Match(TokenType::DIV)) // Match 內建 is end 判斷 // 我認為match 帶來一定誤導 
    {
        if (currentToken().GetType() == TokenType::MUL)
        {   
            Node operation(NodeType::BINARY , "MUL") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseFactor() ;   // call 完 function 記得advance 除非 function 尾部已經advance
            advance() ; 
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
        else if (currentToken().GetType() == TokenType::MOD)
        {
            Node operation(NodeType::BINARY , "MOD") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseFactor() ;
            advance() ; 
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
        else if (currentToken().GetType() == TokenType::DIV)
        {
            Node operation(NodeType::BINARY , "DIV") ;  
            operation.addChild(l_ptr) ; 
            advance() ; 
            shared_ptr<Node> r_ptr = ParseFactor() ;
            advance() ; 
            operation.addChild(r_ptr) ; 
            l_ptr = std::make_shared<Node> (operation) ; 
        }
    }  
    previous() ; 
    shared_ptr<Node> return_pointer = l_ptr ; 
    return  return_pointer ; 
}

shared_ptr<Node> Parser::ParseFactor()
{
    if (currentToken().GetType() == TokenType::SUB || currentToken().GetType() == TokenType::NOT || currentToken().GetType() == TokenType::BITWISE_NOT)
    {
        if (currentToken().GetType() == TokenType::SUB)
        {
            advance() ;  
            Node operation (NodeType::UNARY , "NEG") ; //// 這裡被視為 取負 請勿糾正成 "SUB" 
            shared_ptr<Node> l_ptr = ParseLogic_OR() ; 
            operation.addChild(l_ptr) ;
            shared_ptr<Node> return_pointer  ; 
            return_pointer = std::make_shared<Node> (operation) ; 
            return return_pointer ; //  
        }
        else if (currentToken().GetType() == TokenType::BITWISE_NOT)  /// 
        {
            advance() ; 
            Node operation (NodeType::UNARY , "BITWISE_NOT") ; //// 這裡被視為 按位取反 請勿糾正成 "NOT" 
            shared_ptr<Node> l_ptr = ParseLogic_OR() ; 
            operation.addChild(l_ptr) ;
            shared_ptr<Node> return_pointer  ; 
            return_pointer = std::make_shared<Node> (operation) ; 
            return return_pointer ; 
        }
        else if (currentToken().GetType() == TokenType::NOT)  /// 
        {
            advance() ; 
            Node operation (NodeType::UNARY , "NOT") ; //// 這裡被視為 按位取反 請勿糾正成 "NOT" 
            shared_ptr<Node> l_ptr = ParseLogic_OR() ; 
            operation.addChild(l_ptr) ;
            shared_ptr<Node> return_pointer  ; 
            return_pointer = std::make_shared<Node> (operation) ; 
            return return_pointer ; 
        }
    }    
    if (currentToken().GetType() == TokenType::PAREN_OPEN)
    {
        advance() ; ///                                         這裡必須往後推否則自己讀自己 導致無窮循環 (待驗證    
        if (currentToken().GetType() == TokenType::PAREN_CLOSE)
        {
            cout <<"EXPECTED AN EXPRESSION " <<endl ;             
        }
        shared_ptr<Node> return_pointer =  ParseLogic_OR() ; //  
        advance() ; /// ahh ?? 
        if (currentToken().GetType() == TokenType::PAREN_CLOSE) 
        {
                return return_pointer ; 
        }
        else
        {
            cout <<"PAREN IS NOT PARSED  " <<endl ; 
            cout << currentToken().GetValue() << endl ; 
            return return_pointer ; 
        }
    }



    return ParseDots_Or_NonNumericValue() ; 
}
shared_ptr<Node> Parser::ParseTable()
{

    Node table( NodeType::TABLE , "TABLE") ; 
    
    if (nextToken().GetType() == TokenType::BRACKET_CLOSE) 
    {
        advance() ; 
        return std::make_shared<Node> (table);
    }
    advance() ; 
    shared_ptr<Node> first_data = ParseAssign_Only(); 
    table.addChild(first_data) ;  
    advance() ; 
    while (Match(TokenType::COMMA))
    {
        advance() ; 
        shared_ptr<Node> next_argument = ParseAssign_Only() ; 
        table.addChild(next_argument) ; 

        advance() ; 
    }
    
    if (!Match(TokenType::BRACKET_CLOSE))
    {
        cout << "TABLE IS NOT PARSED " << endl ; 
        cout << currentToken().GetValue() << endl ; 
    }
    return std::make_shared<Node> (table); 



}
shared_ptr<Node> Parser::ParseDots_Or_NonNumericValue()
{

    if (currentToken().GetType() == TokenType::BRACKET_OPEN)
    {
        shared_ptr<Node> return_pointer = ParseTable() ; 
        return return_pointer ; 

    }
    if (currentToken().GetType() == TokenType::NUMBER) 
    {
        return ParseFloat_Double() ;
    }    
    if (currentToken().GetType() == TokenType::IDENTIFIER)
    {
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
    if(currentToken().GetType() == TokenType::BREAK) /// lexer 尚未寫入boolean 判斷
    {
        Node const_node (NodeType::BREAK ,"BREAK") ; 
        shared_ptr<Node> return_pointer = std::make_shared<Node> (const_node) ; 
        return return_pointer ; 
    }
    if(currentToken().GetType() == TokenType::CONTINUE) /// lexer 尚未寫入boolean 判斷
    {
        Node const_node (NodeType::CONTINUE , "CONTINUE") ; 
        shared_ptr<Node> return_pointer = std::make_shared<Node> (const_node) ; 
        return return_pointer ; 
    }



    Node idk (NodeType::UNKNOWN , "UNKNOWN in ParseDots_Or_NonNumericValue ") ;
    cout <<"UNKNOWN in ParseDots_Or_NonNumericValue :"<<currentToken().GetValue() << endl ; 
    
    std::runtime_error ("UNKNOWN TYPE") ; 
    shared_ptr<Node> idk_ptr = std::make_shared<Node> (idk )  ;  
    return  idk_ptr ; 
}
shared_ptr<Node> Parser::ParseFloat_Double()      ///// <float> :: <number> |"."| <number> <double> ::= <number> |".."| <number> 
{
    Node l (NodeType::INT , currentToken().GetValue()) ;  
    shared_ptr<Node> l_ptr = std::make_shared<Node> (l) ; 
    if (nextToken().GetType() == TokenType::DOT || nextToken().GetType() == TokenType::DOTDOT )
    {
        if (nextToken().GetType() == TokenType::DOT)
        {
            advance() ; 
            if(nextToken().GetType() == TokenType::NUMBER)
            {
                advance() ; 
                Node r  (NodeType::NUMBER , currentToken().GetValue()) ; 
                shared_ptr<Node> r_ptr = std::make_shared<Node> (r) ; 
                Node operation (NodeType::FLOAT , l.value+'.'+r.value) ; 
                
                //  operation.addChild(l_ptr) ; 
              //  operation.addChild(r_ptr) ;
                
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

                advance();
                Node r  (NodeType::NUMBER , currentToken().GetValue()) ; 
                shared_ptr<Node> r_ptr = std::make_shared<Node> (r) ; 
                Node operation (NodeType::DOUBLE , l.value+'.'+r.value) ; 
                //operation.addChild(l_ptr) ; 
               // operation.addChild(r_ptr) ;
                
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

shared_ptr<Node> Parser::ParseObject() // suspecious here 
{
    shared_ptr<Node> l_ptr = ParseCallFunction_Or_Variable_Or_Indexing() ; 
  
  
    while (nextToken().GetType() == TokenType::DOT)
    {
        advance() ; 
        if(nextToken().GetType() == TokenType::IDENTIFIER)
        {
            advance() ; 
            shared_ptr<Node> r_ptr = ParseCallFunction_Or_Variable_Or_Indexing() ;   // not here 
            Node operation (NodeType::CHILDREN , "CHILDREN") ; 
            operation.addChild(l_ptr) ; 
            operation.addChild(r_ptr) ; 
            shared_ptr<Node> return_pointer = std::make_shared<Node> (operation) ; 
            l_ptr = return_pointer;

        }
        else
        {
            cout << "INVALID TOKEN " << currentToken().GetValue() << " AFTER '.' "<<endl ; 
        }
    }
    shared_ptr<Node> return_pointer ; 
    return_pointer = l_ptr ; 
    return return_pointer;
} 
shared_ptr<Node> Parser::ParseCallFunction_Or_Variable_Or_Indexing() // here !! 
{
    if (currentToken().GetType() != TokenType::IDENTIFIER )
    {
        cout << "EXPECTED IDENTIFIER IN FUNCTION OR SOMETHING ELSE ;/ " ; 
        cout << currentToken().GetValue() << endl ; 
    }   
    Node var (NodeType::VARIABLE , currentToken().GetValue()) ;
    shared_ptr<Node> var_ptr = std::make_shared<Node> (var) ; 
    //cout << nextToken().GetValue() << endl ; 
    while (nextToken().GetType() == TokenType::PAREN_OPEN || nextToken().GetType() ==TokenType::BRACKET_OPEN )
    {
    
        if (nextToken().GetType() == TokenType::PAREN_OPEN) 
        {           
            Node func (NodeType::CALL_FUNCTION , "CALL_FUNCTION") ; 
            advance();
            func.addChild(var_ptr) ; 
            func.addChild(ParseArguments()) ;
            shared_ptr<Node> f_ptr = std::make_shared<Node> (func ); 
            var_ptr = f_ptr ; 
            continue;
        }
        if (nextToken().GetType() == TokenType::BRACKET_OPEN)
        {
            advance() ; 
            Node indexing (NodeType::INDEX , "INDEX") ; 
            indexing.addChild(var_ptr) ; 
            indexing.addChild(ParseIndexing()) ;
           // cout << indexing.children[1]->value ;
            shared_ptr<Node> i_ptr  = std::make_shared<Node> (indexing ); 
            
            var_ptr= i_ptr ;     
            continue;
        }
    }   
    return var_ptr  ; 

}







shared_ptr<Node> Parser::ParseIndexing()
{


    if (currentToken().GetType() != TokenType::BRACKET_OPEN)
    {
        cout << "EXPECTED '[' PARSING INDEXING " << endl ; 
    }
    if (nextToken().GetType() == TokenType::BRACKET_CLOSE)
    {
        cout << "KEY SHOULD NOT EMPTY WHEN INDEXING " << endl ;
    }
    advance() ;
    shared_ptr<Node> index_key =  ParseAssign_Or_FunctionAsStatemente() ; 
    advance() ; 
    if (currentToken().GetType() != TokenType::BRACKET_CLOSE )
    {
        cout << "EXPECTED ']' PARSING INDEXING " << endl ; 
    }

    return index_key; 
}
















bool Parsing_Finished = false ; 
shared_ptr<Node> Parser::Parsing() {

    

    //ParseRoot(rootPtr); // 开始解析
    
   // return rootPtr; // 返回语法树根节点
}