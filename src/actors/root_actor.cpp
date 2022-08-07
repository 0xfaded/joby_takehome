#include <jobysim/actors/root_actor.hpp>

#include <stdexcept>

#include <iostream>
#include <map>

namespace jobysim {
namespace actors {

RootActor::RootActor(
    const std::shared_ptr<Actor> &scene,
    const std::shared_ptr<Observer> &observer) {

  add_child(scene);
  if (observer) {
    add_child(observer);
  }
}

} // namespace actors
} // namespace jobysim
