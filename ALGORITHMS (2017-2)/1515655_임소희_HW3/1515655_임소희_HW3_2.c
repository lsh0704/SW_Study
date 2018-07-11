/*Hamiltonian cyle problems*/
#include <stdio.h>
#include <stdlib.h>

int bound(int k, int **g, int *x) {
	int i;
	for (i = 0; i < k; i++) {
		if (x[k] == x[i])
			return -1; //false
	}
	if (k == 0) return -1;
	if (g[x[k] - 1][x[k - 1] - 1]) return 1; //true
	else return -1; //false
}

int getnext(int m, int k, int **g, int *x, int *a) {
	int i;
	i = x[k];
	while (i < m) {
		x[k] = a[++i];
		if (bound(k, g, x) == 1) return i;
	}
	if (i == m) {
		x[k] = a[0];
	}
}

void backtrack(int n, int **g, int *x, int m, int *a) {
	int i = 0, j;
	int k = 1;
	int count = 0;
	x[0] = 1;	// 시작점 1로 고정

	while (0 <= k && k < n) {
		i = getnext(m, k, g, x, a);
		if (x[k] == a[0]) k--;
		else {
			if (k == n - 1 && x[0] == 1) //print_solution(x, n);
			{
				if (g[n - 1][0] && g[0][n - 1]) //마지막노드와 첫번째 노드가 연결되어있으면
				{
					for (j = 0; j < n; j++)
						printf("%d-", x[j]);
					printf("%d\n", x[0]);
					count++;
					return;
				}
			}
			else if (k != n - 1)
				k++;
		}
	}
	if (count == 0) printf("There is no HC\n");
}


main() {
	int n; // total number of nodes n in G
	int** g = NULL; /* adjacent matrix of edges with n strings (edges weights in a row)
					g(i,j) = 1 for (i,j) exists an edge, g(i,i) = 0 no edge*/
	int s = 0; //시작 노드
	int d = 0; //도착 노드
	int i, j;
	char c;
	int m; // m=n;
	int *a; // a[0]~a[m]
	int *x; // solution verctor

	while (1) {
		printf("input\n");
		scanf_s("%d", &n);
		if (n < 4 || n > 10) {
			printf("total number of nodes [4,10]\n\n");
			continue;
		}
		break;
	}

	g = (int**)malloc(sizeof(int*) * n);
	for (i = 0; i < n; i++)
		*(g + i) = (int*)malloc(sizeof(int)*n);	//g[i][j]

	for (i = 0; i < n; i++) {	//배열 입력
		for (j = 0; j < n;) {
			c = getchar();
			if (c == ' ' || c == '\n' || c == '\t');	//문자이므로 j count 취소
			else {
				g[i][j] = c - '0';	//숫자인 문자를 받은 경우 -> 문자를 숫자로 변경
				j++;
			}
		}
	}

	m = n;
	printf("\n\n");

	x = (int*)malloc(sizeof(int)*(n)); // solution vector (1-....-1)
	for (i = 0; i < n; i++)	// solution 배열 초기화
		x[i] = 0;

	a = (int*)malloc(sizeof(int)*(m + 1)); // a vector
	for (i = 0; i <= m; i++)	// a 배열 0~m
		a[i] = i;

	printf("output\n");
	backtrack(n, g, x, m, a);
}

