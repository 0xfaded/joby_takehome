#include <jobysim/actors/fleet_observer.hpp>

namespace jobysim {
namespace actors {

FleetObserver::FleetObserver() {
  using namespace std::chrono_literals;
  using State = Aircraft::State;

  // initialize the keys required by compute_statistics()
  // with sentinel values.
  aggregate_state_durations_ = {
    { State::Moving, 0s },
    { State::Charging, 0s },
    { State::Waiting, 0s }
  };
}

void FleetObserver::advance(duration_t t) {
  for (auto &aircraft : aircraft_) {
    aggregate_state_durations_[aircraft->state()] += t;
  }
}

void FleetObserver::observe_aircraft(const std::shared_ptr<Aircraft> &aircraft) {
  aircraft_.push_back(aircraft);
}

FleetStatistics FleetObserver::compute_statistics() const {
  using State = Aircraft::State;

  if (aircraft_.empty()) {
    return FleetStatistics{};
  }

  duration_t total_time_in_flight = aggregate_state_durations_.at(State::Moving);
  duration_t total_time_charging = aggregate_state_durations_.at(State::Charging);
  duration_t total_time_waiting_to_charge = aggregate_state_durations_.at(State::Waiting);

  double total_passenger_miles = std::accumulate(aircraft_.begin(), aircraft_.end(), 0.0,
      [](double sum, const auto& aircraft) {
        return sum + aircraft->distance() * aircraft->spec().passenger_count;
      });

  int max_number_of_faults = std::accumulate(aircraft_.begin(), aircraft_.end(), 0,
      [](int max_faults, const auto& aircraft) {
        return std::max(max_faults, aircraft->num_faults());
      });

  return FleetStatistics {
    total_time_in_flight / aircraft_.size(),
    total_time_charging / aircraft_.size(),
    total_time_waiting_to_charge / aircraft_.size(),
    total_passenger_miles,
    max_number_of_faults };
}

} // namespace actors
} // namespace jobysim
