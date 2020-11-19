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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
    Palette defaultPalette; //!< Default palette
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
    Palette palette;           //!< Current palette
    Filesystem filesystem;     //!< Current virtual filesystem state
    SysResources sysResources; //!< System resources
    bool shouldQuit;
} State;

#define SPRITE_SHEET_WIDTH 16
#define SPRITE_SHEET_HEIGHT 16
#define SPRITE_SHEET_SIZE 256
#define SPRITE_WIDTH 8
#define SPRITE_HEIGHT 8
#define SPRITE_SHEET_WIDTH_PX (SPRITE_SHEET_WIDTH * SPRITE_WIDTH)
#define SPRITE_SHEET_HEIGHT_PX (SPRITE_SHEET_HEIGHT * SPRITE_HEIGHT)

struct SpriteSheet_s {
    ResourceInfo info;
    unsigned char *pixels;
};

struct Palette_s {
    ResourceInfo info;
    int size;
    Color *colors;
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
