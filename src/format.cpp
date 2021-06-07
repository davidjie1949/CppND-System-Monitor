#include <string>

#include "format.h"

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

std::string Format::Pad(std::string s, char c){
    s.insert(s.begin(), 2 - s.size(), c);
    return s;
}

std::string Format::ElapsedTime(long seconds) { 
    long HH = seconds / (60 * 60);
    long MM = (seconds / 60) % 60;
    long SS = seconds % 60;
    return std::string(Format::Pad(to_string(HH),'0') + ":" + Format::Pad(to_string(MM), '0') + ":" + Format::Pad(to_string(SS), '0'));
}