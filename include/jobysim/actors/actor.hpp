#ifndef JOBYSIM_ACTORS_ACTOR_HPP
#define JOBYSIM_ACTORS_ACTOR_HPP

#include <vector>
#include <memory>

#include <jobysim/types.hpp>

namespace jobysim {
namespace actors {

class Actor {
 public:
  virtual ~Actor() {}

  virtual void pre_advance() {}
  virtual void advance(duration_t t) {}
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
