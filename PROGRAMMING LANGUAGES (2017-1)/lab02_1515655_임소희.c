#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

int token; /*holds the current input character for the parse*/

/*declarations to allow arbitrary recursion*/
void command(void);
int expr(void);
int term(void); //곱셈, 나눗셈 (항 계산)
int factor(void);
int number(void);
int digit(void);

int aexp(void); //더하기, 뺄셈 (수식 값 계산)
int bexp(void); //비교연산(==, !=, >, <, !) 

void error(void) 
{
	printf("parse error\n");
	exit(1);
}

void getToken(void)
{ /*tokens are character*/
	token = getchar();
}

void match(char c)
{
	if (token == c) getToken();
	else error();
}

void command(void)
/*command -> expr '\n'*/
{
	int result = expr();
	if (token == '\n') /*end the parse and print the result*/
	{
		if (result == 1)
			printf("The result is : true\n");
		else if (result == 0)
			printf("The result is : false\n");
		else
			printf("The result is : %d\n", result);
	}
	else error();
}

int expr(void)
/* expr -> <aexp> | <bexp> */
{
	int result = bexp();
	return result;
}

int term(void)
/*<term> -> <factor> {* <factor> | / <factor> } */
{
	int result = factor();
	while (token == '*' || token == '/')
	{
		if (token == '*') {
			match('*');
			result *= factor();
		}
		
		if (token == '/') {
			match('/');
			result /= factor();
		}
	}
	return result;
}

int factor(void)
/*<factor> -> <number> | (<aexp>) */
{
	int result;
	if (token == '(')
	{
		match('(');
		result = aexp();
		match(')');
	}
	else
		result = number();
	return result;
}

int number(void)
/*number -> digit {digit} */
{
	int result = digit();
	while (isdigit(token))
		/*the value of a number with a new trailing digit
		is its previous value shifted by a decimal place
		plus the value of the new digit
		*/
		result = 10 * result + digit();
	return result;
}

int digit(void)
/*digit -> '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | '!' | 'T' | 'F' */
{
	int result;
	if (isdigit(token))
	{/*the numeric value of a digit character
	 is the difference between its ascii value and the
	 ascii value of the characer '0'
	 */
		result = token - '0';
		match(token);
	}
	else {
		if (token == '!')//!인 경우 에러 처리 내지 않기
			result = '!';
		else if (token == 'T')//T인 경우 에러 처리 내지 않기
			result = 'T';
		else if (token == 'F')//F인 경우 에러 처리 내지 않기
			result = 'F';
		else
			error();
	}
	return result;
}

void parse(void)
{
	getToken(); /*get the first token*/
	command(); /*call the parsing procedure for the start symbol*/
}

int aexp(void)
/* <aexp> -> <term> { + <term> | - <term> } */
{
	int result = term();

	while (token == '+' || token == '-')
	{
		if (token == '+') {
			match('+');
			result += term();
		}
		if (token == '-') {
			match('-');
			result -= term();
		}
	}
	return result;
}

int bexp(void) //bexp에서 aexp와 bexp 할일을 구분한다
/* <bexp> -> true | false | <aexp> == <aexp> | <aexp> != <aexp>
			| <aexp> < <aexp> | <aexp> > <aexp> | !<bexp> */
{
	int result = aexp();
	int result2;
	
	while (token == 'T' || token == 'F' || token == '=' || token == '!' || token == '<' || token == '>')
	{
		if (token == 'T') {
			match('T');
			result = 1;
		}
		else if (token == 'F') {
			match('F');
			result = 0;
		}
		
		if (token == '=') {
			match('=');
			while (token == '=') {
				if (token == '=') {
					match('=');
					result2 = aexp();
					if (result == result2)
						result = 1;
					else
						result = 0;
				}
			}
		}

		else if (token == '!') {
			match('!');
			while (token == '=') {
				if (token == '=') {
					match('=');
					result2 = aexp();
					if (result != result2) {
						result = 1;
						return result;
					}
					else {
						result = 0;
						return result;
					}
				}
			}
			result2 = bexp();
			if (!result2) //!0이면 TRUE(1), !(0이외의 수)이면 FALSE(0)
			{
				result = 1;
			}
			else {
				result = 0;
			}
		}

		else if (token == '<') {
			match('<');
			result2 = aexp();
			if (result < result2) 
				result = 1;
			else 
				result = 0;
		}
		else if (token == '>') {
			match('>');
			result2 = aexp();
			if (result > result2)
				result = 1;
			else 
				result = 0;			
		}
	}
	return result;
}

main()
{
	parse();
	return 0;
}