#ifndef JOBYSIM_ACTORS_ROOT_ACTOR_HPP
#define JOBYSIM_ACTORS_ROOT_ACTOR_HPP

#include <vector>
#include <queue>
#include <memory>

#include <jobysim/types.hpp>
#include <jobysim/actors/actor.hpp>
#include <jobysim/actors/observer.hpp>

namespace jobysim {
namespace actors {

/// Root actor combines a single scene actor representing simulated
/// actors and a single root observer actor observing the scene.
///
/// During pre_advance(), the observer tree is traversed first.
/// During advance() and post_advance(), the scene tree is traversed first.
///
/// Because this actor relies on the specific iteration order of its
/// children, it is declared final and should not be extended.
class RootActor final : public Actor {
 public:
  explicit RootActor(
      const std::shared_ptr<Actor> &scene,
      const std::shared_ptr<Observer> &observer = nullptr);

  virtual ~RootActor() {}
};

} // namespace actors
} // namespace jobysim

#endif // JOBYSIM_ACTORS_ROOT_ACTOR_HPP
