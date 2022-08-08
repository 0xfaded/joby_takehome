#ifndef JOBYSIM_SIM_EXECUTOR_HPP
#define JOBYSIM_SIM_EXECUTOR_HPP

#include <jobysim/types.hpp>
#include <jobysim/actors/actor.hpp>

namespace jobysim {

/// Executor which drives the simulation.
class SimExecutor {
 public:
  using Actor = actors::Actor;

  /// Constructor
  /// @param root The actor to simulate.
  /// @param tick The duration advanced in each call to advance()
  /// @param wallclock_tick_duration The wallclock time that each tick should
  ///                                pass achieved by the executor sleeping.
  ///                                Zero duration will avoid calls to sleep().
  SimExecutor(
      const std::shared_ptr<Actor>& root,
      duration_t tick,
      duration_t wallclock_tick_duration);

  /// Execute a simulation of length sim_duration
  /// @param sim_duration The total duration of the simulation.
  void execute(duration_t sim_duration);

 private:
  std::shared_ptr<Actor> root_;
  duration_t tick_;
  duration_t wallclock_tick_duration_;
};

} // namespace jobysim

#endif
