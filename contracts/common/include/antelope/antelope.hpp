#pragma once

#include <eosio.system/eosio.system.hpp>

using namespace eosio;
using namespace std;

namespace antelope {

static inline checksum256 default_chain_id            = checksum256();
static inline name        default_system_contract     = name("eosio");
static inline name        default_msig_contract       = name("eosio.msig");
static inline name        default_token_contract      = name("eosio.token");
static inline symbol      default_system_token_symbol = symbol("UNKNOWN", 0);
static inline asset       default_system_token_asset  = asset(0, default_system_token_symbol);

struct token_definition
{
   optional<checksum256> chain;
   name                  contract;
   symbol                symbol;
};

struct token_distribution
{
   asset circulating = default_system_token_asset;
   asset locked      = default_system_token_asset;
   asset max         = default_system_token_asset;
   asset staked      = default_system_token_asset;
   asset supply      = default_system_token_asset;
};

struct token
{
   token_definition             id;
   optional<token_distribution> distribution;
};

struct token_balance
{
   token token;
   asset balance;
};

} // namespace antelope
