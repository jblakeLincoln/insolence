#ifndef AUDIO_TRACK_H
#define AUDIO_TRACK_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "audio_track.h"

/* TODO Issue #19: Logging. */
struct AudioTrack
{
friend class AudioManager;
protected:
	ALuint buffer;
	ALuint source;

	AudioTrack() {}
	~AudioTrack();
public:

	/**
	 * Begin playback of loaded buffer.
	 */
	void Play();

	/**
	 * Whether the source should loop playback at completion.
	 */
	void ShouldLoop(bool b);
};

#endif