#ifndef TIMESPAN_H
#define TIMESPAN_H

#include "time.h"

struct TimeSpan {
protected:
	double current_time;

public:
	TimeSpan& operator+= (const TimeSpan& in);
	TimeSpan& operator-= (const TimeSpan& in);

	TimeSpan();

	/**
	 * TimeSpan
	 *
	 * \param	Time in microseconds.
	 */
	TimeSpan(double time) { current_time = time; }


	/* Return milliseconds elapsed in the current cycle (i.e. 0-1000) */
	int TotalMilliseconds() const;

	/* Return seconds elapsed in the current cycle (i.e. 0-60) */
	int TotalSeconds() const;

	/* Return minutes elapsed in the current cycle (i.e. 0-60)*/
	int TotalMinutes() const;

	/*Return hours elapsed in the current cycle (i.e. 0-23) */
	int TotalHours() const;

	/* Return total number of milliseconds elapsed */
	int ElapsedMilliseconds() const;

	/* Return total number of seconds elapsed */
	int ElapsedSeconds() const;

	/* Return total number of minutes elapsed */
	int ElapsedMinutes() const;

	/* Return total number of hours elapsed */
	int ElapsedHours() const;

	/* Return time in microseconds. */
	double GetTime() const { return current_time;}

	void SetTime(const Time& t) { current_time = t.GetRawTime(); }

	void SetTime(long unsigned int time) { current_time = time;}
};

#endif
