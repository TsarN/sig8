/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *  See https://github.com/TsarN/sig8/blob/master/LICENSE for license info
 */

#include "sig8_internal.h"
#include "stb_ds.h"
#include "stb_image.h"

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

static int DrawCharacter(int x, int y, int color, int ch)
{
    Font font = state->font;
    while (font && !(ch >= font->firstCharCode && ch <= font->lastCharCode)) {
        font = font->fallback;
    }

    if (!font) {
        return 0;
    }

    ch -= font->firstCharCode;
    int w = font->charWidth[ch];
    unsigned char *data = font->charData + ch * font->height * font->bytesPerLine;

    for (int j = 0; j < font->height; ++j) {
        for (int i = 0; i < w; ++i) {
            if ((data[i / 8 + j * font->bytesPerLine] >> (i % 8)) & 1) {
                PutPixel(i + x, j + y, color);
            }
        }
    }

    return w + 1;
}

static inline int NextUtf8Codepoint(const unsigned char **s)
{
    if (!**s) {
        return 0;
    }

    int ret = 0;

    if (*s[0] < 0x80) {
        ret = *s[0];
        *s += 1;
    } else if ((*s[0] & 0xe0) == 0xc0) {
        ret = ((int)(*s[0] & 0x1f) << 6) | (int)(*s[1] & 0x3f);
        *s += 2;
    } else if ((*s[0] & 0xf0) == 0xe0) {
        ret = ((int)(*s[0] & 0x0f) << 12) | ((int)(*s[1] & 0x3f) << 6) |
                (int)(*s[2] & 0x3f);
        *s += 3;
    } else if ((*s[0] & 0xf8) == 0xf0 && (*s[0] <= 0xf4)) {
        ret = ((int)(*s[0] & 0x07) << 18) | ((int)(*s[1] & 0x3f) << 6) |
                ((int)(*s[2] & 0x3f) << 6) | (int)(*s[3] & 0x3f);
        *s += 4;
    } else {
        ret = -1;
        *s += 1;
    }

    if (ret >= 0xd800 && ret <= 0xdfff) {
        ret = -1;
    }

    return ret;
}

void DrawUtf8String(int x, int y, int color, const char *string)
{
    int c;
    while ((c = NextUtf8Codepoint((const unsigned char **) (&string)))) {
        x += DrawCharacter(x, y, color, c);
    }
}

void DrawString(int x, int y, int color, const char *string)
{
    for (const char *c = string; *c; ++c) {
        x += DrawCharacter(x, y, color, (unsigned char)*c);
    }
}
