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

class FleetObserver : public Observer {
 public:
  FleetObserver();
  virtual ~FleetObserver() {}

  void advance(duration_t t) override;

  void observe_aircraft(const std::shared_ptr<Aircraft> &aircraft);
  int num_aircraft() const { return static_cast<int>(aircraft_.size()); }

  FleetStatistics compute_statistics() const;

 private:
  std::vector<std::shared_ptr<Aircraft>> aircraft_;
  std::map<Aircraft::State, duration_t> aggregate_state_durations_;
};

} // namespace actors
} // namespace jobysim

#endif // JOBYSIM_ACTORS_FLEET_OBSERVER_HPP
