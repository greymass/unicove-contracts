#pragma once

#include "delphioracle/delphioracle.hpp"

namespace delphihelper {

const name main_contract = "delphioracle"_n;

struct token_definition
{
   symbol symbol;
   name   contract;
};

struct token_pair
{
   token_definition base;
   token_definition quote;
   asset            price;
   time_point       updated;
};

class [[eosio::contract("delphihelper")]] delphihelper : public contract
{
public:
   using contract::contract;

   [[eosio::action, eosio::read_only]] std::vector<token_pair> getpairs();
   using getpairs_action = action_wrapper<"getpairs"_n, &delphihelper::getpairs>;

   [[eosio::action, eosio::read_only]] token_pair tokenpair();
   using tokenpair_action = action_wrapper<"tokenpair"_n, &delphihelper::tokenpair>;
};

} // namespace delphihelper
