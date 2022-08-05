#ifndef JOBYSIM_SIM_VISITOR_HPP
#define JOBYSIM_SIM_VISITOR_HPP

#include <functional>

#include <jobysim/types.hpp>
#include <jobysim/actors/actor.hpp>

namespace jobysim {

class SimVisitor {
 public:
  using Actor = actors::Actor;

  void pre_advance(Actor &actor) const {
    recurse_before(actor, [](Actor &actor) { actor.pre_advance(); });
  }

  void advance(Actor &actor, duration_t t) const {
    recurse_after(actor, [t](Actor &actor) { actor.advance(t); });
  }

  void post_advance(Actor &actor) const {
    recurse_after(actor, [](Actor &actor) { actor.post_advance(); });
  }

 private:
  void recurse_before(Actor &actor, const std::function<void(Actor &actor)> &fn) const {
    fn(actor);
    for (auto &child : actor.children()) {
      recurse_before(*child, fn);
    }
  }

  void recurse_after(Actor &actor, const std::function<void(Actor &actor)> &fn) const {
    for (auto &child : actor.children()) {
      recurse_after(*child, fn);
    }
    fn(actor);
  }
};

} // namespace jobysim

#endif
