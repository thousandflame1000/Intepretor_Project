程式語言名稱 Multi脆(Multi Crumbles) 


BNF expression 
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
<expression> ::= <bool> | <integer> | <flaot> | <string> | <char> |<table> | 



<identifier> :: = <>