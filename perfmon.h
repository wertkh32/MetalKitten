#pragma once

#include <windows.h>

typedef struct {
     LARGE_INTEGER start;
     LARGE_INTEGER stop;
 } stopWatch;

class perfmon
{
	stopWatch timer;
    LARGE_INTEGER frequency;
	
	float LIToMSecs( LARGE_INTEGER & L)
	{
		return ((float)L.QuadPart /(float)frequency.QuadPart) * 1000 ;
	}
public:
	perfmon(void)
	{
		 timer.start.QuadPart=0;
		 timer.stop.QuadPart=0; 
		 QueryPerformanceFrequency( &frequency ) ;
	}

	void startTimer( ) 
	{
		QueryPerformanceCounter(&timer.start) ;
	}
 
	void stopTimer( ) 
	{
		QueryPerformanceCounter(&timer.stop) ;
	}
 
	float getElapsedTime() 
	{
     LARGE_INTEGER time;
     time.QuadPart = timer.stop.QuadPart - timer.start.QuadPart;
     return LIToMSecs( time) ;
	}

	void print()
	{
		printf("%f\n",getElapsedTime());
	}
};

