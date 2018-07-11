#include <stdio.h>

int M[1000] = { 000 }; /*�޸� : �� �ּҿ� 3�ڸ� ���ڱ��� ����(0~999)*/
int R[10] = { 000 }; /*�������� : �� �������Ϳ� 3�ڸ� ���ڱ��� ����(R0~R9). ��� �������ʹ� 000���� �ʱ�ȭ*/
int PC = 0; // ���� ������ ��ɾ ����� �ּ�. �޸� ����(0~999) ī��Ʈ

//��ɾ�� ���ڸ� ���ڷ� ����
int input(); /*�Է±� : �Է� ���α׷��� �о �޸𸮿� ����*/
int interpret(); /*�ؼ��� : FETCH, DECODE, EXECUTE*/ //if else �� �̿�

int ic=0; //input Ƚ�� ī��Ʈ
int hun; //��ɾ� �Ǻ� �� ��� (���� �ڸ� ��)
int ten; //���� �ڸ� ��
int one; //���� �ڸ� ��
int count=0; //��ɾ� ���� Ƚ��
int j; //�������� ī��Ʈ (0~9)

/*��� ��� : ����� ��ɾ� Ƚ��, �������� ��, �޸��� ���� ��(�޸𸮰� ������ ��, �޸� �ּ� ��x)*/
main() {
	printf("# -1 �Է� ��, INPUT ����\n\n");

	for (PC = 0; PC<1000; PC++) {
		input();
		fflush(stdin);
		if (M[PC]==-1) { //-1 �Է� �� �Է� ����
			break;
		}
		if (!(M[PC]>=0 && M[PC]<1000)) { //���ڸ� ���ڰ� �ƴ� �� �Է� ��, ��� ���� ����
			printf("*���ڸ� ��(000~999)�� �Է����ּ���.\n");
			return -1;
		}
	}
	ic = PC;

	interpret();
}


int input() //�Է±� 
{
	scanf_s("%d", &M[PC]);	
	return M[PC];
}

int interpret() //�ؼ��� (��ɾ� FETCH, DECODE, EXECUTE)
{
	int mp[10] = { -1 }; /* ������ ���� ������ �ѹ� (�޸� ��ȣ)
							���� ���� ���� ��, mp[0]���� �޸� ��ȣ�� ����� ä������
							�̴� ���� ������ �����ϴ��� Ȯ�� �� �� �ִ� ��ǥ�� �� */
	int i;

	for (i=0, PC = 0; PC < ic; PC++) {
		if (M[PC] == 0) {
			mp[i] = PC;
			i++;
		}
	}

	for (PC = 0; PC < ic; PC++) {

		if (M[PC] == 100) { // 100 : halt
			count++;
			break;
		}

		ten = M[PC] % 100;
		ten = ten / 10; // ���� �ڸ� ����
		one = M[PC] % 10; // ���� �ڸ� ����
		hun = M[PC] / 100; // ��ɾ� �Ǻ� ���� ���� �ڸ� ���� ����

		if (hun == 2) /*2dn : register d�� ���� n�� set*/
		{
			R[ten] = one;
		}
		if (hun == 3) /*3dn : register d�� ���� n�� add*/
		{
			R[ten] += one;
			R[ten] %= 1000; // overflow �������� ���ڸ��� ��� (���� �̿�)
		}
		if (hun == 4) /*4dn : register d�� ���� n�� multiply*/
		{
			R[ten] *= one;
			R[ten] %= 1000; // overflow �������� ���ڸ��� ��� (���� �̿�)
		}

		if (hun == 5) /*5ds : register d�� register s�� ���� set*/
		{
			R[ten] = R[one];
		}
		if (hun == 6) /*6ds : register d�� register s�� ���� add*/
		{
			R[ten] += R[one];
			R[ten] %= 1000; // overflow �������� ���ڸ��� ��� (���� �̿�)
		}
		if (hun == 7) /*7ds : register d�� register s�� ���� multiply*/
		{
			R[ten] *= R[one];
			R[ten] %= 1000; // overflow �������� ���ڸ��� ��� (���� �̿�)
		}

		/*d : ������ ��������, a : �ּ� ��������*/
		if (hun == 8) /*8da : �޸� ���� �������Ϳ� ����(load)*/
		{
			R[ten] = M[R[one]];
		}

		if (hun == 9) /*9sa : �������� ���� �޸𸮿� ����(store)*/
		{
			M[R[one]] = R[ten];
		}

		if (hun == 0) /*0ds : register s�� 0�� �ƴϸ� �������� d�� goto*/
		{
			if (R[one] != 0) {
				PC = R[ten]-1; ////pc���� ���� ��ų �����̹Ƿ� 1�� ����� �Ѵ�.
			}
		}

		if (hun == 1) { // ���� �ڸ��� 1�̳� 100�� �ƴ� ��� ���� �� ���� pc�� �ű�

			M[PC] = 100;
			PC--; // �ѹ� pc�� ī��Ʈ �Ǿ����Ƿ� �ٽ� 100 ���� ����, ���� pc�� �ǵ�����.
			count--; //count�Ȱ� ���
		}
		count++;
	}

	printf("\n����� ��ɾ� Ƚ�� : %d\n", count);
	printf("�������� �� : \n");
	for (j = 0; j < 10; j++) {
		printf("R[%d] = %d\t", j, R[j]);
	}

	printf("\n�޸��� ������\n"); //�޸��� ������(�� �޸𸮿� ����Ǿ��ִ� ������ ��)

	for (i = 0; mp[i] != 0; i++) { //������ ���� ��� ���� �ִ� ���
		if (mp[i] != -1)
			printf("M[%d] : %d\t", mp[i], M[mp[i]]);
	}
	printf("\n");
	
	return 0;	
} 
