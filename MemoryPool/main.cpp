
#include "mempool.h"
#include "mempool1.h"

#include "vld.h"

void testMempool() {
	Mempool mp(32);

	void* p1 = mp.Alloc();
	std::cout << p1 << '\n';

	void* p2 = mp.Alloc();
	std::cout << p2 << '\n';

	void* p3 = mp.Alloc();
	std::cout << p3 << '\n';

	void* p4 = mp.Alloc();
	std::cout << p4 << '\n';

	mp.Free(p2);

	void* p5 = mp.Alloc();
	std::cout << p5 << '\n';

	void* p6 = mp.Alloc();
	std::cout << p6 << '\n';
}

void testMempool1() {
	Mempool1 mp{4096};

	void* p1 = mp.Alloc(16);
	std::cout << p1 << '\n';

	void* p2 = mp.Alloc(32);
	std::cout << p2 << '\n';

	void* p3 = mp.Alloc(64);
	std::cout << p3 << '\n';

	void* p4 = mp.Alloc(128);
	std::cout << p4 << '\n';

	void* p5 = mp.Alloc(256);
	std::cout << p5 << '\n';

	void* p6 = mp.Alloc(512);
	std::cout << p6 << '\n';

}

int main() {
	testMempool();
	
	testMempool1();
}
