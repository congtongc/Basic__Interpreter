// 함수 g 선언
function g(int y)
begin
(1 + 2 - 3 + y);  // 1, 2, 3을 더하고 y를 더함 => y
end

// 함수 f 선언
function f(int x)
begin
int b = 1;  // b에 1 입력
int c = 2;  // c에 2 입력

if (b > (b + c))  // b가 b+c보다 큰지 확인 => 1 > 1 + 2 (거짓)
{
    ((b * c) + g(x));  // b와 c를 곱한 값에 g(x)를 더함
}

if (c > b)  // c가 b보다 큰지 확인 => 2 > 1 (참)
{
    (1 + g(x));  // 1과 g(x)의 결과를 더함 => 1 + x
}
end

// main 함수 선언
function main()
begin
int k = 1;  // k에 1 입력
int t = 2;  // t에 2 입력
int x = 3;  // x에 3 입력
((2 + f(x)) - k);  // 2와 f(x)의 결과를 더한 후 k를 뺌 => 2 + 1 + 3 - 1 = 5
end
