#include <jobysim/sim_executor.hpp>
#include <jobysim/sim_visitor.hpp>

#include <chrono>
#include <thread>

namespace jobysim {

using namespace jobysim::actors;

SimExecutor::SimExecutor(
    const std::shared_ptr<Actor>& root,
    duration_t tick,
    duration_t wallclock_tick_duration)
  : root_{root}, tick_{tick}, wallclock_tick_duration_{wallclock_tick_duration} {
}

void SimExecutor::execute(duration_t sim_duration) {
  using std::chrono::duration_cast;

  const bool realtime{wallclock_tick_duration_ > duration_t{0}};

  // for small wallclock_tick_duration, inprecision in sleep_until can drastically increase runtime.
  // Use a simple estimator to estimate the necessary adjustment to wallkclock_tick_duration.
  auto loop_start = realtime ? std::chrono::system_clock::now() : std::chrono::system_clock::time_point{};

  constexpr int beta = 10;
  duration_t adjustment{0};

  int iterations{0};

  for (duration_t t{0}; t < sim_duration; t += tick_) {
    auto start = realtime ? std::chrono::system_clock::now() : std::chrono::system_clock::time_point{};

    SimVisitor visitor{};
    visitor.pre_advance(*root_);
    visitor.advance(*root_, tick_);
    visitor.post_advance(*root_);

    if (realtime) {

      iterations += 1;
      auto average_iteration = (start - loop_start) / iterations;
      auto estimated_error = wallclock_tick_duration_ - duration_cast<duration_t>(average_iteration);

      adjustment += estimated_error / beta;

      // if wait_time becomes negative due to adjustment, this means the simulation is running
      // slower than realtime so don't sleep
      auto wait_time = wallclock_tick_duration_ + adjustment;
      if (wait_time > duration_t{0}) {
        std::this_thread::sleep_until(start + wait_time);
      } else {
        adjustment = -wallclock_tick_duration_;
      }
    }
  }
}

} // namespace jobysim
