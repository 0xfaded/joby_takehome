#include <jobysim/actors/aircraft.hpp>

#include <stdexcept>
#include <iostream>

namespace jobysim {
namespace actors {

using std::chrono::duration_cast;

Aircraft::Aircraft(const aircraft::Spec &spec, int seed)
  : spec_{spec},
    state_{State::Moving},
    energy_{spec.capacity_kwh},
    distance_{0.0},
    has_fault_{false},
    rng_(seed) {
}

void Aircraft::pre_advance() {
  if (has_fault_) {
    throw std::runtime_error("precondition failed: aircraft fault_bit is set");
  }
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
  has_fault_ = check_fault(h);
}

void Aircraft::post_advance() {
  if (energy_ <= 0.0) {
    if (state_ != State::Moving) {
      throw std::runtime_error("vehicle discharged while not moving");
    }
    energy_ = 0.0;
    transition(State::Discharged);
  } else if (energy_ >= spec_.capacity_kwh) {
    if (state_ != State::Moving) {
      throw std::runtime_error("vehicle energy increased while not charging");
    }
    energy_ = spec_.capacity_kwh;
    transition(State::Charging);
  }
}

void Aircraft::transition(State state) {
  if (state == State::Moving && state_ != State::Charged) {
    throw std::runtime_error("invalid state transition to moving");
  } else if (state == State::Discharged && state_ != State::Moving) {
    throw std::runtime_error("invalid state transition to discharged");
  } else if (state == State::Charging && state_ != State::Discharged) {
    throw std::runtime_error("invalid state transition to charging");
  } else if (state == State::Charged && state_ != State::Charging) {
    throw std::runtime_error("invalid state transition charged");
  }
  state_ = state;
}

bool Aircraft::clear_fault() {
  bool fault = has_fault_;
  has_fault_ = false;
  return fault;
}

bool Aircraft::check_fault(double h) {
  std::poisson_distribution p(spec_.fault_prob_per_hour * h);
  return p(rng_) >= 1;
}

} // namespace actors
} // namespace jobysim
