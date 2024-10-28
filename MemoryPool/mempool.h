#pragma once
#include <new>
#include <cassert>
#include <iostream>

constexpr int MEM_PAGE_SIZE = 4096;

// 大小确定，释放时间不确定

struct Mempool {
    Mempool(int block_size) : block_size_{ block_size }, free_count_{ 0 }, free_ptr_{ nullptr }, mem_{ nullptr } {
        if (block_size < 16) block_size_ = 16;
        mem_ = new char[MEM_PAGE_SIZE];

        if (!mem_) throw std::bad_alloc();

        free_ptr_ = mem_;
        free_count_ = MEM_PAGE_SIZE / block_size_;

        auto p = free_ptr_;
        for (int i = 0; i < free_count_ - 1; ++i) {
            *(char**)p = p + block_size_;
            p += block_size_;
        }
        *(char**)p = nullptr;
    }

    ~Mempool() {
        delete[] mem_;
    }

    void* Alloc() {
        if (!free_count_) return nullptr;

        auto p = free_ptr_;
        free_ptr_ = *(char**)p;
        --free_count_;

        return p;
    }

    void Free(void* p) {
        assert(p >= mem_ && p < mem_ + MEM_PAGE_SIZE);
        *(char**)p = free_ptr_; // 将要释放的块的头部设置为当前的 free_ptr_
        free_ptr_ = (char*)p;
        ++free_count_;
    }

    int block_size_;
    int free_count_;
    char* free_ptr_;
    char* mem_;
};