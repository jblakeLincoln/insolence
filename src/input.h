#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdio>
#include <map>
#include <bitset>

#include <glm/vec2.hpp>

#include <GLFW/glfw3.h>

#include "keys.h"

struct Input
{
private:
	Input() {}
	~Input() {}

	/**
	 * Callback for receiving keys and looking up their ASCII values
	 * in our GLFW key dictionary, and finding their state.
	 *
	 * \return	JKEY state.
	 */
	static void key_callback(GLFWwindow *window, int key, int scancode,
			int action, int mods)
	{
		int ascii_key = key_dictionary[key];

		if(ascii_key > 0 && ascii_key < 255)
		{
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
				keys[ascii_key] = true;
			else
				keys[ascii_key] = false;
		}
	}

	static void mouse_callback(GLFWwindow *window, int button, int action,
			int mods)
	{
		if(action == GLFW_PRESS || action == GLFW_REPEAT)
			mouse[button] = true;
		else
			mouse[button] = false;
	}

private:
	static std::bitset<256> keys;
	static std::bitset<256> keys_prev;
	static std::bitset<256> keys_up;
	static std::bitset<256> keys_down;
	static std::map<short, short> key_dictionary;

	static std::bitset<3> mouse;
	static std::bitset<3> mouse_prev;
	static std::bitset<3> mouse_up;
	static std::bitset<3> mouse_down;

	static GLFWwindow *active_window;

public:
	static void AttachWindowToKeyboard(GLFWwindow* window)
	{
		if(key_dictionary.size() == 0)
			FillDictionary();

		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_callback);
	}

	static void SetMouseWindow(GLFWwindow *window)
	{
		active_window = window;
	}

	/**
	 * Update key and mouse states, and sets the current states to previous.
	 */
	static void Update()
	{
		std::bitset<256> key_changes = keys ^ keys_prev;
		std::bitset<3> mouse_changes = mouse ^ mouse_prev;

		keys_down = key_changes & keys;
		keys_up = key_changes & (~keys);
		keys_prev = keys;

		mouse_down = mouse_changes & mouse;
		mouse_up = mouse_changes & (~mouse);
		mouse_prev = mouse;

		double cursor_x, cursor_y;
	}

	static int GetKey(int key)
	{
		if(key < 0 || key > 255)
			return 0;

		if(keys_down[key])
			return JKEY_PRESS;
		if(keys_up[key])
			return JKEY_RELEASE;
		if(keys[key])
			return JKEY_HOLD;

		return 0;
	}

	static int GetMouseButton(int btn)
	{
		if(btn < 0 || btn > 2)
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
		double x, y;

		if(active_window == NULL)
			return glm::vec2(-1);

		glfwGetCursorPos(active_window, &x, &y);

		return glm::vec2(x, y);
	}

private:
	static void FillDictionary()
	{
		key_dictionary[	259	] =		8	;
		key_dictionary[	258	] =		9	;
		key_dictionary[	257	] =		13	;
		key_dictionary[	340	] =		16	;
		key_dictionary[	341	] =		17	;
		key_dictionary[	284	] =		19	;
		key_dictionary[	280	] =		20	;
		key_dictionary[	256	] =		27	;
		key_dictionary[	32	] =		32	;
		key_dictionary[	266	] =		33	;
		key_dictionary[	267	] =		34	;
		key_dictionary[	269	] =		35	;
		key_dictionary[	268	] =		36	;
		key_dictionary[	263	] =		37	;
		key_dictionary[	265	] =		38	;
		key_dictionary[	262	] =		39	;
		key_dictionary[	267	] =		40	;
		key_dictionary[	283	] =		42	;
		key_dictionary[	334	] =		43	;
		key_dictionary[	44	] =		44	;
		key_dictionary[	260	] =		45	;
		key_dictionary[	261	] =		46	;
		key_dictionary[	47	] =		47	;
		key_dictionary[	48	] =		48	;
		key_dictionary[	49	] =		49	;
		key_dictionary[	50	] =		50	;
		key_dictionary[	51	] =		51	;
		key_dictionary[	52	] =		52	;
		key_dictionary[	53	] =		53	;
		key_dictionary[	54	] =		54	;
		key_dictionary[	55	] =		55	;
		key_dictionary[	56	] =		56	;
		key_dictionary[	57	] =		57	;
		key_dictionary[	58	] =		58	;
		key_dictionary[	59	] =		59	;
		key_dictionary[	60	] =		60	;
		key_dictionary[	61	] =		61	;
		key_dictionary[	62	] =		62	;
		key_dictionary[	63	] =		63	;
		key_dictionary[	64	] =		64	;
		key_dictionary[	65	] =		65	;
		key_dictionary[	66	] =		66	;
		key_dictionary[	67	] =		67	;
		key_dictionary[	68	] =		68	;
		key_dictionary[	69	] =		69	;
		key_dictionary[	70	] =		70	;
		key_dictionary[	71	] =		71	;
		key_dictionary[	72	] =		72	;
		key_dictionary[	73	] =		73	;
		key_dictionary[	74	] =		74	;
		key_dictionary[	75	] =		75	;
		key_dictionary[	76	] =		76	;
		key_dictionary[	77	] =		77	;
		key_dictionary[	78	] =		78	;
		key_dictionary[	79	] =		79	;
		key_dictionary[	80	] =		80	;
		key_dictionary[	81	] =		81	;
		key_dictionary[	82	] =		82	;
		key_dictionary[	83	] =		83	;
		key_dictionary[	84	] =		84	;
		key_dictionary[	85	] =		85	;
		key_dictionary[	86	] =		86	;
		key_dictionary[	87	] =		87	;
		key_dictionary[	88	] =		88	;
		key_dictionary[	89	] =		89	;
		key_dictionary[	90	] =		90	;
		key_dictionary[	320	] =		96	;
		key_dictionary[	321	] =		97	;
		key_dictionary[	322	] =		98	;
		key_dictionary[	323	] =		99	;
		key_dictionary[	324	] =		100	;
		key_dictionary[	325	] =		101	;
		key_dictionary[	326	] =		102	;
		key_dictionary[	327	] =		103	;
		key_dictionary[	328	] =		104	;
		key_dictionary[	329	] =		105	;
		key_dictionary[	332	] =		106	;
		key_dictionary[	334	] =		107	;
		key_dictionary[	335	] =		108	;
		key_dictionary[	333	] =		109	;
		key_dictionary[	330	] =		110	;
		key_dictionary[	331	] =		111	;
		key_dictionary[	290	] =		112	;
		key_dictionary[	291	] =		113	;
		key_dictionary[	292	] =		114	;
		key_dictionary[	293	] =		115	;
		key_dictionary[	294	] =		116	;
		key_dictionary[	295	] =		117	;
		key_dictionary[	296	] =		118	;
		key_dictionary[	297	] =		119	;
		key_dictionary[	298	] =		120	;
		key_dictionary[	299	] =		121	;
		key_dictionary[	300	] =		122	;
		key_dictionary[	301	] =		123	;
		key_dictionary[	302	] =		124	;
		key_dictionary[	303	] =		125	;
		key_dictionary[	304	] =		126	;
		key_dictionary[	305	] =		127	;
		key_dictionary[	282	] =		144	;
	}
};

#endif
