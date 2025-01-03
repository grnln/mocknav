#ifndef _MOCKNAV_CONTEXT_H_
#define _MOCKNAV_CONTEXT_H_

#include "types.h"
#include "screen.h"
#include "input.h"

#include <string>
#include <vector>
#include <SDL.h>

#define WINDOW_W            1024
#define WINDOW_H            768
#define ASPECT_RATIO        ((f32)WINDOW_W / (f32)WINDOW_H)

#define BIT(x)              (1 << (x))

#define INPUT_LOCK()		do {context.input_lock = true;} while (0)
#define INPUT_UNLOCK()		do {context.input_lock = false;} while (0)
#define INPUT_PRESS(x)		(context.input_presses & (x))
#define INPUT_REPEAT(x)		(context.input_repeats & (x))
#define INPUT_SET_REPEAT(x) do {context.input_repeats |= (x);} while (0)

typedef struct context {
    bool input_lock;
    u32 input_presses, input_repeats;
    s32 window_w, window_h,
        mouse_x, mouse_y,
        prev_mouse_x, prev_mouse_y,
        current_screen, current_button;
    
    std::string folder;

    SDL_Window *window;
    SDL_Renderer *renderer;
    
    struct {
        std::string name, icon;
        std::vector<screen_t> screens;
    } page_info;
} context_t;

extern context_t context;

#endif