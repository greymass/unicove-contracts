#include "contractname/contractname.hpp"

namespace contractname {

[[eosio::action]] void contractname::foo(const string bar) { require_auth(get_self()); }

} // namespace contractname

// DEBUG (used on testnet)
#ifdef DEBUG
#include "debug.cpp"
#endif
