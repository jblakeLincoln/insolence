#ifndef GAMETIME_H
#define GAMETIME_H

#include <chrono>

#include "timespan.h"

struct GameTime {
private:
	typedef std::chrono::high_resolution_clock chrono_hrc;
	typedef std::chrono::time_point<chrono_hrc> chrono_time_point;

	chrono_time_point start;
	chrono_time_point time_last;
	chrono_time_point time_current;

	TimeSpan frame_time;
	TimeSpan timespan;

	bool time_changed;

public:
	GameTime() {
		start = chrono_hrc::now();
	}

	void Update()
	{
		time_last = time_current;
		time_current = chrono_hrc::now();

		frame_time.SetTime(
				std::chrono::duration_cast<std::chrono::microseconds>
				(time_current - time_last).count());

		timespan.SetTime(
				std::chrono::duration_cast<std::chrono::microseconds>
				(time_current - start).count());
	}

	const TimeSpan& GetElapsedTime() const {
		return timespan;
	}
};

#endif
