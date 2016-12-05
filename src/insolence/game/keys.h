#ifndef JKEYS_H
#define JKEYS_H

#define JKEY_MAX_KEYS                        256
#define JMOUSE_MAX_BTNS                        3

#define JKEY_RELEASE                          -1
#define JKEY_PRESS                             1
#define JKEY_HOLD                              2

#define JMOUSE_RELEASE                        -1
#define JMOUSE_CLICK                           1
#define JMOUSE_HOLD                            2

#define JMOUSE_LEFT                            1
#define JMOUSE_RIGHT                           2
#define JMOUSE_MIDDLE                          3

#define JKEY_KEY_BACKSPACE                     8
#define JKEY_KEY_TAB                           9
#define JKEY_KEY_ENTER                        13
#define JKEY_KEY_LSHIFT                       16
#define JKEY_KEY_LCTRL                        17
#define JKEY_KEY_PAUSE                        19
#define JKEY_KEY_CAPSLOCK                     20
#define JKEY_KEY_ESC                          27
#define JKEY_KEY_SPACE                        32
#define JKEY_KEY_PGUP                         33
#define JKEY_KEY_PGDN                         34
#define JKEY_KEY_END                          35
#define JKEY_KEY_HOME                         36
#define JKEY_KEY_LEFT                         37
#define JKEY_KEY_UP                           38
#define JKEY_KEY_RIGHT                        39
#define JKEY_KEY_DOWN                         40
#define JKEY_KEY_PRINTSCREEN                  42
#define JKEY_KEY_INS                          45
#define JKEY_KEY_DEL                          46
#define JKEY_KEY_0                            48
#define JKEY_KEY_1                            49
#define JKEY_KEY_2                            50
#define JKEY_KEY_3                            51
#define JKEY_KEY_4                            52
#define JKEY_KEY_5                            53
#define JKEY_KEY_6                            54
#define JKEY_KEY_7                            55
#define JKEY_KEY_8                            56
#define JKEY_KEY_9                            57
#define JKEY_KEY_A                            65
#define JKEY_KEY_B                            66
#define JKEY_KEY_C                            67
#define JKEY_KEY_D                            68
#define JKEY_KEY_E                            69
#define JKEY_KEY_F                            70
#define JKEY_KEY_G                            71
#define JKEY_KEY_H                            72
#define JKEY_KEY_I                            73
#define JKEY_KEY_J                            74
#define JKEY_KEY_K                            75
#define JKEY_KEY_L                            76
#define JKEY_KEY_M                            77
#define JKEY_KEY_N                            78
#define JKEY_KEY_O                            79
#define JKEY_KEY_P                            80
#define JKEY_KEY_Q                            81
#define JKEY_KEY_R                            82
#define JKEY_KEY_S                            83
#define JKEY_KEY_T                            84
#define JKEY_KEY_U                            85
#define JKEY_KEY_V                            86
#define JKEY_KEY_W                            87
#define JKEY_KEY_X                            88
#define JKEY_KEY_Y                            89
#define JKEY_KEY_Z                            90
#define JKEY_KEY_MENU                         93
#define JKEY_KEY_F1                          112
#define JKEY_KEY_F2                          113
#define JKEY_KEY_F3                          114
#define JKEY_KEY_F4                          115
#define JKEY_KEY_F5                          116
#define JKEY_KEY_F6                          117
#define JKEY_KEY_F7                          118
#define JKEY_KEY_F8                          119
#define JKEY_KEY_F9                          120
#define JKEY_KEY_F10                         121
#define JKEY_KEY_F11                         122
#define JKEY_KEY_F12                         123
#define JKEY_KEY_F13                         124
#define JKEY_KEY_F14                         125
#define JKEY_KEY_F15                         126
#define JKEY_KEY_F16                         127
#define JKEY_KEY_NUMLOCK                     144

#define JPAD_ONE                               0
#define JPAD_TWO                               1
#define JPAD_THREE                             2
#define JPAD_FOUR                              3
#define JPAD_MAX_PADS                          4

#define JPAD_BTN_A                             1
#define JPAD_BTN_B                             2
#define JPAD_BTN_X                             3
#define JPAD_BTN_Y                             4
#define JPAD_BTN_LB                            5
#define JPAD_BTN_RB                            6
#define JPAD_BTN_BACK                          7
#define JPAD_BTN_START                         8
#define JPAD_BTN_HOME                          9
#define JPAD_BTN_LS                           10
#define JPAD_BTN_RS                           11
#define JPAD_DPAD_UP                          12
#define JPAD_DPAD_DOWN                        13
#define JPAD_DPAD_LEFT                        14
#define JPAD_DPAD_RIGHT                       15
#define JPAD_MAX_BTNS                         16

#define JPAD_AXIS_LS_H                         1
#define JPAD_AXIS_LS_V                         2
#define JPAD_AXIS_LT                           3
#define JPAD_AXIS_RS_H                         4
#define JPAD_AXIS_RS_V                         5
#define JPAD_AXIS_RT                           6
#define JPAD_MAX_AXES                          7

#define JBTN_RELEASE                          -1
#define JBTN_PRESS                             1
#define JBTN_HOLD                              2

#define JTOUCH_UP                             -1
#define JTOUCH_DOWN                            1
#define JTOUCH_HOLD                            2

#define JTOUCH_MAX_TOUCHES                     8

#endif
