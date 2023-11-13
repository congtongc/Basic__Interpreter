// 함수 g 선언
function g(int x)
begin
(1 + 2 - 3 + x);  // 1, 2, 3을 더하고 x를 더함
end

// 함수 f 선언
function f(int a)
begin
int b = 1;		// b에 1 입력
int c = 2;		// c에 2 입력
((b* c) + g(a));  // b와 c를 곱한 값에 g(a)를 더함
end

// main 함수 선언
function main()
begin
int a = 1;		// a에 1 입력
int b = 2;		// b에 2 입력
int c = 3;		// c에 3 입력
((2 + f(c))* a);  // 2와 f(c)의 결과를 더한 후 a로 곱함
end
