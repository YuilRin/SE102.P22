#pragma once
#ifndef __TIMESPAN_H__
#define	__TIMESPAN_H__

#include "define.h"

class TimeSpan
{
public:
	static const UINT64 TicksPerSecond = 10000000;
	static const UINT64 TicksPerMilisecond = 10000;

	TimeSpan(void);
	TimeSpan(UINT64);

	float getMiliSeconds();
	UINT64 getTicks();

	~TimeSpan(void);

	TimeSpan operator +(TimeSpan) const;
	TimeSpan operator +(UINT64) const;

private:
	UINT64 _ticks;
};

#endif // !__TIMESPAN_H__
