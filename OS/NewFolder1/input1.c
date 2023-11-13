// 함수 f 선언
function f(int a)
begin
int b = 6;		// b에 6 입력
int c = 2;		// c에 2 입력
((b + c) / a);  // b와 c를 더한 후 a로 나눔
end

// main 함수 선언
function main()
begin
int a = 1;		// a에 1 입력
int b = 2;		// b에 2 입력
int c = 4;		// c에 4 입력
((6 + f(c)) / b);  // 6과 f(c)의 결과를 더한 후 b로 나눔
end
