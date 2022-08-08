#ifndef JOBYSIM_ACTORS_ACTOR_HPP
#define JOBYSIM_ACTORS_ACTOR_HPP

#include <vector>
#include <memory>

#include <jobysim/types.hpp>

namespace jobysim {
namespace actors {

/// Base class for all actors in simulation.
///
/// An actor (optionally) implements a pre_advance, advance, post_advance
/// interface as follows.
///   pre_advance: respond to external changes in state
///   advance: update continuous processes
///   post_advance: update and reestablish state invariants for self
///     and possibly children.
///
class Actor {
 public:
  virtual ~Actor() {}

  /// Called before advance in pre order (parent first then child,
  /// right to left within children). Can be used to communicate
  /// state transitions to children (and left siblings), as well
  /// as respond to state transitions initiated by actors higher
  /// in the heirachy.
  virtual void pre_advance() {}

  /// Called during continuous part of simulation in post order.
  /// Actor MUST NOT make changes to discrete state, as other actors
  /// must be able to reason about each other during the step.
  ///
  /// @param t    The simulated duration.
  virtual void advance(duration_t t) {}

  /// Called after advance in post order (children first, then parent,
  /// left to right within children). Actors must reestablish state
  /// any invariants violated after call to advance(). Children
  /// (and right siblings) may be observed and manipulated during
  /// this phase, since it is called in post order and these actors
  /// will have already reestablished their invariants.
  virtual void post_advance() {}

  const std::vector<std::shared_ptr<Actor>> &children() const {
    return children_;
  }

  // prevent accidental slicing
  Actor(const Actor&) = delete;
  Actor(Actor&&) = delete;
  Actor& operator=(Actor&) = delete;
  Actor& operator=(Actor&&) = delete;

 protected:
  Actor() = default;

  void add_child(const std::shared_ptr<Actor> &child) {
    children_.push_back(child);
  }

  std::vector<std::shared_ptr<Actor>> children_;
};

} // namespace actors
} // namespace jobysim

#endif // JOBYSIM_ACTORS_ACTOR_HPP
