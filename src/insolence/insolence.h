#ifndef INSOLENCE_H
#define INSOLENCE_H

#ifdef INSOLENCE_ANDROID
	#include "util/android_utils.h"
	#include "util/android_jni.h"
#endif

#include "ecs/entity_manager.h"
#include "ecs/system.h"
#include "game/camera.h"
#include "game/input.h"
#include "game/game.h"
#include "game/game_loop.h"
#include "game/window.h"

#include "system/mesh_renderable_system.h"
#include "system/simple_particle_renderable_system.h"
#include "system/sprite_renderable_system.h"
#include "system/text_renderable_system.h"

#endif
