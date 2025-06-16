#pragma once

#include <eosio/eosio.hpp>

using namespace eosio;
using namespace std;

namespace noact {

class [[eosio::contract("noact")]] noact : public contract
{
public:
   using contract::contract;

   [[eosio::action]] void identityv2(const permission_level permission);
   [[eosio::action]] void identityv3(const permission_level permission, const name scope);

private:

};

} // namespace api
