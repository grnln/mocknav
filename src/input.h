#ifndef _MOCKNAV_INPUT_H_
#define _MOCKNAV_INPUT_H_

#include "types.h"

enum input_actions {
    INPUT_UP            = 0x001,
    INPUT_DOWN          = 0x002,
    INPUT_LEFT          = 0x004,
    INPUT_RIGHT         = 0x008,
    INPUT_LCLICK        = 0x010,
    INPUT_RCLICK        = 0x020,
    INPUT_DEL           = 0x040,
    INPUT_BTN           = 0x080,
    INPUT_TEXT          = 0x100,
    INPUT_ACTIONS_LEN   = 0x009
};

s32 input_init(void);
void input_poll(void);

#endif