/*
  Name: GLtiming.hpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 18/09/10
  Description: perform timing in OpenGL application
*/

#ifndef _GL_TIMING_HPP_
#define	_GL_TIMING_HPP_

#ifndef ull
typedef unsigned long long ull;
#endif
//general note: it's REALLY IMPOSSIBLE to get an overflow working with 64bit variables!

class Timer {
  private:
    //timer info
    bool performance;           //is a performance counter available?
    ull frequency;              //timer frequency (tick/s)
    double resolution;          //timer resolution (s/tick)

    bool active;                //is it running?

    double walkRatio;           //elapsed time speed, e.g. 1.0 means real tick counter
                                //only "elapsed" it's affected by this value

    //you must provide BOTH of these functions every iteration (one iteration at least!)
    ull start;                  //bias tick start value of current iteration
    ull elapsed;                //total ticks elapsed (sum of time deltas)

    //useful variables to FPS counter
    ull partialElapsed;         //partial tick counter
    unsigned int frame;         //frame counter (assume a frame is rendered every "grabStart()"/"updateElapsed()" call)
    unsigned int FPS;           //FPS counter is updated only every real second
    
  public:
    Timer();                         //set timer info and initialize other variables
    ~Timer();                        //destoy timer

    bool getPerformance();           //is a performance counter available?
    ull getFrequency();              //return timer frequency
    double getResolution();          //return timer resolution

    bool getActive();                //is it running?
    void setWalkRatio(double ratio); //set "walkRatio" value
    double getWalkRatio();           //how much is timer fast?

    void grabStart();                //grab "start" value (at the START of the iteration)
    void updateElapsed();            //update "elapsed" value and FPS counter variables (in the END of the iteration)
    
    double getTime();                //return current "elapsed" in sec

    //append current "elapsed" properly formatted to "str":
    //clock mode: HHh MMm SSs
    void getTimeStrClock(char *str);
    //chronometer mode: HH:MM SS, prec is number of digits after decimal point (max 3)
    void getTimeStrChrono(char *str, unsigned int prec);
    
    unsigned int getFPS();           //return FPS (always the same value all through a second)
    //hint: use it to check if "wasteUntil" is working properly (i.e. it REALLY wastes time)
    //note FPS is slightly smaller than 1/sec using "wasteUntil"
    
    void waste(double sec);          //waste time specified, since THIS CALL
                                     //extremely more accurate than "Sleep" with a performance counter
    void wasteUntil(double sec);     //waste time specified, since LAST "grabStart" CALL,
                                     //do nothing if this time has already been passed
    //note: "grabStart", "updateElapsed", "getFPS", "waste"
    //and "wasteUntil" do nothing if timer is paused

    void enable();                   //timer's running (you must activate it once at least)
    void disable();                  //timer's paused
    void change();                   //switch between running/paused mode

    void reset();                    //set "elapsed","partialElapsed","frame" and "FPS" to zero
    //use them to handle timer with keyboard!
};

/*Simple usage:
Timer timer;
timer.enable();

for(...) {
  timer.grabStart();
  ...
  timer.wasteUntil(...);
  timer.updateElapsed();
}*/

#endif
