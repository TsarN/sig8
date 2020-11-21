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

    LoadSystemResources();
    DisplayInit();
    WindowInit();
    UsePalette(NULL);
    UseFont(NULL);
}

void Deinitialize(void)
{
    if (state) {
        WindowDeinit();
        DisplayDeinit();
        UnloadSystemResources();
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

void LoadSystemResources(void)
{
    Filesystem old = state->filesystem;
    sig8_UseResourceBundle(sig8_SystemBundle);

    state->sysResources.defaultPalette = LoadPalette("res://resources/default_palette.png");
    state->sysResources.defaultFont = LoadFont("res://resources/default_font.png");

    state->filesystem = old;
}

void UnloadSystemResources(void)
{
    UnloadPalette(state->sysResources.defaultPalette);
    state->sysResources.defaultPalette = NULL;

    UnloadFont(state->sysResources.defaultFont);
    state->sysResources.defaultFont = NULL;
}
