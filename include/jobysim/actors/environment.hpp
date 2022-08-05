#ifndef JOBYSIM_ACTORS_ENVIRONMENT_HPP
#define JOBYSIM_ACTORS_ENVIRONMENT_HPP

#include <vector>
#include <queue>
#include <memory>

#include <jobysim/types.hpp>
#include <jobysim/actors/actor.hpp>
#include <jobysim/aircraft/spec.hpp>

namespace jobysim {
namespace actors {

class Aircraft;

class Environment : public Actor {
 public:
  explicit Environment(int num_chargers);
  virtual ~Environment() {}

  void add_aircraft(const std::shared_ptr<Aircraft> &aircraft);

  void pre_advance() override;
  void post_advance() override;

 private:
  std::vector<std::shared_ptr<Aircraft>> aircrafts_;
  std::vector<std::shared_ptr<Aircraft>> chargers_;
  std::queue<std::shared_ptr<Aircraft>> charger_queue_;
};

} // namespace actors
} // namespace jobysim

#endif // JOBYSIM_ACTORS_ENVIRONMENT_HPP
