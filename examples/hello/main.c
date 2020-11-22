#include <sig8.h>
#include <stdlib.h>

void update(void)
{
    DrawString(40, 40, 8, "Hello, world!");
    PutPixel(rand() % 128, rand() % 128, rand() % 16);
}

int main(int argc, char **argv)
{
    Initialize(argc, argv);
    Run(update);

    return 0;
}
