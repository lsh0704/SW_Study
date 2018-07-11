/* 
   Copyright (C) B.-M. Chang for Language S Interpreter, 2017
*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "global.h"  

int token; /* holds the current input token for the parse */
int declaration = 0;
int inmatchfun = 0;
int inmatchstmt = 0;
int intry = 0;
int thrown = 0, throwv = 0;

/* declarations to allow arbitrary recursion */
void stmt(void); 
void matchstmt(void); 
void matchfun(void);   /* match function declaration */
void command(void);
int expr(void);
int term(void);
int factor(void);
int number(void);
int digit(void);

void error(void)
{ 
  printf("syntax error : %d : %d\n", lineno, token);   // 오류 시 token 번호가 출력되므로 global.h에서 해당 토큰 번호을 확인해야 함.
  //exit(1);
}

void match(int c) 
{ 
  if (token == c) token = getToken(); 
  else error();
}

void stmt(void)
{  
   int loc, result, whilestart, type, n;

   if (thrown) matchstmt();             // 예외가 발생한 상황일 때
   else 
   switch(token) 
   {
	case ID:  //  <stmt >-> id  =  <expr>
		loc = tokenval; 
		match(ID); 
		if (token == '=') {
			match('=');
			result = expr( ); 
			symtable[loc].val = result; 
		}
		break;

	case '(':    // Lab #3 <stmt> -> ( <stmt> {; <stmt>} )'






		break;

	case IF:	  // Lab #3 <stmt> ->  if ( <expr> )  then <stmt > end   |  if  ( <expr> )  then <stmt> else <stmt>  







		break;

	case WHILE: // Lab #3 <stmt> -> while ( <expr> )  do <stmt>
		match(WHILE);
		whilestart = pc;
		match('(');






		break;

	case READ:	// Lab #3 <stmt> -> read id







		break;

	case PRINT:	// <stmt> -> print <expr> 
		match(PRINT); 
		result = expr();
		printf("%d\n", result);
		break;

	//stmt -> let <type> id = <expr> {, <type> id = <expr> } in <stmt> {; <stmt>} end 
	case LET:      //  Lab #4 블록 선언


  




		break;

	case RAISE: 	// Lab #5 예외 발생 
	                   



		break;

	case TRY: 	// Lab #5 예외 처리 
	                   





		break;


	case RETURN:  // Lab #6 함수 리턴 <stmt> -> return <expr>
	                              






		break;    
   	}
}

void matchfun(void)   // match id (<type> id {, <type> id}) : <stmt>
{
    inmatchfun = 1;
    match(ID);
    match('(');

    while (token == INT | token == BOOL) {
       	match(token);
       	match(ID);
       	if (token == ',') match(',');
		else break;
    }

    match(')');
    match(':');
    matchstmt();
    inmatchfun = 0;
}

void matchstmt(void)
{  
   inmatchstmt = 1;

   switch(token) 
   {
	case ID: 
		match(ID); 
		if (token == '=') {
			match('=');
			expr( ); 
		}
		break;

	case '(':
		match('(');
		matchstmt();
		while(token == ';') {
			match(';');
			matchstmt();
		}
		if (token == ')') 
			match(')');
		break;

	case IF:
		match(IF);
		match('(');
		expr();
		match(')');
		match(THEN);
		matchstmt();
		if (token == END) 
                        match(END);
                   else { 
                        match(ELSE);
                        matchstmt();
                  }
		break;

	case WHILE:
		match(WHILE);
		match('(');
		expr();
		match(')');
		match(DO);
		matchstmt();
		break;

	case READ:
		match(READ);
		match(ID);
		break;

	case PRINT: 
		match(PRINT); 
		expr();
		break;

	case LET: //  Lab #4 블록 선언 match
		



		break;

	case RETURN:
		match(RETURN);
		expr();
		break;

	case RAISE:  // Lab #5 예외 발생 match
	


		break;

	case TRY:   // Lab #5 예외 처리 match



		break;
   }
   inmatchstmt = 0;
}


// <expr> -> !<expr> | true | false | <aexp> 
//                | <exp> == <aexp> | <aexp> != <aexp> | <aexp> < <aexp> | <aexp> > <aexp>  

int expr(void)  
{ int value1, value2, result;

  if (token == '!') {
	match('!');
	result = expr();
	if (result) result = 0;
	else result = 1;
	return result;
  } 
  else if (token == TRUE) {
	match(TRUE); 
	return 1;
  }
  else if (token == FALSE) {
	match(FALSE); 
	return 0;
  }

  result = aexp();
  switch(token) 
  {
	case '=' : 
		match('=');
		match('=');
		value1 = result;
		value2 = aexp();
		if (value1 == value2)
			result = 1;
		else result = 0;
		break;
	case '!' : 
		match('!');
		match('=');
		value1 = result;
		value2 = aexp();
		if (value1 != value2)
			result = 1;
		else result = 0;
		break;
	case '>' : 
		match('>');
		value1 = result;
		value2 = aexp();
		if (value1 > value2)
			result = 1;
		else result = 0;
		break;
	case '<' : 
		match('<');
		value1 = result;
		value2 = aexp();
		if (value1 < value2)
			result = 1;
		else result = 0;
		break;

  }
  return result;
}

int aexp(void)
{ int result;

  result = term();		 // <aexp> -> <term> { + <term> | - <term> } 
  while (token == '+' || token == '-')
  { 
	if (token == '+') {
	   match('+');
	   result += term();
	} else {
	   match('-');
	   result -= term();
	}
  }
  return result;
}

int term(void)        // <term> -> <factor> { * <factor> | / <factor> } 
{ int result = factor();
  while (token == '*'  || token == '/')
  { 
	if (token == '*') {
	   match('*');
	   result *= factor();
	} else {
	   match('/');
	   result /= factor();
	}
  }
  return result;
}

int factor(void)  /* <factor> -> ( <aexp> ) | n | -n | id  | id (<expr> {, <expr>}) */
{   int loc, result, funstart, args[10], i = 0;
    
	switch(token) 
    {
    case '(': 
		match('(');
    		result = aexp();
    		match(')');
		break;
	case '-':  // Lab #3 -n 처리


	 	break;	
  	case NUM: 
     		result = tokenval;
     		token = getToken();
		break;
  	case ID:  // Lab #3  ID 처리
 

		break;
    case FUNID: // Lab #6 함수 호출



        break; 
    }
    return result;
}

void parse(void)
{ 
   token = getToken(); /* CHANGE: get the first token */ 
   stmt(); 	/* CHANGE: call the parsing procedure for the start symbol */
}

int main()
{ 
	printf("Language S version 1.0\n");
	printf("Copyright (c) 2017 Byeong-Mo Chang, Sookmyung W. University\n\n");
	
	init();
	
	for ( ; ; )
	{
	    printf("> ");
	    loading();  
        parse();
	}
    return 0;
}