#ifndef JOBYSIM_ACTORS_ENVIRONMENT_HPP
#define JOBYSIM_ACTORS_ENVIRONMENT_HPP

#include <vector>
#include <queue>
#include <memory>

#include <jobysim/types.hpp>
#include <jobysim/actors/actor.hpp>
#include <jobysim/aircraft/spec.hpp>

namespace jobysim {
namespace actors {

class Aircraft;

/// Simulated environment in which aircraft are simulated. In particular,
/// this actor handles the charging and queuing of aircraft.
class Environment : public Actor {
 public:

  /// Constructor specifying the number of chargers in the environment
  /// @param num_chargers   the number of available chargers.
  explicit Environment(int num_chargers);
  virtual ~Environment() {}

  /// Add an aircraft to the environment for tracking. Aircraft becomes a child actor.
  /// @param aircraft       aircraft to track.
  void add_aircraft(const std::shared_ptr<Aircraft> &aircraft);

  void pre_advance() override;
  void post_advance() override;

  /// Return the number of aircraft waiting to be charged.
  int queue_length() const { return static_cast<int>(charger_queue_.size()); }

  /// Return the number of free chargers
  int free_chargers() const;

 private:
  std::vector<std::shared_ptr<Aircraft>> aircrafts_;
  std::vector<std::shared_ptr<Aircraft>> chargers_;
  std::queue<std::shared_ptr<Aircraft>> charger_queue_;
};

} // namespace actors
} // namespace jobysim

#endif // JOBYSIM_ACTORS_ENVIRONMENT_HPP
