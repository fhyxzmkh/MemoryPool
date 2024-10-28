#pragma once

#include <stdlib.h>

// ��С��ȷ�����ͷ�ʱ��ȷ��

struct MemNode {
	char* free_ptr;
	char* end_ptr;
	MemNode* next;
};

struct Mempool1 {
	Mempool1(int block_size) : first{}, current{}, max{ block_size } {
		void* addr = malloc(block_size);
		MemNode* node = new(addr) MemNode; // placement new �����������Ѿ�������ڴ����

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
			if (size_ <= (node->end_ptr - node->free_ptr)) { // �ɷ���
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