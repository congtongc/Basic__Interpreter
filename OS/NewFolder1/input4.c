// �Լ� g ����
function g(int y)
begin
(1 + 2 - 3 + y);  // 1, 2, 3�� ���ϰ� y�� ���� => y
end

// �Լ� f ����
function f(int x)
begin
int b = 1;  // b�� 1 �Է�
int c = 2;  // c�� 2 �Է�

if (b > (b + c))  // b�� b+c���� ū�� Ȯ�� => 1 > 1 + 2 (����)
{
    ((b * c) + g(x));  // b�� c�� ���� ���� g(x)�� ����
}

if (c > b)  // c�� b���� ū�� Ȯ�� => 2 > 1 (��)
{
    (1 + g(x));  // 1�� g(x)�� ����� ���� => 1 + x
}
end

// main �Լ� ����
function main()
begin
int k = 1;  // k�� 1 �Է�
int t = 2;  // t�� 2 �Է�
int x = 3;  // x�� 3 �Է�
((2 + f(x)) - k);  // 2�� f(x)�� ����� ���� �� k�� �� => 2 + 1 + 3 - 1 = 5
end
