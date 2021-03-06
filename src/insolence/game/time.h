#ifndef TIME_H
#define TIME_H

#include "../insolence_dll.h"

#include <chrono>

typedef std::chrono::high_resolution_clock chrono_hrc;

struct INSOLENCE_API Time {
protected:
	double time;
	double modifier;

public:
	Time() :
		time(0),
		modifier(1)
	{}

	Time(double in) {
		time = in;
	}

	virtual ~Time() {}

	/* Returns time in Microseconds. */
	double GetRawTime() const {
		return time;
	}

	/* Get time in format of derived class. */
	virtual double Get() const {
		return time / modifier;
	}

	/* Set time in format of derived class. */
	virtual void Set(double in) {
		time = in * modifier;
	}

	bool operator >(const double& b) const {
		return (Get() < b);
	}

	bool operator <(const double& b) const {
		return (Get() > b);
	}

	static int NowMilliseconds() {
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(chrono_hrc::now().time_since_epoch()).count();
	}
};

struct INSOLENCE_API Milliseconds : Time {
	Milliseconds(double in) { modifier = 1000.0; Set(in); }

};

#endif
