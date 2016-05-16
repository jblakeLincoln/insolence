#ifdef INSOLENCE_LINKS_OPENAL

#include "audio_manager.h"

AudioManager::~AudioManager()
{
	for(int i = 0; i < tracks.size(); ++i)
		delete tracks[i];

	alutExit();
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

AudioTrack* AudioManager::LoadTrack(const char *filename)
{
	AudioTrack *output;
	ALuint buffer;
	ALuint source;

	tracks.push_back(new AudioTrack());
	output = tracks.back();

	alGenSources((ALuint)1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, -300.f, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	buffer = alutCreateBufferFromFile(filename);

	output->buffer = buffer;
	output->source = source;

	return output;
}

#endif
