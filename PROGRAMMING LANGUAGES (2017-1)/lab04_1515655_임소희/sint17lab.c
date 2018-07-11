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
void type(void);
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
		switch (token)
		{
		case ID:  //  <stmt >-> id  =  <expr>
				  //수식 expr 값을 계산해서 symtable[] 내의 변수 id의 엔트리에 저장한다.
			loc = tokenval;
			match(ID);
			if (token == '=') {
				match('=');
				result = expr();
				symtable[loc].val = result;
			}
			break;

		case INT:
			loc = tokenval;
			match(INT);
			if (token == '=') {
				match('=');
				result = expr();
				symtable[loc].val = result;
			}
			break;

		case BOOL:
			loc = tokenval;
			match(BOOL);
			if (token == '=') {
				match('=');
				result = expr();
				symtable[loc].val = result;
			}
			break;

		case '(':    // Lab #3 <stmt> -> ( <stmt> {; <stmt>} )'
					 //괄호 내의 문장들을 순차적으로 실행한다.
			match('(');
			stmt(); // stmt을 읽으면서 실행한다.
			while (token == ';') // 다음 token이 ';'이면 stmt()를 반복한다.
			{
				match(';');
				stmt();
			}
			// 다음 token이 ')'이면 match(')')하고 끝낸다.
			match(')');
			break;

		case IF:	  // Lab #3 <stmt> ->  if ( <expr> )  then <stmt > end   |  if  ( <expr> )  then <stmt> else <stmt> 
					  //수식 <expr> 값을 계산하고 계산된 값에 따라 stmt 혹은 stmt를 실행한다.
			match(IF);
			match('(');
			result = expr(); //expr을 읽으면서 값을 계산한다. 호출 전후에 괄호 매치
			match(')');
			if (result) //expr() 값이 true이면
			{
				match(THEN); //then 부분
				stmt(); //stmt 실행
				if (token == END)
				{
					match(END);
				}
				else if (token == ELSE)
				{
					match(ELSE); //else 부분
					matchstmt(); //stmt 건너뜀
				}
			}
			else //expr() 값이 false이면
			{
				match(THEN); //then 부분
				matchstmt(); //stmt 건너뜀
				if (token == ELSE)
				{
					match(ELSE); //else 부분
					stmt(); //stmt 실행
				}
				else
					match(END);
			}
			break;

		case WHILE: // Lab #3 <stmt> -> while ( <expr> )  do <stmt>
					//수식 expr과 stmt를 반복적으로 읽으면서 해석한다.
			match(WHILE); //while 문을 처음 만나면 match(WHILE)하고
			whilestart = pc; //시작 위치를 저장한다. (while 문 시작 위치)
			match('(');
			result = expr(); //expr을 읽으면서 값을 계산한다. 호출 전후에 괄호 매치
			match(')');
			while (result) //expr() 값이 true 이면
			{
				match(DO); //match(DO) 후에
				stmt(); //stmt()를 호출하여 실행하고
				pc = whilestart; //저장된 시작위치로 돌아가서
				token = getToken(); //match('(')부터 반복한다. (다음 token 읽기)
				result = expr();
				match(')');
			}
			//expr() 값이 false 이면
			match(DO);
			matchstmt(); //stmt 건너뛰고 진행함
			break;

		case READ:	// Lab #3 <stmt> -> read id
					//정수 값을 읽고 입력된 값을 symtable[]내의 변수 id 엔트리에 저장한다.
			match(READ); /*매치 후에 getToken()을 호출하여 다음 토큰을 읽는다.
						 getToken()은 변수 id를 읽으면 symtable[] 내의 해당 위치를 찾아
						 tokenval에 저장해 둔다.*/
			loc = tokenval; //그 위치를 지역번수 loc에 저장해 둔다.
			if (token == INT)
				match(INT);
			else if (token == BOOL)
				match(BOOL);
			else if (token == ID)
				match(ID);
			scanf_s("%d", &result); //입력 값을 받는다.
			symtable[loc].val = result; //입력 값을 symtable[loc].val에 저장한다.
			break;

		case PRINT:	// <stmt> -> print <expr> 
			match(PRINT);
			loc = tokenval;
			result = expr();
			if (symtable[loc].token == BOOL) {
				if (symtable[loc].val == 1)
					printf("true\n");
				else if (symtable[loc].val == 0)
					printf("false\n");
			}
			else {
				printf("%d\n", result);
			}
			break;

			//stmt -> let <type> id [= <expr>] {, <type> id [= <expr>] } in <stmt> {; <stmt>} end 
		case LET:      //  Lab #4 블록 선언
			match(LET);
			n = 0; //let 변수 선언한 개수 세기 위해
			while (token == INT | token == BOOL)
			{
				type = token;
				n++;
				declaration = 1;
				match(type);
				loc = tokenval;
				declaration = 0;
				match(ID);
				symtable[loc].token = type;
				if (token == '=')
				{
					match('=');
					result = expr();
					symtable[loc].val = result;
				}
				if (token == ',')
					match(',');
				else break;
			}
			match(IN);
			stmt();
			while (token == ';')
			{
				match(';');
				stmt();
			}
			match(END);
			pop(n); //let 변수 개수 만큼 pop
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

	switch (token)
	{
	case ID:
		match(ID);
		if (token == '=') {
			match('=');
			expr();
		}
		break;

	case INT:
		match(INT);
		if (token == '=') {
			match('=');
			expr();
		}
		break;

	case BOOL:
		match(BOOL);
		if (token == '=') {
			match('=');
			expr();
		}
		break;

	case '(':
		match('(');
		matchstmt();
		while (token == ';') {
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
		if (token == INT)
			match(INT);
		else if (token == BOOL)
			match(BOOL);
		else if (token == ID)
			match(ID);
		break;

	case PRINT:
		match(PRINT);
		expr();
		break;

	case LET: //  Lab #4 블록 선언 match 
		match(LET);
		while (token == INT | token == BOOL)
		{
			match(token);
			match(ID);
			if (token == '=')
			{
				match('=');
				expr();
			}
			if (token == ',')
				match(',');
			else break;
		}
		match(IN);
		matchstmt();
		while (token == ';')
		{
			match(';');
			matchstmt();
		}
		match(END);
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
{
	int value1, value2, result;

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
	switch (token)
	{
	case '=':
		match('=');
		match('=');
		value1 = result;
		value2 = aexp();
		if (value1 == value2)
			result = 1;
		else result = 0;
		break;
	case '!':
		match('!');
		match('=');
		value1 = result;
		value2 = aexp();
		if (value1 != value2)
			result = 1;
		else result = 0;
		break;
	case '>':
		match('>');
		value1 = result;
		value2 = aexp();
		if (value1 > value2)
			result = 1;
		else result = 0;
		break;
	case '<':
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
{
	int result;

	result = term();		 // <aexp> -> <term> { + <term> | - <term> } 
	while (token == '+' || token == '-')
	{
		if (token == '+') {
			match('+');
			result += term();
		}
		else {
			match('-');
			result -= term();
		}
	}
	return result;
}

int term(void)        // <term> -> <factor> { * <factor> | / <factor> } 
{
	int result = factor();
	while (token == '*' || token == '/')
	{
		if (token == '*') {
			match('*');
			result *= factor();
		}
		else {
			match('/');
			result /= factor();
		}
	}
	return result;
}

int factor(void)  /* <factor> -> ( <aexp> ) | n | -n | id  | id (<expr> {, <expr>}) */
{
	int loc, result, funstart, args[10], i = 0;

	switch (token)
	{
	case '(':
		match('(');
		result = aexp();
		match(')');
		break;
	case '-':  // Lab #3 -n 처리
		match('-');
		result = -tokenval;
		token = getToken();
		break;
	case NUM:
		result = tokenval;
		token = getToken();
		break;
	case ID:  // Lab #3  ID 처리
		loc = tokenval;
		result = symtable[loc].val;
		token = getToken();
		break;
	case INT: // INT 처리
		loc = tokenval;
		result = symtable[loc].val;
		token = getToken();
		break;
	case BOOL: // BOOL 처리
		loc = tokenval;
		result = symtable[loc].val;
		token = getToken();
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

	for (; ; )
	{
		printf("> ");
		loading();
		parse();
	}
	return 0;
}