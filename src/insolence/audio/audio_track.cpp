#include "audio_track.h"

AudioTrack::~AudioTrack()
{
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
}

void AudioTrack::Play()
{
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);
}

void AudioTrack::ShouldLoop(bool b)
{
	alSourcei(source, AL_LOOPING, b);
}
