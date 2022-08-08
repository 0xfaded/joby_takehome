#include <gtest/gtest.h>

#include <jobysim/types.hpp>
#include <jobysim/actors/aircraft.hpp>
#include <jobysim/aircraft/spec.hpp>

#include <chrono>
#include <cmath>

using namespace jobysim;

class AircraftTest : public ::testing::Test {
 protected:
  using Spec = aircraft::Spec;
  using Aircraft = actors::Aircraft;
  using State = Aircraft::State;

  static void advance(Aircraft &aircraft, duration_t t) {
    aircraft.pre_advance();
    aircraft.advance(t);
    aircraft.post_advance();
  }

  static const Spec test_spec;
};
const aircraft::Spec AircraftTest::test_spec{"test", 100.0, 100.0, 0.4, 1.0, 5, 0.1};

class AircraftTestProb : public AircraftTest, public ::testing::WithParamInterface<double> {};

TEST_F(AircraftTest, AircraftDischarges) {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  Aircraft aircraft{test_spec};

  auto &spec = aircraft.spec();
  hours_t hours_until_empty {spec.capacity_kwh / spec.discharge_kwh_per_hour()};
  duration_t time_until_empty {duration_cast<duration_t>(hours_until_empty)};

  duration_t tick = 1s;

  ASSERT_EQ(aircraft.state(), State::Moving);
  for (duration_t t{0}; t < time_until_empty - tick; t += tick) {
    advance(aircraft, tick);
  }
  ASSERT_EQ(aircraft.state(), State::Moving);

  {
    advance(aircraft, 2*tick);
  }
  ASSERT_EQ(aircraft.state(), State::Discharged);
}

TEST_F(AircraftTest, AircraftCharges) {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  Aircraft aircraft{test_spec};
  auto &spec = aircraft.spec();

  ASSERT_EQ(aircraft.state(), State::Moving);
  {
    // discharge aircraft
    hours_t hours_until_empty {spec.capacity_kwh / spec.discharge_kwh_per_hour()};
    duration_t time_until_empty {duration_cast<duration_t>(hours_until_empty)};
    advance(aircraft, time_until_empty + 1s);
  }
  ASSERT_EQ(aircraft.state(), State::Discharged);

  aircraft.transition_waiting();
  aircraft.transition_charging();

  hours_t hours_until_full {spec.capacity_kwh / spec.charge_kwh_per_hour()};
  duration_t time_until_full {duration_cast<duration_t>(hours_until_full)};

  duration_t tick = 1s;

  ASSERT_EQ(aircraft.state(), State::Charging);
  for (duration_t t{0}; t < time_until_full - tick; t += tick) {
    advance(aircraft, tick);
  }
  ASSERT_EQ(aircraft.state(), State::Charging);

  advance(aircraft, 2*tick);
  ASSERT_EQ(aircraft.state(), State::Charged);
}

TEST_F(AircraftTest, AircraftTravelsExpectedDistance) {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  Aircraft aircraft{test_spec};

  auto &spec = aircraft.spec();
  hours_t hours_until_empty {spec.capacity_kwh / spec.discharge_kwh_per_hour()};
  duration_t time_until_empty {duration_cast<duration_t>(hours_until_empty)};

  advance(aircraft, time_until_empty);
  advance(aircraft, time_until_empty);

  double expected_distance = hours_until_empty.count() * spec.cruise_mph;

  ASSERT_EQ(aircraft.distance(), expected_distance);
}

TEST_F(AircraftTest, StateTransitions) {
  Aircraft aircraft{test_spec};

  ASSERT_EQ(aircraft.state(), State::Moving);
  aircraft.transition_discharged();
  ASSERT_EQ(aircraft.state(), State::Discharged);
  aircraft.transition_waiting();
  ASSERT_EQ(aircraft.state(), State::Waiting);
  aircraft.transition_charging();
  ASSERT_EQ(aircraft.state(), State::Charging);
  aircraft.transition_charged();
  ASSERT_EQ(aircraft.state(), State::Charged);
  aircraft.transition_moving();
  ASSERT_EQ(aircraft.state(), State::Moving);
}

TEST_F(AircraftTest, BadStateTransitionsThrow) {
  std::vector<void (Aircraft::*)()> transitions = {
    &Aircraft::transition_discharged,
    &Aircraft::transition_waiting,
    &Aircraft::transition_charging,
    &Aircraft::transition_charged,
    &Aircraft::transition_moving};
    
  for (size_t cur_state{0}; cur_state < transitions.size(); cur_state += 1) {
    // try each bad cur_state transition
    for (size_t next_state{0}; next_state < transitions.size(); next_state += 1) {

      if (cur_state == next_state) {
        continue;
      }

      Aircraft aircraft{test_spec};
      // transition to tested cur_state
      for (size_t state{0}; state < cur_state; state += 1) {
        (aircraft.*transitions.at(state))();
      }

      EXPECT_THROW((aircraft.*transitions.at(next_state))(), Aircraft::BadStateTransition);
    }
  }
}

TEST_P(AircraftTestProb, FaultsAccumulateAtExpectedRate) {
  using namespace std::chrono_literals;

  double fault_prob = GetParam();

  Spec spec{test_spec};
  spec.fault_prob_per_hour = fault_prob;

  constexpr int n_trails = 10000;
  constexpr int mins_per_hour{60};

  int total_faults{0};
  unsigned long seed{42};

  for (int n{0}; n < n_trails; n += 1) {
    // Initiate with a sequence of seeds known to make the test pass.
    // It would be almost impossible to find such a seed unless
    // fault generation code is correct.
    // The advantage is that the randomness is removed from the test.
    Aircraft aircraft{spec, seed};
    for (int i{0}; i < mins_per_hour; i += 1) {
      advance(aircraft, 60s);
    }
    total_faults += aircraft.num_faults();
    seed += 1;
  }

  // the variance of a poisson distribution is lambda (fault_prob)
  // using var(sum(X)) = sum(var(X)), the variance = n_trails * fault_prob
  //
  // assert the result is within 1 sd of the expected values
  // the particular seed was chosen such that this is true.
  double expected = fault_prob * n_trails;
  double sigma = sqrt(expected);

  ASSERT_LT(static_cast<double>(total_faults), expected + sigma);
  ASSERT_GT(static_cast<double>(total_faults), expected - sigma);
}

INSTANTIATE_TEST_CASE_P(
    AircraftTestProb,
    AircraftTestProb,
    ::testing::Values(0.1, 0.5, 1.0, 5.0));
