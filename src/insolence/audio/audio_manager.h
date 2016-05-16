#ifdef INSOLENCE_LINKS_OPENAL
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "../insolence_dll.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "audio_track.h"

/* TODO Issue #17: How much should AudioManager hold? */
/* TODO Issue #18: AudioManager logging. */
struct INSOLENCE_API AudioManager
{
private:
	ALCdevice *device;
	ALCcontext *context;
	std::vector<AudioTrack*> tracks;

	AudioManager() {}

public:
	/**
	 * Delete the manager and all tracks that are handled by it.
	 */
	~AudioManager();

	/**
	 * Create a new AudioTrack object that is bound to this manager.
	 *
	 * \param filename	Location of .wav file to load.
	 * \return			Pointer to AudioTrack object bound to this manager.
	 */
	AudioTrack* LoadTrack(const char *filename);

	/**
	 * Initialise an AudioManager, which is used to spawn and handle audio tracks.
	 *
	 * \return	Pointer to new AudioManager.
	 */
	static AudioManager* Create()
	{
		AudioManager *output = new AudioManager();
		ALfloat listener_ori[] = { 0.f, 0.f, 1.f, 0.f, 1.f, 0.f };

		alutInit(0, NULL);
		output->device = alcOpenDevice("");
		output->context = alcCreateContext(output->device, NULL);

		if(alcMakeContextCurrent(output->context) == false)
		{
			// Log error
			exit(-1);
		}

		alListener3f(AL_POSITION, 300, 0, 1.f);
		alListener3f(AL_VELOCITY, 0, 0, 0);
		alListenerfv(AL_ORIENTATION, listener_ori);

		return output;
	}
};

#endif
#endif
