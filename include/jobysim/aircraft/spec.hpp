
#ifndef JOBYSIM_AIRCRAFT_SPEC_HPP
#define JOBYSIM_AIRCRAFT_SPEC_HPP

#include <vector>
#include <memory>

#include <jobysim/types.hpp>

namespace jobysim {
namespace aircraft {

struct Spec {
  double cruise_mph;
  double capacity_kwh;
  double charge_time_h;
  double kwh_per_mile;
  int passenger_count;
  double fault_prob_per_hour;

  double charge_kwh_per_hour() const {
    return capacity_kwh / charge_time_h;
  }

  double discharge_kwh_per_hour() const {
    return kwh_per_mile * cruise_mph;
  }
};

constexpr Spec AlphaCompany { 120.0, 320.0, 0.6, 1.6, 4, 0.25 };
constexpr Spec BravoCompany { 100.0, 100.0, 0.2, 1.5, 5, 0.10 };
constexpr Spec CharlieCompany { 160.0, 220.0, 0.8, 2.2, 3, 0.05 };
constexpr Spec DeltaCompany { 90.0, 120.0, 0.62, 0.8, 2, 0.22 };
constexpr Spec EchoCompany { 30.0, 150.0, 0.3, 5.8, 2, 0.61 };

} // namespace aircraft
} // namespace jobysim

#endif // JOBYSIM_AIRCRAFT_SPEC_HPP
