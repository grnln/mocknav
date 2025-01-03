#include "input.h"
#include "context.h"

#include <SDL.h>

/* Local variables */
static const u8 *keyboard;
static const struct {
    u32 mask, key_mask;
    char name[8];
} input_action_maps[INPUT_ACTIONS_LEN] = {
    {INPUT_UP, SDL_SCANCODE_UP, "[UP]"},
    {INPUT_DOWN, SDL_SCANCODE_DOWN, "[DOWN]"},
    {INPUT_LEFT, SDL_SCANCODE_LEFT, "[LEFT]"},
    {INPUT_RIGHT, SDL_SCANCODE_RIGHT, "[RIGHT]"},
    {INPUT_LCLICK, SDL_BUTTON_LMASK, "[CLICK]"},
    {INPUT_RCLICK, SDL_BUTTON_RMASK, "[CLICK]"},
    {INPUT_DEL, SDL_SCANCODE_DELETE, "[DEL]"},
    {INPUT_BTN, SDL_SCANCODE_B, "[BTN]"},
    {INPUT_TEXT, SDL_SCANCODE_T, "[TEXT]"}
};

s32 input_init(void) {
    keyboard = SDL_GetKeyboardState(NULL);
    return 0;
}

static inline u32 mouse_poll(void) {
    f32 aspect_ratio;
    s32 mouse_x, mouse_y, desired_w, desired_h;
    u32 mask;

    /* Mouse position is in the window size range */
    mask = SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_GetRendererOutputSize(context.renderer, &context.window_w, &context.window_h);

    context.prev_mouse_x = context.mouse_x;
    context.prev_mouse_y = context.mouse_y;

    context.mouse_x = mouse_x;
    context.mouse_y = mouse_y;

    return mask;
}

void input_poll(void) {
    u32 mouse_mask;
    
    /* Check for repeats */
    for (u32 i = 0; i < INPUT_ACTIONS_LEN; i++) {
        if (!INPUT_PRESS(input_action_maps[i].mask)) {
			context.input_repeats &= ~(input_action_maps[i].mask);
		} else {
			if (!INPUT_REPEAT(input_action_maps[i].mask)) {
				context.input_repeats |= input_action_maps[i].mask;
			}
		}
    }
    context.input_presses = 0;

    if (context.input_lock) {
        return;
    }

    /* Poll current frame */
    mouse_mask = mouse_poll();
    
    if (mouse_mask & SDL_BUTTON_LMASK && context.mouse_x < (4 * context.window_w / 6)) {
        context.input_presses |= INPUT_LCLICK;
    }

    if (mouse_mask & SDL_BUTTON_RMASK) {
        context.input_presses |= INPUT_RCLICK;
    }

    for (u32 i = 0; i < INPUT_ACTIONS_LEN; i++) {
        if (input_action_maps[i].mask != INPUT_LCLICK
        && input_action_maps[i].mask != INPUT_RCLICK
        && keyboard[input_action_maps[i].key_mask] == 1) {
            context.input_presses |= input_action_maps[i].mask;
        }
    }
}