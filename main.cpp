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
#include<vector>
#include <iostream>
#include<string>
#include "lexer.h"
#include "parser.h"
using namespace std ;




//#include "parser.h"
int main() {
    int a = 0 ;
    int c = 1 ; 
    int b = 2 ; 
    if (true) 
    {

        {
        
    };
    string str = "" ; 
    getline(cin , str ) ; 
 
    Lexer a(str) ;
    
    vector<Token> tks = a.tokenize() ; 
    for (int i = 0 ; i < tks.size() ; i++)
    {
        cout << tks[i].GetValue() << " " ; 
    }
    cout << endl  << endl  ; 
    Parser p (tks) ;
    cout << endl  << endl  ;
    shared_ptr<Node> flo = p.ParsParseAssign_Or_FunctionAsStatemente() ; 
    flo->print() ; 
    (1+1);  

    return 0;
    }
}