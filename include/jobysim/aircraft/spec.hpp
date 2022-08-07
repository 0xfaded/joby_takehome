
#ifndef JOBYSIM_AIRCRAFT_SPEC_HPP
#define JOBYSIM_AIRCRAFT_SPEC_HPP

#include <vector>
#include <memory>

#include <jobysim/types.hpp>

namespace jobysim {
namespace aircraft {

struct Spec {
  std::string name;
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

extern const Spec AlphaCompany, BetaCompany, CharlieCompany, DeltaCompany, EchoCompany;
extern const std::vector<Spec> all_companies;


} // namespace aircraft
} // namespace jobysim

#endif // JOBYSIM_AIRCRAFT_SPEC_HPP
