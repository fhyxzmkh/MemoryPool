#pragma once

#include <stdlib.h>

// 大小不确定，释放时间确定

struct MemNode {
	char* free_ptr;
	char* end_ptr;
	MemNode* next;
};

struct Mempool1 {
	Mempool1(int block_size) : first{}, current{}, max{ block_size } {
		void* addr = malloc(block_size);
		MemNode* node = new(addr) MemNode; // placement new 将对象构造在已经分配的内存块上

		node->free_ptr = static_cast<char*>(addr) + sizeof(MemNode);
		node->end_ptr = static_cast<char*>(addr) + block_size;
		node->next = nullptr;

		first = node;
		current = node;
	}

	~Mempool1() {
		while (first) {
			void* addr = first;
			auto node = static_cast<MemNode*>(addr);

			first = node->next;

			free(addr);
		}
	}

	void* Alloc(int size_) {
		void* addr = current;
		auto node = static_cast<MemNode*>(addr);

		do {
			if (size_ <= (node->end_ptr - node->free_ptr)) { // 可分配
				char* p = node->free_ptr;
				node->free_ptr += size_;

				return p;
			}
			node = node->next;
		} while (node);

		addr = malloc(max);
		node = new(addr) MemNode;

		node->free_ptr = static_cast<char*>(addr) + sizeof(MemNode);
		node->end_ptr = static_cast<char*>(addr) + max;

		node->next = current;
		current = node;

		auto ptr = node->free_ptr;
		node->free_ptr += size_;

		return ptr;
	}


	MemNode* first;
	MemNode* current;
	int max;
};