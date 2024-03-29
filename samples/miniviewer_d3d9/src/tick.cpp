//----------------------------------------------------------------------------//
// tick.cpp                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <sys/time.h>
#endif

#include "tick.h"



//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

Tick::Tick()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

Tick::~Tick()
{
}

//----------------------------------------------------------------------------//
// Get the current millisecond tick count                                     //
//----------------------------------------------------------------------------//

TICK Tick::getTick()
{
#ifdef _WIN32
	LARGE_INTEGER tick;
	QueryPerformanceCounter(&tick);
	return tick.QuadPart;
#endif

#ifdef __linux__
  struct timeval now;
  gettimeofday(&now, 0);
  return now.tv_sec * 1000 + now.tv_usec / 1000;
#endif
}

double Tick::TickToSecond(TICK tick)
{
#ifdef _WIN32
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	double res = tick / (double)freq.QuadPart;
	return res;
#endif
	return 0.0f;
}

//----------------------------------------------------------------------------//
