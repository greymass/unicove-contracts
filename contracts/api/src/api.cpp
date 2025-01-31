#include "api/api.hpp"

namespace api {

[[eosio::action, eosio::read_only]] get_account_response api::account(const name account)
{
   check(is_account(account), "account does not exist");

   config_table _config(get_self(), get_self().value);
   auto         config = _config.get_or_default();

   // get core token balance for the account
   asset                  balance = asset(0, config.system_token_symbol);
   eosio::token::accounts balance_table(config.system_token_contract, account.value);
   auto                   balance_itr = balance_table.find(config.system_token_symbol.code().raw());
   if (balance_itr != balance_table.end()) {
      balance = balance_itr->balance;
   }

   // get pending refund for the account
   eosiosystem::refund_request refund;
   eosiosystem::refunds_table  refunds_table(config.system_contract, account.value);
   auto                        refund_itr = refunds_table.find(account.value);
   if (refund_itr != refunds_table.end()) {
      refund = *refund_itr;
   }

   // get all delegated balances for the account
   std::vector<eosiosystem::delegated_bandwidth> dbw_rows;
   eosiosystem::del_bandwidth_table              dbw_table(config.system_contract, account.value);
   auto                                          dbw_itr = dbw_table.begin();
   while (dbw_itr != dbw_table.end()) {
      dbw_rows.push_back(*dbw_itr);
      dbw_itr++;
   }

   // get all msig proposals from the account
   std::vector<eosio::multisig::proposal> msig_rows;
   eosio::multisig::proposals             msig_table(config.system_contract_msig, account.value);
   auto                                   msig_itr = msig_table.begin();
   while (msig_itr != msig_table.end()) {
      msig_rows.push_back(*msig_itr);
      msig_itr++;
   }

   eosiosystem::rex_balance rexbal;
   eosiosystem::rex_fund    rexfund;
   if (eosiosystem::system_contract::rex_system_initialized()) {
      // get rexbal entry for the account
      eosiosystem::rex_balance_table rexbal_table(config.system_contract, config.system_contract.value);
      auto                           rex_itr = rexbal_table.find(account.value);
      if (rex_itr != rexbal_table.end()) {
         rexbal = *rex_itr;
      }

      // get rexfund entry for the account
      eosiosystem::rex_fund_table rexfund_table(config.system_contract, config.system_contract.value);
      auto                        rexfund_itr = rexfund_table.find(account.value);
      if (rexfund_itr != rexfund_table.end()) {
         rexfund = *rexfund_itr;
      }
   }

   eosiosystem::voter_info   vote;
   eosiosystem::voters_table voters_table(config.system_contract, config.system_contract.value);
   auto                      voter_itr = voters_table.find(account.value);
   if (voter_itr != voters_table.end()) {
      vote = *voter_itr;
   }

   return get_account_response{.account     = account,
                               .balance     = balance,
                               .delegations = dbw_rows,
                               .proposals   = msig_rows,
                               .refund      = refund,
                               .rexbal      = rexbal,
                               .rexfund     = rexfund,
                               .vote        = vote};
}

[[eosio::action, eosio::read_only]] get_available_response api::available(const name account)
{
   return get_available_response{.account = account, .available = !is_account(account)};
}

[[eosio::action, eosio::read_only]] get_network_response api::network()
{
   config_table _config(get_self(), get_self().value);
   auto         config = _config.get_or_default();

   eosiosystem::global_state_singleton global_table(config.system_contract, config.system_contract.value);
   auto                                global = global_table.get_or_default();

   eosiosystem::exchange_state ram;
   eosiosystem::rammarket      rammarket_table(config.system_contract, config.system_contract.value);
   auto                        rammarket_itr = rammarket_table.find(config.system_ramcore_symbol.raw());
   if (rammarket_itr != rammarket_table.end()) {
      ram = *rammarket_itr;
   }

   eosiosystem::rex_pool       rex;
   eosiosystem::rex_pool_table rex_table(config.system_contract, config.system_contract.value);
   auto                        rex_itr = rex_table.begin();
   if (rex_itr != rex_table.end()) {
      rex = *rex_itr;
   }

   eosiosystem::powerup_state_singleton powerup_table(config.system_contract, 0);
   auto                                 powerup = powerup_table.get_or_default();

   token_definition def = {
      .contract = config.system_token_contract,
      .symbol   = config.system_token_symbol,
   };
   const token_supply token = get_token_supply(def);

   return get_network_response{.global = global, .ram = ram, .rex = rex, .powerup = powerup, .token = token};
}

token_supply api::get_token_supply(const token_definition def)
{
   token_supply ts = {
      .def    = def,
      .locked = asset(0, def.symbol),
      .max    = asset(0, def.symbol),
      .supply = asset(0, def.symbol),
   };

   eosio::token::stats stats_table(def.contract, def.symbol.code().raw());
   auto                stats_itr = stats_table.find(def.symbol.code().raw());
   if (stats_itr != stats_table.end()) {
      ts.supply = stats_itr->supply;
      ts.max    = stats_itr->max_supply;
   }

   eosio::token::accounts _accounts(def.contract, def.contract.value);
   auto                   balance_itr = _accounts.find(def.symbol.code().raw());
   if (balance_itr != _accounts.end()) {
      ts.locked = balance_itr->balance;
   }

   return ts;
}

[[eosio::action, eosio::read_only]] token_supply api::supply(const token_definition definition)
{
   return get_token_supply(definition);
}

[[eosio::action, eosio::read_only]] std::vector<asset>
api::balances(const name account, const std::vector<token_definition> tokens, const bool zerobalances = true)
{
   std::vector<asset> balances;
   check(tokens.size() > 0, "tokens must not be empty");

   for (const auto& token : tokens) {
      eosio::token::accounts _accounts(token.contract, account.value);
      auto                   balance_itr = _accounts.find(token.symbol.code().raw());
      if (balance_itr != _accounts.end()) {
         balances.push_back(balance_itr->balance);
      } else if (zerobalances) {
         balances.push_back(asset(0, token.symbol));
      }
   }

   return balances;
}

[[eosio::action]] void api::setconfig(const name   system_contract,
                                      const name   system_contract_msig,
                                      const name   system_token_contract,
                                      const symbol system_token_symbol)
{
   require_auth(get_self());
   config_table _config(get_self(), get_self().value);
   auto         config          = _config.get_or_default();
   config.system_contract       = system_contract;
   config.system_contract_msig  = system_contract_msig;
   config.system_token_contract = system_token_contract;
   config.system_token_symbol   = system_token_symbol;
   _config.set(config, get_self());
}

} // namespace api

// DEBUG (used on testnet)
#ifdef DEBUG
#include "debug.cpp"
#endif
