#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds){ 
    string hour_, min_, sec_;
    int hour = (int)(seconds / 3600);
    int minutes = (int)(seconds / 60 % 60);
    int secs = (int)(seconds % 60);
    if (hour < 10){
        hour_ = "0" + std::to_string((hour));
    } else {
        hour_ = std::to_string(hour);
    };
    if (minutes < 10){
        min_ = "0" + std::to_string(minutes);
    } else {
        min_ = std::to_string(minutes);
    };
    if (secs < 10){
        sec_ = "0" + std::to_string(secs);
    } else {
        sec_ = std::to_string(secs);
    };
    return (hour_ + ":" + min_ + ":" + sec_);
}