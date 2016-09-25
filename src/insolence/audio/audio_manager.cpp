#include "../game/log.h"
#include <cerrno>
#include "audio_manager.h"

AudioManager* AudioManager::Create()
{
	AudioManager *output = new AudioManager();
	ALfloat listener_ori[] = { 0.f, 0.f, 1.f, 0.f, 1.f, 0.f };

	ALCdevice *device;
	ALCcontext *context;

	output->device = alcOpenDevice("");
	output->context = alcCreateContext(output->device, NULL);

	if(alcMakeContextCurrent(output->context) == false)
		log(Log::FATAL, "AudioManager - Failed to create audio context");

	alListener3f(AL_POSITION, 0, 0, 1.f);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listener_ori);

	return output;
}

AudioManager::~AudioManager()
{
	alcMakeContextCurrent(context);

	for(int i = 0; i < tracks.size(); ++i)
		delete tracks[i];

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

bool AudioManager::LoadWaveFile(const char *filename, ALuint *buffer)
{
	FILE *file = fopen(filename, "rb");
	uint32_t count;
	unsigned char *wave_data;
	WaveHeader wave_header;
	ALsizei size;
	ALsizei frequency;
	ALenum format;
	ALenum error;

	if(file == NULL)
	{
		const char *err = (errno == ENOENT) ?
			"File not found" : "fopen error";

		log(Log::ERR, "AudioManager (%s) - %s: \"%s\"",
				__FUNCTION__, err, filename);
		return false;
	}
	count = fread(&wave_header, sizeof(wave_header), 1, file);

	if(count != 1)
		return false;

	if ((wave_header.chunk_id[0] != 'R') ||
			(wave_header.chunk_id[1] != 'I') ||
			(wave_header.chunk_id[2] != 'F') ||
			(wave_header.chunk_id[3] != 'F'))
	{
		log(Log::ERR, "AudioManager (%s) - Invalid chunk ID: %c%c%c%c",
				__FUNCTION__,
				wave_header.chunk_id[0], wave_header.chunk_id[1],
				wave_header.chunk_id[2], wave_header.chunk_id[3]);

		fclose(file);
		return false;
	}

	if ((wave_header.format[0] != 'W') ||
			(wave_header.format[1] != 'A') ||
			(wave_header.format[2] != 'V') ||
			(wave_header.format[3] != 'E'))
	{
		log(Log::ERR, "AudioManager (%s) - Invalid format: %c%c%c%c",
				__FUNCTION__,
				wave_header.format[0], wave_header.format[1],
				wave_header.format[2], wave_header.format[3]);

		fclose(file);
		return false;
	}

	if ((wave_header.subchunk_id[0] != 'f') ||
			(wave_header.subchunk_id[1] != 'm') ||
			(wave_header.subchunk_id[2] != 't') ||
			(wave_header.subchunk_id[3] != ' '))
	{
		log(Log::ERR, "AudioManager (%s) - Invalid sub_chunk_id: %c%c%c%c",
				__FUNCTION__,
				wave_header.subchunk_id[0], wave_header.subchunk_id[1],
				wave_header.subchunk_id[2], wave_header.subchunk_id[3]);

		fclose(file);
		return false;
	}

	if(wave_header.subchunk_size > 16)
		fseek(file, sizeof(short), SEEK_CUR);

	size = wave_header.data_size;
	frequency = wave_header.sample_rate;

	if(wave_header.num_channels == 1)
		format = wave_header.bits_per_sample == 8 ?
			AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
	else if(wave_header.num_channels == 2)
		format = wave_header.bits_per_sample == 8 ?
			AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;

	fseek(file, sizeof(WaveHeader), SEEK_SET);
	wave_data = new unsigned char[wave_header.data_size];

	if(wave_data == NULL)
	{
		fclose(file);
		return false;
	}

	count = fread(wave_data, 1, wave_header.data_size, file);

	if(count != wave_header.data_size)
	{
		fclose(file);
		delete[] wave_data;
		return false;
	}

	alGenBuffers(1, buffer);
	alBufferData(*buffer, format, (void*)wave_data, size, frequency);

	if((error = alGetError()) != AL_NO_ERROR)
	{
		const ALchar *err_string = alGetString(error);

		log(Log::ERR, "AudioManager (%s) - alBuffer data error: %s",
				__FUNCTION__, err_string);

		fclose(file);
		delete[] wave_data;
		return false;
	}

	log(Log::INFO, "AudioManager (%s) - Successfully loaded %s",
			__FUNCTION__, filename);

	fclose(file);
	delete[] wave_data;
	return true;
}

AudioTrack* AudioManager::LoadTrack(const char *filename)
{
	AudioTrack *output;
	ALuint buffer;
	ALuint source;
	alcMakeContextCurrent(context);

	if(LoadWaveFile(filename, &buffer) == false)
	{
		log(Log::FATAL, "Audio Manager (%s) - Failed to load \"%s\"",
				__FUNCTION__, filename);
		return NULL;
	}

	tracks.emplace_back(new AudioTrack());
	output = tracks.back();

	alGenSources((ALuint)1, &source);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	output->buffer = buffer;
	output->source = source;

	return output;
}
