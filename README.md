Forked from https://playground.arduino.cc/Code/SimpleTimer/

# NusabotSimpleTimer
This is (yet another) simple library to launch timed actions.

It's based on millis(), thus it has 1 ms resolution.

It uses polling, so no guarantee can be made about the exact time when a callback is fired. For example, if you setup the library so that it calls a function every 2ms, but this function requires 5ms to complete, then you'll have an invocation every 5ms.

For applications where non-strict timing is enough, not using interrupts avoids potential problems with global variables shared between the interrupt service routine and the main program, and doesn't consume a hardware timer.

## Theory
The base goal is to be able to execute a particular piece of code every n milliseconds, without using interrupts.

The algorithm looks like this:
```cpp
lastMillis = 0
forever do:
    if (millis() - lastMillis > n)
        call the particular piece of code
        lastMillis = millis()
    end
end
```

## API
#### SimpleTimer()
The constructor. You usually need only one SimpleTimer object in a sketch. 

`SimpleTimer timer;`

#### int setInterval(long d, timer_callback f)
Call function f every d milliseconds. The callback function must be declared as void f().

```cpp
void repeatMe() {
    // do something
}

timerId = timer.setInterval(1000, repeatMe);
```
#### int setTimeout(long d, timer_callback f)
Call function f once after d milliseconds. The callback function must be declared as `void f()`. After f has been called, the interval is deleted, therefore the value timerId is no longer valid.

```cpp
void callMeLater() {
    // do something
}

timerId = timer.setTimeout(1000, callMeLater);
```

#### int setTimer(long d, timer_callback f, int n)
Call function f every d milliseconds for n times. The callback function must be declared as void `f()`. After f has been called the specified number of times, the interval is deleted, therefore the value timerId is no longer valid.

```cpp
void repeatMeFiveTimes() {
    // do something
}

timerId = timer.setTimer(1000, repeatMeFiveTimes, 5);
```

#### boolean isEnabled(int timerId)
Returns true if the specified timer is enabled 

```cpp
if(timer.isEnabled(timerId) {
    // do domething
}
```

#### void enable(int timerId)
Enables the specified timer. 

```cpp
timer.enable(timerId);
```

#### void disable(int timerId)
Disables the specified timer.

```cpp
timer.disable(timerId);
```

#### void toggle(int timerId)
Enables the specified timer if it's currently disabled, and vice-versa.

```cpp
timer.toggle(timerId);
```

#### void restartTimer(int timerId)

Causes the specified timer to start counting from "now", i.e. the instant when restartTimer is called. The timer callback is not fired. A use case for this function is for example the implementation of a watchdog timer (pseudocode follows).

```cpp
void wdCallback() {
    alert user or perform action to restore
    program state (e.g. reset the microprocessor)
}

wd_timer_id;

void setup() {
    wd_timer_id = timer.setInterval(10000, wdCallback);
}

void loop() {
    timer.run();

    big complex critical code

    timer.restartTimer(wd_timer_id);
}
```

#### void deleteTimer(int timerId)

Free the specified timerId slot. You should need to call this only if you have interval slots that you don't need anymore. The other timer types are automatically deleted once the specified number of repetitions have been executed. 

#### void getNumTimers()
Return the number of used slots in a timer object.

```cpp
n = timer.getNumTimers();
```

## Example
```cpp
#include <NusabotSimpleTimer.h>
 
// There must be one global SimpleTimer object.
// More SimpleTimer objects can be created and run,
// although there is little point in doing so.
NusabotSimpleTimer timer;
 
// function to be called repeatedly
void RepeatTask() {
  Serial.println("15 second timer");        
}
 
// function to be called just once
void OnceOnlyTask() {
  Serial.println("This timer only triggers once");  
}
 
// function to be called exactly 10 times
void TenTimesTask() {
  static int k = 0;
  k++;
  Serial.print("called ");
  Serial.print(k);
  Serial.println(" / 10 times.");
}

void setup() {
  Serial.begin(9600);
 
  // welcome message
  Serial.println("SimpleTimer Example");
  Serial.println("One timer is triggered every 15 seconds");
  Serial.println("Another timer is set to trigger only once after 10 seconds");
  Serial.println("Another timer is set to trigger 10 times");
  Serial.println();
 
  // timed actions setup
  timer.setInterval(15000, RepeatTask);
  timer.setTimeout(10000, OnceOnlyTask);
  timer.setInterval(1000, DigitalClockDisplay);
  timer.setTimer(1200, TenTimesTask, 10);
}
 
void loop() {
  // this is where the "polling" occurs
  timer.run();
}
```
