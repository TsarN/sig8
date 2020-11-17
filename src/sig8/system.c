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

void sig8_Initialize()
{
    if (state) {
        fprintf(stderr, "Attempt to initialize sig8 twice");
        exit(1);
    }

    state = calloc(1, sizeof(State));
    atexit(Deinitialize);

    DisplayInit();
    WindowInit();
}

void Deinitialize(void)
{
    if (state) {
        DisplayDeinit();
        free(state);
        state = NULL;
    }
}

void Run(void (*update)(void))
{
    state->update = update;
    while (!state->shouldQuit) {
        update();
        WindowDraw();
        WindowHandleEvents();
    }
}
