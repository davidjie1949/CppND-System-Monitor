#ifndef FORMAT_H
#define FORMAT_H

#include <string>

using std::string;
using std::to_string;

namespace Format {
std::string Pad(std::string s, char c);
std::string ElapsedTime(long seconds);  // TODO: See src/format.cpp
};                                    // namespace Format

#endif