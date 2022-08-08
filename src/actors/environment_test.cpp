#include <gtest/gtest.h>

#include <jobysim/sim_visitor.hpp>
#include <jobysim/types.hpp>
#include <jobysim/actors/environment.hpp>
#include <jobysim/actors/aircraft.hpp>
#include <jobysim/aircraft/spec.hpp>

#include <memory>
#include <chrono>

using namespace jobysim;

class EnvironmentTest : public ::testing::Test {
 protected:
  using Spec = aircraft::Spec;
  using Environment = actors::Environment;
  using Aircraft = actors::Aircraft;
  using State = Aircraft::State;

  void SetUp() override {
    environment = std::make_shared<Environment>(num_chargers);
    aircraft = std::make_shared<Aircraft>(test_spec);
  }

  static void advance(Environment &environment, duration_t t) {
    SimVisitor visitor;

    visitor.pre_advance(environment);
    visitor.advance(environment, t);
    visitor.post_advance(environment);
  }

  static constexpr int num_chargers = 3;

  static const Spec test_spec;
  std::shared_ptr<Aircraft> aircraft;
  std::shared_ptr<Environment> environment;
};

const aircraft::Spec EnvironmentTest::test_spec{"test", 100.0, 100.0, 1.0, 1.0, 5, 0.1};

TEST_F(EnvironmentTest, AircraftChargeCycle) {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  environment->add_aircraft(aircraft);

  auto &spec = aircraft->spec();
  hours_t hours_until_empty {spec.capacity_kwh / spec.discharge_kwh_per_hour()};
  hours_t hours_until_full {spec.capacity_kwh / spec.charge_kwh_per_hour()};

  duration_t time_until_empty {duration_cast<duration_t>(hours_until_empty)};
  duration_t time_until_full {duration_cast<duration_t>(hours_until_full)};

  duration_t tick = 1s;
  ASSERT_EQ(aircraft->state(), State::Moving);
  for (duration_t t{0}; t < time_until_empty; t += tick) {
    advance(*environment, tick);
  }
  ASSERT_EQ(aircraft->state(), State::Moving);

  // we want to observe the state transition from discharged -> waiting -> charging
  // because of floating arithmetic, is may happen on either this or next tick.
  int discharged_count{0};
  int waiting_count{0};
  int charging_count{0};
  auto check_states = [this, &discharged_count, &waiting_count, &charging_count]() {
    if (aircraft->state() == State::Discharged) discharged_count += 1;
    if (aircraft->state() == State::Waiting) waiting_count += 1;
    if (aircraft->state() == State::Charging) charging_count += 1;
  };

  // advance actors according to how a SimVisitor would
  for (int i = 0; i < 2; i += 1) {
    // pre order
    check_states(); environment->pre_advance();
    check_states(); aircraft->pre_advance();

    // post order
    check_states(); aircraft->advance(tick);
    check_states(); environment->advance(tick);

    // post order
    check_states(); aircraft->post_advance();
    check_states(); environment->post_advance();
  }
  check_states();

  ASSERT_EQ(aircraft->state(), State::Charging);

  ASSERT_EQ(discharged_count, 1);
  ASSERT_EQ(waiting_count, 1);
  ASSERT_GE(charging_count, 1); // one or more charging cycles depending on fp
}

TEST_F(EnvironmentTest, AircraftQueueCorrectly) {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  // build fleet of aircraft with differing energy capacities
  constexpr double kwh_base {100.0};
  constexpr double kwh_delta {10.0};
  constexpr size_t fleet_size {2 * num_chargers + 1};
  std::vector<Spec> fleet_spec(fleet_size, test_spec);
  std::vector<std::shared_ptr<Aircraft>> fleet;
  std::vector<duration_t> discharge_times;

  for (size_t i{0}; i < fleet_size; i += 1) {
    auto &spec = fleet_spec.at(i);

    spec.capacity_kwh = kwh_base + kwh_delta * i;
    fleet.push_back(std::make_shared<Aircraft>(spec));
    environment->add_aircraft(fleet.at(i));

    hours_t hours_until_empty {spec.capacity_kwh / spec.discharge_kwh_per_hour()};
    duration_t time_until_empty {duration_cast<duration_t>(hours_until_empty)};
    discharge_times.push_back(time_until_empty);
  }

  duration_t tick = 1s;
  size_t i{0};
  duration_t t{0};
  for (; i < num_chargers; i += 1) {
    ASSERT_EQ(fleet[i]->state(), State::Moving);

    duration_t time_until_empty = discharge_times[i];
    for (; t < time_until_empty + 2*tick; t += tick) {
      advance(*environment, tick);
    }

    ASSERT_EQ(fleet[i]->state(), State::Charging);
    ASSERT_EQ(environment->free_chargers(), num_chargers - i - 1);
    ASSERT_EQ(environment->queue_length(), 0);
  }

  for (; i < fleet_size; i += 1) {
    ASSERT_EQ(fleet[i]->state(), State::Moving);

    duration_t time_until_empty = discharge_times[i];
    for (; t < time_until_empty + 2*tick; t += tick) {
      advance(*environment, tick);
    }

    // next aircraft should be left in waiting state
    ASSERT_EQ(fleet[i]->state(), State::Waiting);
    ASSERT_EQ(environment->free_chargers(), 0);
    ASSERT_EQ(environment->queue_length(), i - num_chargers + 1);
  }

  for (t = 0s; t < 3h; t += tick) {
    advance(*environment, tick);
  }
  // check that chargers are still fully utilized
  ASSERT_EQ(environment->free_chargers(), 0);
}

TEST_F(EnvironmentTest, TotalEnergyMatchesTotalDistance) {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  // build fleet of aircraft with differing energy capacities
  constexpr size_t fleet_size {20*num_chargers + 1};
  std::vector<std::shared_ptr<Aircraft>> fleet;

  hours_t test_hours{10};
  duration_t test_duration {duration_cast<duration_t>(test_hours)};

  // we need to subtract off the initial period when all aircraft are charged
  hours_t hours_until_empty {test_spec.capacity_kwh / test_spec.discharge_kwh_per_hour()};
  double total_energy {(test_hours - hours_until_empty).count() *
    num_chargers * test_spec.charge_kwh_per_hour()};

  for (size_t i{0}; i < fleet_size; i += 1) {
    fleet.push_back(std::make_shared<Aircraft>(test_spec));
    environment->add_aircraft(fleet.at(i));

    total_energy += test_spec.capacity_kwh;
  }

  duration_t tick = 1s;
  for (duration_t t{0}; t < test_duration; t += tick) {
    advance(*environment, tick);
  }

  double remaining_energy = std::accumulate(fleet.begin(), fleet.end(), 0.0,
      [](double total, auto &aircraft) {
        return total + aircraft->energy();
      });

  double total_expected_distance {(total_energy - remaining_energy) * test_spec.kwh_per_mile};
  double total_distance = std::accumulate(fleet.begin(), fleet.end(), 0.0,
      [](double total, auto &aircraft) {
        return total + aircraft->distance();
      });

  // fpe error due to discrete time tick
  EXPECT_NEAR(total_distance, total_expected_distance, 0.001*total_expected_distance);
}

