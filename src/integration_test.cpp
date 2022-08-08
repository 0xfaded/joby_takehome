#include <gtest/gtest.h>

#include <jobysim/sim.hpp>
#include <jobysim/aircraft/spec.hpp>

#include <memory>
#include <chrono>

using namespace jobysim;
using namespace std::chrono_literals;


class IntegrationTest : public ::testing::TestWithParam<std::tuple<int, int>> {
 protected:
  using Spec = aircraft::Spec;

  void SetUp() override {
    num_aircraft = std::get<0>(GetParam());
    num_chargers = std::get<1>(GetParam());

    SimBuilder builder(num_aircraft, num_chargers, aircraft::all_companies, seed);
    auto [sim_root, fleet_observers] = builder.build();

    SimExecutor executor{sim_root, sim_tick};
    executor.execute(sim_duration);

    this->fleet_observers = std::move(fleet_observers);
    seed += 1;
  }

  const Spec& get_spec(const std::string& name) const {
    return *std::find_if(aircraft::all_companies.begin(), aircraft::all_companies.end(),
        [&name](auto &spec) { return spec.name == name; });
  }

  static constexpr duration_t sim_duration = 30h;
  static constexpr duration_t sim_tick = 10s;

  ObserverMap fleet_observers;
  int num_aircraft;
  int num_chargers;
  unsigned long seed = 0;
};

TEST_P(IntegrationTest, CorrectDurationSum) {
  using std::chrono::duration_cast;

  const double sim_hours = duration_cast<hours_t>(sim_duration).count();

  for (auto& name_obs : fleet_observers) {
    auto& [name, obs] = name_obs;

    auto stats = obs->compute_statistics();

    if (obs->num_aircraft() == 0) {
      ASSERT_EQ(stats.average_time_in_flight, duration_t{0});
      ASSERT_EQ(stats.average_time_charging, duration_t{0});
      ASSERT_EQ(stats.average_time_waiting_to_charge, duration_t{0});
    } else {
      duration_t total_ave_duration =
        stats.average_time_in_flight +
        stats.average_time_charging +
        stats.average_time_waiting_to_charge;

      double total_ave_hours = duration_cast<hours_t>(total_ave_duration).count();

      // allow for accumulated fpe error
      ASSERT_NEAR(total_ave_hours, sim_hours, sim_hours * 1e-3);
    }
  }
}

TEST_P(IntegrationTest, CorrectEnergySum) {
  using std::chrono::duration_cast;

  const double sim_hours = duration_cast<hours_t>(sim_duration).count();

  double total_used_energy{0.0};
  double total_starting_energy{0.0};
  double total_charging_energy{0.0};

  for (auto& name_obs : fleet_observers) {
    auto& [name, obs] = name_obs;

    auto &spec = get_spec(name);
    auto stats = obs->compute_statistics();

    if (obs->num_aircraft() == 0) {
      continue;
    }

    double average_hours_in_flight = duration_cast<hours_t>(stats.average_time_in_flight).count();
    double average_hours_charging = duration_cast<hours_t>(stats.average_time_charging).count();
    double average_used_energy = average_hours_in_flight * spec.discharge_kwh_per_hour();

    total_used_energy += obs->num_aircraft() * average_used_energy;
    total_starting_energy += obs->num_aircraft() * spec.capacity_kwh;
    total_charging_energy += obs->num_aircraft() * average_hours_charging * spec.charge_kwh_per_hour();
  }

  // allow for simulation fpe
  ASSERT_LE(total_used_energy, 1.001 * (total_starting_energy + total_charging_energy));
}


INSTANTIATE_TEST_CASE_P(
    IntegrationTest,
    IntegrationTest,
    ::testing::Combine(
      ::testing::Values(0, 1, 2, 3, 10, 20, 50, 100),
      ::testing::Values(0, 1, 2, 3)));
