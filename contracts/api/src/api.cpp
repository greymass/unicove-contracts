#include "api/api.hpp"

const name   SYSTEM_CONTRACT_ACCOUNT = name("eosio");
const name   MSIG_CONTRACT_ACCOUNT   = name("eosio.msig");
const name   SYSTEM_TOKEN_ACCOUNT    = name("eosio.token");
const symbol SYSTEM_TOKEN_SYMBOL     = eosiosystem::system_contract::get_core_symbol();

namespace api {

[[eosio::action, eosio::read_only]] get_account_response api::getaccount(const name account)
{
   // get core token balance for the account
   asset                  balance;
   eosio::token::accounts balance_table(SYSTEM_TOKEN_ACCOUNT, account.value);
   auto                   balance_itr = balance_table.get(SYSTEM_TOKEN_SYMBOL.code().raw(), "no balance object found");
   balance                            = balance_itr.balance;

   // get pending refund for the account
   eosiosystem::refund_request refund;

   eosiosystem::refunds_table refunds_table(SYSTEM_CONTRACT_ACCOUNT, account.value);
   auto                       refund_itr = refunds_table.find(account.value);
   if (refund_itr != refunds_table.end()) {
      refund = *refund_itr;
   }

   // get all delegated balances for the account
   std::vector<eosiosystem::delegated_bandwidth> dbw_rows;
   eosiosystem::del_bandwidth_table              dbw_table(SYSTEM_CONTRACT_ACCOUNT, account.value);
   auto                                          dbw_itr = dbw_table.begin();
   while (dbw_itr != dbw_table.end()) {
      dbw_rows.push_back(*dbw_itr);
      dbw_itr++;
   }

   // get all msig proposals from the account
   std::vector<eosio::multisig::proposal> msig_rows;
   eosio::multisig::proposals             msig_table(MSIG_CONTRACT_ACCOUNT, account.value);
   auto                                   msig_itr = msig_table.begin();
   while (msig_itr != msig_table.end()) {
      msig_rows.push_back(*msig_itr);
      msig_itr++;
   }

   eosiosystem::rex_balance rexbal;
   eosiosystem::rex_fund    rexfund;

   if (eosiosystem::system_contract::rex_system_initialized()) {
      // get rexbal entry for the account
      eosiosystem::rex_balance_table rexbal_table(SYSTEM_CONTRACT_ACCOUNT, SYSTEM_CONTRACT_ACCOUNT.value);
      auto                           rex_itr = rexbal_table.find(account.value);
      if (rex_itr != rexbal_table.end()) {
         rexbal = *rex_itr;
      }

      // get rexfund entry for the account
      eosiosystem::rex_fund_table rexfund_table(SYSTEM_CONTRACT_ACCOUNT, SYSTEM_CONTRACT_ACCOUNT.value);
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

} // namespace api

// DEBUG (used on testnet)
#ifdef DEBUG
#include "debug.cpp"
#endif
