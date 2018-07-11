#include <stdio.h>
#include <stdlib.h>

void Floyd(int n, int **d, int **w, int **p) {	// n=��尳��, d=reulst �迭, w=given �迭, p=vertex �迭
	int i, j, k;
	for (i = 0; i < n; i++) //-1�� not direct(infinite) ����̹Ƿ� ������ �ſ� ū���� �ٲ���
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
	for (i = 0; i < n; i++) //-1�� �ٽ� �ٲ��ֱ�
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
	int n = 0; //node ����
	int** w = NULL; /* adjacent matrix of edges with n strings (edges weights in a row)
		   w(i,j) > 0 for (i,j) edge, w(i,i) = 0, w(i,j) = -1 if (i,j)�� E(G)�� ������ ������*/
	int s = 0; //���� ���
	int d = 0; //���� ���
	int i, j;
	char c;
	int minus=0; //���� ǥ�� ('-' ������ 1, ��ҿ��� '0')

	int **r = NULL; //result �迭
	int **p = NULL; //vertext �迭

	scanf_s("%d", &n);	//��� ���� �Է�

	w = (int**)malloc(sizeof(int*) * n);
	for (i = 0; i < n; i++)
		*(w+i) = (int*)malloc(sizeof(int)*n);	//w[i][j]

	for (i = 0; i < n; i++) {	//�迭 �Է�
		for (j = 0; j < n;) {
			c = getchar();
			if (c == ',' || c == ' ' || c=='"');	//�����̹Ƿ� j count ���
			else {
				if (c == '-') {
					minus = 1;
					c = getchar();
				}
				if (minus == 1) w[i][j] =-( c - '0');	//������ ���ڸ� ���� ��� -> ���ڸ� ���ڷ� ����
				else w[i][j] = c - '0';
				j++;
				minus = 0;
			}
		}
	}

	i = 0;
	while ((c = getchar()) != '\n') {	//s, d �Է�
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
	for (i = 0; i < n; i++) {	/*D ���*/
		for (j = 0; j < n; j++) {
			printf("%-4d", r[i][j]);
		}
		printf("\n\t");
	}
	printf("P\n\t");
	for (i = 0; i < n; i++) {	/*P ���*/
		for (j = 0; j < n; j++) {
			printf("%-4d", p[i][j]);
		}
		printf("\n\t");
	}

	printf("%d-", s); //���۳��
	Path(p,s-1,d-1);
	printf("%d\n", d); //�������
	
	for (int i = 0; i < n; i++)	//���� �迭 ����
		free(w[i]);
	free(w);
}