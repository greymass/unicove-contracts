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

void noact::message(const string text)
{
   check(false, "Messages cannot be broadcasted to the network.");
}

void noact::nonce(const uint64_t nonce)
{
   check(false, "Nonce values cannot be broadcasted to the network.");
}

} // namespace noact
