/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *  See https://github.com/TsarN/sig8/blob/master/LICENSE for license info
 */

void DisplayInit(void)
{
    state->display.width = 128;
    state->display.height = 128;
    state->display.pixels = malloc(sizeof(Color) * state->display.width * state->display.height);

    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 128; ++j) {
            SetPixelColor(i, j, (Color) {
                .r = 0,
                .g = 0,
                .b = 0,
            });
        }
    }
}

void DisplayDeinit(void)
{
    free(state->display.pixels);
}

static inline void SetPixelColor(int x, int y, Color color)
{
    assert(x >= 0 && y >= 0 && x < state->display.width && y < state->display.height && state->display.pixels);
    state->display.pixels[x + y * state->display.width] = color;
}

static inline Color GetPixelColor(int x, int y)
{
    assert(x >= 0 && y >= 0 && x < state->display.width && y < state->display.height && state->display.pixels);
    return state->display.pixels[x + y * state->display.width];
}
