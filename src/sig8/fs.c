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

#include "sig8_internal.h"

void sig8_UseResourceBundle(const unsigned char *bundle)
{
    state->filesystem.resourcePath = NULL;
    state->filesystem.resourceBundle = bundle;
}

void sig8_UseResourcePath(const char *path)
{
    state->filesystem.resourceBundle = NULL;
    state->filesystem.resourcePath = path;
}

static const unsigned char *BundleSeek(const char *path, int *size)
{
    if (!state->filesystem.resourceBundle) {
        *size = 0;
        return NULL;
    }

    const unsigned char *ptr = state->filesystem.resourceBundle;
    while (*ptr) {
        bool ok = false;
        if (!strcmp((const char*)ptr, path)) {
            ok = true;
        }

        ptr += strlen((const char*)ptr) + 1;
        *size = atoi((const char*)ptr);
        ptr += strlen((const char*)ptr) + 1;

        if (ok) {
            return ptr;
        }

        ptr += *size;
    }

    *size = 0;
    return NULL;
}

unsigned char *ReadFileContents(const char *path, int *sizePtr)
{
    int size;
    uint8_t *result;

    if (!strncmp(path, RESOURCE_SCHEMA, strlen(RESOURCE_SCHEMA))) {
        const char *stripped = path + strlen(RESOURCE_SCHEMA);

        const uint8_t *data = BundleSeek(stripped, &size);
        if (!data) {
            if (state->filesystem.resourcePath) {
                char *fullPath = malloc(
                        strlen(state->filesystem.resourcePath) +
                            strlen(stripped) + 1);
                sprintf(fullPath, "%s%s", state->filesystem.resourcePath,
                        stripped);
                result = ReadFileContents(fullPath, sizePtr);
                free(fullPath);

                return result;
            } else {
                return NULL;
            }
        }

        result = malloc(size + 1);
        memcpy(result, data, size);
    } else {
        FILE *file = fopen(path, "rb");
        if (!file) {
            return NULL;
        }

        fseek(file, 0L, SEEK_END);
        size = ftell(file);
        fseek(file, 0L, SEEK_SET);

        result = malloc(size + 1);
        fread(result, size, 1, file);
        fclose(file);
    }

    if (sizePtr) {
        *sizePtr = size;
    }

    result[size] = 0;

    return result;
}
