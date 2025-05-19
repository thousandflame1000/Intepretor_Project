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
#include "evaluate.h"
using namespace std ;

int& sum_reference  ( int& ref )
{
    return ref  ; 
}


//#include "parser.h"s
int main() {
    srand(time(nullptr));
    int c = 1 ; 
    int b = 2 ; 

    string str = "" ;
    getline(cin , str) ; 
    
 
    Lexer a(str) ;
    int mm  = 2 ;
    sum_reference(mm) = 3 ;    
    vector<Token> tks = a.tokenize() ; 

   
    for (int i = 0 ;  i < tks.size()  ; i++)
    {
        cout << tks[i].GetValue()  << " "  ; 
    }
    
    string aabb ; 
    cout << endl  << endl  ; 
    Parser p (tks) ; 
    shared_ptr<Node> flo = p.ParseBlock() ; 
    cin >> aabb ; 
    flo->print();  
    Interpreter machine  ("idk") ; 
    machine.Code_Generation(flo) ;
    
    machine.printByteCode();
    
    cin >> aabb   ;
    machine.execute() ; 






//    flo->print() ; 

    //flo->print() ; 

    (1+1);  
    return 0;
   
}