#include <jobysim/actors/aircraft.hpp>

#include <iostream>
#include <map>

namespace jobysim {
namespace actors {

using std::chrono::duration_cast;

Aircraft::Aircraft(const aircraft::Spec &spec, unsigned long seed)
  : spec_{spec},
    state_{State::Moving},
    energy_{spec.capacity_kwh},
    distance_{0.0},
    num_faults_{0},
    rng_(seed) {
}

void Aircraft::advance(duration_t t) {
  double h = duration_cast<hours_t>(t).count();

  if (state_ == State::Moving) {
    energy_ -= h * spec_.discharge_kwh_per_hour();
    distance_ += h * spec_.cruise_mph;
  } else if (state_ == State::Charging) {
    energy_ += h * spec_.charge_kwh_per_hour();
  }

  // TODO[carl]: Clarify if faults occur only while moving
  num_faults_ += check_faults(h);
}

void Aircraft::post_advance() {
  if ((state_ == State::Moving) && (energy_ <= 0.0)) {
    energy_ = 0.0;
    transition_discharged();
  } else if ((state_ == State::Charging) && (energy_ >= spec_.capacity_kwh)) {
    energy_ = spec_.capacity_kwh;
    transition_charged();
  }
}

void Aircraft::transition(State state) {
  if (state == State::Moving && state_ != State::Charged) {
    throw BadStateTransition("invalid state transition to moving");
  } else if (state == State::Discharged && state_ != State::Moving) {
    throw BadStateTransition("invalid state transition to discharged");
  } else if (state == State::Waiting && state_ != State::Discharged) {
    throw BadStateTransition("invalid state transition to waiting");
  } else if (state == State::Charging && state_ != State::Waiting) {
    throw BadStateTransition("invalid state transition to charging");
  } else if (state == State::Charged && state_ != State::Charging) {
    throw BadStateTransition("invalid state transition charged");
  }
  state_ = state;
}

int Aircraft::check_faults(double h) {
  std::poisson_distribution p(spec_.fault_prob_per_hour * h);
  return p(rng_);
}

} // namespace actors
} // namespace jobysim
