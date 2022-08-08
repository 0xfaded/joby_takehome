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

/// Simulated aircraft modelled by a state machine while tracking
/// continous state (energy, distance traveled, num_faults, etc.).
class Aircraft : public Actor {
 public:
  /// simple state machine.
  ///
  /// Valid states are:
  ///   moving -> discharged,
  ///   discharged -> waiting,
  ///   waiting -> charging,
  ///   charging -> charged,
  ///   charged -> moving.
  ///
  /// Other transitions are invalid.
  enum class State {
    Moving,
    Discharged,
    Waiting,
    Charging,
    Charged,
  };

  /// Exception thrown on bad state transition.
  struct BadStateTransition : public std::logic_error {
    BadStateTransition(const std::string &what_arg) : std::logic_error(what_arg) {}
  };

  /// Construct an aircraft according to the properties in spec.
  ///
  /// @param spec   aircraft specifications
  /// @param seed   random seed initializing the fault generation randomness.
  explicit Aircraft(
      const aircraft::Spec &spec,
      unsigned long seed = std::random_device()());
  virtual ~Aircraft() {}

  void advance(duration_t t) override;
  void post_advance() override;

  /// Return the current aircraft state
  State state() const { return state_; }

  /// Return the current distance travelled
  double distance() const { return distance_; };

  /// Return the current aircraft battery energy in kwh.
  double energy() const { return energy_; };

  /// Return the accumulated number of faults encountered.
  int num_faults() const { return num_faults_; };

  /// Return the aircraft specification 
  const aircraft::Spec& spec() const { return spec_; };

  /// Checked transition to Discharged state.
  void transition_discharged() { transition(State::Discharged); }

  /// Checked transition to Waiting state.
  void transition_waiting() { transition(State::Waiting); }

  /// Checked transition to Charging state.
  void transition_charging() { transition(State::Charging); }

  /// Checked transition to Charged state.
  void transition_charged() { transition(State::Charged); }

  /// Checked transition to Moving state.
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
