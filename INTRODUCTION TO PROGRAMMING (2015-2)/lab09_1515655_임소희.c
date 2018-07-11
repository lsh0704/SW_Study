#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#define MAXOP 100

main()
{
	int type;
	double op2;
	char s[MAXOP];
	int opert[100], operand[100], opsp = -1;
	int symbol;

	while ((type = getop(s)) != EOF)
	{

		switch (type)
		{
		case NUMBER:
			push(atof(s));

			for (; opsp>= 0 && size()==operand[opsp]+2 ; opsp--) 
			{
				symbol=opert[opsp];
				switch (symbol)
				{
					case '+':
						push(pop() + pop());
						break;
					case '*':
						push(pop() * pop());
						break;
					case '-':
						op2 = pop();
						push(pop() - op2);
						break;
					case '/':
						op2 = pop();
						if (op2 != 0.0)
							push(pop() / op2);
						else
							printf("error: zero divisor\n");	
						break;
				}
				
			}
			break;
		case '+':
		case '*':
		case '-':
		case '/':
			opert[++opsp] = type;
			operand[opsp] = size();
			break;

		case '\n':
			printf("\t%.8g\n", pop());
			break;

		default:
			printf("error: unknown command %s\n", s);
			break;
		}
	}
	
}
