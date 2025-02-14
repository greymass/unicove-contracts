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
   checksum256                                   contracthash;
   asset                                         balance;
   std::vector<eosiosystem::delegated_bandwidth> delegations;
   std::vector<eosio::multisig::proposal>        proposals;
   eosiosystem::refund_request                   refund;
   eosiosystem::rex_balance                      rexbal;
   eosiosystem::rex_fund                         rexfund;
   eosiosystem::voter_info                       vote;
   eosiosystem::gifted_ram                       giftedram;
};

struct get_available_response
{
   name account;
   bool available;
};

struct token_definition
{
   name   contract;
   symbol symbol;
};

struct token_supply
{
   token_definition def         = {.contract = name("eosio.token"), .symbol = symbol("EOS", 4)};
   asset            circulating = asset(0, symbol("EOS", 4));
   asset            locked      = asset(0, symbol("EOS", 4));
   asset            max         = asset(0, symbol("EOS", 4));
   asset            supply      = asset(0, symbol("EOS", 4));
};

struct get_network_response
{
   eosiosystem::eosio_global_state global;
   eosiosystem::powerup_state      powerup;
   eosiosystem::exchange_state     ram;
   eosiosystem::rex_pool           rex;
   token_supply                    token;
   int64_t                         ram_gift_bytes = eosiosystem::ram_gift_bytes;
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
      symbol system_ramcore_symbol = eosiosystem::system_contract::ramcore_symbol;
      symbol system_ram_symbol     = eosiosystem::system_contract::ram_symbol;
      symbol system_rex_symbol     = eosiosystem::system_contract::rex_symbol;
      bool   gifted_ram_enabled    = false;
   };
   typedef eosio::singleton<"config"_n, config_row> config_table;

   [[eosio::action]] void setconfig(const name   system_contract,
                                    const name   system_contract_msig,
                                    const name   system_token_contract,
                                    const symbol system_token_symbol,
                                    const symbol system_ramcore_symbol,
                                    const symbol system_ram_symbol,
                                    const symbol system_rex_symbol,
                                    const bool   gifted_ram_enabled);

   [[eosio::action, eosio::read_only]] get_account_response account(const name account);
   using account_action = action_wrapper<"account"_n, &api::account>;

   [[eosio::action, eosio::read_only]] get_available_response available(const name account);
   using available_action = action_wrapper<"available"_n, &api::available>;

   [[eosio::action, eosio::read_only]] std::vector<asset>
   balances(const name account, const std::vector<token_definition> tokens, const bool zerobalances);
   using balances_action = action_wrapper<"balances"_n, &api::balances>;

   [[eosio::action, eosio::read_only]] eosiosystem::abi_hash contracthash(const name account);
   using contracthash_action = action_wrapper<"contracthash"_n, &api::contracthash>;

   [[eosio::action, eosio::read_only]] get_network_response network();
   using network_action = action_wrapper<"network"_n, &api::network>;

   [[eosio::action, eosio::read_only]] eosiosystem::powerup_state powerup();
   using powerup_action = action_wrapper<"powerup"_n, &api::powerup>;

   [[eosio::action, eosio::read_only]] eosiosystem::rex_pool rex();
   using rex_action = action_wrapper<"rex"_n, &api::rex>;

   [[eosio::action, eosio::read_only]] eosiosystem::gifted_ram giftedram(const name account);
   using giftedram_action = action_wrapper<"giftedram"_n, &api::giftedram>;

   [[eosio::action, eosio::read_only]] eosiosystem::eosio_global_state global();
   using global_action = action_wrapper<"global"_n, &api::global>;

   [[eosio::action, eosio::read_only]] eosiosystem::exchange_state ram();
   using ram_action = action_wrapper<"ram"_n, &api::ram>;

   [[eosio::action, eosio::read_only]] token_supply supply(const token_definition def);
   using supply_action = action_wrapper<"supply"_n, &api::supply>;

   [[eosio::action, eosio::read_only]] eosiosystem::refund_request refund(const name account);
   using refund_action = action_wrapper<"refund"_n, &api::refund>;

   [[eosio::action, eosio::read_only]] std::vector<eosiosystem::delegated_bandwidth> delegations(const name account);
   using delegated_action = action_wrapper<"delegations"_n, &api::delegations>;

   [[eosio::action, eosio::read_only]] std::vector<eosio::multisig::proposal> proposals(const name account);
   using proposals_action = action_wrapper<"proposals"_n, &api::proposals>;

   [[eosio::action, eosio::read_only]] eosiosystem::rex_balance rexbal(const name account);
   using rexbal_action = action_wrapper<"rexbal"_n, &api::rexbal>;

   [[eosio::action, eosio::read_only]] eosiosystem::rex_fund rexfund(const name account);
   using rexfund_action = action_wrapper<"rexfund"_n, &api::rexfund>;

   [[eosio::action, eosio::read_only]] eosiosystem::voter_info votes(const name account);
   using vote_action = action_wrapper<"votes"_n, &api::votes>;

#ifdef DEBUG
   [[eosio::action]] void wipe();
   [[eosio::action]] void reset();
#endif

private:
   config_row                                    get_config();
   token_supply                                  get_token_supply(const token_definition def);
   eosiosystem::gifted_ram                       get_gifted_ram(const api::config_row config, const name account);
   eosiosystem::eosio_global_state               get_global(const config_row config);
   eosiosystem::exchange_state                   get_rammarket(const config_row config);
   eosiosystem::rex_pool                         get_rex_pool(const config_row config);
   eosiosystem::powerup_state                    get_powerup(const config_row config);
   eosiosystem::refund_request                   get_refund_request(const config_row config, const name account);
   eosiosystem::rex_balance                      get_rex_balance(const config_row config, const name account);
   eosiosystem::rex_fund                         get_rex_fund(const config_row config, const name account);
   eosiosystem::voter_info                       get_voter_info(const config_row config, const name account);
   std::vector<eosiosystem::delegated_bandwidth> get_delegated_bandwidth(const config_row config, const name account);
   std::vector<eosio::multisig::proposal>        get_msig_proposals(const config_row config, const name account);
   asset                                         get_system_token_balance(const config_row config, const name account);

#ifdef DEBUG
   template <typename T>
   void clear_table(T& table, uint64_t rows_to_clear);
   void reset_singletons();
   void wipe_singletons();
   void wipe_tables();
#endif
};

} // namespace api
