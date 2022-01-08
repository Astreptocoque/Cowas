#include <list>
#include "Sample.h"
#include "Serial_output.h"

extern Serial_output output;

std::list<Sample> samples; // list of samples

Sample::Sample(uint8_t _hour, uint8_t _minutes, uint8_t _from_day, uint16_t _depth)
{
    date.time.hour = _hour;
    date.time.minutes = _minutes;
    from_day = _from_day;
    type = unique;
    frequency = 0;
    depth = _depth;
}

Sample::Sample(uint8_t _hour, uint8_t _minutes, uint8_t _from_day, uint16_t _depth, uint8_t _frequency)
{
    date.time.hour = _hour;
    date.time.minutes = _minutes;
    from_day = _from_day;
    type = repeated;
    frequency = _frequency;
    depth = _depth;
}

uint8_t Sample::get_day()
{
    return from_day;
}

uint8_t Sample::get_hour()
{
    return date.time.hour;
}

uint8_t Sample::get_minutes()
{
    return date.time.minutes;
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

// Sample::~Sample(){
//     output.println("Sample deleted");
// }

uint8_t add_sample(uint8_t _hour, uint8_t _minutes, uint8_t _from_day, uint16_t _depth)
{

    Sample sample(_hour, _minutes, _from_day, _depth);
    uint8_t sample_nb = samples.size();
    bool inserted = false;
    uint8_t i = 0;

    if (samples.size() == 0)
        samples.push_back(sample);
    else
    {
        // initializing list iterator to beginning
        auto it = samples.begin();

        while (i < sample_nb && inserted)
        {
            advance(it, i);
            // insert sample before next samples in time if nearer in time
            if (it->get_day() >= _from_day && it->get_hour() >= _hour && it->get_minutes() > _minutes)
            {
                samples.insert(it, sample);
                inserted = true;
            }
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

uint8_t add_sample(uint8_t _hour, uint8_t _minutes, uint8_t _from_day, uint16_t _depth, uint8_t _frequency)
{
    // add a sample without frequency
    uint8_t index = add_sample(_hour, _minutes, _from_day, _depth, _frequency);
    
    // add the frequency to the new added sample at index
    auto it = samples.begin();
    advance(it, index);
    it->set_frequency(_frequency);

    return index;
}

void display_samples(){
    auto it = samples.begin();
    for(uint8_t i = 0; i < samples.size(); i++){
        advance(it, i);
        output.print("Sample " + String(i+1) + " : ");

        String hour = String(it->get_hour());
        String day = String(it->get_day());
        String minutes = String(it->get_minutes());
        String depth = String(it->get_depth());
        String frequency = String(it->get_frequency());

        output.println(hour + "h" + minutes + " in " + day + " days at depth " + depth + "cm and frequency " + frequency);
    }
}

