#include "noact/noact.hpp"

namespace noact {

void noact::identityv2(const permission_level permission)
{
   check(false, "Identity requests cannot be broadcasted to the network.");
}

void noact::identityv3(const permission_level permission, const name scope)
{
   check(false, "Identity requests cannot be broadcasted to the network.");
}

} // namespace noact
