/* Code from Chapter 2 of ASU Dragon Book.   Oct 8 2007
   Modified by B.-M. Chang for S Interpreter.		May 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "global.h"

char lexbuf[BSIZE];
int lineno = 1;
int tokenval = NONE;
int pc = 0;  // index of M[]

char lexemes[STRMAX];
int lastchar = -1;    	// last position in lexemes
struct entry symtable[SYMMAX];
int lastentry = 0;     	// statck top in symtable

/* env pointer in symtable  */
int ep = 0;     	

size_t strlen(char *);
char *strcpy(char *, char *);

int lookup(char s[])   	// returns position of entry for s
{
	int p;
	for (p = lastentry; p >0; p--)
		if (strcmp(symtable[p].lexptr,s) == 0)
			return p;
	return 0;
}

int push(char s[], int tok) 	// push s and return top position
{
	int len;
	len = strlen(s); 	// string length of s
	if (lastentry + 1 >= SYMMAX)
		lerror("symbol table full");
	if (lastchar + len + 1 >= STRMAX)
		lerror("lexemes array full");
	lastentry++;
	symtable[lastentry].token =  tok;
	symtable[lastentry].lexptr = &lexemes[lastchar + 1];
	lastchar = lastchar + len + 1;
	strcpy(symtable[lastentry].lexptr, s);
	symtable[lastentry].val = 0;
	return lastentry;
}

pop(int n)
{
	lastentry -= n;
}

/* lexical analyzer
keyword IF:	return IF;  
...
keyword PRINT:	return PRINT; 
number:  	return NUM; tokenval = value of the number
name:  		return ID; tokenval = i such that symtable[i] contains the name 
other char c:  	return c; tokenval = NONE
*/

int getToken( )  /* lexical analyzer */
{
	int t;
	while (1)
	{
		t = GETCHAR( );
		if (t == ' ' || t == '\t') 
			;  // skip out white space
		else if (t == '\n')  // new line
		{
			lineno++;
			tokenval = NONE;
			//return t;
		}
		else if  (isdigit(t))   // t is a digit
		{	int result = 0;	
			while (isdigit(t)) {
				result = 10 * result + t - '0';
				t = GETCHAR( );
			}

			if (t != EOF)
				UNGETC(t, stdin);

			tokenval = result;
			return NUM;
		}
		else if (isalpha(t))  // t is a letter
		{
			int p, b = 0;
			while (isalnum(t)) // t is alphanumeric
			{
				lexbuf[b] = t;
				t = GETCHAR( );
				b++;
				if (b >= BSIZE)
					lerror("lexical error");
			}
			lexbuf[b] = EOS;
			if (t != EOF)
				UNGETC(t, stdin);

			p = lookup(lexbuf);

		    // don't push ID into symtable within matchstmt or matchfun
			if (p == 0 && (inmatchfun || inmatchstmt)) 
				return ID;  

			if (p == 0 || declaration)  // push ID into symtable
				p = push(lexbuf, ID);
			
			tokenval = p;
			return symtable[p].token;
		}
		else if (t == EOF)
			return DONE;
		else 
		{
			tokenval = NONE;
			return t;
		}
	}
}

struct entry keywords[] =   	// keywords
{
		"if", IF, 0,
		"then", THEN, 0,
		"else", ELSE, 0,
		"while", WHILE, 0,
		"do", DO, 0,
		"read", READ, 0,
		"print", PRINT, 0,
		"true", TRUE, 0,
		"false", FALSE, 0,
		"let", LET, 0,
		"in", IN, 0,
		"end", END, 0,
		"int", INT, 0,
		"bool", BOOL, 0,
		"void", VOID, 0,
		"fun", FUN, 0,
		"return", RETURN, 0,   	// return Ãß°¡ 
		"try", TRY, 0,
		"handle", HANDLE, 0,
		"raise", RAISE, 0,
		"exc", EXC, 0,
		0, 0, 0
};

init( )
{
	struct entry *p;

	for (p = keywords; p->token ; p++ ) // push keywords into symtable[]
		push(p->lexptr, p->token); 
}

loading( )  // load input program into M[] and push keywords into symtable[]
{
	char c;

	pc = 0; 
	lineno=1;
	while ((c=getchar()) != '.') // load input into memory
	   M[pc++] = c;
    
	c = getchar(); // read ''\n' character
	if ( pc ==  0) {
		exit(0);
	}
    
	M[pc] = EOF; 	// store EOF
	pc = 0;  	// index of M[] : start from the beginning
}

lerror(char *m) // print error messages
{
	fprintf(stderr, "line %d: %s\n", lineno, m);
	exit(1);
}