#pragma once

#include <eosio.msig/eosio.msig.hpp>
#include <eosio.system/eosio.system.hpp>
#include <eosio.token/eosio.token.hpp>
#include <eosio/contract.hpp>
#include <eosio/singleton.hpp>

using namespace eosio;
using namespace std;

namespace accounts {

static constexpr symbol EOS = symbol{"EOS", 4};

class [[eosio::contract("accounts")]] accounts : public contract
{
public:
   using contract::contract;

   //    struct [[eosio::table("tokens")]] token_row
   //    {
   //       uint64_t id;
   //       name     contract;
   //       symbol   symbol;
   //       uint64_t primary_key() const { return id; }
   //    };
   //    typedef eosio::multi_index<"tokens"_n, token_row> token_table;

   //    [[eosio::action]] void addtoken(const token_definition token);

   //    [[eosio::action, eosio::read_only]] std::vector<asset> getbalances(const name                          account,
   //                                                                       const std::vector<token_definition> tokens);
   //    using getbalances_action = action_wrapper<"getbalances"_n, &tokens::getbalances>;

   //    [[eosio::action, eosio::read_only]] std::vector<token_definition> gettokens();
   //    using gettokens_action = action_wrapper<"gettokens"_n, &tokens::gettokens>;

#ifdef DEBUG
   [[eosio::action]] void wipe();
   [[eosio::action]] void reset();
#endif

private:
   //    void                          add_token(const token_definition token);
   //    std::vector<token_definition> get_token_definitions();

#ifdef DEBUG
   template <typename T>
   void clear_table(T& table, uint64_t rows_to_clear);
   void reset_singletons();
   void wipe_singletons();
   void wipe_tables();
#endif
};

} // namespace accounts
