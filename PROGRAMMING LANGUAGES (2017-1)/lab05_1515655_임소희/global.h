#define BSIZE 128
#define NONE -1
#define EOS '\0'

// define tokens
#define NUM 256		
#define IF 257 	
#define THEN 258	
#define ELSE 259
#define WHILE 260	
#define DO 261	
#define READ 262	
#define PRINT 263
#define TRUE 264
#define FALSE 265
#define LET 266
#define IN 267
#define END 268
#define INT 269
#define BOOL 270
#define VOID 271
#define FUN 272
#define ID 273		// for name(identifier)
#define DONE 274 	// for EOF

/* for funtion call and return */
#define RETURN 275 	// return 
#define FUNID 276 	// function name
#define RV 277 		// return value 
#define CL 278 		// control link 
#define RA 279 		// return address 

/* for exception handling */
#define TRY 280
#define HANDLE 281
#define RAISE 282
#define EXC 283
#define EXCID 284
#define REFID 285

/* Memeory M */
#define MSIZE 10000 	  // Memory size
#define GETCHAR() M[pc++] // getchar() from Memory M
#define UNGETC(C,S) pc--  // ungetc() from Memory M
char M[MSIZE];

extern int pc;   	// index of M[] 
extern int tokenval; 	// value of token
extern int lineno;
extern int lastentry;

/* env pointer */
extern int ep;
extern int inmatchfun;
extern int inmatchstmt;
extern int declaration;

#define STRMAX 999
#define SYMMAX 999

struct entry { 	// symbol table entry
	char *lexptr;
	int token;
	int val;
};

struct entry symtable[SYMMAX]; // symbol table
