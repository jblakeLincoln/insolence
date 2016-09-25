#ifndef INSOLENCE_ANDROID
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "../insolence_dll.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <cstdint>
#include <vector>

#include "audio_track.h"

/* TODO Issue #17: How much should AudioManager hold? */
struct INSOLENCE_API AudioManager
{
private:
	AudioManager() {}

	struct WaveHeader
	{
		char chunk_id[4];
		uint32_t chunk_size;
		char format[4];
		char subchunk_id[4];
		uint32_t subchunk_size;
		uint16_t audio_format;
		uint16_t num_channels;
		uint32_t sample_rate;
		uint32_t bytes_per_second;
		uint16_t block_align;
		uint16_t bits_per_sample;
		char data_chunk_id[4];
		uint32_t data_size;
	};

	ALCdevice *device;
	ALCcontext *context;
	std::vector<AudioTrack*> tracks;

	/**
	 * Open a RIFF WAVE file and load it to an alBuffer;
	 *
	 * \param filename  WAVE file to load.
	 * \param buffer    Buffer to be allocated with alGenBuffers.
	 * \return          True on success with allocated buffer,
	 *                  otherwise false with a NULL buffer.
	 */
	static bool LoadWaveFile(const char *filename, ALuint *buffer);

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
	 * Initialise an AudioManager, which is used to spawn and handle audio
	 * tracks.
	 *
	 * \return	Pointer to new AudioManager.
	 */
	static AudioManager* Create();
};

#endif
#endif
