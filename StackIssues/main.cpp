#include <iostream>
char x[10];
char buffer[16]{};
void GlobalDataIssues() {
	auto p = buffer;
	p[-1] = 'a';
}

void BufferOverflow() {
	char buff[8]{};
	auto p = buff;
	//p[-1] = 'a';
	p[8] = 'b';
}

char* ptr;
char* Use() {
	char buff[8];
	ptr = buff;
	return ptr;
}
void StackUseAfterReturn() {
	Use();
	std::cout << ptr << '\n';
}

void StackUseAfterScope() {
	int* p{};
	for (int i = 0; i < 5; ++i) {
		if (i == 3) {
			p = &i;
		}
	}
	std::cout << *p << '\n';
}
int main() {
	//BufferOverflow();
	//StackUseAfterReturn();
	StackUseAfterScope();
}