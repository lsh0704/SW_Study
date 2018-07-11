#include <stdio.h>

int M[1000] = { 000 }; /*메모리 : 한 주소에 3자리 숫자까지 저장(0~999)*/
int R[10] = { 000 }; /*레지스터 : 한 레지스터에 3자리 숫자까지 저장(R0~R9). 모든 레지스터는 000으로 초기화*/
int PC = 0; // 다음 실행할 명령어가 저장된 주소. 메모리 숫자(0~999) 카운트

//명령어는 세자리 숫자로 구성
int input(); /*입력기 : 입력 프로그램을 읽어서 메모리에 저장*/
int interpret(); /*해석기 : FETCH, DECODE, EXECUTE*/ //if else 문 이용

int ic=0; //input 횟수 카운트
int hun; //명령어 판별 시 사용 (백의 자리 수)
int ten; //십의 자리 수
int one; //일의 자리 수
int count=0; //명령어 실행 횟수
int j; //레지스터 카운트 (0~9)

/*결과 출력 : 실행된 명령어 횟수, 레지스터 값, 메모리의 변수 값(메모리가 가지는 값, 메모리 주소 값x)*/
main() {
	printf("# -1 입력 시, INPUT 종료\n\n");

	for (PC = 0; PC<1000; PC++) {
		input();
		fflush(stdin);
		if (M[PC]==-1) { //-1 입력 시 입력 종료
			break;
		}
		if (!(M[PC]>=0 && M[PC]<1000)) { //세자리 숫자가 아닌 수 입력 시, 경고문 띄우며 종료
			printf("*세자리 수(000~999)만 입력해주세요.\n");
			return -1;
		}
	}
	ic = PC;

	interpret();
}


int input() //입력기 
{
	scanf_s("%d", &M[PC]);	
	return M[PC];
}

int interpret() //해석기 (명령어 FETCH, DECODE, EXECUTE)
{
	int mp[10] = { -1 }; /* 변수를 위한 기억공간 넘버 (메모리 번호)
							변수 공간 존재 시, mp[0]부터 메모리 번호인 양수가 채워진다
							이는 변수 공간이 존재하는지 확인 할 수 있는 지표가 됨 */
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
		ten = ten / 10; // 십의 자리 추출
		one = M[PC] % 10; // 일의 자리 추출
		hun = M[PC] / 100; // 명령어 판별 위한 백의 자리 숫자 추출

		if (hun == 2) /*2dn : register d에 숫자 n을 set*/
		{
			R[ten] = one;
		}
		if (hun == 3) /*3dn : register d에 숫자 n을 add*/
		{
			R[ten] += one;
			R[ten] %= 1000; // overflow 방지위해 세자리로 계산 (보수 이용)
		}
		if (hun == 4) /*4dn : register d에 숫자 n을 multiply*/
		{
			R[ten] *= one;
			R[ten] %= 1000; // overflow 방지위해 세자리로 계산 (보수 이용)
		}

		if (hun == 5) /*5ds : register d에 register s의 값을 set*/
		{
			R[ten] = R[one];
		}
		if (hun == 6) /*6ds : register d에 register s의 값을 add*/
		{
			R[ten] += R[one];
			R[ten] %= 1000; // overflow 방지위해 세자리로 계산 (보수 이용)
		}
		if (hun == 7) /*7ds : register d에 register s의 값을 multiply*/
		{
			R[ten] *= R[one];
			R[ten] %= 1000; // overflow 방지위해 세자리로 계산 (보수 이용)
		}

		/*d : 데이터 레지스터, a : 주소 레지스터*/
		if (hun == 8) /*8da : 메모리 값을 레지스터에 적재(load)*/
		{
			R[ten] = M[R[one]];
		}

		if (hun == 9) /*9sa : 레지스터 값을 메모리에 저장(store)*/
		{
			M[R[one]] = R[ten];
		}

		if (hun == 0) /*0ds : register s가 0이 아니면 레지스터 d로 goto*/
		{
			if (R[one] != 0) {
				PC = R[ten]-1; ////pc값을 증가 시킬 예정이므로 1을 빼줘야 한다.
			}
		}

		if (hun == 1) { // 백의 자리가 1이나 100이 아닌 경우 정지 후 이전 pc로 옮김

			M[PC] = 100;
			PC--; // 한번 pc가 카운트 되었으므로 다시 100 연산 위해, 이전 pc로 되돌린다.
			count--; //count된거 취소
		}
		count++;
	}

	printf("\n실행된 명령어 횟수 : %d\n", count);
	printf("레지스터 값 : \n");
	for (j = 0; j < 10; j++) {
		printf("R[%d] = %d\t", j, R[j]);
	}

	printf("\n메모리의 변수값\n"); //메모리의 변수값(주 메모리에 저장되어있는 변수의 값)

	for (i = 0; mp[i] != 0; i++) { //변수를 위한 기억 공간 있는 경우
		if (mp[i] != -1)
			printf("M[%d] : %d\t", mp[i], M[mp[i]]);
	}
	printf("\n");
	
	return 0;	
} 
