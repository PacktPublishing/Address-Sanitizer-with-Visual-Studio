#include <thread>
#include <iostream>
void BufferOverflow1() {
	double* d = new double{};
	*d = 1.0;
	auto a = d + 1;
	*a = 1.0;
}
void BufferOverflow2() {
	char* p = new char[5] {};
	std::jthread t1{ [&]() {
		std::cout << p[-1] << ' ';
		p[-1] = 'a';
	} };
	t1.join();
}
void DoubleDelete() {
	char* p = new char[5] {};
	std::jthread t1{ [&]() {
		p[0] = 'a';
		std::cout << p[0] << ' ';
		delete[]p;
	} };
	t1.join();
	delete[]p;
}
void HeapUseAfterFree() {
	char* p = new char[5] {};
	std::jthread t1{ [&]() {
		p[0] = 'a';
		std::cout << p[0] << ' ';
		delete[]p;
	} };
	t1.join();
	std::cout << p[0] << ' ';
}
void InvalidFree() {
	char p[10]{};
	std::jthread t1{ [&]() {
		p[0] = 'a';
		std::cout << p[0] << ' ';
		delete[]p;
	} };
	t1.join();
	std::cout << p[0] << ' ';
}
void AllocDeallocMismatch1() {
	char* p = new char[5] {};
	std::jthread t1{ [&]() {
		p[0] = 'a';
		std::cout << p[0] << ' ';
		delete p;
	} };
	t1.join();
}
void AllocDeallocMismatch2() {
	char* p = new char[5] {};
	std::jthread t1{ [&]() {
		p[0] = 'a';
		std::cout << p[0] << ' ';
		free(p);
	} };
	t1.join();
}
int main() {
	//BufferOverflow1();
	//BufferOverflow2();
	//DoubleDelete();
	//HeapUseAfterFree();
	//InvalidFree();
	//AllocDeallocMismatch1();
	AllocDeallocMismatch2();

}