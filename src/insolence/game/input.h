#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../insolence_dll.h"

#include <bitset>
#include <cstdio>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/vec2.hpp>

#include "keys.h"

struct INSOLENCE_API Input
{
private:
	Input() {}
	~Input() {}

	static std::unordered_map<short, short> key_dictionary;
	static std::unordered_map<short, short> mouse_dictionary;
	static std::unordered_map<short, short> pad_dictionary;
	static std::unordered_map<short, short> axis_dictionary;

	static std::bitset<JKEY_MAX_KEYS> keys;
	static std::bitset<JKEY_MAX_KEYS> keys_prev;
	static std::bitset<JKEY_MAX_KEYS> keys_up;
	static std::bitset<JKEY_MAX_KEYS> keys_down;

	static std::bitset<JMOUSE_MAX_BTNS> mouse;
	static std::bitset<JMOUSE_MAX_BTNS> mouse_prev;
	static std::bitset<JMOUSE_MAX_BTNS> mouse_up;
	static std::bitset<JMOUSE_MAX_BTNS> mouse_down;

	struct GamePad {
		std::bitset<JPAD_MAX_BTNS> pad_btns;
		std::bitset<JPAD_MAX_BTNS> pad_btns_prev;
		std::bitset<JPAD_MAX_BTNS> pad_btns_up;
		std::bitset<JPAD_MAX_BTNS> pad_btns_down;
		float axes[JPAD_MAX_AXES];
	}; static GamePad pads[JPAD_MAX_PADS];

	static std::unordered_map<int, SDL_GameController*> game_controllers;

public:
	static void OpenJoystick(int id)
	{
		game_controllers[id] = SDL_GameControllerOpen(id);
	}

	static void CloseJoystick(int id)
	{
		SDL_GameController *sdl_pad = game_controllers[id];

		if(sdl_pad == NULL)
			return;

		SDL_GameControllerClose(sdl_pad);
		game_controllers[id] = NULL;
	}

	static void SetGamepadAxis(uint32_t pad, uint32_t btn, int val)
	{
		if(IsPadPresent(pad) == false)
			return;

		if(val < 0)
			val++;

		btn = axis_dictionary[btn];
		pads[pad].axes[btn] = val / 32767.f;
	}

	static void SetGamepadButton(uint32_t pad, uint32_t btn, bool state)
	{
		if(IsPadPresent(pad) == false)
			return;

		btn = pad_dictionary[btn];
		pads[pad].pad_btns[btn] = state;
	}

	static void SetKey(uint32_t key, bool state)
	{
		key = key_dictionary[key];
		keys[key] = state;
	}

	static void SetMouseButton(uint32_t btn, bool state)
	{
		btn = mouse_dictionary[btn];
		mouse[btn] = state;
	}

	/**
	 * Update key and mouse states, and sets the current states to previous.
	 */
	static void Update()
	{
		std::bitset<JKEY_MAX_KEYS> key_changes = keys ^ keys_prev;
		std::bitset<JMOUSE_MAX_BTNS> mouse_changes = mouse ^ mouse_prev;

		keys_down = key_changes & keys;
		keys_up = key_changes & (~keys);
		keys_prev = keys;

		mouse_down = mouse_changes & mouse;
		mouse_up = mouse_changes & (~mouse);
		mouse_prev = mouse;

		for(int i = 0; i < JPAD_MAX_PADS; ++i)
		{
			if(IsPadPresent(i) == false)
				continue;

			GamePad *p = &pads[i];
			std::bitset<JPAD_MAX_BTNS> pad_changes =
				p->pad_btns ^ p->pad_btns_prev;

			p->pad_btns_down = pad_changes & p->pad_btns;
			p->pad_btns_up = pad_changes & (~p->pad_btns);
			p->pad_btns_prev = p->pad_btns;
		}
	}

	static int GetKey(uint32_t key)
	{
		if(key > JKEY_MAX_KEYS - 1)
			return 0;

		if(keys_down[key])
			return JKEY_PRESS;
		if(keys_up[key])
			return JKEY_RELEASE;
		if(keys[key])
			return JKEY_HOLD;

		return 0;
	}

	static int GetMouseButton(uint32_t btn)
	{
		if(btn > JPAD_MAX_BTNS - 1)
			return 0;

		if(mouse_down[btn])
			return JMOUSE_CLICK;
		if(mouse_up[btn])
			return JMOUSE_HOLD;
		if(mouse[btn])
			return JMOUSE_HOLD;

		return 0;
	}

	static glm::vec2 GetCursorPos()
	{
		int x, y;

		SDL_GetMouseState(&x, &y);
		return glm::vec2(x, y);
	}

	static bool IsPadPresent(uint32_t id)
	{
		if(id > JPAD_MAX_PADS - 1)
			return false;

		return game_controllers[id] != NULL;
	}

	static const char* GetPadName(uint32_t pad)
	{
		if(IsPadPresent(pad) == false)
			return NULL;

		return SDL_GameControllerName(game_controllers[pad]);
	}

	static int GetPadButton(uint32_t pad, uint32_t btn)
	{
		if(IsPadPresent(pad) == false || btn > JPAD_MAX_BTNS - 1)
			return 0;

		if(pads[pad].pad_btns_down[btn])
			return JBTN_PRESS;
		if(pads[pad].pad_btns_up[btn])
			return JBTN_RELEASE;
		if(pads[pad].pad_btns[btn])
			return JBTN_HOLD;

		return 0;
	}

	static float GetPadAxis(uint32_t pad, uint32_t axis)
	{
		if(IsPadPresent(pad) == false || axis > JPAD_MAX_AXES - 1)
			return 0;

		return pads[pad].axes[axis];
	}
};

#endif
