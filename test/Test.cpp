#include "Test.h"

void Test::Init()
{
    a = 1;
    b = 2;
    c = 3;
    d = 4;
    e = (char *) malloc(16 * sizeof (char));
    memcpy(e, "一头汗", sizeof ("一头汗"));
}

void Test::Show()
{
    printf("%d\n", a);
    printf("%f\n", b);
    printf("%d\n", c);
    printf("%f\n", d);
    //printf("%s\n", e);
}
