#include "input.h"

std::bitset<JKEY_MAX_KEYS> Input::keys;
std::bitset<JKEY_MAX_KEYS> Input::keys_prev;
std::bitset<JKEY_MAX_KEYS> Input::keys_up;
std::bitset<JKEY_MAX_KEYS> Input::keys_down;

std::bitset<JMOUSE_MAX_BTNS> Input::mouse;
std::bitset<JMOUSE_MAX_BTNS> Input::mouse_prev;
std::bitset<JMOUSE_MAX_BTNS> Input::mouse_up;
std::bitset<JMOUSE_MAX_BTNS> Input::mouse_down;

std::bitset<JTOUCH_MAX_TOUCHES> Input::touches;
std::bitset<JTOUCH_MAX_TOUCHES> Input::touches_prev;
std::bitset<JTOUCH_MAX_TOUCHES> Input::touches_up;
std::bitset<JTOUCH_MAX_TOUCHES> Input::touches_down;


Input::GamePad Input::pads[JPAD_MAX_PADS];
std::unordered_map<int, SDL_GameController*> Input::game_controllers;

Input::TouchInfo Input::default_touch;
std::unordered_map<int, int> Input::touch_ids;
Input::TouchInfo Input::touch_info[JTOUCH_MAX_TOUCHES];

std::unordered_map<short, short> Input::mouse_dictionary =
{
	{ SDL_BUTTON_LEFT,                JMOUSE_LEFT                      },
	{ SDL_BUTTON_RIGHT,               JMOUSE_RIGHT                     },
	{ SDL_BUTTON_MIDDLE,              JMOUSE_MIDDLE                    }
};


std::unordered_map<short, short> Input::pad_dictionary =
{
	{ SDL_CONTROLLER_BUTTON_A,                 JPAD_BTN_A            },
	{ SDL_CONTROLLER_BUTTON_B,                 JPAD_BTN_B            },
	{ SDL_CONTROLLER_BUTTON_X,                 JPAD_BTN_X            },
	{ SDL_CONTROLLER_BUTTON_Y,                 JPAD_BTN_Y            },
	{ SDL_CONTROLLER_BUTTON_LEFTSHOULDER,      JPAD_BTN_LB           },
	{ SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,     JPAD_BTN_RB           },
	{ SDL_CONTROLLER_BUTTON_BACK,              JPAD_BTN_BACK         },
	{ SDL_CONTROLLER_BUTTON_START,             JPAD_BTN_START        },
	{ SDL_CONTROLLER_BUTTON_GUIDE,             JPAD_BTN_HOME         },
	{ SDL_CONTROLLER_BUTTON_LEFTSTICK,         JPAD_BTN_LS           },
	{ SDL_CONTROLLER_BUTTON_RIGHTSTICK,        JPAD_BTN_RS           },
	{ SDL_CONTROLLER_BUTTON_DPAD_UP,           JPAD_DPAD_UP          },
	{ SDL_CONTROLLER_BUTTON_DPAD_DOWN,         JPAD_DPAD_DOWN        },
	{ SDL_CONTROLLER_BUTTON_DPAD_LEFT,         JPAD_DPAD_LEFT        },
	{ SDL_CONTROLLER_BUTTON_DPAD_RIGHT,        JPAD_DPAD_RIGHT       }
};

std::unordered_map<short, short> Input::axis_dictionary =
{
	{ SDL_CONTROLLER_AXIS_LEFTX,               JPAD_AXIS_LS_H        },
	{ SDL_CONTROLLER_AXIS_LEFTY,               JPAD_AXIS_LS_V        },
	{ SDL_CONTROLLER_AXIS_RIGHTX,              JPAD_AXIS_RS_H        },
	{ SDL_CONTROLLER_AXIS_RIGHTY,              JPAD_AXIS_RS_V        },
	{ SDL_CONTROLLER_AXIS_TRIGGERLEFT,         JPAD_AXIS_LT          },
	{ SDL_CONTROLLER_AXIS_TRIGGERRIGHT,        JPAD_AXIS_RT          }
};

std::unordered_map<short, short> Input::key_dictionary =
{
	{ SDLK_BACKSPACE,                 JKEY_KEY_BACKSPACE               },
	{ SDLK_TAB,                       JKEY_KEY_TAB                     },
	{ SDLK_RETURN,                    JKEY_KEY_ENTER                   },
	{ SDLK_LSHIFT,                    JKEY_KEY_LSHIFT                  },
	{ SDLK_LCTRL,                     JKEY_KEY_LCTRL                   },
	{ SDLK_PAUSE,                     JKEY_KEY_PAUSE                   },
	{ SDLK_CAPSLOCK,                  JKEY_KEY_CAPSLOCK                },
	{ SDLK_ESCAPE,                    JKEY_KEY_ESC                     },
	{ SDLK_SPACE,                     JKEY_KEY_SPACE                   },
	{ SDLK_PAGEUP,                    JKEY_KEY_PGUP                    },
	{ SDLK_PAGEDOWN,                  JKEY_KEY_PGDN                    },
	{ SDLK_END,                       JKEY_KEY_END                     },
	{ SDLK_HOME,                      JKEY_KEY_HOME                    },
	{ SDLK_LEFT,                      JKEY_KEY_LEFT                    },
	{ SDLK_UP,                        JKEY_KEY_UP                      },
	{ SDLK_RIGHT,                     JKEY_KEY_RIGHT                   },
	{ SDLK_DOWN,                      JKEY_KEY_DOWN                    },
	{ SDLK_PRINTSCREEN,               JKEY_KEY_PRINTSCREEN             },
	{ SDLK_INSERT,                    JKEY_KEY_INS                     },
	{ SDLK_DELETE,                    JKEY_KEY_DEL                     },
	{ SDLK_0,                         JKEY_KEY_0                       },
	{ SDLK_1,                         JKEY_KEY_1                       },
	{ SDLK_2,                         JKEY_KEY_2                       },
	{ SDLK_3,                         JKEY_KEY_3                       },
	{ SDLK_4,                         JKEY_KEY_4                       },
	{ SDLK_5,                         JKEY_KEY_5                       },
	{ SDLK_6,                         JKEY_KEY_6                       },
	{ SDLK_7,                         JKEY_KEY_7                       },
	{ SDLK_8,                         JKEY_KEY_8                       },
	{ SDLK_9,                         JKEY_KEY_9                       },
	{ SDLK_a,                         JKEY_KEY_A                       },
	{ SDLK_b,                         JKEY_KEY_B                       },
	{ SDLK_c,                         JKEY_KEY_C                       },
	{ SDLK_d,                         JKEY_KEY_D                       },
	{ SDLK_e,                         JKEY_KEY_E                       },
	{ SDLK_f,                         JKEY_KEY_F                       },
	{ SDLK_g,                         JKEY_KEY_G                       },
	{ SDLK_h,                         JKEY_KEY_H                       },
	{ SDLK_i,                         JKEY_KEY_I                       },
	{ SDLK_j,                         JKEY_KEY_J                       },
	{ SDLK_k,                         JKEY_KEY_K                       },
	{ SDLK_l,                         JKEY_KEY_L                       },
	{ SDLK_m,                         JKEY_KEY_M                       },
	{ SDLK_n,                         JKEY_KEY_N                       },
	{ SDLK_o,                         JKEY_KEY_O                       },
	{ SDLK_p,                         JKEY_KEY_P                       },
	{ SDLK_q,                         JKEY_KEY_Q                       },
	{ SDLK_r,                         JKEY_KEY_R                       },
	{ SDLK_s,                         JKEY_KEY_S                       },
	{ SDLK_t,                         JKEY_KEY_T                       },
	{ SDLK_u,                         JKEY_KEY_U                       },
	{ SDLK_v,                         JKEY_KEY_V                       },
	{ SDLK_w,                         JKEY_KEY_W                       },
	{ SDLK_x,                         JKEY_KEY_X                       },
	{ SDLK_y,                         JKEY_KEY_Y                       },
	{ SDLK_z,                         JKEY_KEY_Z                       },
	{ SDLK_MENU,                      JKEY_KEY_MENU                    },
	{ SDLK_F1,                        JKEY_KEY_F1                      },
	{ SDLK_F2,                        JKEY_KEY_F2                      },
	{ SDLK_F3,                        JKEY_KEY_F3                      },
	{ SDLK_F4,                        JKEY_KEY_F4                      },
	{ SDLK_F5,                        JKEY_KEY_F5                      },
	{ SDLK_F6,                        JKEY_KEY_F6                      },
	{ SDLK_F7,                        JKEY_KEY_F7                      },
	{ SDLK_F8,                        JKEY_KEY_F8                      },
	{ SDLK_F9,                        JKEY_KEY_F9                      },
	{ SDLK_F10,                       JKEY_KEY_F10                     },
	{ SDLK_F11,                       JKEY_KEY_F11                     },
	{ SDLK_F12,                       JKEY_KEY_F12                     },
	{ SDLK_F13,                       JKEY_KEY_F13                     },
	{ SDLK_F14,                       JKEY_KEY_F14                     },
	{ SDLK_F15,                       JKEY_KEY_F15                     },
	{ SDLK_F16,                       JKEY_KEY_F16                     },
	{ SDLK_NUMLOCKCLEAR,              JKEY_KEY_NUMLOCK                 }
};
