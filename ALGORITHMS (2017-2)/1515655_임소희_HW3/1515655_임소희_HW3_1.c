/*Graph coloring problems*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int counter = 1; //�̹� ù��° ��忡 ������ �����̹Ƿ� vising node count=1�� ����
int pcount = 0; //print Ƚ��

int bound(int k, int **g, int *x) {
	int i;
	for (i = 0; i < k; i++) {
		if (x[k] == x[i] && g[k][i])	// ���� color && k���� i��尡 �̿� ����
			return -1;
	}
	return 1; //true
}

int getnext(int m, int k, int **g, int *x, int *a) {
	int i;
	i = x[k];
	while (i < m) {
		i++;
		x[k] = a[i];
		++counter;
		//printf("x[%d] : %d\n", k, a[i]);
		if (bound(k, g, x) == 1) return;
	}
	if (i == m) {
		x[k] = a[0];
		//printf("x[k] : ")
	}
}

void backtrack(int n, int **g, int *x, int m, int *a) {
	int i = 0, j;
	int k = 0;

	while (0 <= k && k < n) {
		getnext(m, k, g, x, a);
		if (x[k] == a[0]) k--;
		else {
			if (k == n - 1) //print_solution(x, n);
			{
				for (j = 0; j < n; j++)
					printf("%d ", x[j]);
				printf("\n");
				pcount++;
			}
			else  k++;
		}
	}
	if (pcount == 0) printf("No answer\n");
}


main() {
	int n; // total number of nodes n in G
	int** g = NULL; /* adjacent matrix of edges with n strings (edges weights in a row)
					g(i,j) = 1 for (i,j) exists an edge, g(i,i) = 0 no edge*/
	int s = 0; //���� ���
	int d = 0; //���� ���
	int i, j;
	char c;
	int m; // number of colors
	int *a; // a[0]~a[m]
	int *x; // solution verctor
	int total; // total nodes
	double percent; // total visits/total nodes * 100

	while (1) {
		printf("input\n");
		scanf_s("%d", &n);
		if (n < 3 || n > 10) {
			printf("total number of nodes [3,10]\n\n");
			continue;
		}
		break;
	}

	g = (int**)malloc(sizeof(int*) * n);
	for (i = 0; i < n; i++)
		*(g + i) = (int*)malloc(sizeof(int)*n);	//g[i][j]

	for (i = 0; i < n; i++) {	//�迭 �Է�
		for (j = 0; j < n;) {
			c = getchar();
			if (c == ' ' || c == '\n' || c == '\t');	//�����̹Ƿ� j count ���
			else {
				g[i][j] = c - '0';	//������ ���ڸ� ���� ��� -> ���ڸ� ���ڷ� ����
				j++;
			}
		}
	}

	scanf_s("%d", &m);
	printf("\n\n\n");

	x = (int*)malloc(sizeof(int)*n); // solution vector
	for (i = 0; i < n; i++)	// solution �迭 �ʱ�ȭ
		x[i] = 0;

	a = (int*)malloc(sizeof(int)*(m + 1)); // a vector
	for (i = 0; i <= m; i++)	// a �迭 0~m
		a[i] = i;

	printf("output\n");
	backtrack(n, g, x, m, a);

	if (pcount > 0) {
		total = (pow(m, n + 1) - 1) / (m - 1);
		percent = ((double)counter / total) * 100;
		printf("%d, %d, %0.f%%\n", counter, total, percent);
	}
}

