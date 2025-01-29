#pragma once

#include <eosio.msig/eosio.msig.hpp>
#include <eosio.system/eosio.system.hpp>
#include <eosio.token/eosio.token.hpp>
#include <eosio/contract.hpp>
#include <eosio/singleton.hpp>

using namespace eosio;
using namespace std;

namespace api {

struct get_account_response
{
   name                                          account;
   asset                                         balance;
   std::vector<eosiosystem::delegated_bandwidth> delegations;
   std::vector<eosio::multisig::proposal>        proposals;
   eosiosystem::refund_request                   refund;
   eosiosystem::rex_balance                      rexbal;
   eosiosystem::rex_fund                         rexfund;
};

struct token_definition
{
   name   contract;
   symbol symbol;
};

class [[eosio::contract("api")]] api : public contract
{
public:
   using contract::contract;

   struct [[eosio::table("config")]] config_row
   {
      name   system_contract       = name("eosio");
      name   system_contract_msig  = name("eosio.msig");
      name   system_token_contract = name("eosio.token");
      symbol system_token_symbol   = symbol("EOS", 4);
   };
   typedef eosio::singleton<"config"_n, config_row> config_table;

   [[eosio::action]] void setconfig(const name   system_contract,
                                    const name   system_contract_msig,
                                    const name   system_token_contract,
                                    const symbol system_token_symbol);
   /**
    * account readonly action
    */
   [[eosio::action, eosio::read_only]] get_account_response account(const name account);
   using account_action = action_wrapper<"account"_n, &api::account>;

   [[eosio::action, eosio::read_only]] std::vector<asset>
   balances(const name account, const std::vector<token_definition> tokens, const bool zerobalances);
   using balances_action = action_wrapper<"balances"_n, &api::balances>;

#ifdef DEBUG
   [[eosio::action]] void wipe();
   [[eosio::action]] void reset();
#endif

private:
#ifdef DEBUG
   template <typename T>
   void clear_table(T& table, uint64_t rows_to_clear);
   void reset_singletons();
   void wipe_singletons();
   void wipe_tables();
#endif
};

} // namespace api
