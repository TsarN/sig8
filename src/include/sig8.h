/*
 *
 */


#ifndef SIG8_H
#define SIG8_H

#ifdef __cplusplus
    #define EXTERN_C extern "C"
#else
    #define EXTERN_C
#endif

#define API EXTERN_C

API void sig8_Initialize(void);
API void sig8_UseResourceBundle(const unsigned char *bundle);
API void sig8_UseResourcePath(const char *path);

#ifdef SIG8_USE_DEFAULT_BUNDLE
extern const unsigned char *sig8_DefaultBundle;
#endif

//
// Subsystem: [system]
//

#if !defined(SIG8_BUILD_LIB) && !defined(SIG8_NO_INITIALIZE)

//! Initialize the library, must be the first function called.
static inline void Initialize(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    sig8_Initialize();

#ifdef SIG8_USE_RESOURCE_PATH
    sig8_UseResourcePath(SIG8_USE_RESOURCE_PATH);
#endif

#ifdef SIG8_USE_DEFAULT_BUNDLE
    sig8_UseResourceBundle(sig8_DefaultBundle);
#endif
}

#endif

//! Deinitialize the library, called automatically upon exiting from Run().
API void Deinitialize(void);

//! Run the game with provided update function.
API void Run(void (*update)(void));

//
// Subsystem: [graphics]
//

typedef struct Palette_s *Palette;

//! Load a palette from an image, max 256 colors.
API Palette LoadPalette(const char *path);

//! Unload a palette freeing the allocated memory.
API void UnloadPalette(Palette palette);

//! Use the palette for further drawing.
//! If palette is NULL then use the default palette.
API void UsePalette(Palette palette);

//! Put a pixel from the palette on the screen.
API void DrawPixel(int x, int y, int color);

//! Draw a line on the screen.
API void DrawLine(int x0, int y0, int x1, int y1, int color);

//! Clear the screen using a color from the palette.
API void ClearScreen(int color);

typedef struct Font_s *Font;

//! Load a font from an image.
API Font LoadFont(const char *path);

//! Load a font from an image using extended options.
API Font LoadFontEx(const char *path,
                    int firstCharCode,
                    int lastCharCode,
                    int offset,
                    Font fallback);

//! Unload a font freeing the allocated memory.
API void UnloadFont(Font font);

//! Use the font for future text rendering.
//! If font is NULL then use default font.
API void UseFont(Font font);

//! Draw UTF-8 encoded string at coordinates using the current font.
API void DrawUtf8String(int x, int y, int color, const char *string);

//! Draw ASCII string at coordinates using the current font.
API void DrawString(int x, int y, int color, const char *string);

typedef struct Image_s *Image;

//! Create a new image with specified dimensions.
API Image NewImage(int width, int height);

//! Unload an image freeing the allocated memory.
API void UnloadImage(Image image);

//
// Subsystem: [fs]
//

//! Read file contents, resolving special paths (e.g. res://).
//! Returned pointer is owned by the caller and should be freed using free().
unsigned char *ReadFileContents(const char *path, int *size);

#undef API
#undef EXTERN_C
#endif
