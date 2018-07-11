#include <stdio.h>
#include <stdlib.h>
#include <float.h> 
//isnan() 사용위해 (실수가 0.000이 되거나 오버플로우 확인)->이 코드에선 사용X

void calculate_ex();
void calculate_sinx();
void calculate_ln();
void Bisection();
void Newton_Raphson();
void Secant();
void Equation();

double factorial(int n); //factorial 계산
double pow(double x, int n);
double absolute(double n); //ea는 '크기'를 나타내야 하므로 절대값
double f(double a, double b, double c, double d, double x); //evaluate f(x)
double f_(double a, double b, double c, double x); // evaluate f'(x)

int i, j; //for문 돌릴 때 사용

main()
{
	printf("[ ID: 1515655 ]\n");
	printf("[ Name: 임소희 ]\n\n");

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
		scanf_s("%*c"); //fflush와 비슷한 기능(버퍼삭제)

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
			return 0; //종료
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
	double ea=0.0; //approximate relative error (근사상대오차) = (현재 근사값-이전 근사값 / 현재 근사값)*100
	double old_term, new_term; //이전 근사값, 반복시 새로 추가되는 값
	double xi = 1; // x^1, x^2, ... x^n 할 때 사용
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

		xi = 1; //xi 초기화
		ea = ((taylor_sum - old_term) / taylor_sum) * 100; // 근사상대오차 구함
		
		//printf("ea : %lf\n\n", ea);
		ea = absolute(ea); //ea 절대값 처리
		
		if (ea < 0.000001)
			break;
	}

	ex = taylor_sum; //근사답
	printf("e^%g = %lf\n\n", x, ex);//display taylor_sum
}
void calculate_sinx()
{
	double x; double sinx; //input, answer
	double ea; //approximate relative error (근사상대오차) = (현재 근사값-이전 근사값 / 현재 근사값)*100
	double old_term=0.0, new_term=0.0;  //이전 근사값, 반복시 새로 추가되는 값
	double xi = 1; // x^1, x^2, ... x^n 할 때 사용
	double taylor_sum = 0.0;

	printf("x = ");
	scanf_s("%lf", &x); //receive x
	scanf_s("%*c"); //buffer 삭제

	//x=0인 경우는 sin0=0

	if (x!=0) {
		for (i = 0, new_term = 0.0; ; i++) {
			old_term = taylor_sum;

			xi=pow(x, (2*i + 1));  //x^(2n+1) 구하기

			if (i % 2 != 0)
				xi = -xi; //i가 홀수번이면 (-1)^n이므로 음수처리

			new_term = (xi / factorial(2 * i + 1));

			if (new_term > DBL_MAX || new_term <= -DBL_MAX)
				break;

			taylor_sum += new_term;
		
			/*printf("i = %d\n", i);
			printf("new_term = %lf\n", new_term);
			printf("taylor_sum : %lf\n", taylor_sum);*/

			xi = 1; //xi 초기화

			ea = ((taylor_sum - old_term) / taylor_sum) * 100; // 근사상대오차 구함
			ea = absolute(ea); //ea 절대값 처리
			//printf("ea : %lf\n\n", ea);
			
			if (ea < 0.00001)
				break;
		}
	}

	sinx = taylor_sum; //근사답
	printf("sin %g = %lf\n\n", x, sinx);//display taylor_sum
}

void calculate_ln() 
{
	double x; double lnx; //input, answer
	double ea; //approximate relative error (근사상대오차) = (현재 근사값-이전 근사값 / 현재 근사값)*100
	double old_term = 0.0, new_term = 0.0; //이전 근사값, 현재 근사값
	double xi = 1; // x^1, x^2, ... x^n 할 때 사용

	printf("x = ");
	scanf_s("%lf", &x); //receive x

	if (x <= 0 || x>2)
		return -1; // 범위 밖의 수 입력 시 메뉴로

	if (x != 1) {
		for (i = 1, new_term = 0.0; ; i++) {
			if (i == 100000)
				printf("이 수는 계산 과정이 오래 걸립니다. 잠시만 기다려주세요.\n"); //x==2이면 i=288539가 되어야 답 도출됨.
			old_term = new_term;

			if (x != 1) 
				for (j = 1; j <= i; j++) { //x^n 구하기
					xi *= (x - 1);
				}

				if (i % 2 == 0)
					xi = -xi; //i가 홀수번이면 (-1)^n이므로 음수처리


				new_term += (xi / i);
				/*printf("i = %d\n", i);
				printf("새로 더한 값 = %lf\n", xi / i);
				printf("new_Term : %lf\n", new_term);*/

				xi = 1; //xi 초기화

				ea = ((new_term - old_term) / new_term) * 100; // 근사상대오차 구함
				ea = absolute(ea); //ea 절대값 처리
				//printf("ea : %lf\n\n", ea);

				if (i > 0 && ea < 0.0005)
					break;
		}
	}
	lnx = new_term; //근사답
	printf("ln %g = %lf\n\n", x, lnx);//display taylor_sum
}

void Bisection() //Find a root of ax^3 + bx^2 + cx + d = 0
{
	double a, b, c, d; //방정식 계수
	double xl, xu, es; //xl~xu 구간, es=엡실론s(stopping criterion)
	double r; //root
	int imax=50000; //반복 가능한 최대 횟수. i>=imax가 되면 함수종료
	double ea=1.0; /*approximate error. 반복문을 돌고 나온 값이 <ea되면 답으로 간주하고 함수종료
				  초기값 크게 설정한 이유는 반복문 돌아가게 하기위함. (ea<es)*/
	double fxl, fxr; /*f(xl), f(xr). 이렇게 해두면 나중에 중복계산 할 필요 없다.
					 xl   xr    xu 구간 중, 만약 f(xl)f(xr) <0이 나오면 xl~xr 구간에 근이 있음을 의미.
					 따라서, xr이 새로운 xu가 되고, xl은 그대로다.
					 xl을 fxl에 저장했으므로, f(xl)을 또 계산할 필요가 없다.*/
	//double xrold; // 이전 xr
	double xr; // (xl+xu)/2
	double test; // f(xl)*f(xr)


	printf("a = "); scanf_s("%lf", &a); 
	scanf_s("%*c"); /*다음에 scanf 사용하기 전, 버퍼 비우는 fflush와 유사한 역할
	                공백 등의 원치않는 input을 대신 받는 역할*/
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

	fxl = f(a, b, c, d, xl); //f(xl) 계산
	
	for (i = 0; ; i++) {
		//printf("\n%d번째\n", i);
		//printf("f(xl) = %lf\n", fxl);

		/*if (i!=0) // 처음 수행 시 xrold 없으므로, i=0일시 이 부분은 skip
		{
			xrold = xr;
			printf("xrold = %lf\n", xrold);
		}*/
		
		xr = (xl + xu) / 2;
		//printf("xr = %lf\n", xr);

		fxr = f(a, b, c, d, xr); //f(xr) 계산		
		//printf("f(xr) = %lf\n", fxr);


		/*if (xr)*/ {
			//ea = ((xr - xrold) / xr) * 100; //근사상대오차
			ea = ((xu - xl) / (xu + xl)) * 100; //근사상대오차
			ea = absolute(ea); //ea 절대값 처리
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
			ea = 0; // 사실상 반복문 빠져나오게 함. ea<es인데, es가 음수일리는 없기 때문!

		if (ea < es || i >= imax)
			break;
	}
	r = xr; //반복문에서 구한 근사값을 root에 대입
	printf("\nFound root = %lf\n\n", r);
} 

void Newton_Raphson() 
{
	double a, b, c, d; //방정식 계수
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

	xr = x0; //초기값
	for (i = 0; ; i++) {
		//printf("%d번째 수행\n", i);
		xrold = xr;
		//printf("xrold = %lf\n", xrold);

		xr = xrold-(f(a,b,c,d,xrold)/f_(a, b, c, xrold)); // xi+1 = xi - f(xi)/f'(xi)
		//printf("xr = %lf\n", xr);

		if (xr) {
			ea = absolute((xr - xrold) / xr) * 100; //근사상대오차
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
	double a, b, c, d; //방정식 계수
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

	xrold = x_1; //초기값
	xr = x0; //초기값
	for (i = 0; ; i++) {
		//printf("%d번째 수행\n", i);
		if (i) //i=0일때는 x_1가 xrold, x0가 xr이므로
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
			ea = absolute((xrnew - xr) / xrnew) * 100; //근사상대오차
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
	double factor; // 소거위해 곱해야 하는 수
	double sum = 0;
	int singular = -1; // singular system인지 확인 (-1:아님, 1:맞음)

	printf("n = "); scanf_s("%d", &n);
	scanf_s("%*c");

	/*A, B 집합 동적할당*/
	double **a = (double**)malloc(sizeof(double*)*n); // n*n matrix [A], 행
	double *b = (double*)malloc(sizeof(double)*n); // n*1 matrix {B}
	double *x = (double*)malloc(sizeof(double)*n); // answer, [A]{X}={B}

	for (i = 0; i < n; i++) { //n=row
		a[i] = (double*)malloc(sizeof(double)*n); // 렬, n=col
	}

	
	printf("[A] = "); //A배열 넣기
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			scanf_s("%lf", &a[i][j]);
			fflush(stdin);
		}
	}

	printf("{B} = ");//B배열 넣기
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

	//Singular system인지 확인 (근이 없거나 무한근이 나오는 경우)
	for (i = 0; i < n; i++) {
		if (a[i][i] == 0.00){
			singular = 1;
			printf("{X} = The system is singular!\n\n");
			return; //후진대입 할 필요없으므로 종료
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
		printf("{X} = "); //x 출력
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