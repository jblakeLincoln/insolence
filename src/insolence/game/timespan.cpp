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

int TimeSpan::ElapsedMilliseconds() const
{
	return current_time / 1000;
}

int TimeSpan::ElapsedSeconds() const
{
	return current_time / 1000000;
}

int TimeSpan::ElapsedMinutes() const
{
	return ElapsedSeconds() / 60;
}

int TimeSpan::ElapsedHours() const
{
	return ElapsedSeconds() / 3600;
}

int TimeSpan::TotalMilliseconds() const
{
	return ElapsedMilliseconds() % 1000;
}

int TimeSpan::TotalSeconds() const
{
	return ElapsedSeconds() % 60;
}

int TimeSpan::TotalMinutes() const
{
	return ElapsedMinutes() % 60;
}

int TimeSpan::TotalHours() const
{
	return ElapsedHours() % 24;
}

