// �Լ� g ����
function g(int x)
begin
(1 + 2 - 3 + x);  // 1, 2, 3�� ���ϰ� x�� ����
end

// �Լ� f ����
function f(int a)
begin
int b = 1;		// b�� 1 �Է�
int c = 2;		// c�� 2 �Է�
((b* c) + g(a));  // b�� c�� ���� ���� g(a)�� ����
end

// main �Լ� ����
function main()
begin
int a = 1;		// a�� 1 �Է�
int b = 2;		// b�� 2 �Է�
int c = 3;		// c�� 3 �Է�
((2 + f(c))* a);  // 2�� f(c)�� ����� ���� �� a�� ����
end
