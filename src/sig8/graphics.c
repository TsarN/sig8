/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *  See https://github.com/TsarN/sig8/blob/master/LICENSE for license info
 */

static void GraphicsInit(void)
{
    size_t size = state->display.width *state->display.height;
    state->graphics.colors = malloc(size);
    memset(state->graphics.colors, 0xff, size);

    UsePalette(NULL);
    UseFont(NULL);
}

static void GraphicsDeinit(void)
{
    free(state->graphics.colors);
}

static void Flip(void)
{
    int paletteSize = state->graphics.palette->size;
    int n = state->display.width * state->display.height;
    Color *colors = state->graphics.palette->colors;

    for (int i = 0; i < n; ++i) {
        int c = state->graphics.colors[i];
        if (c < paletteSize) {
            state->display.pixels[i] = colors[c];
        }
        state->graphics.colors[i] = 0xff;
    }
}

void DrawPixel(int x, int y, int color)
{
    if (color < 0 || color >= state->graphics.palette->size) {
        return;
    }

    if (x < 0 || y < 0 || x >= state->display.width || y >= state->display.height) {
        return;
    }

    state->graphics.colors[x + y * state->display.width] = color;
}

static int DrawCharacter(int x, int y, int color, int ch)
{
    Font font = state->graphics.font;
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
                DrawPixel(i + x, j + y, color);
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

static void DrawLineImpl(int x0, int y0, int x1, int y1, bool swap, int color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int delta = 1;
    if (dy < 0) {
        delta = -1;
        dy = -dy;
    }

    int err = 2 * dy - dx;
    int y = y0;

    for (int x = x0; x <= x1; ++x) {
        if (swap) {
            DrawPixel(x, y, color);
        } else {
            DrawPixel(y, x, color);
        }

        if (err > 0) {
            y += delta;
            err -= 2 * dx;
        }
        err += 2 * dy;
    }
}

void DrawLine(int x0, int y0, int x1, int y1, int color)
{
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            DrawLineImpl(x1, y1, x0, y0, true, color);
        } else {
            DrawLineImpl(x0, y0, x1, y1, true, color);
        }
    } else {
        if (y0 > y1) {
            DrawLineImpl(y1, x1, y0, x0, false, color);
        } else {
            DrawLineImpl(y0, x0, y1, x1, false, color);
        }
    }
}

void ClearScreen(int color)
{
    size_t size = state->display.width *state->display.height;
    memset(state->graphics.colors, color, size);
}
