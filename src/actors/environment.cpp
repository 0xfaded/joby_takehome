#include <jobysim/actors/aircraft.hpp>
#include <jobysim/actors/environment.hpp>

#include <stdexcept>
#include <iostream>

namespace jobysim {
namespace actors {

using std::chrono::duration_cast;

Environment::Environment(int num_chargers) : chargers_(num_chargers) {
}

void Environment::add_aircraft(const std::shared_ptr<Aircraft> &aircraft) {
  aircrafts_.push_back(aircraft);
  add_child(aircraft);
}

void Environment::pre_advance() {
  // during pre_advance, we may only modify state
  // of the actors below us in hierarchy, not our state.
  for (auto &aircraft: chargers_) {
    if (aircraft && aircraft->state() == Aircraft::State::Discharged) {
      aircraft->transition_charging();
    }
  }

  for (auto &aircraft : aircrafts_) {
    aircraft->clear_fault();
  }
}

void Environment::post_advance() {
  // enqueue discharged aircraft
  for (auto &aircraft : aircrafts_) {
    if (aircraft->state() == Aircraft::State::Discharged) {
      charger_queue_.push(aircraft);
    }
  }

  // assign aircraft to available chargers
  for (auto &aircraft: chargers_) {
    if (aircraft && aircraft->state() == Aircraft::State::Charged) {
      aircraft->transition_moving();
      aircraft.reset();
    }
    if (!aircraft && !charger_queue_.empty()) {
      aircraft = charger_queue_.front();
      charger_queue_.pop();
    }
  }
}

} // namespace actors
} // namespace jobysim
