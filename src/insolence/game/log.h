#ifndef LOG_H
#define LOG_H

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <ctime>

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

static void log_to_console(Log::LOG type, const struct tm *time,
		const char *format, va_list *args)
{
	int year	= time->tm_year + 1900;
	int month	= time->tm_mon + 1;
	int day		= time->tm_mday;
	int hour	= time->tm_hour;
	int min		= time->tm_min;
	int sec		= time->tm_sec;

	printf("%04d/%02d/%02d %02d:%02d:%02d: ",
			year, month, day, hour, min, sec);

	for(int i = 0; i < type; ++i)
		printf("*");
	printf(" ");

	vprintf(format, *args);
	printf("\n");
}

static void log_to_file(Log::LOG type, const struct tm *time,
		const char *format, va_list *args)
{
	FILE *f;
	int year	= time->tm_year + 1900;
	int month	= time->tm_mon + 1;
	int day		= time->tm_mday;
	int hour	= time->tm_hour;
	int min		= time->tm_min;
	int sec		= time->tm_sec;

	if(Log::log_name == NULL)
	{
		const char *name = "-insolence.txt";
		asprintf(&Log::log_name, "%04d%02d%02d-%02d%02d%02d%s",
				year, month, day, hour, min, sec, name);
	}

	f = fopen(Log::log_name, "a");

	if(f == NULL)
		return;

	fprintf(f, "%04d/%02d/%02d %02d:%02d:%02d: ",
			year, month, day, hour, min, sec);
	vfprintf(f, format, *args);
	fprintf(f, "\n");

	fclose(f);
}

static void log(Log::LOG type, const char *format, ...)
{
	if(type < Log::Console && Log::File == Log::NONE)
		return;

	time_t t_now	= time(0);
	struct tm *now	= localtime(&t_now);

	va_list args_console;
	va_list args_file;
	va_copy(args_file, args_console);

	if(type >= Log::Console)
	{
		va_start(args_console, format);
		log_to_console(type, now, format, &args_console);
		va_end(args_console);
	}

	if(type >= Log::File)
	{
		va_start(args_file, format);
		log_to_file(type, now, format, &args_file);
		va_end(args_file);
	}

	if(type == Log::FATAL)
		exit(1);
}

#endif
