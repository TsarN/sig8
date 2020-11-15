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


#include <assert.h>
#include "sig8_internal.h"

void DisplayInit(void)
{
    state->display.width = 128;
    state->display.height = 128;
    state->display.pixels = malloc(sizeof(Color) * state->display.width * state->display.height);

    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 128; ++j) {
            SetPixelColor(i, j, (Color) {
                .r = rand() & 0xff,
                .g = rand() & 0xff,
                .b = rand() & 0xff,
                .a = rand() & 0xff,
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
