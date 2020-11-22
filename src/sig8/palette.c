/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *  See https://github.com/TsarN/sig8/blob/master/LICENSE for license info
 */

void UsePalette(Palette palette)
{
    if (!palette) {
        palette = state->sysResources.defaultPalette;
    }

    state->graphics.palette = palette;
}

Palette LoadPalette(const char *path)
{
    int width, height, channels, fileSize;
    unsigned char *contents = ReadFileContents(path, &fileSize);
    unsigned char *data = NULL;

    if (contents) {
        data = stbi_load_from_memory(contents, fileSize, &width, &height, &channels, 3);
    }

    int nColors = 0;
    struct { Color key; char value; } *colors = NULL;
    Color *colorArr = NULL;

    if (!data) {
        fprintf(stderr, "sig8: Failed to load palette '%s'\n", path);
        return NULL;
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Color color;
            color.r = data[3 * (j + i * width)];
            color.g = data[3 * (j + i * width) + 1];
            color.b = data[3 * (j + i * width) + 2];
            if (hmgeti(colors, color) == -1) {
                        arrput(colorArr, color);
                hmput(colors, color, 0);
                nColors++;

                if (nColors > MAX_PALETTE_COLORS) {
                    break;
                }
            }
        }

        if (nColors > MAX_PALETTE_COLORS) {
            break;
        }
    }

    Palette palette = malloc(sizeof(struct Palette_s));
    palette->info.system = state->filesystem.resourceBundle == sig8_SystemBundle;
    palette->info.path = strdup(path);
    palette->size = nColors;
    palette->colors = malloc(nColors * sizeof(Color));
    memcpy(palette->colors, colorArr, nColors * sizeof(Color));

            arrfree(colorArr);
            hmfree(colors);

    return palette;
}

void UnloadPalette(Palette palette)
{
    if (palette == state->graphics.palette) {
        state->graphics.palette = state->sysResources.defaultPalette;
    }

    free(palette->colors);
    free(palette->info.path);
    free(palette);
}
