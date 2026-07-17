#include <new>
#include <iostream>
#include <thread>

__declspec(no_sanitize_address)
void BufferOverflow1() {
	double* d = new double{};
	*d = 1.0;
	auto a = d + 1;
	*a = 1.0;
}

__declspec(no_sanitize_address)
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
__declspec(no_sanitize_address)
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

__declspec(no_sanitize_address)
void AllocDeallocMismatch1() {
	char* p = new char[5] {};
	std::jthread t1{ [&]() {
		p[0] = 'a';
		std::cout << p[0] << ' ';
		delete p;
	} };
	t1.join();
}

char x[10];
char buffer[16]{};
__declspec(no_sanitize_address)
void GlobalDataIssues() {
	auto p = buffer;
	p[-1] = 'a';
}

__declspec(no_sanitize_address)
void StackBufferOverflow() {
	char buff[8]{};
	auto p = buff;
	//p[-1] = 'a';
	p[8] = 'b';
}


#ifdef __SANITIZE_ADDRESS__
extern "C" {
	void __asan_poison_memory_region(void const volatile* addr, size_t size);
	void __asan_unpoison_memory_region(void const volatile* addr, size_t size);
}
#define ASAN_POISON(address, size) __asan_poison_memory_region(address,size)
#define ASAN_UNPOISON(address, size) __asan_unpoison_memory_region(address,size)
#define LOG std::cout << "Compiling with Address Sanitizer enabled!\n"
#else
#define ASAN_POISON(address, size) void (0)
#define ASAN_UNPOISON(address, size) void (0)
#define LOG std::cout << "Address Sanitizer is disabled!\n"
#endif
__declspec(no_sanitize_address)
void CustomMemoryPool() {
	char* pool = new char[32] {};
	ASAN_POISON(pool, 32);

	ASAN_UNPOISON(pool, 4);
	size_t free_mem_index = 0;
	int* p1 = new(pool)int{ 5 };

	free_mem_index += sizeof(int);
	//int* p2 = new(pool + free_mem_index)int{ 5 };
	//ASAN_UNPOISON(pool + 18, 1);

	char* p3 = pool + 18;
	*p3 = 'a';
}
int main() {
	LOG;
	BufferOverflow1();
	//DoubleDelete();
	HeapUseAfterFree();
	//AllocDeallocMismatch1();
	GlobalDataIssues();
	//StackBufferOverflow();
	CustomMemoryPool();
	return 0;
}