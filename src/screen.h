#ifndef _MOCKNAV_SCREEN_H_
#define _MOCKNAV_SCREEN_H_

#include "types.h"
#include "texture.h"

#include <string>
#include <vector>

typedef struct button {
    s32 x, y, w, h;
    std::string screen;
} button_t;

typedef struct screen {
    f32 h;
    std::string name;
    std::string texture_path;
    std::vector <button_t> buttons;
    texture_t texture;
} screen_t;

screen_t screen_create(std::string name, std::string texture_path);
void screen_add_button(screen_t *screen, s32 x, s32 y, s32 w, s32 h, s32 bg_x, s32 bg_y, std::string to);
void screen_render_buttons(screen_t *screen, s32 x, s32 y);
void screen_free(screen_t *screen);

void page_load(char *path);
void page_save(void);
void page_compile(void);

#endif