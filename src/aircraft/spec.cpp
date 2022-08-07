#include <jobysim/aircraft/spec.hpp>

namespace jobysim {
namespace aircraft {

const Spec AlphaCompany { "Alpha", 120.0, 320.0, 0.6, 1.6, 4, 0.25 };
const Spec BetaCompany { "Beta", 100.0, 100.0, 0.2, 1.5, 5, 0.10 };
const Spec CharlieCompany { "Charlie", 160.0, 220.0, 0.8, 2.2, 3, 0.05 };
const Spec DeltaCompany { "Delta", 90.0, 120.0, 0.62, 0.8, 2, 0.22 };
const Spec EchoCompany { "Echo", 30.0, 150.0, 0.3, 5.8, 2, 0.61 };

const std::vector<Spec> all_companies {
  AlphaCompany,
  BetaCompany,
  CharlieCompany,
  DeltaCompany,
  EchoCompany };


} // namespace aircraft
} // namespace jobysim
