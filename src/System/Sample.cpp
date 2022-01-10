#include <list>
#include "Sample.h"
#include "Serial_output.h"
#include "TimeLib.h"

extern Serial_output output;

std::list<Sample> samples;          // list of samples
uint8_t next_sample_place = 0;      // next sample on real system to be used    

Sample::Sample(uint8_t _hour, uint8_t _minutes, uint8_t _day, uint8_t _month, uint16_t _year, uint16_t _depth)
{
    date.time.hour = _hour;
    date.time.minutes = _minutes;
    date.day = _day;
    date.month = _month;
    date.year = _year;

    // set epoch time, seconds from 1970
    tmElements_t tm;
    tm.Day = _day;
    tm.Hour = _hour;
    tm.Minute = _minutes;
    tm.Month = _month;
    tm.Second = 1;
    tm.Year = _year-1970; // see makeTime function
    date.epoch = makeTime(tm);
    type = unique;
    frequency = 0;
    depth = _depth;
}

Sample::Sample(uint8_t _hour, uint8_t _minutes,  uint8_t _day, uint8_t _month, uint16_t _year, uint16_t _depth, uint8_t _frequency)
{
    Sample(_hour, _minutes, _day, _month, _year, _depth);
    type = repeated;
    frequency = _frequency;
}

Sample::Sample(time_t _epoch, uint16_t _depth, uint8_t _frequency){
    date.time.hour = hour(_epoch);
    date.time.minutes = minute(_epoch);
    date.day = day(_epoch);
    date.month = month(_epoch);
    date.year = year(_epoch);
    depth = _depth;
    frequency = _frequency;
    if(frequency == 0)
        type = unique;
    else
        type = repeated;
}

uint8_t Sample::get_day()
{
    return date.day;
}

uint8_t Sample::get_hour()
{
    return date.time.hour;
}

uint8_t Sample::get_minutes()
{
    return date.time.minutes;
}

uint8_t Sample::get_month()
{
    return date.month;
}

uint16_t Sample::get_year(){
    return date.year;
}

time_t Sample::get_epoch()
{
    return date.epoch;
}

uint16_t Sample::get_depth()
{
    return depth;
}

uint8_t Sample::get_frequency()
{
    return frequency;
}

void Sample::set_frequency(uint8_t _frequency)
{
    frequency = _frequency;
}

uint8_t add_sample(uint8_t _hour, uint8_t _minutes, uint8_t _day, uint8_t _month, uint16_t _year, uint16_t _depth)
{
    Sample sample(_hour, _minutes, _day, _month, _year, _depth);
    uint8_t sample_nb = samples.size();
    bool inserted = false;
    uint8_t i = 0;
    if (samples.size() == 0){
        samples.push_back(sample);
    }else
    {
        // initializing list iterator to beginning
        auto it = samples.begin();

        while (i < sample_nb && !inserted)
        {
            advance(it, i);
            // insert sample before next samples in time if nearer in time
            if (it->get_epoch() > sample.get_epoch())
            {
                samples.insert(it, sample);
                inserted = true;
            }
            else
                i++;
        }
        // if sample was not inserted -> furstest away in time, then insert at the end
        if(!inserted){
            samples.push_back(sample);
        }
    }

    // return position of inserted sample
    return i;
}

uint8_t add_sample(uint8_t _hour, uint8_t _minutes, uint8_t _day, uint8_t _month, uint16_t _year, uint16_t _depth, uint8_t _frequency)
{

    // add a sample without frequency
    uint8_t index = add_sample(_hour, _minutes, _day, _month, _year, _depth);
    
    // add the frequency to the new added sample at index
    auto it = samples.begin();
    advance(it, index);
    it->set_frequency(_frequency);

    return index;
}

uint8_t add_sample(time_t _epoch, uint16_t _depth, uint8_t _frequency)
{

    Sample sample(_epoch, _depth, _frequency);
    uint8_t sample_nb = samples.size();
    bool inserted = false;
    uint8_t i = 0;
    if (samples.size() == 0){
        samples.push_back(sample);
    }else
    {
        // initializing list iterator to beginning
        auto it = samples.begin();

        while (i < sample_nb && !inserted)
        {
            advance(it, i);
            // insert sample before next samples in time if nearer in time
            if (it->get_epoch() > sample.get_epoch())
            {
                samples.insert(it, sample);
                inserted = true;
            }
            else
                i++;
        }
        // if sample was not inserted -> furstest away in time, then insert at the end
        if(!inserted){
            samples.push_back(sample);
        }
    }

    // return position of inserted sample
    return i;
}

time_t get_next_sample_time(){

    auto it = samples.begin();

    return it->get_epoch();
}

Sample get_sample(uint8_t number){

    auto it = samples.begin();
    uint8_t list_size = samples.size();
    if(number >= list_size){
        output.println("Unable to get requested samples, index out of range");
        number = list_size-1;
    }
    advance(it, number);
    Sample sample(it->get_hour(), it->get_minutes(), it->get_day(), it->get_month(), it->get_year(), it->get_depth());
    return sample;
}

uint8_t get_next_sample_place(){
    return next_sample_place;
}

void validate_sample(){
    // log the sample
    output.println("Sample done");
    display_sample(0);
     // set next sample on real hardware
    next_sample_place++;

    auto it = samples.begin();
    if(it->get_frequency() > 0){
        Sample sample = get_sample(0);
        
        // delete the sample from the known list
        samples.pop_front();

        add_sample(sample.get_epoch() + sample.get_frequency() * SECS_PER_DAY, sample.get_depth(), sample.get_frequency());
        
    }else{
        // delete the sample from the known list
        samples.pop_front();
    }
}

void display_samples(){
    auto it = samples.begin();
    for(uint8_t i = 0; i < samples.size(); i++){
        
        output.print("Sample " + String(i+1) + " : ");

        String hour = String(it->get_hour());
        String day = String(it->get_day());
        String minutes = String(it->get_minutes());
        String month = String(it->get_month());
        String year = String(it->get_year());
        String depth = String(it->get_depth());
        String frequency = String(it->get_frequency());

        output.println(hour + "h" + minutes + " on " + day + "." + month + "." + year + " at depth " + depth + "cm and day frequency " + frequency);
        advance(it, 1);
    }
}

void display_sample(uint8_t number){
    
    auto it = samples.begin();
    uint8_t list_size = samples.size();
    if(number >= list_size){
        output.println("Unable to display requested sample, index out of range");
        number = list_size-1;
    }
    advance(it, number);
    output.print("Sample on place " + String(next_sample_place) + " scheduled at : ");

    String hour = String(it->get_hour());
    String day = String(it->get_day());
    String minutes = String(it->get_minutes());
    String month = String(it->get_month());
    String year = String(it->get_year());
    String depth = String(it->get_depth());
    String frequency = String(it->get_frequency());

    output.println(hour + "h" + minutes + " on " + day + "." + month + "." + year + " at depth " + depth + "cm and day frequency " + frequency);
    
}


