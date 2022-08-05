
#ifndef JOBYSIM_ACTORS_AIRCRAFT_HPP
#define JOBYSIM_ACTORS_AIRCRAFT_HPP

#include <vector>
#include <memory>
#include <random>

#include <jobysim/types.hpp>
#include <jobysim/actors/actor.hpp>
#include <jobysim/aircraft/spec.hpp>

namespace jobysim {
namespace actors {

class Aircraft : public Actor {
 public:
  // simple state machine.
  // moving -> discharged
  // discharged -> charging
  // charging -> moving
  //
  // other transitions invalid
  enum class State {
    Moving,
    Discharged,
    Charging,
    Charged,
  };

  explicit Aircraft(
      const aircraft::Spec &spec,
      int seed = std::random_device()());
  virtual ~Aircraft() {}

  void pre_advance() override;
  void advance(duration_t t) override;
  void post_advance() override;

  State state() const { return state_; }
  bool clear_fault();

  void transition_discharged() { transition(State::Discharged); }
  void transition_charging() { transition(State::Charging); }
  void transition_charged() { transition(State::Charged); }
  void transition_moving() { transition(State::Moving); }

 private:
  void transition(State state);
  bool check_fault(double h);

  const aircraft::Spec spec_;

  State state_;
  double energy_;
  double distance_;
  bool has_fault_;

  std::mt19937 rng_;
};

} // namespace actors
} // namespace jobysim

#endif // JOBYSIM_ACTORS_AIRCRAFT_HPP
