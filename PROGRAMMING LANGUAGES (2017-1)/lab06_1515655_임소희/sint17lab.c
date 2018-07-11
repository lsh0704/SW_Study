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
int thrown = 0, throwv = 0; /*thrown : 예외 발생 여부(true or false)
							throwv : 발생된 예외 id(symtable 내의 해당 엔트리 인덱스)*/

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

	if (thrown) { // 예외가 발생한 상황일 때
		matchstmt();
		return;
	}
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
			else if (symtable[loc].token == REFID) {
				printf("exception : ");
				printf("%s\n", symtable[symtable[loc].val].lexptr);
			}
			else {
				printf("%d\n", result);
			}
			break;

			//stmt -> let <type> id [= <expr>] {, <type> id [= <expr>] } in <stmt> {; <stmt>} end 
			//stmt -> let exc id in <stmt> end
			//stmt -> let fun <type'> id(<type> id {,<type> id}) : <stmt> in <stmt> end
		case LET:      //  Lab #4 블록 선언 & Lab #5 예외 정의 & Lab #6 함수 구현
			match(LET);
			n = 0; //let 변수 및 예외 정의 선언한 개수 세기 위해
			// let int a=0, int b = 0, fun int square(int x) : return x * x
			while (token == ',' || token == FUN || token == ID || token == INT | token == BOOL) { //token이 ','일 때 돌도록
				if (token == ',') match(',');
				if (token == FUN)
				{
					n++;
					match(FUN);
					declaration = 1;
					match(token);	// 반환타입 : <type'> -> int | bool | void
					declaration = 0;

					if (token == ID)	// 함수 이름
					{
						loc = tokenval;
						symtable[loc].token = FUNID;
						symtable[loc].val = pc;	// 현재 위치 pc 값
						matchfun();				// 나머지 선언 부분 skip
					}
				}
				//let exc negative, exc toobig, int x = 0, int y = 1 in //예제
				//let exc negative, int x = 0, int y = 1 in //예제
				if (token == EXC) //예외 정의
				{
					n++;
					declaration = 1;
					match(EXC);
					loc = tokenval;
					declaration = 0;
					match(ID);
					symtable[loc].token = EXCID;
					if (token == '=')
					{
						match('=');
						result = expr();
						symtable[loc].val = result;
					}
					if (token == ',')
						continue; 
				}
				if (token == INT | token == BOOL) //블록 선언
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
						continue; 
				}
			}
			match(IN);
			stmt();
			while (token == ';')
			{
				match(';');
				stmt();
			}
			match(END);
			pop(n); //let 변수 및 예외 정의 개수 만큼 pop
			break;

			//raise id
		case RAISE: 	// Lab #5 예외 발생 
			match(RAISE);
			if (intry)//try 블록 안 일 때 처리 가능함
			{
				thrown = 1; /*thrown = true : 예외 발생시*/
				loc = tokenval;
				match(EXCID);
				throwv = loc; //발생된 예외 id(symtable 내의 해당 엔트리 인덱스)
			}
			else { ; } //try 블록 밖일 때 오류로 프로그램 종료
			break;

			//try <stmt> handle (id x) <stmt> /*id = EXCID,  x = reference ID*/
		case TRY: 	// Lab #5 예외 처리 
			match(TRY);
			intry++; //try 블록 내 (true)
			stmt();
			match(HANDLE);
			match('(');
			loc = tokenval;
			match(EXCID);
			symtable[tokenval].token = REFID;
			symtable[tokenval].val = loc;
			match(ID);			
			match(')');
			thrown = 0; //thrown=false : 예외 처리 후 정상적으로 진행
			if (loc == throwv) stmt(); /*발생된 예외 id과 처리 예외 id2 비교하여 같으면
																	s3 실행하여 예외 처리하고 후에는 정상적으로 진행*/
			else matchstmt();
			intry--; //try 블록 밖 (false)
			throwv = 0; //try 블록 종료 되었으므로 초기화
			break;

		case RETURN:  // Lab #6 함수 리턴 <stmt> -> return <expr>
			match(RETURN);
			result = expr();	// 반환 값 계산
			symtable[ep - 1].val = result;	// RV 엔트리에 값 저장(반환)
											/*ep(Environment pointer) : 스택에 대한 포인터*/
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

	case LET: //  Lab #4 블록 선언 match & Lab #5 예외 정의 & Lab #6 함수 구현
		match(LET);
		while (token == ',' || token == FUN || token == EXC || token == INT || token == BOOL)
		{
			if (token == ',') match(',');
			if (token == FUN)
			{
				match(FUN);
				match(token);	// 반환타입 : <type'> -> int | bool | void
				match(FUNID);
				matchfun();				// 나머지 선언 부분 skip
			}
			if (token == EXC) //예외 정의
			{
				match(EXC);
				match(EXCID);
				if (token == '=')
				{
					match('=');
					expr();
				}
				if (token == ',')
					continue; 
			}
			if (token == INT | token == BOOL)
			{
				match(token);
				match(token);
				if (token == '=')
				{
					match('=');
					expr();
				}
				if (token == ',')
					continue; 
			}
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
		match(RAISE);
		match(EXCID);
		break;

	case TRY:   // Lab #5 예외 처리 match
		match(TRY);
		matchstmt();
		match(HANDLE);
		match('(');
		match(EXCID);
		match(ID);
		match(')');
		matchstmt();
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
	case VOID: // VOID 처리
		loc = tokenval;
		result = symtable[loc].val;
		token = getToken();
		break;
	case EXCID: //EXCID 처리
		loc = tokenval;
		result = symtable[loc].val;
		token = getToken();
		break;
	case REFID: //REFID 처리
		loc = tokenval;
		result = symtable[loc].val;
		token = getToken();
		break;
	case FUNID: // Lab #6 함수 호출(function call) f(E,{,E})
		if (inmatchfun || inmatchstmt) {
			match(FUNID);
			match('(');
			if (token != ')')
				expr();
			while (token == ',') {
				match(',');
				expr();
			}
			match(')');
			result = -1;
			break;
		}

		loc = tokenval;
		funstart = symtable[loc].val; //함수 시작 위치
		token = getToken();

		if (token == '(')	//실매개변수 계산
		{
			match('(');
			args[i++] = expr();/*실매개변수 계산하여 임시저장;*/
			while (token == ',') {
				match(',');
				args[i++] = expr(); }
		}

		//set up activation record
		/*symtable에 RV,CL 엔트리를 만든다;*/
		symtable[++lastentry].token = RV;	
		symtable[lastentry].lexptr = "RV";
		symtable[++lastentry].token = CL;
		symtable[lastentry].lexptr = "CL";
		symtable[lastentry].val=ep;/*CL 엔트리 <-현재 ep 값*/		// control link 저장
		ep = lastentry;/*ep 값 갱신*/					// 새로운 AR 가리키도록
		/*symtable에 RA 엔트리를 만든다;*/
		symtable[++lastentry].token = RA;
		symtable[lastentry].lexptr = "RA";
		symtable[lastentry].val=pc;/*RA 엔트리 <-현재 pc 값*/		//return address 저장
		match(')');

		//GOTO	(T x {,T x}) : S
		pc = funstart;	//함수 시작 위치 값(funstart);
		token = getToken();
		match('(');
		i = 0;
		/*형식 매개변수를 위한 엔트리 생성;*/
		while (token == INT || token == BOOL) {
			match(token); //INT || BOOL
			symtable[tokenval].token = token;
			symtable[tokenval].val = args[i++];/*형식 매개변수 엔트리 < -실매개변수 값;*/	 //매개변수 전달
			match(token); //형식 매개변수 match		
			if (token == ',') match(',');
			else break;
		}  
		match(')');
		match(':');
		stmt();	//호출된 함수의 본체 문장 실행
					// 내부에서 return 문 실행
		//pop AR	//lastentry를 Return value로 옮김
		pc = symtable[ep + 1].val;	//return address 회복
		token = getToken();
		lastentry = ep - 1;
		ep = symtable[ep].val;	// ep -> caller's AR

		result = symtable[lastentry].val;	//RV 값
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