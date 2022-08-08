#ifndef JOBYSIM_ACTORS_FLEET_OBSERVER_HPP
#define JOBYSIM_ACTORS_FLEET_OBSERVER_HPP

#include <vector>
#include <map>
#include <memory>

#include <jobysim/types.hpp>
#include <jobysim/actors/observer.hpp>
#include <jobysim/actors/aircraft.hpp>

namespace jobysim {
namespace actors {

/// Observer which monitors how much time is spent in each state by
/// observed aircraft.
class FleetObserver : public Observer {
 public:
  FleetObserver();
  virtual ~FleetObserver() {}

  void advance(duration_t t) override;

  /// Add an aircraft to observe. Aircraft does not become a child actor.
  /// @param aircraft   aircraft to observe.
  void observe_aircraft(const std::shared_ptr<Aircraft> &aircraft);

  /// Return the number of aircraft being observed.
  int num_aircraft() const { return static_cast<int>(aircraft_.size()); }

  /// Compute and return a FleetStatstics from the observed aircraft.
  FleetStatistics compute_statistics() const;

 private:
  std::vector<std::shared_ptr<Aircraft>> aircraft_;
  std::map<Aircraft::State, duration_t> aggregate_state_durations_;
};

} // namespace actors
} // namespace jobysim

#endif // JOBYSIM_ACTORS_FLEET_OBSERVER_HPP
