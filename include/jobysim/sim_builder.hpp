#ifndef JOBYSIM_SIM_BUILDER_HPP
#define JOBYSIM_SIM_BUILDER_HPP

#include <random>

#include <jobysim/actors/actor.hpp>
#include <jobysim/actors/fleet_observer.hpp>
#include <jobysim/aircraft/spec.hpp>

namespace jobysim {

using ObserverMap = std::map<std::string, std::shared_ptr<actors::FleetObserver>>;
using SimObsMap = std::tuple<std::shared_ptr<actors::Actor>, ObserverMap>;
class SimBuilder {
 public:
  SimBuilder(
      int num_aircraft,
      int num_chargers,
      const std::vector<aircraft::Spec>& aircraft_specs,
      unsigned long seed = std::random_device()());

  SimObsMap build();

 private:
  const int num_aircraft_;
  const int num_chargers_;
  const std::vector<aircraft::Spec> aircraft_specs_;
  std::mt19937 rng_;
};

} // namespace jobysim

#endif
