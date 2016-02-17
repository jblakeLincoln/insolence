#include "timespan.h"

TimeSpan::TimeSpan()
{
	current_time = 0;
}

TimeSpan& TimeSpan::operator+=(const TimeSpan& in)
{
	current_time += in.current_time;
	return *this;
}

TimeSpan& TimeSpan::operator-=(const TimeSpan& in)
{
	current_time -= in.current_time;

	return *this;
}

double TimeSpan::ElapsedMilliseconds() const
{
	return current_time / 1000.0;
}

int TimeSpan::ElapsedSeconds() const
{
	return (current_time / 1000000.f);
}

int TimeSpan::ElapsedMinutes() const
{
	return ((current_time / 1000000.f) / 60.f);
}

int TimeSpan::ElapsedHours() const
{
	return (int)(current_time / (1000*60*60));
}

int TimeSpan::TotalMilliseconds() const
{
	return (int)ElapsedMilliseconds() % 1000;
}

int TimeSpan::TotalSeconds() const
{
	return ElapsedSeconds() % 60;
}

int TimeSpan::TotalMinutes() const
{
	return (((ElapsedMinutes()) / 1000) / 60 % 60);
}

int TimeSpan::TotalHours() const
{
	return (int) ((ElapsedHours() / (1000*60*60)) % 24);
}

