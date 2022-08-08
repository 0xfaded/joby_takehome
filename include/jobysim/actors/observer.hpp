#ifndef JOBYSIM_ACTORS_OBSERVER_HPP
#define JOBYSIM_ACTORS_OBSERVER_HPP

#include <vector>
#include <memory>

#include <jobysim/types.hpp>
#include <jobysim/actors/actor.hpp>

namespace jobysim {
namespace actors {

/// Observation actor which exists in the scene heirachy but
/// does not modify other actors. Unfortunately, constness is not
/// enforced through the type system.
class Observer : public Actor {
 public:
  virtual ~Observer() {}

  /// Add another observer as a child actor to this observer.
  /// @param observer     observer to add to observer heirachy.
  void add_observer(const std::shared_ptr<Observer> &observer) {
    add_child(observer);
  }
};

} // namespace actors
} // namespace jobysim

#endif // JOBYSIM_ACTORS_OBSERVER_HPP
