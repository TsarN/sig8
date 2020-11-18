/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *  See https://github.com/TsarN/sig8/blob/master/LICENSE for license info
 */

#include "sig8_internal.h"
#include "stb_image.h"

void UsePalette(Palette palette)
{
    state->palette = palette;
}

void PutPixel(int x, int y, int color)
{
    if (color < 0 || color >= state->palette->size) {
        return;
    }

    Color c = state->palette->colors[color];
    PutPixelRGB(x, y, c.r, c.g, c.b);
}

void PutPixelRGB(int x, int y, int r, int g, int b)
{
    if (x < 0 || y < 0 || x >= state->display.width || y >= state->display.height) {
        return;
    }

    SetPixelColor(x, y, (Color) { .r = r, .g = g, .b = b });
}

void GetPixelRGB(int x, int y, int *r, int *g, int *b)
{
    if (x < 0 || y < 0 || x >= state->display.width || y >= state->display.height) {
        return;
    }

    Color c = GetPixelColor(x, y);
    *r = c.r;
    *g = c.g;
    *b = c.b;
}
