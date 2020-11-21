/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *  See https://github.com/TsarN/sig8/blob/master/LICENSE for license info
 */

#include "sig8_internal.h"

void UseFont(Font font)
{
    if (!font) {
        font = state->sysResources.defaultFont;
    }

    state->font = font;
}

Font LoadFontEx(const char *path,
                int firstCharCode,
                int lastCharCode,
                int offset,
                Font fallback)
{
    int width, height, channels, fileSize;
    unsigned char *contents = ReadFileContents(path, &fileSize);
    unsigned char *data = NULL;

    if (contents) {
        data = stbi_load_from_memory(contents, fileSize, &width, &height, &channels, 2);
    }

    if (!data) {
        fprintf(stderr, "sig8: Failed to load font '%s'\n", path);
        return NULL;
    }

    int cw, ch, c = 0, n = lastCharCode - firstCharCode + 1;
    for (cw = 0; data[2 * cw + 1]; ++cw);
    for (ch = 0; data[2 * ch * width + 1]; ++ch);

    int bytesPerLine = (cw + 7) / 8;
    unsigned char *charData = calloc(n, ch * bytesPerLine);
    int *charWidth = malloc(sizeof(int) * n);

    for (int _j = 0; _j < height; _j += ch + 1) {
        for (int _i = 0; _i < width; _i += cw + 1) {
            if (offset > 0) {
                --offset;
                continue;
            }

            int w;
            for (w = 0; data[2 * (_j * width +_i + w) + 1]; ++w);
            charWidth[c] = w;

            for (int j = _j; j < _j + ch; ++j) {
                for (int i = _i; i < _i + w; ++i) {
                    if ((data[2 * (j * width + i)] >> (j - _j)) & 1) {
                        charData[c * ch * bytesPerLine + j - _j + (i - _i) / 8] |= 1u << (unsigned)((i - _i) % 8);
                    }
                }
            }

            ++c;
        }
    }

    Font font = malloc(sizeof(struct Font_s));
    font->info.system = state->filesystem.resourceBundle == sig8_SystemBundle;
    font->info.path = strdup(path);
    font->height = ch;
    font->bytesPerLine = bytesPerLine;
    font->firstCharCode = firstCharCode;
    font->lastCharCode = lastCharCode;
    font->charData = charData;
    font->charWidth = charWidth;
    font->fallback = fallback;

    return font;
}

Font LoadFont(const char *path)
{
    return LoadFontEx(path, 32, 127, 0, NULL);
}

void UnloadFont(Font font)
{
    if (font == state->font) {
        state->font = state->sysResources.defaultFont;
    }

    free(font->charData);
    free(font->charWidth);
    free(font->info.path);
    free(font);
}
