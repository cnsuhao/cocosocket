#ifndef TEST_H
#define TEST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

class Test {
public:

    Test(int t) : a(-1) {
        if (a == -1) {
            cout << "哈哈猜中了" << endl;
        }
        a = t;
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
