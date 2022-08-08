#include <chrono>
#include <iostream>

#include <jobysim/sim.hpp>

using namespace jobysim;

int main(int argc, char **argv) {
  using namespace std::chrono_literals;

  constexpr int num_aircraft = 20;
  constexpr int num_chargers = 3;
  constexpr duration_t sim_duration = 3h;
  constexpr duration_t sim_tick = 1s;
  constexpr duration_t wallclock_tick = sim_tick / 3600;

  SimBuilder builder(num_aircraft, num_chargers, aircraft::all_companies);
  auto [sim_root, fleet_observers] = builder.build();

  SimExecutor executor{sim_root, sim_tick, wallclock_tick};
  executor.execute(sim_duration);

  for (auto fleet_obs : fleet_observers) {
    int num_aircraft = fleet_obs.second->num_aircraft();
    auto stats = fleet_obs.second->compute_statistics();

    std::cout << fleet_obs.first << "(" << num_aircraft << "):\n" << stats << "\n";
  }

  return 0;
}
