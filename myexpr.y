%{
#include <stdio.h>
#include <ctype.h>

#define YYSTEP ITEM  /*进入yacc栈的数据类型*/

%}


%start expr   /*定义文法开始的符号*/

/*类型标记*/
%token ANA
%token DGT
%token PUL
%token DGTA
%token TIME
%token STEP
%token NUMBER     /*数字字符等字母*/
%token GEQUAL  /* >= */
%token LEQUAL  /* <= */
%token NEQUAL  /* != */
%token XOR
%token MAX
%token MIN
%token AVE
%token SUM
%token MAXT
%token MINT
%token ERR

/*运算符 从低到高排序*/
%left '|' XOR 
%left '&'