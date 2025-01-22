#include "api/api.hpp"

namespace api {

[[eosio::action, eosio::read_only]] get_account_response api::getaccount(const name account)
{
   // get all delegated balances for the account
   std::vector<eosiosystem::delegated_bandwidth> dbw_rows;
   eosiosystem::del_bandwidth_table              dbw_table("eosio"_n, account.value);
   auto                                          dbw_itr = dbw_table.begin();
   while (dbw_itr != dbw_table.end()) {
      dbw_rows.push_back(*dbw_itr);
      dbw_itr++;
   }

   // get all msig proposals from the account
   std::vector<eosio::multisig::proposal> msig_rows;
   eosio::multisig::proposals             msig_table("eosio.msig"_n, account.value);
   auto                                   msig_itr = msig_table.begin();
   while (msig_itr != msig_table.end()) {
      msig_rows.push_back(*msig_itr);
      msig_itr++;
   }

   // get rexbal entry for the account
   eosiosystem::rex_balance_table rexbal_table("eosio"_n, "eosio"_n.value);
   auto                           rex_itr = rexbal_table.find(account.value);
   eosiosystem::rex_balance       rexbal;
   if (rex_itr != rexbal_table.end()) {
      rexbal = *rex_itr;
   }

   // get rexfund entry for the account
   eosiosystem::rex_fund_table rexfund_table("eosio"_n, "eosio"_n.value);
   auto                        rexfund_itr = rexfund_table.find(account.value);
   eosiosystem::rex_fund       rexfund;
   if (rexfund_itr != rexfund_table.end()) {
      rexfund = *rexfund_itr;
   }

   return get_account_response{
      .account = account, .delegations = dbw_rows, .proposals = msig_rows, .rexbal = rexbal, .rexfund = rexfund};
}

} // namespace api

// DEBUG (used on testnet)
#ifdef DEBUG
#include "debug.cpp"
#endif
