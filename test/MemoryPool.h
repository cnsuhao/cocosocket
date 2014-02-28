#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <stdio.h>
#include <stdlib.h>

struct MemoryList {
    void * memory;
    MemoryList * next;
};

class MemoryPool {
public:
    MemoryPool(unsigned int size, unsigned int increase = 64);
    ~MemoryPool();
    void * Alloc();
    void Free(void *m);

private:
    unsigned int m_size;
    unsigned int m_increase;
    MemoryList * m_memory_list_header;
    MemoryList * m_handle;
};


#define REGISTER_MEMORYPOOL(PoolNameSpace, ClassName, IncreaseNum) \
	namespace PoolNameSpace\
{\
	MemoryPool g_##ClassName##_mem_pool(sizeof(ClassName), IncreaseNum);\
}\
	void *ClassName::operator new(size_t size)\
{\
	void *mem = PoolNameSpace::g_##ClassName##_mem_pool.Alloc();\
	return mem;\
}\
	void ClassName::operator delete(void *m)\
{\
	PoolNameSpace::g_##ClassName##_mem_pool.Free(m);\
}

#endif