#include "input.h"

std::bitset<256> Input::keys;
std::bitset<256> Input::keys_prev;
std::bitset<256> Input::keys_up;
std::bitset<256> Input::keys_down;
std::map<short, short> Input::key_dictionary;

std::bitset<3> Input::mouse;
std::bitset<3> Input::mouse_prev;
std::bitset<3> Input::mouse_up;
std::bitset<3> Input::mouse_down;

GLFWwindow *Input::active_window = NULL;
