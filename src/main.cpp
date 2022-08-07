#include <chrono>
#include <iostream>

#include <jobysim/types.hpp>
#include <jobysim/sim_visitor.hpp>

#include <jobysim/sim_builder.hpp>

using namespace jobysim;
using namespace jobysim::actors;

int main(int argc, char **argv) {
  using namespace std::chrono_literals;

  SimBuilder builder(20, 3, aircraft::all_companies);
  auto [root, fleet_observers] = builder.build();

  SimVisitor visitor;

  for (int i = 0; i < 36000*3; i += 1) {
    visitor.pre_advance(*root);
    visitor.advance(*root, 1s);
    visitor.post_advance(*root);
  }

  for (auto fleet_obs : fleet_observers) {
    int num_aircraft = fleet_obs.second->num_aircraft();
    auto stats = fleet_obs.second->compute_statistics();

    std::cout << fleet_obs.first << "(" << num_aircraft << "):\n" << stats << "\n";
  }

  return 0;
}
