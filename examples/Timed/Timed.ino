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
  timer.setTimer(1200, TenTimesTask, 10);
}
 
void loop() {
  // this is where the "polling" occurs
  timer.run();
}