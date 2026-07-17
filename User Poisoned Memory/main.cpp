#include <new>
#include <iostream>
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
	CustomMemoryPool();
}