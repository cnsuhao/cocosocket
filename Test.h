#ifndef TEST_H
#define TEST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Test {
public:

    Test() {
    }

    ~Test() {
    }
    void Show();
    void Init();
    void * operator new(size_t size);
    void operator delete(void *m);
private:
    int a;
    float b;
    char c;
    double d;
    char * e;
};

#endif
