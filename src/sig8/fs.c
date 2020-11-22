/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *  See https://github.com/TsarN/sig8/blob/master/LICENSE for license info
 */

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
