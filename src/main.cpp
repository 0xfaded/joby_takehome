#include <chrono>

#include <jobysim/types.hpp>
#include <jobysim/actor.hpp>
#include <jobysim/sim_visitor.hpp>

using namespace jobysim;

int main(int argc, char **argv) {
  using namespace std::chrono_literals;

  Actor actor;
  SimVisitor visitor;

  visitor.advance(actor, 1s);
  return 0;
}
