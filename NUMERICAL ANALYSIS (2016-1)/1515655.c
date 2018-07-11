#include <stdio.h>
#include <stdlib.h>
#include <float.h> 
//isnan() ������� (�Ǽ��� 0.000�� �ǰų� �����÷ο� Ȯ��)->�� �ڵ忡�� ���X

void calculate_ex();
void calculate_sinx();
void calculate_ln();
void Bisection();
void Newton_Raphson();
void Secant();
void Equation();

double factorial(int n); //factorial ���
double pow(double x, int n);
double absolute(double n); //ea�� 'ũ��'�� ��Ÿ���� �ϹǷ� ���밪
double f(double a, double b, double c, double d, double x); //evaluate f(x)
double f_(double a, double b, double c, double x); // evaluate f'(x)

int i, j; //for�� ���� �� ���

main()
{
	printf("[ ID: 1515655 ]\n");
	printf("[ Name: �Ӽ��� ]\n\n");

	int Nmenu;

	while (1) {
		//display menu
		printf("1. e^x (-10.0 <= x <= 10.0)\n");
		printf("2. sin x (-10.0 <= x <= 10.0)\n");
		printf("3. ln x (0.0 < x <= 2.0)\n");
		printf("4. Cubic Equation solver (Bisection ver.)\n");
		printf("5. Cubic Equation solver (Newton-Raphson ver.)\n");
		printf("6. Cubic Equation solver (Sscant ver.)\n");
		printf("7. Equation System solver\n");
		printf("8. Quit\n");
		printf("\n\n");

		scanf_s("%d", &Nmenu); //receive menu number
		scanf_s("%*c"); //fflush�� ����� ���(���ۻ���)

		switch (Nmenu) {
		case 1:
			calculate_ex();
			break;
		case 2:
			calculate_sinx();
			break;
		case 3:
			calculate_ln();
			break;
		case 4:
			Bisection();
			break;
		case 5:
			Newton_Raphson();
			break;
		case 6:
			Secant();
			break;
		case 7:
			Equation();
			break;
		case 8:
			return 0; //����
			break;			
		default:
			printf("You inputted wrong number! One more time.\n\n");
			continue;
		}
	}
}

double factorial(int i) { //n!
	double answer = 1;
	if (i == 0 || i == 1) ;
	else
		for (j=2; j<=i; j++)
			answer *= j;
	return answer;
}

double pow(double x, int n) { //x^n
	int i;
	double answer=1;
	if (n == 0);
	else
		for (i = 1; i <= n; i++)
			answer *= x;
	return answer;
}

double absolute(double n) {
	if (n < 0) return -n;
	else return n;
}

double f(double a, double b, double c, double d, double x) //evaluate f(x)
{
	return a*x*x*x + b*x*x + c*x + d;
}
double f_(double a, double b, double c, double x) // evaluate f'(x)
{
	return 3 * a*x*x + 2 * b*x + c;
}


void calculate_ex()
{
	double x; double ex; //input, answer
	double ea=0.0; //approximate relative error (�ٻ������) = (���� �ٻ簪-���� �ٻ簪 / ���� �ٻ簪)*100
	double old_term, new_term; //���� �ٻ簪, �ݺ��� ���� �߰��Ǵ� ��
	double xi = 1; // x^1, x^2, ... x^n �� �� ���
	double taylor_sum = 0.0;

	printf("x = ");
	scanf_s("%lf", &x); //receive x

	for (i = 0, new_term = 0.0; ; i++) {
		old_term = taylor_sum;

		xi = pow(x, i); //x^i

		new_term = (xi / factorial(i));
		
		/*if (_isnan(new_term))
			break;*/
		if (new_term > DBL_MAX || new_term <= -DBL_MAX)
			break;

		taylor_sum += new_term;
		/*
		printf("i = %d\n", i);
		printf("new_term = %lf\n", new_term);
		printf("taylor_sum : %lf\n", taylor_sum);*/

		xi = 1; //xi �ʱ�ȭ
		ea = ((taylor_sum - old_term) / taylor_sum) * 100; // �ٻ������ ����
		
		//printf("ea : %lf\n\n", ea);
		ea = absolute(ea); //ea ���밪 ó��
		
		if (ea < 0.000001)
			break;
	}

	ex = taylor_sum; //�ٻ��
	printf("e^%g = %lf\n\n", x, ex);//display taylor_sum
}
void calculate_sinx()
{
	double x; double sinx; //input, answer
	double ea; //approximate relative error (�ٻ������) = (���� �ٻ簪-���� �ٻ簪 / ���� �ٻ簪)*100
	double old_term=0.0, new_term=0.0;  //���� �ٻ簪, �ݺ��� ���� �߰��Ǵ� ��
	double xi = 1; // x^1, x^2, ... x^n �� �� ���
	double taylor_sum = 0.0;

	printf("x = ");
	scanf_s("%lf", &x); //receive x
	scanf_s("%*c"); //buffer ����

	//x=0�� ���� sin0=0

	if (x!=0) {
		for (i = 0, new_term = 0.0; ; i++) {
			old_term = taylor_sum;

			xi=pow(x, (2*i + 1));  //x^(2n+1) ���ϱ�

			if (i % 2 != 0)
				xi = -xi; //i�� Ȧ�����̸� (-1)^n�̹Ƿ� ����ó��

			new_term = (xi / factorial(2 * i + 1));

			if (new_term > DBL_MAX || new_term <= -DBL_MAX)
				break;

			taylor_sum += new_term;
		
			/*printf("i = %d\n", i);
			printf("new_term = %lf\n", new_term);
			printf("taylor_sum : %lf\n", taylor_sum);*/

			xi = 1; //xi �ʱ�ȭ

			ea = ((taylor_sum - old_term) / taylor_sum) * 100; // �ٻ������ ����
			ea = absolute(ea); //ea ���밪 ó��
			//printf("ea : %lf\n\n", ea);
			
			if (ea < 0.00001)
				break;
		}
	}

	sinx = taylor_sum; //�ٻ��
	printf("sin %g = %lf\n\n", x, sinx);//display taylor_sum
}

void calculate_ln() 
{
	double x; double lnx; //input, answer
	double ea; //approximate relative error (�ٻ������) = (���� �ٻ簪-���� �ٻ簪 / ���� �ٻ簪)*100
	double old_term = 0.0, new_term = 0.0; //���� �ٻ簪, ���� �ٻ簪
	double xi = 1; // x^1, x^2, ... x^n �� �� ���

	printf("x = ");
	scanf_s("%lf", &x); //receive x

	if (x <= 0 || x>2)
		return -1; // ���� ���� �� �Է� �� �޴���

	if (x != 1) {
		for (i = 1, new_term = 0.0; ; i++) {
			if (i == 100000)
				printf("�� ���� ��� ������ ���� �ɸ��ϴ�. ��ø� ��ٷ��ּ���.\n"); //x==2�̸� i=288539�� �Ǿ�� �� �����.
			old_term = new_term;

			if (x != 1) 
				for (j = 1; j <= i; j++) { //x^n ���ϱ�
					xi *= (x - 1);
				}

				if (i % 2 == 0)
					xi = -xi; //i�� Ȧ�����̸� (-1)^n�̹Ƿ� ����ó��


				new_term += (xi / i);
				/*printf("i = %d\n", i);
				printf("���� ���� �� = %lf\n", xi / i);
				printf("new_Term : %lf\n", new_term);*/

				xi = 1; //xi �ʱ�ȭ

				ea = ((new_term - old_term) / new_term) * 100; // �ٻ������ ����
				ea = absolute(ea); //ea ���밪 ó��
				//printf("ea : %lf\n\n", ea);

				if (i > 0 && ea < 0.0005)
					break;
		}
	}
	lnx = new_term; //�ٻ��
	printf("ln %g = %lf\n\n", x, lnx);//display taylor_sum
}

void Bisection() //Find a root of ax^3 + bx^2 + cx + d = 0
{
	double a, b, c, d; //������ ���
	double xl, xu, es; //xl~xu ����, es=���Ƿ�s(stopping criterion)
	double r; //root
	int imax=50000; //�ݺ� ������ �ִ� Ƚ��. i>=imax�� �Ǹ� �Լ�����
	double ea=1.0; /*approximate error. �ݺ����� ���� ���� ���� <ea�Ǹ� ������ �����ϰ� �Լ�����
				  �ʱⰪ ũ�� ������ ������ �ݺ��� ���ư��� �ϱ�����. (ea<es)*/
	double fxl, fxr; /*f(xl), f(xr). �̷��� �صθ� ���߿� �ߺ���� �� �ʿ� ����.
					 xl   xr    xu ���� ��, ���� f(xl)f(xr) <0�� ������ xl~xr ������ ���� ������ �ǹ�.
					 ����, xr�� ���ο� xu�� �ǰ�, xl�� �״�δ�.
					 xl�� fxl�� ���������Ƿ�, f(xl)�� �� ����� �ʿ䰡 ����.*/
	//double xrold; // ���� xr
	double xr; // (xl+xu)/2
	double test; // f(xl)*f(xr)


	printf("a = "); scanf_s("%lf", &a); 
	scanf_s("%*c"); /*������ scanf ����ϱ� ��, ���� ���� fflush�� ������ ����
	                ���� ���� ��ġ�ʴ� input�� ��� �޴� ����*/
	printf("b = "); scanf_s("%lf", &b);
	scanf_s("%*c");
	printf("c = "); scanf_s("%lf", &c);
	scanf_s("%*c");
	printf("d = "); scanf_s("%lf", &d);
	scanf_s("%*c");

	printf("xl = "); scanf_s("%lf", &xl);
	scanf_s("%*c");
	printf("xu = "); scanf_s("%lf", &xu);
	scanf_s("%*c");
	printf("es = "); scanf_s("%lf", &es);
	scanf_s("%*c");

	fxl = f(a, b, c, d, xl); //f(xl) ���
	
	for (i = 0; ; i++) {
		//printf("\n%d��°\n", i);
		//printf("f(xl) = %lf\n", fxl);

		/*if (i!=0) // ó�� ���� �� xrold �����Ƿ�, i=0�Ͻ� �� �κ��� skip
		{
			xrold = xr;
			printf("xrold = %lf\n", xrold);
		}*/
		
		xr = (xl + xu) / 2;
		//printf("xr = %lf\n", xr);

		fxr = f(a, b, c, d, xr); //f(xr) ���		
		//printf("f(xr) = %lf\n", fxr);


		/*if (xr)*/ {
			//ea = ((xr - xrold) / xr) * 100; //�ٻ������
			ea = ((xu - xl) / (xu + xl)) * 100; //�ٻ������
			ea = absolute(ea); //ea ���밪 ó��
		}
		//printf("ea = %lf\n\n", ea);

		test = fxl*fxr;
		if (test < 0)
			xu = xr;
		else if (test > 0) {
			xl = xr;
			fxl = fxr;
		}
		else
			ea = 0; // ��ǻ� �ݺ��� ���������� ��. ea<es�ε�, es�� �����ϸ��� ���� ����!

		if (ea < es || i >= imax)
			break;
	}
	r = xr; //�ݺ������� ���� �ٻ簪�� root�� ����
	printf("\nFound root = %lf\n\n", r);
} 

void Newton_Raphson() 
{
	double a, b, c, d; //������ ���
	double x0, es;
	double r; //root
	int imax = 10000;
	double ea = 1.0; //|((xi+1 + xi)/xi+1)|*100%
	double xrold, xr; //xr=g'(xrold)

	printf("a = "); scanf_s("%lf", &a);
	scanf_s("%*c"); 
	printf("b = "); scanf_s("%lf", &b);
	scanf_s("%*c");
	printf("c = "); scanf_s("%lf", &c);
	scanf_s("%*c");
	printf("d = "); scanf_s("%lf", &d);
	scanf_s("%*c");

	printf("x0 = "); scanf_s("%lf", &x0);
	scanf_s("%*c");
	printf("es = "); scanf_s("%lf", &es);
	scanf_s("%*c");

	xr = x0; //�ʱⰪ
	for (i = 0; ; i++) {
		//printf("%d��° ����\n", i);
		xrold = xr;
		//printf("xrold = %lf\n", xrold);

		xr = xrold-(f(a,b,c,d,xrold)/f_(a, b, c, xrold)); // xi+1 = xi - f(xi)/f'(xi)
		//printf("xr = %lf\n", xr);

		if (xr) {
			ea = absolute((xr - xrold) / xr) * 100; //�ٻ������
		}
		//printf("ea = %lf\n\n", ea);
		
		if (ea < es || i >= imax)
			break;
	}
	r = xr;
	printf("\nFound root = %lf\n\n", r);
}

void Secant() 
{
	double a, b, c, d; //������ ���
	double x_1, x0, es;
	double r; //root
	int imax = 10000;
	double ea = 1.0; //|((xi+1 + xi)/xi+1)|*100%
	double xrold, xr, xrnew; //xi-1, xi, xi+1
	double fx, f_x; //f(xi), f(xi-1)

	printf("a = "); scanf_s("%lf", &a);
	scanf_s("%*c"); 
	printf("b = "); scanf_s("%lf", &b);
	scanf_s("%*c");
	printf("c = "); scanf_s("%lf", &c);
	scanf_s("%*c");
	printf("d = "); scanf_s("%lf", &d);
	scanf_s("%*c");

	printf("x_1 = "); scanf_s("%lf", &x_1); 
	scanf_s("%*c");
	printf("x0 = "); scanf_s("%lf", &x0);
	scanf_s("%*c");
	printf("es = "); scanf_s("%lf", &es);
	scanf_s("%*c");

	xrold = x_1; //�ʱⰪ
	xr = x0; //�ʱⰪ
	for (i = 0; ; i++) {
		//printf("%d��° ����\n", i);
		if (i) //i=0�϶��� x_1�� xrold, x0�� xr�̹Ƿ�
		{
			xrold = xr;
			xr = xrnew;
		}
		//printf("xrold = %lf\n", xrold);
		
		fx = f(a, b, c, d, xr); //f(xi)
		f_x = f(a, b, c, d, xrold); //f(xi-1)
		xrnew = xr - (fx*(xrold-xr)) / (f_x-fx); // xi+1 = xi - f(xi)/f'(xi)
		//printf("xr = %lf\n", xr);
		//printf("xrnew = %lf\n", xrnew);

		if (xr) {
			ea = absolute((xrnew - xr) / xrnew) * 100; //�ٻ������
		}
		//printf("ea = %lf\n\n", ea);

		if (ea < es || i >= imax)
			break;
	}
	r = xrnew;
	printf("\nFound root = %lf\n\n", r);
}

void Equation() 
{
	int n=0; // number of equations (=unknowns) n (3<=n<=6)
	int i, j, k;
	double factor; // �Ұ����� ���ؾ� �ϴ� ��
	double sum = 0;
	int singular = -1; // singular system���� Ȯ�� (-1:�ƴ�, 1:����)

	printf("n = "); scanf_s("%d", &n);
	scanf_s("%*c");

	/*A, B ���� �����Ҵ�*/
	double **a = (double**)malloc(sizeof(double*)*n); // n*n matrix [A], ��
	double *b = (double*)malloc(sizeof(double)*n); // n*1 matrix {B}
	double *x = (double*)malloc(sizeof(double)*n); // answer, [A]{X}={B}

	for (i = 0; i < n; i++) { //n=row
		a[i] = (double*)malloc(sizeof(double)*n); // ��, n=col
	}

	
	printf("[A] = "); //A�迭 �ֱ�
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			scanf_s("%lf", &a[i][j]);
			fflush(stdin);
		}
	}

	printf("{B} = ");//B�迭 �ֱ�
	for (i = 0; i < n; i++) {
		scanf_s("%lf", &b[i]);
		fflush(stdin);
	}


/*Forward Elimination*/
	for (k = 0; k < n - 1; k++) {
		for (i = k+1; i < n; i++){
			factor = a[i][k] / a[k][k];
			//printf("factor = %lf\n", factor); //check
			for (j = k+1; j < n; j++) {
				a[i][j] = a[i][j] - factor*a[k][j];
				//printf("a[%d][%d] = %lf\n", i, j, a[i][j]); //check
			}
			b[i] = b[i] - factor*b[k];
			//printf("b[%d] = %lf\n\n", i, b[i]); //check
		}
	}	

	//Singular system���� Ȯ�� (���� ���ų� ���ѱ��� ������ ���)
	for (i = 0; i < n; i++) {
		if (a[i][i] == 0.00){
			singular = 1;
			printf("{X} = The system is singular!\n\n");
			return; //�������� �� �ʿ�����Ƿ� ����
		}
	}

/*Back Substitution*/
	x[n - 1] = b[n - 1] / a[n - 1][n - 1];
	//printf("x[%d] = %lf\n", n-1, x[n-1]); //check
	for (i = n - 2; i >= 0; i--) {
		sum = b[i];
		for (j = i+1; j < n; j++) {
			sum = sum - a[i][j] * x[j];
		}
		x[i] = sum / a[i][i];
		//printf("x[%d] = %lf\n\n", i, x[i]); //check
	}

	if (singular == -1) {
		printf("{X} = "); //x ���
		for (i = 0; i < n; i++)
			printf("%lf ", x[i]);
		printf("\n\n");
	}

	for (i = 0; i < n; i++) {
		free(a[i]);
	}
	free(a);
	free(b);
	free(x);
}