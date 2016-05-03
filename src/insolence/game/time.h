#ifndef TIME_H
#define TIME_H

struct Time
{
protected:
	double time;
	double modifier;

public:
	Time() : time(0), modifier(1) {}

	Time(double in) { time = in; }

	/* Returns time in Microseconds. */
	double GetRawTime() const { return time; }

	/* Get time in format of derived class. */
	virtual double Get() const { return time / modifier; }

	/* Set time in format of derived class. */
	virtual void Set(double in) { time = in * modifier; }

	bool operator >(const double& b) const
	{
		return (Get() < b);
	}

	bool operator <(const double& b) const
	{
		return (Get() > b);
	}
};

struct Milliseconds : Time
{
	Milliseconds(double in) { modifier = 1000.0; Set(in); }

};

#endif
