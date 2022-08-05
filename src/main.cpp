#include <chrono>

#include <jobysim/types.hpp>
#include <jobysim/sim_visitor.hpp>

#include <jobysim/actors/aircraft.hpp>
#include <jobysim/actors/environment.hpp>
#include <jobysim/aircraft/spec.hpp>

using namespace jobysim;
using namespace jobysim::actors;

int main(int argc, char **argv) {
  using namespace std::chrono_literals;

  auto alpha0 = std::make_shared<Aircraft>(aircraft::AlphaCompany);
  auto alpha1 = std::make_shared<Aircraft>(aircraft::AlphaCompany);
  auto bravo0 = std::make_shared<Aircraft>(aircraft::AlphaCompany);
  auto bravo1 = std::make_shared<Aircraft>(aircraft::AlphaCompany);

  Environment root(3);
  root.add_aircraft(alpha0);
  root.add_aircraft(alpha1);
  root.add_aircraft(bravo0);
  root.add_aircraft(bravo1);

  SimVisitor visitor;

  for (int i = 0; i < 3600*3; i += 1) {
    visitor.pre_advance(root);
    visitor.advance(root, 1s);
    visitor.post_advance(root);
  }
  return 0;
}
