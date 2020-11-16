/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "sig8_internal.h"

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
