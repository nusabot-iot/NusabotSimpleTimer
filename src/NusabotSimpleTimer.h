#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#ifndef __AVR__
#include <functional>
#endif // __AVR__

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef __AVR__
typedef std::function<void(void)> timer_callback;
#else
typedef void (*timer_callback)();
#endif // __AVR__

class NusabotSimpleTimer {

public:
    // maximum number of timers
    const static int MAX_TIMERS = 10;

    // setTimer() constants
    const static int RUN_FOREVER = 0;
    const static int RUN_ONCE = 1;

    // constructor
    NusabotSimpleTimer();

    // this function must be called inside loop()
    void run();

    // call function f every d milliseconds
    int setInterval(unsigned long d, timer_callback f);

    // call function f once after d milliseconds
    int setTimeout(unsigned long d, timer_callback f);

    // call function f every d milliseconds for n times
    int setTimer(unsigned long d, timer_callback f, int n);

    // destroy the specified timer
    void deleteTimer(int numTimer);

    // restart the specified timer
    void restartTimer(int numTimer);

    // returns true if the specified timer is enabled
    boolean isEnabled(int numTimer);

    // enables the specified timer
    void enable(int numTimer);

    // disables the specified timer
    void disable(int numTimer);

    // enables the specified timer if it's currently disabled,
    // and vice-versa
    void toggle(int numTimer);

    // returns the number of used timers
    int getNumTimers();

    // returns the number of available timers
    int getNumAvailableTimers() { return MAX_TIMERS - numTimers; };

private:
    // deferred call constants
    const static int DEFCALL_DONTRUN = 0;       // don't call the callback function
    const static int DEFCALL_RUNONLY = 1;       // call the callback function but don't delete the timer
    const static int DEFCALL_RUNANDDEL = 2;      // call the callback function and delete the timer

    // find the first available slot
    int findFirstFreeSlot();

    // value returned by the millis() function
    // in the previous run() call
    unsigned long prev_millis[MAX_TIMERS];

    // pointers to the callback functions
    timer_callback callbacks[MAX_TIMERS];

    // delay values
    unsigned long delays[MAX_TIMERS];

    // number of runs to be executed for each timer
    int maxNumRuns[MAX_TIMERS];

    // number of executed runs for each timer
    int numRuns[MAX_TIMERS];

    // which timers are enabled
    boolean enabled[MAX_TIMERS];

    // deferred function call (sort of) - N.B.: this array is only used in run()
    int toBeCalled[MAX_TIMERS];

    // actual number of timers in use
    int numTimers;
};

#endif
