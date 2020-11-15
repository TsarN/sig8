#include "sig8.h"

void setup(void);
void loop(void);

int main(int argc, char **argv)
{
    Initialize();
    return sig8_DefaultMain(setup, loop, argc, argv);
}
