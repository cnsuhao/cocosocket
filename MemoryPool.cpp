#include "MemoryPool.h"

MemoryPool::MemoryPool(unsigned int size, unsigned int increase /*= 64*/)
{
    m_size = size;
    m_increase = increase;
    MemoryList *list = (MemoryList*) malloc(sizeof (MemoryList));
    m_handle = (MemoryList*) malloc(sizeof (MemoryList));
    if (list == NULL || m_handle == NULL)
    {
        return;
    }
    list->memory = malloc(size);
    m_memory_list_header = list;
    MemoryList *handle = NULL;
    for (unsigned int i = 1; i < m_increase * 2; ++i)
    {
        handle = (MemoryList*) malloc(sizeof (MemoryList));
        handle->memory = malloc(size);
        list->next = handle;
        list = list->next;
    }
    list->next = NULL;
}

MemoryPool::~MemoryPool()
{
    MemoryList *handle = NULL;
    while (m_memory_list_header->next != NULL)
    {
        handle = m_memory_list_header;
        m_memory_list_header = m_memory_list_header->next;
        free(handle);
    }
}

void * MemoryPool::Alloc()
{
    static void * handle = NULL;
    if (m_memory_list_header->next == NULL)
    {
        MemoryList *handle = NULL;
        MemoryList *list = (MemoryList*) malloc(sizeof (MemoryList));
        if (list == NULL)
        {
            return NULL;
        }
        list->memory = malloc(m_size);
        m_memory_list_header->next = list;
        for (unsigned int i = 1; i < m_increase; ++i)
        {
            handle = (MemoryList*) malloc(sizeof (MemoryList));
            if (handle == NULL)
            {
                break;
            }
            handle->memory = malloc(m_size);
            list->next = handle;
            list = list->next;
        }
        list->next = NULL;
    }
    handle = m_memory_list_header->memory;
    m_memory_list_header = m_memory_list_header->next;
    return handle;
}

void MemoryPool::Free(void *m)
{
    if (m == NULL)
    {
        return;
    }
    if (m_handle == NULL)
    {
        m_handle = (MemoryList*) malloc(sizeof (MemoryList));
    }
    m_handle->memory = m;
    m_handle->next = m_memory_list_header;
    m_memory_list_header = m_handle;
}
