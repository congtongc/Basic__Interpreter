/* Basic Interpreter by H?eyin Uslu raistlinthewiz@hotmail.com */
/* Code licenced under GPL */


#include <stdio.h>			// ǥ�� ����� ���� ���
#include <conio.h>			// �ܼ� ����� ���� ���
#include <string.h>			// ���ڿ� ���� �Լ���
#include <stdlib.h>			// �Ϲ����� ��ƿ��Ƽ �Լ���
#include <ctype.h>			// ���� ���� �Լ���

struct node {				// 'Node' ��� ����ü�� ����
	int type;				// ����� ������ Ÿ�� (1: ����, 2: �Լ�, 3: �Լ� ȣ��, 4: ����, 5: ��)
	/* system stack-> 1 for variable, 2 for function , 3 for function call
	4 for begin 5 for end */

	char exp_data;  // ǥ���� ������
	int val;  // ��
	int line;  // ���� ��ȣ
	struct node* next;  // ���� ��带 ����Ű�� ������

};

typedef struct node Node;	// typedef�� ����Ͽ� struct node�� Node�� ��Ī

struct stack {			// 'Stack' �̶�� ����ü�� ����
	Node* top;			// ������ �� �� ��带 ����Ű�� ������
};

typedef struct stack Stack;	// typedef�� ����Ͽ� struct stack�� Stack���� ��Ī


struct opnode {				// 'opNode' ��� ����ü�� ����
	char op;				// ������
	struct opnode* next;	// ���� ��带 ����Ű�� ������
};
typedef struct opnode opNode;	// typedef�� ����Ͽ� struct opnode�� opNode�� ��Ī

struct opstack {			// 'OpStack' �̶�� ����ü�� ����
	opNode* top;			// ������ ������ �� �� ��带 ����Ű�� ������
};

typedef struct opstack OpStack;	// typedef�� ����Ͽ� struct opstack�� opStack���� ��Ī

struct postfixnode {
	int val;				// ��
	struct postfixnode* next;	// ���� ��带 ����Ű�� ������
};

typedef struct postfixnode Postfixnode;	// typedef�� ����Ͽ� postfixnode�� Postfixnode�� ��Ī

struct postfixstack {			// 'PostfixStack' �̶�� ����ü�� ����
	Postfixnode* top;			// ���� ǥ��� ������ �� �� ��带 ����Ű�� ������
};

typedef struct postfixstack PostfixStack;	// typedef�� ����Ͽ� postfixstack�� PostfixStack���� ��Ī

int GetVal(char, int*, Stack*);				// GetVal �Լ� ����
int GetLastFunctionCall(Stack*);			// GetLastFunctionCall �Լ� ����

Stack* FreeAll(Stack*);						// ���޹��� ������ �ʱ�ȭ�ϰ�, ���ÿ� �ִ� ��� ����� �޸𸮸� ���� 

Stack* Push(Node sNode, Stack* stck)	// Push �Լ��� ���ÿ� ���ο� ��带 �߰�
{
	Node* newnode;							

	if ((newnode = (Node*)malloc(sizeof(Node))) == NULL) {	// malloc�� ����Ͽ� ���ο� ��忡 �޸𸮸� �Ҵ�
		printf("ERROR, Couldn't allocate memory...");		// �Ҵ��� �����ϸ� ������ ��� 
		return NULL;										// �Լ��� ����
	}
	else
	{
		newnode->type = sNode.type; // ���ο� ����� type ����� sNode�� type ���� ����
		newnode->val = sNode.val;	// ���ο� ����� val ����� sNode�� val ���� ����
		newnode->exp_data = sNode.exp_data;	// ���ο� ����� exp_data ����� sNode�� exp_data ���� ����
		newnode->line = sNode.line; // ���ο� ����� line ����� sNode�� line ���� ����
		newnode->next = stck->top; // ���ο� ����� next ����� ���� ������ �� ���� �ִ� ��带 ����Ű�� �����͸� ����
		stck->top = newnode;	// ������ top �����͸� ���� �߰��� ��带 ����Ű���� ������Ʈ
		return stck;	// ������Ʈ�� ������ ��ȯ
	}
}

OpStack* PushOp(char op, OpStack* opstck)	// PushOp �Լ��� ������ ���ÿ� ���ο� ������ ��带 �߰�
{
	opNode* newnode;			
	if ((newnode = (opNode*)malloc(sizeof(opNode))) == NULL) {	// malloc�� ����Ͽ� ���ο� ������ ��忡 �޸𸮸� �Ҵ�
		printf("ERROR, Couldn't allocate memory...");			// �Ҵ��� �����ϸ� ������ ����ϰ� �Լ��� ����
		return NULL;
	}
	else
	{

		newnode->op = op; // ���ο� ������ ����� op ����� ���޹��� ������(op)�� ����
		newnode->next = opstck->top; // ���ο� ������ ����� next ����� ���� ������ ������ �� ���� �ִ� ��带 ����Ű�� �����͸� ����
		opstck->top = newnode; // ������ ������ top �����͸� ���� �߰��� ������ ��带 ����Ű���� ������Ʈ
		return opstck; // ������Ʈ�� ������ ������ ��ȯ
	}
}

char PopOp(OpStack* opstck)	// PopOp �Լ��� ������ ���ÿ��� �����ڸ� ����
{
	opNode* temp;					
	char op;
	if (opstck->top == NULL)		// ������ ��������� 
	{
		printf("ERROR, empty stack...");	// ������ ����ϰ� �Լ��� ����
	}
	else
	{
		op = opstck->top->op; // �����ڸ� ����
		temp = opstck->top; // ������ ��带 �ӽ÷� ����
		opstck->top = opstck->top->next; // ������ ������ top �����͸� ���� ��带 ����Ű���� ������Ʈ
		free(temp); // �ӽ÷� ������ ������ ����� �޸𸮸� ����
		return op; // ������ �����ڸ� ��ȯ
	}
	return NULL; // ������ ������ ��������� NULL ��ȯ
}


PostfixStack* PushPostfix(int val, PostfixStack* poststck) // PushPostfix �Լ��� ���� ǥ��� ���ÿ� ���� �߰�
{
	Postfixnode* newnode;				
	if ((newnode = (Postfixnode*)malloc(sizeof(Postfixnode))) == NULL) {	// malloc�� ����Ͽ� ���ο� ���� ǥ��� ��忡 �޸𸮸� �Ҵ�
		printf("ERROR, Couldn't allocate memory...");		// �Ҵ��� �����ϸ� ������ ���
		return NULL;										// �Լ��� ����
	}
	else
	{
		newnode->val = val; // ���ο� ���� ǥ��� ����� val ����� ���޹��� ��(val)�� ����
		newnode->next = poststck->top; // ���ο� ���� ǥ��� ����� next ����� ���� ���� ǥ��� ������ �� ���� �ִ� ��带 ����Ű�� �����͸� ����
		poststck->top = newnode; // ���� ǥ��� ������ top �����͸� ���� �߰��� ���� ǥ��� ��带 ����Ű���� ������Ʈ
		return poststck; // ������Ʈ�� ���� ǥ��� ������ ��ȯ
	}
}

char PopPostfix(PostfixStack* poststck) // PopPostfix �Լ��� ���� ǥ��� ���ÿ��� ���� ����
{
	Postfixnode* temp;			
	int val;
	if (poststck->top == NULL)	// ������ ��������� 
	{
		printf("ERROR, empty stack..."); // ������ ����ϰ� �Լ��� ����
	}
	else
	{ 
		val = poststck->top->val; // ���� ���� 
		temp = poststck->top; // ���� ǥ��� ��带 �ӽ÷� ����
		poststck->top = poststck->top->next; // ���� ǥ��� ������ top �����͸� ���� ��带 ����Ű���� ������Ʈ
		free(temp); // �ӽ÷� ������ ���� ǥ��� ����� �޸𸮸� ����
		return val; // ������ ���� ��ȯ
	}
	return NULL; // ���� ǥ��� ������ ��������� NULL ��ȯ
}

void Pop(Node* sNode, Stack* stck) // Pop �Լ��� ���ÿ��� ���� ����
{
	Node* temp;			

	if (stck->top == NULL) // ������ ���������
	{
		printf("ERROR, empty stack...");	// ������ ����ϰ� �Լ��� ����
	}
	else
	{
		sNode->exp_data = stck->top->exp_data; // ���� ������ �� ���� �ִ� ����� exp_data ���� sNode�� exp_data ����� ����
		sNode->type = stck->top->type; // ���� ������ �� ���� �ִ� ����� type ���� sNode�� type ����� ����
		sNode->line = stck->top->line; // ���� ������ �� ���� �ִ� ����� line ���� sNode�� line ����� ����
		sNode->val = stck->top->val; // ���� ������ �� ���� �ִ� ����� val ���� sNode�� val ����� ����
		temp = stck->top; // ���� ������ top �����͸� ���� ��带 ����Ű���� ������Ʈ
		stck->top = stck->top->next; // ���� ���� �̵�
		free(temp); // ����� ����� �޸𸮸� ����
	}
}

int isStackEmpty(OpStack* stck)
{						// isStackEmpty �Լ��� ������ ������ ����ִ��� Ȯ��
	if (stck->top == 0)	// ������ ��������� 
		return 1;		// 1�� ��ȯ
	return 0;			// �׷��� ������ 0�� ��ȯ
}

/*void printAllStack(Stack* stck) {
    Node tempNode;

    // ��� ���
    printf("\n------------------\n");
    printf("Dumping the stack...\n");

    // ������ ������� ���� ���� �ݺ�
    // 1. ������ �� ���� �ִ� ��带 �����ͼ� tempNode�� ����
    // 2. tempNode�� ������ ��� (exp_data, type, val, line)
    // 3. ������ top �����͸� ���� ��带 ����Ű���� ������Ʈ
    Pop(&tempNode, stck);
    printf("exp=%c type=%d val=%d line=%d\n", tempNode.exp_data, tempNode.type, tempNode.val, tempNode.line);

    // ������ ������� ���� ���� �ݺ��Ͽ� ������ ������ ���
    while (stck->top != NULL) {
        // 1. ������ �� ���� �ִ� ��带 �����ͼ� tempNode�� ����
        // 2. tempNode�� ������ ��� (exp_data, type, val, line)
        // 3. ������ top �����͸� ���� ��带 ����Ű���� ������Ʈ
        Pop(&tempNode, stck);
        printf("exp=%c type=%d val=%d line=%d\n", tempNode.exp_data, tempNode.type, tempNode.val, tempNode.line);
    }
} */

int Priotry(char operator) {
	// �־��� �����ڿ� ���� �켱������ ��ȯ
	if ((operator == '+') || (operator == '-'))
		return 1;  // ���� �Ǵ� ���� �������� ��� �켱���� 1 ��ȯ
	else if ((operator == '/') || (operator == '*'))
		return 2;  // ���� �Ǵ� ������ �������� ��� �켱���� 2 ��ȯ
	return 0;  // �� ���ǿ� �ش����� �ʴ� ��� �⺻���� 0 ��ȯ
}

int main(int argc, char** argv)
{
	char line[4096];            // ���ڿ��� ������ �迭
	char dummy[4096];           // �ӽ÷� ���Ǵ� ���ڿ��� ������ �迭
	char lineyedek[4096];        // ����� ���ڿ��� ������ �迭

	char postfix[4096];          // ���� ǥ����� ������ �迭

	char* firstword;             // ���ڿ����� ù ��° �ܾ ����Ű�� ������

	int val1;                    // ���� �� 1
	int val2;                    // ���� �� 2

	int LastExpReturn;           // �ֱ� ǥ������ ��ȯ ��
	int LastFunctionReturn = -999;  // �ֱ� �Լ� ȣ���� ��ȯ �� (�⺻��: -999)
	int CalingFunctionArgVal;    // �Լ� ȣ�� �� ���޵� �μ��� ��

	Node tempNode;               // ��带 �ӽ÷� ������ ����

	OpStack* MathStack;          // ������ ��꿡 ���Ǵ� ������ ����
	FILE* filePtr;               // ���� ������

	PostfixStack* CalcStack;     // ���� ǥ��� ��꿡 ���Ǵ� ����
	int resultVal;               // ��� ��� ��

	Stack* STACK;                // �Ϲ����� �����͸� �����ϴ� ����

	int curLine = 0;             // ���� ó�� ���� ���� ��ȣ
	int foundMain = 0;           // main �Լ��� ã�Ҵ��� ���� (�⺻��: 0)
	int WillBreak = 0;           // �������� Ż������ ���� (�⺻��: 0)

	MathStack->top = NULL;       // ������ ��꿡 ���Ǵ� ������ ���� �ʱ�ȭ
	CalcStack->top = NULL;       // ���� ǥ��� ��꿡 ���Ǵ� ���� �ʱ�ȭ
	STACK->top = NULL;           // �Ϲ����� �����͸� �����ϴ� ���� �ʱ�ȭ

	clrscr();                    // ȭ���� ����

	if (argc != 2)
	{
		// ������ ������ 2�� �ƴ� ���
		printf("Incorrect arguments!\n");
		printf("Usage: %s <inputfile.spl>", argv[0]);
		return 1;  // ���α׷� ����, ��ȯ �ڵ� 1
	}

	// ������ ������ ��
	if ((filePtr = fopen(argv[1], "r")) == NULL)
	{
		// ������ �� �� ���� ���
		printf("Can't open %s. Check the file please", argv[1]);
		return 2;  // ���α׷� ����, ��ȯ �ڵ� 2
	}

	while (!feof(filePtr)) // ������ ���� ������ ������ �ݺ��ϴ� ����
	{
		int k = 0;  // �ݺ������� ����� ī���� ���� �ʱ�ȭ

		fgets(line, 4096, filePtr);  // ���Ͽ��� �� �پ� �о����

		// �о�� �ٿ��� �� ���ڸ� ��ĵ�Ͽ� �������� ��ü
		while (line[k] != '\0')
		{
			if (line[k] == '\t')
			{
				line[k] = ' ';  // �� ���ڸ� �������� ����
			}
			k++;
		}

		strcpy(lineyedek, line);  // �о�� ���� �����Ͽ� �ٸ� �迭�� ����

		curLine++;  // ���� ó�� ���� ���� ��ȣ ����
		tempNode.val = -999;  // �ӽ� ����� �� �ʱ�ȭ
		tempNode.exp_data = ' ';  // �ӽ� ����� ǥ�� ������ �ʱ�ȭ
		tempNode.line = -999;  // �ӽ� ����� ���� ��ȣ �ʱ�ȭ
		tempNode.type = -999;  // �ӽ� ����� Ÿ�� �ʱ�ȭ

		if (!strcmpi("begin\n", line) | !strcmpi("begin", line))
		{
			// "begin" �Ǵ� "begin\n"�� ���� ���� ���� ���� ���
			if (foundMain)
			{
				tempNode.type = 4;          // tempNode�� Ÿ���� 4�� ���� (begin ǥ��)
				STACK = Push(tempNode, STACK);  // ���ÿ� tempNode�� Ǫ��
			}
		}

		else if (!strcmpi("end\n", line) | !strcmpi("end", line))
		{
			if (foundMain) // "end" �Ǵ� "end\n"�� ���� ���� ���� ���� ���
			{
				int sline;  // �Լ� ȣ���� �߻��� ���� ��ȣ�� ������ ����

				tempNode.type = 5;          // tempNode�� Ÿ���� 5�� ���� (end ǥ��)
				STACK = Push(tempNode, STACK);  // ���ÿ� tempNode�� Ǫ��

				sline = GetLastFunctionCall(STACK);  // ���ÿ��� ������ �Լ� ȣ���� �߻��� ���� ��ȣ�� ����

				if (sline == 0)
				{
					printf("Output=%d", LastExpReturn); // ������ �Լ� ȣ���� ������ ����� ���
				}
				else
				{
					int j;  // �ݺ��� ����
					int foundCall = 0;  // �Լ� ȣ���� ã�Ҵ��� ���θ� ��Ÿ���� ����
					LastFunctionReturn = LastExpReturn;  // ������ �Լ� ȣ���� ����� ����

					// ������ �ݰ� �ٽ� ��� ó������ �б� ����
					fclose(filePtr);
					filePtr = fopen(argv[1], "r");
					curLine = 0;

					for (j = 1; j < sline; j++) // �Լ� ȣ���� �߻��� ���α��� �̵�
					{
						fgets(dummy, 4096, filePtr);  // ���Ͽ��� �� �پ� �о��
						curLine++;
					}

					while (foundCall == 0) // ���ÿ��� ������ �Լ� ȣ����� ��� ����
					{
						Pop(&tempNode, STACK);
						if (tempNode.type == 3)  // �Լ� ȣ���� ã����
						{
							foundCall = 1;  // foundCall�� 1�� �����Ͽ� �ݺ��� ����
						}
					}
				}
			}
		}
		else
		{
			// 'else' ���: 'end' �Ǵ� 'end\n'�� ���� ��찡 �ƴ� ���
			
			firstword = strtok(line, " "); // ��ũ����¡�� ����
			if (!strcmpi("int", firstword)) // 'int' Ű���尡 ������
			{
				if (foundMain) // foundMain�� ���̰�(main �Լ� �ȿ� ���� ��)
				{
					tempNode.type = 1;  // ������ Ÿ���� 1�� ���� (����)
					firstword = strtok(NULL, " ");  // ���� ��ū�� ������
					tempNode.exp_data = firstword[0];  // ��ū�� ù ��° ���ڸ� ������ exp_data�� ����
					firstword = strtok(NULL, " ");  // ���� ��ū�� ������

					if (!strcmpi("=", firstword)) // '=' ��ȣ�� �ִ��� Ȯ��
					{
						firstword = strtok(NULL, " ");  // ���� ��ū�� ������
					}
					tempNode.val = atoi(firstword); // ������ ���� ������ ��ȯ�Ͽ� ����
					tempNode.line = 0;  // ���� ������ 0���� ����
					STACK = Push(tempNode, STACK);  // ���ÿ� ��带 Ǫ��
				}
			}
			else if (!strcmpi("function", firstword))
			{
				// 'function' Ű���尡 ���� ���
				tempNode.type = 2;  // �Լ��� Ÿ���� 2�� ����
				firstword = strtok(NULL, " ");  // ���� ��ū�� ������
				tempNode.exp_data = firstword[0];  // ��ū�� ù ��° ���ڸ� �Լ��� exp_data�� ����
				tempNode.line = curLine;  // �Լ��� ���� ������ ����
				tempNode.val = 0;  // �Լ��� ���� 0���� ����
				STACK = Push(tempNode, STACK);  // ���ÿ� �Լ� ������ Ǫ��

				// ���� �Լ��� 'main' �Լ����
				if ((firstword[0] == 'm') & (firstword[1] == 'a') & (firstword[2] == 'i') & (firstword[3] == 'n'))
				{
					foundMain = 1; // 'main' �Լ��� ã�����Ƿ� foundMain�� 1�� ����
				}
				else
				{
					if (foundMain) // 'main' �Լ��� �ƴ϶��
					{
						firstword = strtok(NULL, " ");  // ���� ��ū�� ������
						tempNode.type = 1;  // ������ Ÿ���� 1�� ���� (����)
						tempNode.exp_data = firstword[0];  // ��ū�� ù ��° ���ڸ� ������ exp_data�� ����
						tempNode.val = CalingFunctionArgVal;  // �Լ� ȣ���� ���� ���� ������ ������ ����
						tempNode.line = 0;  // ������ ���� ������ 0���� ����
						STACK = Push(tempNode, STACK);  // ���ÿ� ���� ������ Ǫ��
					}
				}
			}
			else if (firstword[0] == '(')
			{
				// '('�� ���� ���
				if (foundMain)
				{
					// 'main' �Լ� �ȿ��� '('�� ���� ���
					int i = 0;
					int y = 0;
					MathStack->top = NULL;  // ������ ����� ���� ���� �ʱ�ȭ

					// �־��� ������ ���� ǥ������� ����ϱ� ���� ����
					while (lineyedek[i] != '\x0')
					{
						// ������ ���Ͽ� ���� ǥ������� ��ȯ
						if (isdigit(lineyedek[i]))
						{
							// ���� ���ڰ� ������ ���, ���� ǥ��� ���Ŀ� �߰�
							postfix[y] = lineyedek[i];
							y++;
						}
						else if (lineyedek[i] == ')') // ���� ���ڰ� ')'�� ���
						{
							if (!isStackEmpty(MathStack)) // MathStack�� ���Ұ� �ִ� ���
							{
								postfix[y] = PopOp(MathStack); // ���� ���ÿ��� �����ڸ� pop�Ͽ�
								y++;	// ���� ǥ��� ���Ŀ� �߰�
							}
						}
						else if ((lineyedek[i] == '+') | (lineyedek[i] == '-') | (lineyedek[i] == '*') | (lineyedek[i] == '/'))
						{
							// ���� ���ڰ� �������� ���
							/*operators*/
							if (isStackEmpty(MathStack))
							{
								// ������ ��������� ���� �����ڸ� ���ÿ� push
								MathStack = PushOp(lineyedek[i], MathStack);
							}
							else
							{
								// ������ ������� ���� ���
								/* check for precedence (������ �켱���� Ȯ��) */
								if (Priotry(lineyedek[i]) <= Priotry(MathStack->top->op))
								{
									// ���� �������� �켱������ ���� �� �� �����ں��� ���ų� ������
									// ���� �� ���� �����ڸ� pop�Ͽ� postfix�� �߰�
									postfix[y] = PopOp(MathStack);
									y++;
									// ���� �����ڸ� ���ÿ� push
									MathStack = PushOp(lineyedek[i], MathStack);
								}
								else
								{
									// ���� �������� �켱������ ���� �� �� �����ں��� ������
									// ���� �����ڸ� ���ÿ� push
									MathStack = PushOp(lineyedek[i], MathStack);
								}
							}
						}
						else if (isalpha(lineyedek[i]) > 0)
						{
							// ���� ���ڰ� ���ĺ��� ��� (���� �Ǵ� �Լ� ȣ��)
							int codeline = 0;
							int dummyint = 0;
							// ���� �Ǵ� �Լ� ȣ�� ���� Ȯ��
							int retVal = 0;
							// ���� ����(lineyedek[i])�� ���� ���� �Ǵ� �Լ� ȣ�� ���� Ȯ��
							retVal = GetVal(lineyedek[i], &codeline, STACK);

							if ((retVal != -1) & (retVal != -999)) // ���� �������
							{
								postfix[y] = retVal + 48; // ���� ���� postfix�� �߰�
								y++;
							}
							else
							{
								// ���� �Լ� ȣ���̰�, ���� �Լ� ȣ���� ����� ���� ���� ���
								if (LastFunctionReturn == -999)
								{
									int j; // �ݺ������� ����� ���� j ����
									tempNode.type = 3; // ����� Ÿ���� 3���� ���� (�Լ� ȣ��)
									tempNode.line = curLine; // ����� ���� ������ ���� �������� ����
									STACK = Push(tempNode, STACK); // ���ÿ� ��带 Ǫ��

									CalingFunctionArgVal = GetVal(lineyedek[i + 2], &dummyint, STACK); // �Լ� ȣ���� ���� �� ���
									fclose(filePtr); // ���� ������ �ݱ�
									filePtr = fopen(argv[1], "r"); // ������ �ٽ� �б� ���� ����
									curLine = 0; // ���� ���� ���� �ʱ�ȭ

									for (j = 1; j < codeline; j++)
									{
										fgets(dummy, 4096, filePtr); // ������ ���� ������ ����
										curLine++;	// ���� ���� ��ȣ(curLine)�� 1 ����
									}
									WillBreak = 1; // �ݺ��� ���Ḧ ���� WillBreak ������ 1�� ����
									break; // �ݺ��� ����

								}
								else
								{
									// ���� �Լ� ȣ���̰�, ���� �Լ� ȣ���� ����� �ִ� ���
									// ���� �Լ� ȣ���� ����� postfix�� �߰�
									postfix[y] = LastFunctionReturn + 48; /* ASCII ���̺��� ���� ���� 48���� �����մϴ�. */
									y++;                   // postfix �迭�� ���� ��ġ�� �̵�
									i = i + 3;             // �Լ� ȣ�� ǥ������ ������ �̵�
									LastFunctionReturn = -999;  // �Լ� ȣ���� �������Ƿ� LastFunctionReturn �ʱ�ȭ
								}
							}
						}

						i++; // ���� ��ġ�� �̵�
					}
					// �Լ� ������ �ߴ����� �ʾƾ� �ϴ��� Ȯ��
					if (WillBreak == 0) {
						// MathStack�� ��� ���� ���� ���� �ݺ�
						while (isStackEmpty(MathStack) == 0) {
							postfix[y] = PopOp(MathStack); // MathStack���� �����ڸ� ���� postfix ǥ���Ŀ� �߰�
							y++; // postfix �迭�� ���� ��ġ�� �̵�
						}
						postfix[y] = '\0';  // postfix ǥ������ �� ���ڷ� ����
						i = 0;  // postfix ǥ���� ��ȸ�� ���� �ε��� �ʱ�ȭ
						CalcStack->top = NULL;  // postfix ǥ���� ����� ���� ���� �ʱ�ȭ
						// postfix �迭�� ���� ��ġ���� �� ����('\x0')�� ���� ������ �ݺ�
						while (postfix[i] != '\x0') {
							// ���� ��ġ�� ���ڰ� �������� Ȯ��
							if (isdigit(postfix[i])) {
								// ������ ���, �ش� ���ڸ� ���ÿ� Ǫ��
								CalcStack = PushPostfix(postfix[i] - '0', CalcStack);
							}
							// ���� ��ġ�� ���ڰ� ������(+, -, *, /)�� ���
							else if ((postfix[i] == '+') | (postfix[i] == '-') | (postfix[i] == '*') | (postfix[i] == '/')) {
								// �� �ǿ����ڸ� ���ϰ� ������ ������ �� ����� �ٽ� CalcStack�� Ǫ��
								// ���� ��ġ�� �����ڿ� ���� ������ ������ �� �ǿ����ڸ� ��
								val1 = PopPostfix(CalcStack);
								val2 = PopPostfix(CalcStack);
								// ���� ��ġ�� �����ڿ� ���� ���� ���� �� ����� resultVal�� ����
								switch (postfix[i]) {
								case '+': resultVal = val2 + val1; break;
								case '-': resultVal = val2 - val1; break;
								case '/': resultVal = val2 / val1; break;
								case '*': resultVal = val2 * val1; break;
								}
								// ����� �ٽ� CalcStack�� Ǫ��
								CalcStack = PushPostfix(resultVal, CalcStack);
								// ���� ��ġ�� �̵�
								i++;
						}
						// ���� ����� LastExpReturn�� ����
						LastExpReturn = CalcStack->top->val;
					}
					WillBreak = 0;  // ���� �ݺ��� ���� WillBreak �÷��� �ʱ�ȭ
				}
			}
		}
	}

	fclose(filePtr); // ���� �����͸� ����
	//printAllStack(STACK);
	STACK = FreeAll(STACK);
	// ���ÿ� ���� �ִ� ��� ��带 �����ϰ� ���� ��ü�� ����
	printf("\nPress a key to exit...");
	getch();
	// �޽����� ����ϰ� Ű �Է��� ����ϸ� ���α׷� ����
	return 0;
}

Stack* FreeAll(Stack* stck)
{
	Node* temp;
	Node* head;

	if (stck->top != NULL) // ������ ������� ���� ��쿡�� ����
	{	
		head = stck->top; // head�� ������ �� ���� ����
		do // head�� NULL�� �ƴ� ���� �ݺ�
		{
			temp = head; // temp�� ���� head�� ����		
			head = head->next; // head�� ���� ���� �̵�
			free(temp); // temp�� ����Ű�� ��带 ���� (�޸� ��ȯ)
		} while (head->next != NULL);  // head�� ���� ��尡 NULL�� �ƴ� ���� �ݺ�
	}
	return NULL;  // ������ ����ִ� ��� NULL�� ��ȯ
}

int GetLastFunctionCall(Stack* stck)
{
	Node* head;
	// ������ ��������� ���� �޽��� ���
	if (stck->top == NULL)
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		head = stck->top;	// head�� ������ �� ���� ����
		do  // head�� NULL�� �ƴ� ���� �ݺ�
		{	
			if (head->type == 3)
			{	
				return head->line; // ���� ����� type�� 3 (�Լ�)�� ��� �ش� �Լ��� ���� ��ȣ�� ��ȯ
			}
			else
			{
				head = head->next; // ���� ���� �̵�
			}
		} while (head->next != NULL); // head�� ���� ��尡 NULL�� �ƴ� ���� �ݺ�
	}
	return 0; // ������ ����ְų� �Լ� ��带 ã�� ���� ��� 0�� ��ȯ
}

int GetVal(char exp_name, int* line, Stack* stck)
{
	Node* head;   // Node Ÿ���� ������ head ����
	*line = 0;    // line ������ 0���� �ʱ�ȭ
	if (stck->top == NULL) // ������ ������� �� ���� ���
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		head = stck->top;  // ������ top�� ��ġ�� ��带 head�� �Ҵ�

		do
		{
			// ����� exp_data�� ã���� �ϴ� exp_name�� ���� ���
			if (head->exp_data == exp_name)
			{
				if (head->type == 1) // ������ ���
				{
					return head->val; // ������ ���� ��ȯ
				}
				else if (head->type == 2) // �Լ��� ���
				{
					*line = head->line;  // ���ڷ� ���� ������ ���� line�� ���� ����� line �ʵ� ���� �Ҵ�
					return -1;  // ���� ��尡 �Լ��� ��Ÿ���� ����� ���, -1�� ��ȯ
				}
			}
			else
			{
				head = head->next;  // ���� ���� �̵�
			}
		} while (head->next != NULL);  // head�� ���� ��尡 NULL�� �ƴ� ���� �ݺ�

		// ������ ������ �˻� �� �ѹ� �� üũ
		if (head->exp_data == exp_name)
		{
			if (head->type == 1) // ������ ���
			{
				return head->val; // ������ ���� ��ȯ
			}
			else if (head->type == 2) // �Լ��� ���
			{
				*line = head->line;  // �Լ��� ���� ������ line ������ ����
				return -1;  // �Լ� ȣ���̹Ƿ� -1�� ��ȯ
			}
		}
	}
	// ������ ����ְų� �ش��ϴ� �̸��� ã�� �� ���� ��� -999 ��ȯ
	return -999;
}