#ifndef JOBYSIM_ACTORS_OBSERVER_HPP
#define JOBYSIM_ACTORS_OBSERVER_HPP

#include <vector>
#include <memory>

#include <jobysim/types.hpp>
#include <jobysim/actors/actor.hpp>

namespace jobysim {
namespace actors {

class Observer : public Actor {
 public:
  virtual ~Observer() {}

  void add_observer(const std::shared_ptr<Observer> &observer) {
    add_child(observer);
  }
};

} // namespace actors
} // namespace jobysim

#endif // JOBYSIM_ACTORS_OBSERVER_HPP
