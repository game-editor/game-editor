#include <stdio.h>


void T1()
{
    char buf[256];
    FILE * f = fopen("testscanf3.aux", "r");
    int j = 1, maxj = 100;
    while ((fscanf(f,"%[^\n]\n",buf) != EOF) && (j < maxj))
        printf("Line %i: %s\n", j++, buf);

    fclose(f);
}

int  main(void)
{
    T1();
    return 0;
}

#ifdef EiCTeStS
main();
#endif






    








