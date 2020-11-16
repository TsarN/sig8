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

#ifndef SIG8_INTERNAL_H
#define SIG8_INTERNAL_H

#include "config.h"
#include "sig8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned char u8;

typedef struct {
    u8 r, g, b;
} Color;

//! Display state.
typedef struct {
    int width, height;
    Color *pixels;
} Display;

typedef struct {
    void *window;             //!< Window object
    void *context;            //!< GL context

    unsigned screenVBO;       //!< Vertex buffer object for the screen rect
    unsigned screenPBO;       //!< Pixel buffer object for the screen texture
    unsigned screenTexture;   //!< Screen texture
    unsigned screenVAO;       //!< Vertex array object for the screen
    unsigned shader;          //!< Shader program for the screen
    int offLoc;               //!< Location of the "offset" uniform in the shader

    int width;                //!< Width of the window in pixels
    int height;               //!< Height of the window in pixels
    float offsetX, offsetY;
} Window;

//! Struct containing all the information about current application state.
typedef struct {
    void (*update)(void); //!< Update function, called on every frame
    Display display;      //!< Current display state
    Window window;        //!< Current window state
    Palette palette;      //!< Current palette
    bool shouldQuit;
} State;

typedef struct {
    char *path;  //!< Owned string, path to the resource
    bool system; //!< True for builtin resources (like editor sprites)
} ResourceInfo;


#define SPRITE_SHEET_WIDTH 16
#define SPRITE_SHEET_HEIGHT 16
#define SPRITE_SHEET_SIZE 256
#define SPRITE_WIDTH 8
#define SPRITE_HEIGHT 8
#define SPRITE_SHEET_WIDTH_PX (SPRITE_SHEET_WIDTH * SPRITE_WIDTH)
#define SPRITE_SHEET_HEIGHT_PX (SPRITE_SHEET_HEIGHT * SPRITE_HEIGHT)

struct SpriteSheet_s {
    ResourceInfo info;
    u8 *pixels;
};

struct Palette_s {
    ResourceInfo info;
    int size;
    Color *colors;
};

static State *state;

//! Flags for sig8_Initialize
enum {
    FLAG_INIT = 0x01,        //!< perform initialization
    FLAG_ATEXIT = 0x02,      //!< install atexit handlers
    FLAG_INIT_WINDOW = 0x04  //!< initialize [window]
};

//! Initialize display with default settings.
static void DisplayInit(void);

//! Deinitialize display.
static void DisplayDeinit(void);

//! Set display pixel color, assuming the coordinates are inside screen bounds.
static inline void SetPixelColor(int x, int y, Color color);

//! Get display pixel color, assuming the coordinates are inside screen bounds.
static inline Color GetPixelColor(int x, int y);

//! Create and initialize SDL window.
static void WindowInit(void);

//! Draw a frame.
static void WindowDraw(void);

//! Handle window events.
static void WindowHandleEvents(void);

#endif
