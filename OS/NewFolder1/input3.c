// 함수 g 선언
function g(int x)
begin
(1 + 2 - 3 + x);  // 1, 2, 3을 더하고 x를 더함 => x
end

// 함수 f 선언
function f(int a)
begin
int b = 1; // b에 1 입력
int c = 2; // c에 2 입력
if (b < (b + c))  // b가 b+c보다 작은지 확인 => 1 < 1 + 2 (참)
{
    ((b * c) + g(a));  // b와 c를 곱한 값에 g(a)를 더함 => 1 * 2 + a = a + 2
}
end

// main 함수 선언
function main()
begin
int a = 1;
int b = 2;
int c = 3;
((2 + f(c)) * a);  // 2와 f(c)의 결과를 더한 후 a로 곱함 => {2 + (3 + 2)} * 1 = 7
end
