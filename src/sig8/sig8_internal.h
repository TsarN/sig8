/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *  See https://github.com/TsarN/sig8/blob/master/LICENSE for license info
 */

#ifndef SIG8_INTERNAL_H
#define SIG8_INTERNAL_H

#define SIG8_BUILD_LIB

#include "sig8.h"
#include "sig8_system_resources.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stb_ds.h"
#include "stb_image.h"

#define MAX_PALETTE_COLORS 255

typedef struct {
    unsigned char r, g, b;
} Color;

//! Display state.
typedef struct {
    int width, height;
    Color *pixels;
} Display;

typedef struct {
    bool initialized;
    void *window;             //!< Window object

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

typedef struct {
    const char *resourcePath;
    const unsigned char *resourceBundle;
} Filesystem;

typedef struct {
    unsigned char *colors; //!< Colors (indexed into the palette), 0xff = transparent
    Palette palette;       //!< Current palette
    Font font;             //!< Current font
} Graphics;

typedef struct {
    Palette defaultPalette; //!< Default palette
    Font defaultFont;       //!< Default font
} SysResources;

#define RESOURCE_SCHEMA "res://"

typedef struct {
    char *path;  //!< Owned string, path to the resource
    bool system; //!< True for builtin resources (like editor sprites)
} ResourceInfo;

//! Struct containing all the information about current application state.
typedef struct {
    void (*update)(void);      //!< Update function, called on every frame
    Display display;           //!< Current display state
    Window window;             //!< Current window state
    Filesystem filesystem;     //!< Current virtual filesystem state
    Graphics graphics;         //!< Graphics subsystem state
    SysResources sysResources; //!< System resources
    bool shouldQuit;
} State;

struct Palette_s {
    ResourceInfo info;
    int size;
    Color *colors;
};

struct Font_s {
    ResourceInfo info;

    int height;
    int bytesPerLine;
    int firstCharCode;
    int lastCharCode;
    unsigned char *charData;
    int *charWidth;

    Font fallback;
};

struct Image_s {
    ResourceInfo info;

    int width, height;
    unsigned char *colors; //!< Colors (indexed into the palette)
};

static State *state;

//! Initialize display with default settings.
static void DisplayInit(void);

//! Deinitialize display.
static void DisplayDeinit(void);

//! Set display pixel color, assuming the coordinates are inside screen bounds.
static inline void SetPixelColor(int x, int y, Color color);

//! Get display pixel color, assuming the coordinates are inside screen bounds.
static inline Color GetPixelColor(int x, int y);

//! Initialize graphics.
static void GraphicsInit(void);

//! Deinitialize graphics.
static void GraphicsDeinit(void);

//! Put all pixels from palette-indexed buffer into the RGB one.
static void Flip(void);

//! Create and initialize glfw window.
static void WindowInit(void);

//! Deinitialize glfw window
static void WindowDeinit(void);

//! Draw a frame.
static void WindowDraw(void);

//! Handle window events.
static void WindowHandleEvents(void);

//! Load system resources (e.g. default palette)
static void LoadSystemResources(void);

//! Unload system resources
static void UnloadSystemResources(void);

#endif
