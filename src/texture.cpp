#include "texture.h"
#include "context.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

texture_t texture_load(const char *path) {
    u8 *buffer;
    texture_t out;

    buffer = stbi_load(path, &out.w, &out.h, &out.ch, 0);

    out.data = SDL_CreateTexture(
        context.renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STATIC,
        out.w,
        out.h
    );
    SDL_UpdateTexture(out.data, NULL, buffer, out.w * out.ch);
    stbi_image_free(buffer);
    return out;
}

void texture_render(texture_t *texture, s32 x, s32 y, s32 u, s32 v, s32 w, s32 h) {
    SDL_Rect src, dst;

    src.x = u;
    src.y = v;
    src.w = w;
    src.h = h;

    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;

    if (texture->data != NULL) {
        SDL_RenderCopy(context.renderer, texture->data, &src, &dst);
    }
}

void texture_free(texture_t *texture) {
    if (texture->data != NULL) {
        SDL_DestroyTexture(texture->data);
    }
}