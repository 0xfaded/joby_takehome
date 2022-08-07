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
