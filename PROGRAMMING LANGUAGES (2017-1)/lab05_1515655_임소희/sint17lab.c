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
int thrown = 0, throwv = 0; /*thrown : ���� �߻� ����(true or false)
							throwv : �߻��� ���� id(symtable ���� �ش� ��Ʈ�� �ε���)*/

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
	printf("syntax error : %d : %d\n", lineno, token);   // ���� �� token ��ȣ�� ��µǹǷ� global.h���� �ش� ��ū ��ȣ�� Ȯ���ؾ� ��.
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

	if (thrown) { // ���ܰ� �߻��� ��Ȳ�� ��
		matchstmt();
		return;
	}
	else
		switch (token)
		{
		case ID:  //  <stmt >-> id  =  <expr>
				  //���� expr ���� ����ؼ� symtable[] ���� ���� id�� ��Ʈ���� �����Ѵ�.
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
					 //��ȣ ���� ������� ���������� �����Ѵ�.
			match('(');
			stmt(); // stmt�� �����鼭 �����Ѵ�.
			while (token == ';') // ���� token�� ';'�̸� stmt()�� �ݺ��Ѵ�.
			{
				match(';');
				stmt();
			}
			// ���� token�� ')'�̸� match(')')�ϰ� ������.
			match(')');
			break;

		case IF:	  // Lab #3 <stmt> ->  if ( <expr> )  then <stmt > end   |  if  ( <expr> )  then <stmt> else <stmt> 
					  //���� <expr> ���� ����ϰ� ���� ���� ���� stmt Ȥ�� stmt�� �����Ѵ�.
			match(IF);
			match('(');
			result = expr(); //expr�� �����鼭 ���� ����Ѵ�. ȣ�� ���Ŀ� ��ȣ ��ġ
			match(')');
			if (result) //expr() ���� true�̸�
			{
				match(THEN); //then �κ�
				stmt(); //stmt ����
				if (token == END)
				{
					match(END);
				}
				else if (token == ELSE)
				{
					match(ELSE); //else �κ�
					matchstmt(); //stmt �ǳʶ�
				}
			}
			else //expr() ���� false�̸�
			{
				match(THEN); //then �κ�
				matchstmt(); //stmt �ǳʶ�
				if (token == ELSE)
				{
					match(ELSE); //else �κ�
					stmt(); //stmt ����
				}
				else
					match(END);
			}
			break;

		case WHILE: // Lab #3 <stmt> -> while ( <expr> )  do <stmt>
					//���� expr�� stmt�� �ݺ������� �����鼭 �ؼ��Ѵ�.
			match(WHILE); //while ���� ó�� ������ match(WHILE)�ϰ�
			whilestart = pc; //���� ��ġ�� �����Ѵ�. (while �� ���� ��ġ)
			match('(');
			result = expr(); //expr�� �����鼭 ���� ����Ѵ�. ȣ�� ���Ŀ� ��ȣ ��ġ
			match(')');
			while (result) //expr() ���� true �̸�
			{
				match(DO); //match(DO) �Ŀ�
				stmt(); //stmt()�� ȣ���Ͽ� �����ϰ�
				pc = whilestart; //����� ������ġ�� ���ư���
				token = getToken(); //match('(')���� �ݺ��Ѵ�. (���� token �б�)
				result = expr();
				match(')');
			}
			//expr() ���� false �̸�
			match(DO);
			matchstmt(); //stmt �ǳʶٰ� ������
			break;

		case READ:	// Lab #3 <stmt> -> read id
					//���� ���� �а� �Էµ� ���� symtable[]���� ���� id ��Ʈ���� �����Ѵ�.
			match(READ); /*��ġ �Ŀ� getToken()�� ȣ���Ͽ� ���� ��ū�� �д´�.
						 getToken()�� ���� id�� ������ symtable[] ���� �ش� ��ġ�� ã��
						 tokenval�� ������ �д�.*/
			loc = tokenval; //�� ��ġ�� �������� loc�� ������ �д�.
			if (token == INT)
				match(INT);
			else if (token == BOOL)
				match(BOOL);
			else if (token == ID)
				match(ID);
			scanf_s("%d", &result); //�Է� ���� �޴´�.
			symtable[loc].val = result; //�Է� ���� symtable[loc].val�� �����Ѵ�.
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
		case LET:      //  Lab #4 ��� ���� & Lab #5 ���� ����
			match(LET);
			n = 0; //let ���� �� ���� ���� ������ ���� ���� ����
			while (token == INT | token == BOOL) //��� ����
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
			//let exc negative, exc toobig, int x = 0, int y = 1 in //����
			//let exc negative, int x = 0, int y = 1 in //����
			while (token == EXC) //���� ����
			{
				n++;
				declaration = 1;
				match(EXC);
				loc = tokenval;
				declaration = 0;
				match(ID);
				symtable[loc].token = EXCID;
				while (token == ',') //���� �����ϴ� ���
				{
					match(',');
					n++;
					type = token;
					if (token == EXC)
					{
						declaration = 1;
						match(token); //EXC
						loc = tokenval;
						declaration = 0;
						match(ID);
						symtable[loc].token = EXCID; //token ID->EXCID�� ����
					}
					if (token == INT || token == BOOL)
					{
						declaration = 1;
						type = token;
						match(token); //INT || BOOL
						loc = tokenval;
						declaration = 0;
						match(ID);
						symtable[loc].token = type; //token ID->INT || BOOL�� ����
						if (token == '=')
						{
							match('=');
							result = expr();
							symtable[loc].val = result;
						}
					}
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
			pop(n); //let ���� �� ���� ���� ���� ��ŭ pop
			break;

			//raise id
		case RAISE: 	// Lab #5 ���� �߻� 
			match(RAISE);
			if (intry)//try ��� �� �� �� ó�� ������
			{
				thrown = 1; /*thrown = true : ���� �߻���*/
				loc = tokenval;
				match(EXCID);
				throwv = loc; //�߻��� ���� id(symtable ���� �ش� ��Ʈ�� �ε���)
			}
			else { ; } //try ��� ���� �� ������ ���α׷� ����
			break;

			//try <stmt> handle (id x) <stmt> /*id = EXCID,  x = reference ID*/
		case TRY: 	// Lab #5 ���� ó�� 
			match(TRY);
			intry++; //try ��� �� (true)
			stmt();
			match(HANDLE);
			match('(');
			loc = tokenval;
			match(EXCID);
			symtable[tokenval].token = REFID;
			symtable[tokenval].val = loc;
			match(ID);			
			match(')');
			thrown = 0; //thrown=false : ���� ó�� �� ���������� ����
			if (loc == throwv) stmt(); /*�߻��� ���� id�� ó�� ���� id2 ���Ͽ� ������
																	s3 �����Ͽ� ���� ó���ϰ� �Ŀ��� ���������� ����*/
			else matchstmt();
			intry--; //try ��� �� (false)
			throwv = 0; //try ��� ���� �Ǿ����Ƿ� �ʱ�ȭ
			break;

		case RETURN:  // Lab #6 �Լ� ���� <stmt> -> return <expr>







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

	case LET: //  Lab #4 ��� ���� match 
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
		while (token == EXC) //���� ����
		{
			match(EXC);
			match(EXCID);
			while (token == ',') //���� �����ϴ� ���
			{
				match(',');
				if (token == EXC)
				{
					match(token); //EXC
								  //loc = tokenval;
					match(ID);
				}
				if (token == INT || token == BOOL)
				{
					match(token); //INT || BOOL
					match(ID);
					if (token == '=')
					{
						match('=');
						expr();
					}
				}
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

	case RAISE:  // Lab #5 ���� �߻� match
		match(RAISE);
		match(EXCID);
		break;

	case TRY:   // Lab #5 ���� ó�� match
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
	case '-':  // Lab #3 -n ó��
		match('-');
		result = -tokenval;
		token = getToken();
		break;
	case NUM:
		result = tokenval;
		token = getToken();
		break;
	case ID:  // Lab #3  ID ó��
		loc = tokenval;
		result = symtable[loc].val;
		token = getToken();
		break;
	case INT: // INT ó��
		loc = tokenval;
		result = symtable[loc].val;
		token = getToken();
		break;
	case BOOL: // BOOL ó��
		loc = tokenval;
		result = symtable[loc].val;
		token = getToken();
		break;
	case FUNID: // Lab #6 �Լ� ȣ��



		break;
	case EXCID: //EXCID ó��
		loc = tokenval;
		result = symtable[loc].val;
		token = getToken();
		break;
	case REFID: //REFID ó��
		loc = tokenval;
		result = symtable[loc].val;
		token = getToken();
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