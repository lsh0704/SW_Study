#include <stdio.h>
#include <stdlib.h>

void Floyd(int n, int **d, int **w, int **p) {	// n=노드개수, d=reulst 배열, w=given 배열, p=vertex 배열
	int i, j, k;
	for (i = 0; i < n; i++) //-1은 not direct(infinite) 경로이므로 비교위해 매우 큰수로 바꿔줌
		for (j = 0; j < n; j++) {
			if (w[i][j]==-1) w[i][j] = 100000;
		}
	for (i = 0; i < n; i++) //initialize
		for (j = 0; j < n; j++) {
			d[i][j] = w[i][j];
			p[i][j] = 0;
		}
	for (k=0; k<n; k++)
		for (i=0; i<n; i++)
			for (j = 0; j < n; j++) 
					if (d[i][j] > d[i][k] + d[k][j]) {
						d[i][j] = d[i][k] + d[k][j];
						p[i][j] = k + 1;
					}
	for (i = 0; i < n; i++) //-1로 다시 바꿔주기
		for (j = 0; j < n; j++) {
			if (w[i][j] == 100000) w[i][j] = -1;
		}
}

void Path(int **p,int q,int r) {
	if (p[q][r] != 0) {
		Path(p,q, p[q][r]-1);
		printf("%d-", p[q][r]);
		Path(p,p[q][r]-1, r);
	}
}

main() 
{
	int n = 0; //node 개수
	int** w = NULL; /* adjacent matrix of edges with n strings (edges weights in a row)
		   w(i,j) > 0 for (i,j) edge, w(i,i) = 0, w(i,j) = -1 if (i,j)가 E(G)에 속하지 않을때*/
	int s = 0; //시작 노드
	int d = 0; //도착 노드
	int i, j;
	char c;
	int minus=0; //음수 표시 ('-' 읽으면 1, 평소에는 '0')

	int **r = NULL; //result 배열
	int **p = NULL; //vertext 배열

	scanf_s("%d", &n);	//노드 개수 입력

	w = (int**)malloc(sizeof(int*) * n);
	for (i = 0; i < n; i++)
		*(w+i) = (int*)malloc(sizeof(int)*n);	//w[i][j]

	for (i = 0; i < n; i++) {	//배열 입력
		for (j = 0; j < n;) {
			c = getchar();
			if (c == ',' || c == ' ' || c=='"');	//문자이므로 j count 취소
			else {
				if (c == '-') {
					minus = 1;
					c = getchar();
				}
				if (minus == 1) w[i][j] =-( c - '0');	//숫자인 문자를 받은 경우 -> 문자를 숫자로 변경
				else w[i][j] = c - '0';
				j++;
				minus = 0;
			}
		}
	}

	i = 0;
	while ((c = getchar()) != '\n') {	//s, d 입력
		if (c == ',' || c == ' ' || c == '"');
		else {
			if (i == 0) { s = c - '0'; i++; }
			else if (i==1) d = c - '0';
		}
	}

	r = (int**)malloc(sizeof(int*) * n);
	for (i = 0; i < n; i++)
		*(r + i) = (int*)malloc(sizeof(int)*n);	//r[i][j]
	p = (int**)malloc(sizeof(int*) * n);
	for (i = 0; i < n; i++)
		*(p + i) = (int*)malloc(sizeof(int)*n);	//p[i][j]

	Floyd(n, r, w, p);
	printf("\tD\n\t");
	for (i = 0; i < n; i++) {	/*D 출력*/
		for (j = 0; j < n; j++) {
			printf("%-4d", r[i][j]);
		}
		printf("\n\t");
	}
	printf("P\n\t");
	for (i = 0; i < n; i++) {	/*P 출력*/
		for (j = 0; j < n; j++) {
			printf("%-4d", p[i][j]);
		}
		printf("\n\t");
	}

	printf("%d-", s); //시작노드
	Path(p,s-1,d-1);
	printf("%d\n", d); //도착노드
	
	for (int i = 0; i < n; i++)	//동적 배열 해제
		free(w[i]);
	free(w);
}