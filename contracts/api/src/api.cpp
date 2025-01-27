#include "api/api.hpp"

namespace api {

[[eosio::action, eosio::read_only]] get_account_response api::getaccount(const name account)
{
   config_table _config(get_self(), get_self().value);
   auto         config = _config.get_or_default();

   // get core token balance for the account
   asset                  balance;
   eosio::token::accounts balance_table(config.system_token_contract, account.value);
   auto balance_itr = balance_table.get(config.system_token_symbol.code().raw(), "no balance object found");
   balance          = balance_itr.balance;

   // get pending refund for the account
   eosiosystem::refund_request refund;

   eosiosystem::refunds_table refunds_table(config.system_contract, account.value);
   auto                       refund_itr = refunds_table.find(account.value);
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

   return get_account_response{.account     = account,
                               .balance     = balance,
                               .delegations = dbw_rows,
                               .proposals   = msig_rows,
                               .refund      = refund,
                               .rexbal      = rexbal,
                               .rexfund     = rexfund};
}

[[eosio::action, eosio::read_only]] std::vector<asset> api::getbalances(const name account)
{
   token_table        _tokens(get_self(), get_self().value);
   auto               token_itr = _tokens.begin();
   std::vector<asset> balances;

   while (token_itr != _tokens.end()) {
      eosio::token::accounts _accounts(token_itr->contract, account.value);
      auto                   balance_itr = _accounts.find(token_itr->symbol.code().raw());
      if (balance_itr != _accounts.end()) {
         balances.push_back(balance_itr->balance);
      } else {
         balances.push_back(asset(0, token_itr->symbol));
      }
      token_itr++;
   }

   return balances;
}

[[eosio::action]] void api::addtoken(const name contract, const symbol symbol)
{
   require_auth(get_self());
   token_table tokens(get_self(), get_self().value);
   tokens.emplace(get_self(), [&](auto& row) {
      row.id       = tokens.available_primary_key();
      row.contract = contract;
      row.symbol   = symbol;
   });
}
[[eosio::action]] void api::removetoken(const uint64_t id)
{
   require_auth(get_self());
   token_table tokens(get_self(), get_self().value);
   auto        token_itr = tokens.find(id);
   check(token_itr != tokens.end(), "token not found");
   tokens.erase(token_itr);
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
