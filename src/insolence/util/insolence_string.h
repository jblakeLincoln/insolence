#ifndef INSOLENCE_STRING_H
#define INSOLENCE_STRING_H
#ifdef WINDOWS

#include <cstdarg>
#include <cstdio>
#include <cstdlib>


static int vasprintf(char **ret, const char *format, va_list va)
{
	int size;

	if(format == NULL)
		return 0;

	size = _vscprintf(format, va);

	if(size > 0)
	{
		*ret = (char*)malloc(++size);

		if(ret != 0)
			_vsnprintf(*ret, size, format, va);
	}

	return size;
}

static int asprintf(char **ret, const char *format, ...)
{
	va_list args;

	va_start(args, format);
	int size = vasprintf(ret, format, args);
	va_end(args);

	return size;
}

#endif
#endif
