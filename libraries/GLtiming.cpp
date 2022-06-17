/*
  Name: GLtiming.cpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 18/09/10
  Description: perform timing in OpenGL application
*/

#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "GLtiming.hpp"

Timer::Timer() {
  //check if a performance counter is available, otherwise use the processor clock timer
  if(QueryPerformanceFrequency((LARGE_INTEGER *)&frequency)) {
    performance=1;
  } else {
    performance=0;
    frequency=(ull)CLOCKS_PER_SEC;
  }

  resolution=1.0/((double)frequency);
  active=0;
  walkRatio=1.0;

  elapsed=0;
  partialElapsed=0;
  frame=0;
  FPS=0;
}

Timer::~Timer() {}

bool Timer::getPerformance() {return performance;}
ull Timer::getFrequency() {return frequency;}
double Timer::getResolution() {return resolution;}

bool Timer::getActive() {return active;}
void Timer::setWalkRatio(double ratio) {if(ratio>0) walkRatio=ratio;}
double Timer::getWalkRatio() {return walkRatio;}

void Timer::grabStart() {
  if(active) {
    if(performance)
      QueryPerformanceCounter((LARGE_INTEGER *)&start);
    else
      start=(ull)clock();
  }
}

void Timer::updateElapsed() {
  if(active) {
    ull tick;
    if(performance) {
      QueryPerformanceCounter((LARGE_INTEGER *)&tick);
    } else {
      tick=(ull)clock();
    }
    tick-=start;
    elapsed+=(ull)((double)tick*walkRatio);
    partialElapsed+=tick;
    frame++;
    
    //check if a second has passed
    if(partialElapsed>=frequency) {
      FPS=frame;  //FPS is slightly rounded (since the >= in if condition)
      partialElapsed=0;
      frame=0;
    }
  
  }
}

double Timer::getTime() {return ((double)elapsed)*resolution;}

void Timer::getTimeStrClock(char *str) {  
  unsigned int hour,min;
  char temp[12];
  
  ull time=elapsed/frequency;
  hour=time/3600;
  time-=3600*hour;
  min=time/60;
  time-=60*min;

  sprintf(temp,"%.2uh %.2um %.2us",hour,min,time);
  strcat(str,temp);
}

void Timer::getTimeStrChrono(char *str, unsigned int prec) {
  if(prec>3)
    prec=3;

  unsigned int hour,min;
  char temp[13];

  switch(prec) {  //prec becomes 10^prec
    case 0: prec=1; break;
    case 1: prec=10; break;
    case 2: prec=100; break;
    case 3: prec=1000; break;
  }
  
  ull time=(elapsed*prec)/frequency;
  hour=time/(3600*prec);
  time-=3600*prec*hour;
  min=time/(60*prec);
  time-=60*prec*min;
  
  switch(prec) {
    case 1:    sprintf(temp,"%.2u:%.2u %.2u",hour,min,time); break;
    case 10:   sprintf(temp,"%.2u:%.2u %4.1f",hour,min,(float)time/(float)prec); break;
    case 100:  sprintf(temp,"%.2u:%.2u %5.2f",hour,min,(float)time/(float)prec); break;
    case 1000: sprintf(temp,"%.2u:%.2u %6.3f",hour,min,(float)time/(float)prec); break;
  }
  strcat(str,temp);
}

unsigned int Timer::getFPS() {if(active) return FPS; else return 0;}

void Timer::waste(double sec) {
  if(active && sec>0) {
    
    if(performance) {
      ull start,tick;
      QueryPerformanceCounter((LARGE_INTEGER *)&start);
      do {
        QueryPerformanceCounter((LARGE_INTEGER *)&tick);
      } while((double)(tick-start)*resolution<=sec);
    } else {
      clock_t start=clock();
      while((double)(clock()-start)*resolution<=sec){};
    }
  
  }

}

void Timer::wasteUntil(double sec) {
  if(active && sec>0) {

    if(performance) {
      ull tick;
      do {
        QueryPerformanceCounter((LARGE_INTEGER *)&tick);
      } while((double)(tick-start)*resolution<=sec);
    } else {
      while((double)((ull)clock()-start)*resolution<=sec){};
    }
  
  }
}

void Timer::enable() {active=1;}
void Timer::disable() {active=0;}
void Timer::change() {~active;}

void Timer::reset() {
  elapsed=0;
  partialElapsed=0;
  frame=0;
  FPS=0;
}
