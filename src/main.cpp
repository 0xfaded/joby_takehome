#include <chrono>

#include <jobysim/types.hpp>
#include <jobysim/sim_visitor.hpp>

#include <jobysim/actors/aircraft.hpp>
#include <jobysim/aircraft/spec.hpp>

using namespace jobysim;
using namespace jobysim::actors;

int main(int argc, char **argv) {
  using namespace std::chrono_literals;

  Aircraft aircraft(aircraft::BravoCompany);
  SimVisitor visitor;

  visitor.advance(aircraft, 1s);
  return 0;
}
