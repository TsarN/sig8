#include "sig8.h"

int main(int argc, char **argv)
{
    Initialize();
    return sig8_DefaultMain(setup, loop, argc, argv);
}
