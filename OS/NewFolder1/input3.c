// �Լ� g ����
function g(int x)
begin
(1 + 2 - 3 + x);  // 1, 2, 3�� ���ϰ� x�� ���� => x
end

// �Լ� f ����
function f(int a)
begin
int b = 1; // b�� 1 �Է�
int c = 2; // c�� 2 �Է�
if (b < (b + c))  // b�� b+c���� ������ Ȯ�� => 1 < 1 + 2 (��)
{
    ((b * c) + g(a));  // b�� c�� ���� ���� g(a)�� ���� => 1 * 2 + a = a + 2
}
end

// main �Լ� ����
function main()
begin
int a = 1;
int b = 2;
int c = 3;
((2 + f(c)) * a);  // 2�� f(c)�� ����� ���� �� a�� ���� => {2 + (3 + 2)} * 1 = 7
end
