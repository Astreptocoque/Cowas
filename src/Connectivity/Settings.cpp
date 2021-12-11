#include "Settings.h"

extern Serial_output output;

// Variables to store the time
unsigned long currentMillis = 0;
// Int is enough to store the elapsed time
int elapsedTimeUpdateMillis = 0;
unsigned long lastTimeUpdateMillis = 0;
// Variables to store the time
unsigned int days = 0;
unsigned int hours = 0;
unsigned int minutes = 0;
unsigned long seconds = 0;

void update_time()
{

    currentMillis = millis();
    elapsedTimeUpdateMillis = currentMillis - lastTimeUpdateMillis;



    seconds = elapsedTimeUpdateMillis/1000;
    minutes = elapsedTimeUpdateMillis/60000;
    hours = elapsedTimeUpdateMillis/3600000;
    days = elapsedTimeUpdateMillis/86400000;

    output.println("seconds : " + String(seconds));
    output.println("minutes : " + String(minutes));
    output.println("hours : " + String(hours));
    output.println("days : " + String(days));

}

void set_time_manual(){
    output.println("Enter hour");
}

   
