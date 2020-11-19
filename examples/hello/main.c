#include <sig8.h>
#include <stdio.h>
#include <stdlib.h>

void update(void)
{
    PutPixel(rand() % 128, rand() % 128, rand() % 16);
}

int main(int argc, char **argv)
{
    Initialize(argc, argv);
    Run(update);

    return 0;
}
