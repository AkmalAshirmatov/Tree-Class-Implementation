#pragma once
#include "MemoryManager.h"

// Простейший менеджер памяти, использует ::new и ::delete
class Mem: public MemoryManager {

public:
    Mem(size_t sz): MemoryManager(sz) {}

    void* allocMem(size_t sz) {
        return new char[sz];
    }

    void freeMem(void* ptr) {
        delete[] ptr;
    }
};

//How to use?
//void* addr = mem.alloc(sizeof(Node));
//root = new (addr) Node();