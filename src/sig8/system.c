/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *  See https://github.com/TsarN/sig8/blob/master/LICENSE for license info
 */

#include "sig8_internal.h"

void sig8_Initialize()
{
    if (state) {
        fprintf(stderr, "Attempt to initialize sig8 twice");
        exit(1);
    }

    state = calloc(1, sizeof(State));

    DisplayInit();
    WindowInit();
}

void Deinitialize(void)
{
    if (state) {
        WindowDeinit();
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

    Deinitialize();
}
