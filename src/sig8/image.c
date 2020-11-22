/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *  See https://github.com/TsarN/sig8/blob/master/LICENSE for license info
 */

Image NewImage(int width, int height)
{
    Image image = calloc(1, sizeof(struct Image_s));

    image->width = width;
    image->height = height;
    image->colors = calloc(width * height, 1);

    return image;
}

void UnloadImage(Image image)
{
    free(image->info.path);
    free(image->colors);
    free(image);
}
