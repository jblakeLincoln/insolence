#ifndef LOG_H
#define LOG_H

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "../util/insolence_string.h"

#ifdef INSOLENCE_ANDROID
#include <SDL2/SDL.h>
#endif

namespace Log {
	enum LOG {
		NONE,
		INFO,
		WARN,
		ERR,
		FATAL
	};

	static const int Console = WARN;
	static const int File = INFO;
	static char *log_name = NULL;
}

static void log_to_console(Log::LOG &type, const struct tm *time,
		const char *line)
{
	int year	= time->tm_year + 1900;
	int month	= time->tm_mon + 1;
	int day		= time->tm_mday;
	int hour	= time->tm_hour;
	int min		= time->tm_min;
	int sec		= time->tm_sec;

#ifdef INSOLENCE_ANDROID
	SDL_Log("%04d/%02d/%02d %02d:%02d:%02d: %s",
			year, month, day, hour, min, sec, line);
#else
	printf("%04d/%02d/%02d %02d:%02d:%02d: ",
			year, month, day, hour, min, sec);

	for(int i = 0; i < type; ++i)
		printf("*");
	printf(" %s\n", line);
#endif

}

static void log_to_file(Log::LOG &type, const struct tm *time,
		const char *line)
{
	FILE *f;
	int year	= time->tm_year + 1900;
	int month	= time->tm_mon + 1;
	int day		= time->tm_mday;
	int hour	= time->tm_hour;
	int min		= time->tm_min;
	int sec		= time->tm_sec;

#ifdef INSOLENCE_ANDROID
	return;
#endif

	if(Log::log_name == NULL)
	{
		const char *name = "-insolence.txt";
		if(asprintf(&Log::log_name, "%04d%02d%02d-%02d%02d%02d%s",
					year, month, day, hour, min, sec, name) < 0)
			printf("**** Error creating logging statement ****\n");
	}

	f = fopen(Log::log_name, "a");

	if(f == NULL)
		return;

	fprintf(f, "%04d/%02d/%02d %02d:%02d:%02d: %s\n",
			year, month, day, hour, min, sec, line);

	fclose(f);
}

static void log(Log::LOG type, const char *format, ...)
{
	if(type < Log::Console && Log::File == Log::NONE)
		return;

	time_t t_now	= time(0);
	struct tm *now	= localtime(&t_now);

	char *line;
	va_list args;

	va_start(args, format);
	if(vasprintf(&line, format, args) < 0)
		printf("**** Error creating logging statement ****\n");
	va_end(args);

	if(type >= Log::Console)
		log_to_console(type, now, line);

	if(type >= Log::File)
		log_to_file(type, now, line);

	free(line);

	if(type == Log::FATAL)
		exit(1);
}

#endif
