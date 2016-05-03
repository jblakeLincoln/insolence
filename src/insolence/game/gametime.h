#ifndef GAMETIME_H
#define GAMETIME_H

#include <chrono>
#include "timespan.h"

struct GameTime {
private:
	typedef std::chrono::high_resolution_clock chrono_hrc;
	typedef std::chrono::time_point<chrono_hrc> chrono_time_point;

	long unsigned int start;
	long unsigned int time_last;
	long unsigned int time_current;

	TimeSpan frame_time;
	TimeSpan elapsed_time;

public:
	GameTime() {
		start = time_current = std::chrono::duration_cast
			<std::chrono::microseconds>	(chrono_hrc::now()
					.time_since_epoch()).count();

		frame_time = TimeSpan(0);
	}

	void Update()
	{
		time_last = time_current;
		time_current = std::chrono::duration_cast<std::chrono::microseconds>
			(chrono_hrc::now().time_since_epoch()).count();

		frame_time.SetTime(time_current - time_last);
		elapsed_time.SetTime(time_current - start);
	}

	const TimeSpan& GetElapsedTime() const {
		return elapsed_time;
	}

	const TimeSpan& GetFrameTime() const {
		return frame_time;
	}
};

#endif
