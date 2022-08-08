#ifndef JOBYSIM_ACTORS_AIRCRAFT_HPP
#define JOBYSIM_ACTORS_AIRCRAFT_HPP

#include <vector>
#include <memory>
#include <random>
#include <stdexcept>

#include <jobysim/types.hpp>
#include <jobysim/actors/actor.hpp>
#include <jobysim/aircraft/spec.hpp>

namespace jobysim {
namespace actors {

class Aircraft : public Actor {
 public:
  // simple state machine.
  // moving -> discharged
  // discharged -> waiting
  // waiting -> charging
  // charging -> charged
  // charged -> moving
  //
  // other transitions invalid
  enum class State {
    Moving,
    Discharged,
    Waiting,
    Charging,
    Charged,
  };
  struct BadStateTransition : public std::logic_error {
    BadStateTransition(const std::string &what_arg) : std::logic_error(what_arg) {}
  };

  explicit Aircraft(
      const aircraft::Spec &spec,
      unsigned long seed = std::random_device()());
  virtual ~Aircraft() {}

  void advance(duration_t t) override;
  void post_advance() override;

  State state() const { return state_; }
  double distance() const { return distance_; };
  double energy() const { return energy_; };
  int num_faults() const { return num_faults_; };
  const aircraft::Spec& spec() const { return spec_; };

  void transition_discharged() { transition(State::Discharged); }
  void transition_waiting() { transition(State::Waiting); }
  void transition_charging() { transition(State::Charging); }
  void transition_charged() { transition(State::Charged); }
  void transition_moving() { transition(State::Moving); }

 private:
  void transition(State state);
  int check_faults(double h);

  const aircraft::Spec spec_;

  State state_;
  double energy_;
  double distance_;
  int num_faults_;

  std::mt19937 rng_;
};

} // namespace actors
} // namespace jobysim

#endif // JOBYSIM_ACTORS_AIRCRAFT_HPP
