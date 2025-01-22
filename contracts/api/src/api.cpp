#include "api/api.hpp"

namespace api {

[[eosio::action, eosio::read_only]] get_account_response api::getaccount(const name account)
{
   return get_account_response{.account = account};
}

} // namespace api

// DEBUG (used on testnet)
#ifdef DEBUG
#include "debug.cpp"
#endif
