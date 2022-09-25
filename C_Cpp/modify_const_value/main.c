#include <stdio.h>

int main(int argc, char * argv[]) {
    const int i = 10;
    const int **p1;
    int *p2;

    printf("i = %d\n", i);

    p1 = &p2;
    *p1 = &i;
    *p2 = 20;

    printf("i = %d\n", i);

    return 0;
}
