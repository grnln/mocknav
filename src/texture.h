#ifndef _MOCKNAV_TEXTURE_H_
#define _MOCKNAV_TEXTURE_H_

#include "types.h"

#include <SDL.h>

typedef struct texture {
    s32 w, h, ch;
    SDL_Texture *data;
} texture_t;

texture_t texture_load(const char *path);
void texture_render(texture_t *texture, s32 x, s32 y, s32 u, s32 v, s32 w, s32 h);
void texture_free(texture_t *texture);

#endif