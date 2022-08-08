#include <jobysim/sim_builder.hpp>
#include <jobysim/types.hpp>

#include <jobysim/actors/root_actor.hpp>
#include <jobysim/actors/observer.hpp>
#include <jobysim/actors/environment.hpp>

#include <stdexcept>

namespace jobysim {

using namespace jobysim::actors;

SimBuilder::SimBuilder(
    int num_aircraft,
    int num_chargers,
    const std::vector<aircraft::Spec>& aircraft_specs,
    unsigned long seed)
  : num_aircraft_{num_aircraft}, num_chargers_{num_chargers},
    aircraft_specs_{aircraft_specs}, rng_{seed} {

  if (aircraft_specs_.empty()) {
    throw new std::runtime_error("SimBuilder requires at least one aircraft specification");
  }
}

SimObsMap SimBuilder::build() {
  std::shared_ptr<Environment> environment {std::make_shared<Environment>(num_chargers_)};
  std::shared_ptr<Observer> obs_root {std::make_shared<Observer>()};

  ObserverMap obs_map;
  for (auto &spec : aircraft_specs_) {
    obs_map[spec.name] = std::make_shared<FleetObserver>();
    obs_root->add_observer(obs_map.at(spec.name));
  }

  std::uniform_int_distribution<size_t> spec_i_dist(0, aircraft_specs_.size() - 1);
  for (int i = 0; i < num_aircraft_; i += 1) {
    size_t spec_i {spec_i_dist(rng_)};

    auto &spec = aircraft_specs_.at(spec_i);
    auto aircraft = std::make_shared<Aircraft>(spec, rng_());

    environment->add_aircraft(aircraft);
    obs_map[spec.name]->observe_aircraft(aircraft);
  }

  std::shared_ptr<RootActor> root = std::make_shared<RootActor>(environment, obs_root);
  return {root, obs_map};
}

} // namespace jobysim
