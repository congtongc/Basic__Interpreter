/* Basic Interpreter by H?eyin Uslu raistlinthewiz@hotmail.com */
/* Code licenced under GPL */


#include <stdio.h>			// 표준 입출력 관련 기능
#include <conio.h>			// 콘솔 입출력 관련 기능
#include <string.h>			// 문자열 관련 함수들
#include <stdlib.h>			// 일반적인 유틸리티 함수들
#include <ctype.h>			// 문자 관련 함수들

struct node {				// 'Node' 라는 구조체를 정의
	int type;				// 노드의 데이터 타입 (1: 변수, 2: 함수, 3: 함수 호출, 4: 시작, 5: 끝)
	/* system stack-> 1 for variable, 2 for function , 3 for function call
	4 for begin 5 for end */

	char exp_data;  // 표현식 데이터
	int val;  // 값
	int line;  // 라인 번호
	struct node* next;  // 다음 노드를 가리키는 포인터

};

typedef struct node Node;	// typedef를 사용하여 struct node를 Node로 별칭

struct stack {			// 'Stack' 이라는 구조체를 정의
	Node* top;			// 스택의 맨 위 노드를 가리키는 포인터
};

typedef struct stack Stack;	// typedef를 사용하여 struct stack을 Stack으로 별칭


struct opnode {				// 'opNode' 라는 구조체를 정의
	char op;				// 연산자
	struct opnode* next;	// 다음 노드를 가리키는 포인터
};
typedef struct opnode opNode;	// typedef를 사용하여 struct opnode를 opNode로 별칭

struct opstack {			// 'OpStack' 이라는 구조체를 정의
	opNode* top;			// 연산자 스택의 맨 위 노드를 가리키는 포인터
};

typedef struct opstack OpStack;	// typedef를 사용하여 struct opstack을 opStack으로 별칭

struct postfixnode {
	int val;				// 값
	struct postfixnode* next;	// 다음 노드를 가리키는 포인터
};

typedef struct postfixnode Postfixnode;	// typedef를 사용하여 postfixnode를 Postfixnode로 별칭

struct postfixstack {			// 'PostfixStack' 이라는 구조체를 정의
	Postfixnode* top;			// 후위 표기법 스택의 맨 위 노드를 가리키는 포인터
};

typedef struct postfixstack PostfixStack;	// typedef를 사용하여 postfixstack을 PostfixStack으로 별칭

int GetVal(char, int*, Stack*);				// GetVal 함수 정의
int GetLastFunctionCall(Stack*);			// GetLastFunctionCall 함수 정의

Stack* FreeAll(Stack*);						// 전달받은 스택을 초기화하고, 스택에 있는 모든 노드의 메모리를 해제 

Stack* Push(Node sNode, Stack* stck)	// Push 함수는 스택에 새로운 노드를 추가
{
	Node* newnode;							

	if ((newnode = (Node*)malloc(sizeof(Node))) == NULL) {	// malloc을 사용하여 새로운 노드에 메모리를 할당
		printf("ERROR, Couldn't allocate memory...");		// 할당이 실패하면 오류를 출력 
		return NULL;										// 함수를 종료
	}
	else
	{
		newnode->type = sNode.type; // 새로운 노드의 type 멤버에 sNode의 type 값을 복사
		newnode->val = sNode.val;	// 새로운 노드의 val 멤버에 sNode의 val 값을 복사
		newnode->exp_data = sNode.exp_data;	// 새로운 노드의 exp_data 멤버에 sNode의 exp_data 값을 복사
		newnode->line = sNode.line; // 새로운 노드의 line 멤버에 sNode의 line 값을 복사
		newnode->next = stck->top; // 새로운 노드의 next 멤버에 현재 스택의 맨 위에 있는 노드를 가리키는 포인터를 설정
		stck->top = newnode;	// 스택의 top 포인터를 새로 추가된 노드를 가리키도록 업데이트
		return stck;	// 업데이트된 스택을 반환
	}
}

OpStack* PushOp(char op, OpStack* opstck)	// PushOp 함수는 연산자 스택에 새로운 연산자 노드를 추가
{
	opNode* newnode;			
	if ((newnode = (opNode*)malloc(sizeof(opNode))) == NULL) {	// malloc을 사용하여 새로운 연산자 노드에 메모리를 할당
		printf("ERROR, Couldn't allocate memory...");			// 할당이 실패하면 오류를 출력하고 함수를 종료
		return NULL;
	}
	else
	{

		newnode->op = op; // 새로운 연산자 노드의 op 멤버에 전달받은 연산자(op)를 복사
		newnode->next = opstck->top; // 새로운 연산자 노드의 next 멤버에 현재 연산자 스택의 맨 위에 있는 노드를 가리키는 포인터를 설정
		opstck->top = newnode; // 연산자 스택의 top 포인터를 새로 추가된 연산자 노드를 가리키도록 업데이트
		return opstck; // 업데이트된 연산자 스택을 반환
	}
}

char PopOp(OpStack* opstck)	// PopOp 함수는 연산자 스택에서 연산자를 꺼냄
{
	opNode* temp;					
	char op;
	if (opstck->top == NULL)		// 스택이 비어있으면 
	{
		printf("ERROR, empty stack...");	// 오류를 출력하고 함수를 종료
	}
	else
	{
		op = opstck->top->op; // 연산자를 저장
		temp = opstck->top; // 연산자 노드를 임시로 저장
		opstck->top = opstck->top->next; // 연산자 스택의 top 포인터를 다음 노드를 가리키도록 업데이트
		free(temp); // 임시로 저장한 연산자 노드의 메모리를 해제
		return op; // 꺼내온 연산자를 반환
	}
	return NULL; // 연산자 스택이 비어있으면 NULL 반환
}


PostfixStack* PushPostfix(int val, PostfixStack* poststck) // PushPostfix 함수는 후위 표기법 스택에 값을 추가
{
	Postfixnode* newnode;				
	if ((newnode = (Postfixnode*)malloc(sizeof(Postfixnode))) == NULL) {	// malloc을 사용하여 새로운 후위 표기법 노드에 메모리를 할당
		printf("ERROR, Couldn't allocate memory...");		// 할당이 실패하면 오류를 출력
		return NULL;										// 함수를 종료
	}
	else
	{
		newnode->val = val; // 새로운 후위 표기법 노드의 val 멤버에 전달받은 값(val)을 복사
		newnode->next = poststck->top; // 새로운 후위 표기법 노드의 next 멤버에 현재 후위 표기법 스택의 맨 위에 있는 노드를 가리키는 포인터를 설정
		poststck->top = newnode; // 후위 표기법 스택의 top 포인터를 새로 추가된 후위 표기법 노드를 가리키도록 업데이트
		return poststck; // 업데이트된 후위 표기법 스택을 반환
	}
}

char PopPostfix(PostfixStack* poststck) // PopPostfix 함수는 후위 표기법 스택에서 값을 꺼냄
{
	Postfixnode* temp;			
	int val;
	if (poststck->top == NULL)	// 스택이 비어있으면 
	{
		printf("ERROR, empty stack..."); // 오류를 출력하고 함수를 종료
	}
	else
	{ 
		val = poststck->top->val; // 값을 저장 
		temp = poststck->top; // 후위 표기법 노드를 임시로 저장
		poststck->top = poststck->top->next; // 후위 표기법 스택의 top 포인터를 다음 노드를 가리키도록 업데이트
		free(temp); // 임시로 저장한 후위 표기법 노드의 메모리를 해제
		return val; // 꺼내온 값을 반환
	}
	return NULL; // 후위 표기법 스택이 비어있으면 NULL 반환
}

void Pop(Node* sNode, Stack* stck) // Pop 함수는 스택에서 값을 꺼냄
{
	Node* temp;			

	if (stck->top == NULL) // 스택이 비어있으면
	{
		printf("ERROR, empty stack...");	// 오류를 출력하고 함수를 종료
	}
	else
	{
		sNode->exp_data = stck->top->exp_data; // 현재 스택의 맨 위에 있는 노드의 exp_data 값을 sNode의 exp_data 멤버에 복사
		sNode->type = stck->top->type; // 현재 스택의 맨 위에 있는 노드의 type 값을 sNode의 type 멤버에 복사
		sNode->line = stck->top->line; // 현재 스택의 맨 위에 있는 노드의 line 값을 sNode의 line 멤버에 복사
		sNode->val = stck->top->val; // 현재 스택의 맨 위에 있는 노드의 val 값을 sNode의 val 멤버에 복사
		temp = stck->top; // 현재 스택의 top 포인터를 다음 노드를 가리키도록 업데이트
		stck->top = stck->top->next; // 다음 노드로 이동
		free(temp); // 사용한 노드의 메모리를 해제
	}
}

int isStackEmpty(OpStack* stck)
{						// isStackEmpty 함수는 연산자 스택이 비어있는지 확인
	if (stck->top == 0)	// 스택이 비어있으면 
		return 1;		// 1을 반환
	return 0;			// 그렇지 않으면 0을 반환
}

/*void printAllStack(Stack* stck) {
    Node tempNode;

    // 헤더 출력
    printf("\n------------------\n");
    printf("Dumping the stack...\n");

    // 스택이 비어있지 않은 동안 반복
    // 1. 스택의 맨 위에 있는 노드를 꺼내와서 tempNode에 저장
    // 2. tempNode의 내용을 출력 (exp_data, type, val, line)
    // 3. 스택의 top 포인터를 다음 노드를 가리키도록 업데이트
    Pop(&tempNode, stck);
    printf("exp=%c type=%d val=%d line=%d\n", tempNode.exp_data, tempNode.type, tempNode.val, tempNode.line);

    // 스택이 비어있지 않은 동안 반복하여 스택의 내용을 출력
    while (stck->top != NULL) {
        // 1. 스택의 맨 위에 있는 노드를 꺼내와서 tempNode에 저장
        // 2. tempNode의 내용을 출력 (exp_data, type, val, line)
        // 3. 스택의 top 포인터를 다음 노드를 가리키도록 업데이트
        Pop(&tempNode, stck);
        printf("exp=%c type=%d val=%d line=%d\n", tempNode.exp_data, tempNode.type, tempNode.val, tempNode.line);
    }
} */

int Priotry(char operator) {
	// 주어진 연산자에 따라 우선순위를 반환
	if ((operator == '+') || (operator == '-'))
		return 1;  // 덧셈 또는 뺄셈 연산자인 경우 우선순위 1 반환
	else if ((operator == '/') || (operator == '*'))
		return 2;  // 곱셈 또는 나눗셈 연산자인 경우 우선순위 2 반환
	return 0;  // 위 조건에 해당하지 않는 경우 기본값인 0 반환
}

int main(int argc, char** argv)
{
	char line[4096];            // 문자열을 저장할 배열
	char dummy[4096];           // 임시로 사용되는 문자열을 저장할 배열
	char lineyedek[4096];        // 복사된 문자열을 저장할 배열

	char postfix[4096];          // 후위 표기식을 저장할 배열

	char* firstword;             // 문자열에서 첫 번째 단어를 가리키는 포인터

	int val1;                    // 정수 값 1
	int val2;                    // 정수 값 2

	int LastExpReturn;           // 최근 표현식의 반환 값
	int LastFunctionReturn = -999;  // 최근 함수 호출의 반환 값 (기본값: -999)
	int CalingFunctionArgVal;    // 함수 호출 중 전달된 인수의 값

	Node tempNode;               // 노드를 임시로 저장할 변수

	OpStack* MathStack;          // 수학적 계산에 사용되는 연산자 스택
	FILE* filePtr;               // 파일 포인터

	PostfixStack* CalcStack;     // 후위 표기식 계산에 사용되는 스택
	int resultVal;               // 계산 결과 값

	Stack* STACK;                // 일반적인 데이터를 저장하는 스택

	int curLine = 0;             // 현재 처리 중인 라인 번호
	int foundMain = 0;           // main 함수를 찾았는지 여부 (기본값: 0)
	int WillBreak = 0;           // 루프에서 탈출할지 여부 (기본값: 0)

	MathStack->top = NULL;       // 수학적 계산에 사용되는 연산자 스택 초기화
	CalcStack->top = NULL;       // 후위 표기식 계산에 사용되는 스택 초기화
	STACK->top = NULL;           // 일반적인 데이터를 저장하는 스택 초기화

	clrscr();                    // 화면을 지움

	if (argc != 2)
	{
		// 인자의 개수가 2가 아닌 경우
		printf("Incorrect arguments!\n");
		printf("Usage: %s <inputfile.spl>", argv[0]);
		return 1;  // 프로그램 종료, 반환 코드 1
	}

	// 파일을 열었을 때
	if ((filePtr = fopen(argv[1], "r")) == NULL)
	{
		// 파일을 열 수 없는 경우
		printf("Can't open %s. Check the file please", argv[1]);
		return 2;  // 프로그램 종료, 반환 코드 2
	}

	while (!feof(filePtr)) // 파일의 끝에 도달할 때까지 반복하는 루프
	{
		int k = 0;  // 반복문에서 사용할 카운터 변수 초기화

		fgets(line, 4096, filePtr);  // 파일에서 한 줄씩 읽어오기

		// 읽어온 줄에서 탭 문자를 스캔하여 공백으로 대체
		while (line[k] != '\0')
		{
			if (line[k] == '\t')
			{
				line[k] = ' ';  // 탭 문자를 공백으로 변경
			}
			k++;
		}

		strcpy(lineyedek, line);  // 읽어온 줄을 복사하여 다른 배열에 저장

		curLine++;  // 현재 처리 중인 라인 번호 증가
		tempNode.val = -999;  // 임시 노드의 값 초기화
		tempNode.exp_data = ' ';  // 임시 노드의 표현 데이터 초기화
		tempNode.line = -999;  // 임시 노드의 라인 번호 초기화
		tempNode.type = -999;  // 임시 노드의 타입 초기화

		if (!strcmpi("begin\n", line) | !strcmpi("begin", line))
		{
			// "begin" 또는 "begin\n"과 현재 읽은 줄이 같은 경우
			if (foundMain)
			{
				tempNode.type = 4;          // tempNode의 타입을 4로 설정 (begin 표시)
				STACK = Push(tempNode, STACK);  // 스택에 tempNode를 푸시
			}
		}

		else if (!strcmpi("end\n", line) | !strcmpi("end", line))
		{
			if (foundMain) // "end" 또는 "end\n"과 현재 읽은 줄이 같은 경우
			{
				int sline;  // 함수 호출이 발생한 라인 번호를 저장할 변수

				tempNode.type = 5;          // tempNode의 타입을 5로 설정 (end 표시)
				STACK = Push(tempNode, STACK);  // 스택에 tempNode를 푸시

				sline = GetLastFunctionCall(STACK);  // 스택에서 마지막 함수 호출이 발생한 라인 번호를 얻음

				if (sline == 0)
				{
					printf("Output=%d", LastExpReturn); // 마지막 함수 호출이 없으면 결과를 출력
				}
				else
				{
					int j;  // 반복문 변수
					int foundCall = 0;  // 함수 호출을 찾았는지 여부를 나타내는 변수
					LastFunctionReturn = LastExpReturn;  // 마지막 함수 호출의 결과를 저장

					// 파일을 닫고 다시 열어서 처음부터 읽기 시작
					fclose(filePtr);
					filePtr = fopen(argv[1], "r");
					curLine = 0;

					for (j = 1; j < sline; j++) // 함수 호출이 발생한 라인까지 이동
					{
						fgets(dummy, 4096, filePtr);  // 파일에서 한 줄씩 읽어옴
						curLine++;
					}

					while (foundCall == 0) // 스택에서 마지막 함수 호출까지 모두 제거
					{
						Pop(&tempNode, STACK);
						if (tempNode.type == 3)  // 함수 호출을 찾으면
						{
							foundCall = 1;  // foundCall을 1로 설정하여 반복문 종료
						}
					}
				}
			}
		}
		else
		{
			// 'else' 블록: 'end' 또는 'end\n'와 같은 경우가 아닌 경우
			
			firstword = strtok(line, " "); // 토크나이징을 수행
			if (!strcmpi("int", firstword)) // 'int' 키워드가 나오면
			{
				if (foundMain) // foundMain이 참이고(main 함수 안에 있을 때)
				{
					tempNode.type = 1;  // 변수의 타입을 1로 설정 (정수)
					firstword = strtok(NULL, " ");  // 다음 토큰을 가져옴
					tempNode.exp_data = firstword[0];  // 토큰의 첫 번째 문자를 변수의 exp_data에 설정
					firstword = strtok(NULL, " ");  // 다음 토큰을 가져옴

					if (!strcmpi("=", firstword)) // '=' 기호가 있는지 확인
					{
						firstword = strtok(NULL, " ");  // 다음 토큰을 가져옴
					}
					tempNode.val = atoi(firstword); // 변수의 값을 정수로 변환하여 설정
					tempNode.line = 0;  // 라인 정보를 0으로 설정
					STACK = Push(tempNode, STACK);  // 스택에 노드를 푸시
				}
			}
			else if (!strcmpi("function", firstword))
			{
				// 'function' 키워드가 나온 경우
				tempNode.type = 2;  // 함수의 타입을 2로 설정
				firstword = strtok(NULL, " ");  // 다음 토큰을 가져옴
				tempNode.exp_data = firstword[0];  // 토큰의 첫 번째 문자를 함수의 exp_data에 설정
				tempNode.line = curLine;  // 함수의 라인 정보를 설정
				tempNode.val = 0;  // 함수의 값은 0으로 설정
				STACK = Push(tempNode, STACK);  // 스택에 함수 정보를 푸시

				// 만약 함수가 'main' 함수라면
				if ((firstword[0] == 'm') & (firstword[1] == 'a') & (firstword[2] == 'i') & (firstword[3] == 'n'))
				{
					foundMain = 1; // 'main' 함수를 찾았으므로 foundMain을 1로 설정
				}
				else
				{
					if (foundMain) // 'main' 함수가 아니라면
					{
						firstword = strtok(NULL, " ");  // 다음 토큰을 가져옴
						tempNode.type = 1;  // 변수의 타입을 1로 설정 (정수)
						tempNode.exp_data = firstword[0];  // 토큰의 첫 번째 문자를 변수의 exp_data에 설정
						tempNode.val = CalingFunctionArgVal;  // 함수 호출의 인자 값을 변수의 값으로 설정
						tempNode.line = 0;  // 변수의 라인 정보를 0으로 설정
						STACK = Push(tempNode, STACK);  // 스택에 변수 정보를 푸시
					}
				}
			}
			else if (firstword[0] == '(')
			{
				// '('가 나온 경우
				if (foundMain)
				{
					// 'main' 함수 안에서 '('가 나온 경우
					int i = 0;
					int y = 0;
					MathStack->top = NULL;  // 수식의 계산을 위한 스택 초기화

					// 주어진 수식을 후위 표기법으로 계산하기 위한 과정
					while (lineyedek[i] != '\x0')
					{
						// 수식을 평가하여 후위 표기법으로 변환
						if (isdigit(lineyedek[i]))
						{
							// 현재 문자가 숫자인 경우, 후위 표기법 수식에 추가
							postfix[y] = lineyedek[i];
							y++;
						}
						else if (lineyedek[i] == ')') // 현재 문자가 ')'인 경우
						{
							if (!isStackEmpty(MathStack)) // MathStack에 원소가 있는 경우
							{
								postfix[y] = PopOp(MathStack); // 수식 스택에서 연산자를 pop하여
								y++;	// 후위 표기법 수식에 추가
							}
						}
						else if ((lineyedek[i] == '+') | (lineyedek[i] == '-') | (lineyedek[i] == '*') | (lineyedek[i] == '/'))
						{
							// 현재 문자가 연산자인 경우
							/*operators*/
							if (isStackEmpty(MathStack))
							{
								// 스택이 비어있으면 현재 연산자를 스택에 push
								MathStack = PushOp(lineyedek[i], MathStack);
							}
							else
							{
								// 스택이 비어있지 않은 경우
								/* check for precedence (연산자 우선순위 확인) */
								if (Priotry(lineyedek[i]) <= Priotry(MathStack->top->op))
								{
									// 현재 연산자의 우선순위가 스택 맨 위 연산자보다 낮거나 같으면
									// 스택 맨 위의 연산자를 pop하여 postfix에 추가
									postfix[y] = PopOp(MathStack);
									y++;
									// 현재 연산자를 스택에 push
									MathStack = PushOp(lineyedek[i], MathStack);
								}
								else
								{
									// 현재 연산자의 우선순위가 스택 맨 위 연산자보다 높으면
									// 현재 연산자를 스택에 push
									MathStack = PushOp(lineyedek[i], MathStack);
								}
							}
						}
						else if (isalpha(lineyedek[i]) > 0)
						{
							// 현재 문자가 알파벳인 경우 (변수 또는 함수 호출)
							int codeline = 0;
							int dummyint = 0;
							// 변수 또는 함수 호출 여부 확인
							int retVal = 0;
							// 현재 문자(lineyedek[i])에 대한 변수 또는 함수 호출 여부 확인
							retVal = GetVal(lineyedek[i], &codeline, STACK);

							if ((retVal != -1) & (retVal != -999)) // 만약 변수라면
							{
								postfix[y] = retVal + 48; // 변수 값을 postfix에 추가
								y++;
							}
							else
							{
								// 만약 함수 호출이고, 직전 함수 호출의 결과가 아직 없는 경우
								if (LastFunctionReturn == -999)
								{
									int j; // 반복문에서 사용할 변수 j 선언
									tempNode.type = 3; // 노드의 타입을 3으로 설정 (함수 호출)
									tempNode.line = curLine; // 노드의 라인 정보를 현재 라인으로 설정
									STACK = Push(tempNode, STACK); // 스택에 노드를 푸시

									CalingFunctionArgVal = GetVal(lineyedek[i + 2], &dummyint, STACK); // 함수 호출의 인자 값 계산
									fclose(filePtr); // 파일 포인터 닫기
									filePtr = fopen(argv[1], "r"); // 파일을 다시 읽기 모드로 열기
									curLine = 0; // 현재 라인 정보 초기화

									for (j = 1; j < codeline; j++)
									{
										fgets(dummy, 4096, filePtr); // 파일을 라인 단위로 읽음
										curLine++;	// 현재 라인 번호(curLine)를 1 증가
									}
									WillBreak = 1; // 반복문 종료를 위해 WillBreak 변수를 1로 설정
									break; // 반복문 종료

								}
								else
								{
									// 만약 함수 호출이고, 직전 함수 호출의 결과가 있는 경우
									// 직전 함수 호출의 결과를 postfix에 추가
									postfix[y] = LastFunctionReturn + 48; /* ASCII 테이블에서 숫자 값은 48부터 시작합니다. */
									y++;                   // postfix 배열의 다음 위치로 이동
									i = i + 3;             // 함수 호출 표현식의 끝으로 이동
									LastFunctionReturn = -999;  // 함수 호출이 끝났으므로 LastFunctionReturn 초기화
								}
							}
						}

						i++; // 다음 위치로 이동
					}
					// 함수 실행을 중단하지 않아야 하는지 확인
					if (WillBreak == 0) {
						// MathStack이 비어 있지 않은 동안 반복
						while (isStackEmpty(MathStack) == 0) {
							postfix[y] = PopOp(MathStack); // MathStack에서 연산자를 꺼내 postfix 표현식에 추가
							y++; // postfix 배열의 다음 위치로 이동
						}
						postfix[y] = '\0';  // postfix 표현식을 널 문자로 종료
						i = 0;  // postfix 표현식 순회를 위한 인덱스 초기화
						CalcStack->top = NULL;  // postfix 표현식 계산을 위한 스택 초기화
						// postfix 배열의 현재 위치에서 널 문자('\x0')를 만날 때까지 반복
						while (postfix[i] != '\x0') {
							// 현재 위치의 문자가 숫자인지 확인
							if (isdigit(postfix[i])) {
								// 숫자인 경우, 해당 숫자를 스택에 푸시
								CalcStack = PushPostfix(postfix[i] - '0', CalcStack);
							}
							// 현재 위치의 문자가 연산자(+, -, *, /)인 경우
							else if ((postfix[i] == '+') | (postfix[i] == '-') | (postfix[i] == '*') | (postfix[i] == '/')) {
								// 두 피연산자를 팝하고 연산을 수행한 뒤 결과를 다시 CalcStack에 푸시
								// 현재 위치의 연산자에 따라 연산을 수행할 두 피연산자를 팝
								val1 = PopPostfix(CalcStack);
								val2 = PopPostfix(CalcStack);
								// 현재 위치의 연산자에 따라 연산 수행 및 결과를 resultVal에 저장
								switch (postfix[i]) {
								case '+': resultVal = val2 + val1; break;
								case '-': resultVal = val2 - val1; break;
								case '/': resultVal = val2 / val1; break;
								case '*': resultVal = val2 * val1; break;
								}
								// 결과를 다시 CalcStack에 푸시
								CalcStack = PushPostfix(resultVal, CalcStack);
								// 다음 위치로 이동
								i++;
						}
						// 식의 결과를 LastExpReturn에 저장
						LastExpReturn = CalcStack->top->val;
					}
					WillBreak = 0;  // 다음 반복을 위해 WillBreak 플래그 초기화
				}
			}
		}
	}

	fclose(filePtr); // 파일 포인터를 닫음
	//printAllStack(STACK);
	STACK = FreeAll(STACK);
	// 스택에 남아 있는 모든 노드를 해제하고 스택 자체를 해제
	printf("\nPress a key to exit...");
	getch();
	// 메시지를 출력하고 키 입력을 대기하며 프로그램 종료
	return 0;
}

Stack* FreeAll(Stack* stck)
{
	Node* temp;
	Node* head;

	if (stck->top != NULL) // 스택이 비어있지 않은 경우에만 수행
	{	
		head = stck->top; // head를 스택의 맨 위로 설정
		do // head가 NULL이 아닌 동안 반복
		{
			temp = head; // temp에 현재 head를 저장		
			head = head->next; // head를 다음 노드로 이동
			free(temp); // temp가 가리키는 노드를 해제 (메모리 반환)
		} while (head->next != NULL);  // head의 다음 노드가 NULL이 아닌 동안 반복
	}
	return NULL;  // 스택이 비어있는 경우 NULL을 반환
}

int GetLastFunctionCall(Stack* stck)
{
	Node* head;
	// 스택이 비어있으면 오류 메시지 출력
	if (stck->top == NULL)
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		head = stck->top;	// head를 스택의 맨 위로 설정
		do  // head가 NULL이 아닌 동안 반복
		{	
			if (head->type == 3)
			{	
				return head->line; // 현재 노드의 type이 3 (함수)인 경우 해당 함수의 라인 번호를 반환
			}
			else
			{
				head = head->next; // 다음 노드로 이동
			}
		} while (head->next != NULL); // head의 다음 노드가 NULL이 아닌 동안 반복
	}
	return 0; // 스택이 비어있거나 함수 노드를 찾지 못한 경우 0을 반환
}

int GetVal(char exp_name, int* line, Stack* stck)
{
	Node* head;   // Node 타입의 포인터 head 선언
	*line = 0;    // line 변수를 0으로 초기화
	if (stck->top == NULL) // 스택이 비어있을 때 오류 출력
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		head = stck->top;  // 스택의 top에 위치한 노드를 head에 할당

		do
		{
			// 노드의 exp_data가 찾고자 하는 exp_name과 같은 경우
			if (head->exp_data == exp_name)
			{
				if (head->type == 1) // 변수일 경우
				{
					return head->val; // 변수의 값을 반환
				}
				else if (head->type == 2) // 함수일 경우
				{
					*line = head->line;  // 인자로 받은 포인터 변수 line에 현재 노드의 line 필드 값을 할당
					return -1;  // 현재 노드가 함수를 나타내는 노드인 경우, -1을 반환
				}
			}
			else
			{
				head = head->next;  // 다음 노드로 이동
			}
		} while (head->next != NULL);  // head의 다음 노드가 NULL이 아닌 동안 반복

		// 마지막 노드까지 검사 후 한번 더 체크
		if (head->exp_data == exp_name)
		{
			if (head->type == 1) // 변수일 경우
			{
				return head->val; // 변수의 값을 반환
			}
			else if (head->type == 2) // 함수일 경우
			{
				*line = head->line;  // 함수의 라인 정보를 line 변수에 저장
				return -1;  // 함수 호출이므로 -1을 반환
			}
		}
	}
	// 스택이 비어있거나 해당하는 이름을 찾을 수 없는 경우 -999 반환
	return -999;
}