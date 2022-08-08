#ifndef JOBYSIM_SIM_BUILDER_HPP
#define JOBYSIM_SIM_BUILDER_HPP

#include <random>

#include <jobysim/actors/actor.hpp>
#include <jobysim/actors/fleet_observer.hpp>
#include <jobysim/aircraft/spec.hpp>

namespace jobysim {

using ObserverMap = std::map<std::string, std::shared_ptr<actors::FleetObserver>>;
using SimObsMap = std::tuple<std::shared_ptr<actors::Actor>, ObserverMap>;

/// Builder class for RootActor representing a simulated scene with fleet observers.
class SimBuilder {
 public:

  /// Constructor specifying all scene information.
  /// @param num_aircraft   Number of aircraft in the scene. Type will
  ///                       be randomly selected from aircraft_specs.
  /// @param num_chargers   Number of chargers available in the environment.
  /// @param aircraft_specs Specifications the can be assigned aircraft.
  /// @param seed           Randomness for the specification selection.
  SimBuilder(
      int num_aircraft,
      int num_chargers,
      const std::vector<aircraft::Spec>& aircraft_specs,
      unsigned long seed = std::random_device()());

  /// Build and return a tuple of RootActor representing the scene and an
  /// observer map containing a FleetObserver for each Spec passed to the
  /// constructor.
  SimObsMap build();

 private:
  const int num_aircraft_;
  const int num_chargers_;
  const std::vector<aircraft::Spec> aircraft_specs_;
  std::mt19937 rng_;
};

} // namespace jobysim

#endif
