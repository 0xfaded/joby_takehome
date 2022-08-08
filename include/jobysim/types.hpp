#ifndef JOBYSIM_TYPES_HPP
#define JOBYSIM_TYPES_HPP

#include <chrono>
#include <cstdint>
#include <ostream>

namespace jobysim {

using duration_t = std::chrono::nanoseconds;
using hours_t = std::chrono::duration<double, std::ratio<3600>>;

/// Statistics computed by simulation interface
struct FleetStatistics {
  duration_t average_time_in_flight;
  duration_t average_time_charging;
  duration_t average_time_waiting_to_charge;
  double total_passenger_miles;
  int max_number_of_faults;
};

inline std::ostream &operator<<(std::ostream& stream, const FleetStatistics& stats) {
  using std::chrono::duration_cast;

  stream <<
    "average_time_in_flight: " <<
    duration_cast<hours_t>(stats.average_time_in_flight).count() << "h\n" <<

    "average_time_charging: " <<
    duration_cast<hours_t>(stats.average_time_charging).count() << "h\n" <<

    "average_time_waiting_to_charge: " <<
    duration_cast<hours_t>(stats.average_time_waiting_to_charge).count() << "h\n" <<

    "total_passenger_miles: " << stats.total_passenger_miles << "\n" <<
    "max_number_of_faults: " << stats.max_number_of_faults;

  return stream;
}

} // namespace jobysim

#endif // JOBYSIM_TYPES_HPP
