#ifndef JOBYSIM_TYPES_HPP
#define JOBYSIM_TYPES_HPP

#include <chrono>
#include <cstdint>

namespace jobysim {

using duration_t = std::chrono::microseconds;
using hours_t = std::chrono::duration<double, std::ratio<3600>>;

} // namespace jobysim

#endif // JOBYSIM_TYPES_HPP
