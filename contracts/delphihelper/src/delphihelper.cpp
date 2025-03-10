#include "delphihelper/delphihelper.hpp"

namespace delphihelper {

[[eosio::action, eosio::read_only]] std::vector<token_pair> delphihelper::getpairs()
{
   std::vector<token_pair> pairs;

   delphioracle::pairstable pairs_table(main_contract, main_contract.value);
   auto                     pairs_itr = pairs_table.begin();

   while (pairs_itr != pairs_table.end()) {
      if (pairs_itr->active) {
         delphioracle::datapointstable datapoints_table(main_contract, pairs_itr->name.value);
         auto                          datapoints_itr = datapoints_table.begin();

         token_definition base_token = {.symbol = pairs_itr->base_symbol, .contract = pairs_itr->base_contract};

         // Correct the quoted token symbol
         const symbol     quoted_symbol = symbol(pairs_itr->quote_symbol.code(), pairs_itr->quoted_precision);
         token_definition quote_token   = {.symbol = quoted_symbol, .contract = pairs_itr->quote_contract};

         pairs.push_back({.base    = base_token,
                          .quote   = quote_token,
                          .price   = asset(datapoints_itr->value, quoted_symbol),
                          .updated = datapoints_itr->timestamp});
      }
      pairs_itr++;
   }

   return pairs;
}

// This is a dummy action that returns an empty token_pair, to force the ABI to export the token_pair struct
[[eosio::action, eosio::read_only]] token_pair delphihelper::tokenpair()
{
   token_pair pair;
   return pair;
}

} // namespace delphihelper
