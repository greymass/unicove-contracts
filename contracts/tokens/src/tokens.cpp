#include "tokens/tokens.hpp"
#include "api/api.hpp"

namespace tokens {

[[eosio::action, eosio::read_only]] std::vector<token_definition> tokens::gettokens()
{
   return get_token_definitions();
}

[[eosio::action, eosio::read_only]] std::vector<asset> tokens::getbalances(const name                          account,
                                                                           const std::vector<token_definition> tokens)
{
   std::vector<asset> balances;

   std::vector<token_definition> token_definitions = tokens;
   if (token_definitions.size() == 0) {
      token_definitions = get_token_definitions();
   }

   for (const auto& token : tokens) {
      eosio::token::accounts _accounts(token.contract, account.value);
      auto                   balance_itr = _accounts.find(token.symbol.code().raw());
      if (balance_itr != _accounts.end()) {
         balances.push_back(balance_itr->balance);
      } else {
         balances.push_back(asset(0, token.symbol));
      }
   }

   return balances;
}

void tokens::add_token(const token_definition token)
{
   require_auth(get_self());
   token_table tokens(get_self(), get_self().value);
   tokens.emplace(get_self(), [&](auto& row) {
      row.id       = tokens.available_primary_key();
      row.contract = token.contract;
      row.symbol   = token.symbol;
   });
}

[[eosio::action]] void tokens::addtoken(const token_definition token) { add_token(token); }

[[eosio::action]] void tokens::addtokens(const std::vector<token_definition> tokens)
{
   for (const auto& token : tokens) {
      add_token(token);
   }
}

[[eosio::action]] void tokens::removetoken(const uint64_t id)
{
   require_auth(get_self());
   token_table tokens(get_self(), get_self().value);
   auto        token_itr = tokens.find(id);
   check(token_itr != tokens.end(), "token not found");
   tokens.erase(token_itr);
}

} // namespace tokens

// DEBUG (used on testnet)
#ifdef DEBUG
#include "debug.cpp"
#endif
