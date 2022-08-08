#ifndef JOBYSIM_SIM_EXECUTOR_HPP
#define JOBYSIM_SIM_EXECUTOR_HPP

#include <jobysim/types.hpp>
#include <jobysim/actors/actor.hpp>

namespace jobysim {

class SimExecutor {
 public:
  using Actor = actors::Actor;

  SimExecutor(
      const std::shared_ptr<Actor>& root,
      duration_t tick,
      duration_t wallclock_tick_duration);

  void execute(duration_t sim_duration);

 private:
  std::shared_ptr<Actor> root_;
  duration_t tick_;
  duration_t wallclock_tick_duration_;
};

} // namespace jobysim

#endif
