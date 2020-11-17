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


#ifndef SIG8_H
#define SIG8_H

#ifdef __cplusplus
    #define EXTERN_C extern "C"
#else
    #include <stdbool.h>
    #define EXTERN_C
#endif

#define API EXTERN_C

//! A private function that performs various initialization tasks.
//! The function's argument is a bitmask of the following flags:
//!
//! - 0x01 - perform initialization, if 0 then only assert that
//!          not already initialized
//! - 0x02 - install atexit handler to call Deinitialize()
//! - 0x04 - initialize [window]
API void sig8_Initialize(unsigned);

API int sig8_DefaultMain(void (*setup)(void), void (*loop)(void), int argc, char **argv);

#if !defined(SIG8_BUILD_LIB) && defined(SIG8_USE_MAIN)
EXTERN_C void setup(void);
EXTERN_C void loop(void);
#endif

//
// Subsystem: [system]
//

#if !defined(SIG8_BUILD_LIB) && !defined(SIG8_NO_INITIALIZE)

//! Initialize the library, must be the first function called.
static inline void Initialize(void)
{
    sig8_Initialize(0);
    sig8_Initialize(
        0x01u | // initialize
        0x02u | // install atexit handler
        0x04u   // initialize [window]
    );
}

#endif

//! Deinitialize the library, should be called automatically on <code>atexit</code>.
API void Deinitialize(void);

//! Run the game with provided update function.
API void Run(void (*update)(void));

//
// Subsystem: [graphics]
//

typedef struct Palette_s *Palette;

//! Load a palette from an image, max 256 colors
API Palette LoadPalette(const char *path);

//! Unload a palette freeing the allocated memory
API void UnloadPalette(Palette palette);

//! Use the palette for further drawing
API void UsePalette(Palette palette);

//! Put a pixel from the palette on the screen
API void PutPixel(int x, int y, int color);

//! Put an RGB pixel on the screen
API void PutPixelRGB(int x, int y, int r, int g, int b);

//! Read an RGB pixel from the screen
API void GetPixelRGB(int x, int y, int *r, int *g, int *b);

//! Clear the screen using a color from the palette
API void ClearScreen(int color);

//! Clear the screen using an RGB color
API void ClearScreenRGB(int r, int g, int b);

typedef struct SpriteSheet_s *SpriteSheet;

//! Load a sprite sheet from an image
API SpriteSheet LoadSpriteSheet(const char *path);

//! Unload a sprite sheet freeing the allocated memory
API void UnloadSpriteSheet(SpriteSheet spriteSheet);

//! Use the sprite sheet for further drawing
API void UseSpriteSheet(SpriteSheet spriteSheet);

#undef API
#undef EXTERN_C
#endif
