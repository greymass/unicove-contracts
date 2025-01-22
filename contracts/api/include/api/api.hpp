#pragma once

#include <eosio.msig/eosio.msig.hpp>
#include <eosio.system/eosio.system.hpp>
#include <eosio/contract.hpp>
#include <eosio/singleton.hpp>

using namespace eosio;
using namespace std;

namespace api {

struct get_account_response
{
   name                                          account;
   std::vector<eosiosystem::delegated_bandwidth> delegations;
   std::vector<eosio::multisig::proposal>        proposals;
   std::optional<eosiosystem::rex_balance>       rexbal;
   std::optional<eosiosystem::rex_fund>          rexfund;
};

class [[eosio::contract("api")]] api : public contract
{
public:
   using contract::contract;

   /**
    * getaccount readonly action
    */
   [[eosio::action]] get_account_response getaccount(const name account);
   using getaccount_action = action_wrapper<"getaccount"_n, &api::getaccount>;

// DEBUG (used to help testing)
#ifdef DEBUG
   [[eosio::action]] void
   cleartable(const name table_name, const optional<name> scope, const optional<uint64_t> max_rows);
   [[eosio::action]] void wipe();
#endif

private:
// DEBUG (used to help testing)
#ifdef DEBUG
   // @debug
   template <typename T>
   void clear_table(T& table, uint64_t rows_to_clear);
#endif
};

} // namespace api
