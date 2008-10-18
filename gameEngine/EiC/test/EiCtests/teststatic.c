#include <assert.h>

int p = 7;
int q = 5;

#include teststatic1.aux
#include teststatic2.aux
#include teststatic3.aux

void T1()
{
    assert(getp1() == 33);
    assert(getq1() == 15.5);
    assert(p == 7);
    assert(q == 5);
}

void T2(void)
{
    assert(getbar() == 3);
}

void T3()
{
    assert(getp3() == 7);
    assert(getq3() == 44);

    setp1(50);
    assert(p == 7);
    assert(getp1() == 50);
    assert(getp3() == 7);

    setp3(60);
    assert(p == 7);
    assert(getp1() == 50);
    assert(getp3() == 60);

    p = 22;
    assert(p == 22);
    assert(getp1() == 50);
    assert(getp3() == 60);
}


int main()
{
    T1();
    T2();
    T1();
    T3();
    return 0;
}

#ifdef EiCTeSts
main();
#endif 








