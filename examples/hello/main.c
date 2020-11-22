#include <sig8.h>
#include <math.h>

void update(void)
{
    static float t = 0.0f;

    ClearScreen(0);

    t += 1/60.0f;

    for (int i = 0; i < 128; i += 8) {
        int x = (int)roundf(fmodf(i + t * 16.0f, 128.0f));
        DrawLine(0, x, 128 - x, 0, 12);
        DrawLine(x, 127, 127, 128 - x, 8);
    }

    for (int i = 0; i < 64; i += 16) {
        int x = (int)roundf(fmodf(i + t * 16.0f, 64.0f)) + 32;
        DrawLine(127, 0, x, x, 7);
        DrawLine(0, 127, x, x, 7);
        DrawLine(127, 0, 128 - x, 128 - x, 7);
        DrawLine(0, 127, 128 - x, 128 - x, 7);
    }

}

int main(int argc, char **argv)
{
    Initialize(argc, argv);
    Run(update);

    return 0;
}
