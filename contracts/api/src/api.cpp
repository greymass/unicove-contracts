#include "api/api.hpp"

namespace api {

api::config_row api::get_config()
{
   config_table _config(get_self(), get_self().value);
   return _config.get_or_default();
}

antelope::token api::get_system_token(const config_row config, const bool distribution = false)
{
   antelope::token system_token = {.id = get_system_token_definition(config)};
   if (distribution) {
      system_token.distribution = get_token_distribution(system_token.id);
   }
   return system_token;
}

antelope::token_definition api::get_system_token_definition(const config_row config)
{
   return {.chain = config.chain_id, .contract = config.system_token_contract, .symbol = config.system_token_symbol};
}

antelope::token_balance api::get_system_token_balance(const api::config_row config, const name account)
{
   antelope::token_balance balance = {.token   = get_system_token_definition(config),
                                      .balance = asset(0, config.system_token_symbol)};

   eosio::token::accounts balance_table(config.system_token_contract, account.value);
   auto                   balance_itr = balance_table.find(config.system_token_symbol.code().raw());
   if (balance_itr != balance_table.end()) {
      balance.balance = balance_itr->balance;
   }
   return balance;
}

eosiosystem::refund_request api::get_refund_request(const api::config_row config, const name account)
{
   eosiosystem::refund_request refund;
   eosiosystem::refunds_table  refunds_table(config.system_contract, account.value);
   auto                        refund_itr = refunds_table.find(account.value);
   if (refund_itr != refunds_table.end()) {
      // Balances should use the system token regardless of what the table responds with
      refund            = *refund_itr;
      refund.cpu_amount = asset(refund_itr->cpu_amount.amount, config.system_token_symbol);
      refund.net_amount = asset(refund_itr->net_amount.amount, config.system_token_symbol);
   }
   return refund;
}

[[eosio::action, eosio::read_only]] eosiosystem::refund_request api::refund(const name account)
{
   return get_refund_request(get_config(), account);
}

vector<eosiosystem::delegated_bandwidth> api::get_delegated_bandwidth(const api::config_row config, const name account)
{
   vector<eosiosystem::delegated_bandwidth> dbw_rows;
   eosiosystem::del_bandwidth_table         dbw_table(config.system_contract, account.value);
   auto                                     dbw_itr = dbw_table.begin();
   while (dbw_itr != dbw_table.end()) {
      // Balances should use the system token regardless of what the table responds with
      eosiosystem::delegated_bandwidth updated = *dbw_itr;
      updated.cpu_weight                       = asset(dbw_itr->cpu_weight.amount, config.system_token_symbol);
      updated.net_weight                       = asset(dbw_itr->net_weight.amount, config.system_token_symbol);
      dbw_rows.push_back(updated);
      dbw_itr++;
   }
   return dbw_rows;
}

[[eosio::action, eosio::read_only]] vector<eosiosystem::delegated_bandwidth> api::delegations(const name account)
{
   return get_delegated_bandwidth(get_config(), account);
}

eosiosystem::gifted_ram api::get_gifted_ram(const api::config_row config, const name account)
{
   eosiosystem::gifted_ram gr;
   if (config.gifted_ram_enabled) {
      eosiosystem::gifted_ram_table gr_table(config.system_contract, config.system_contract.value);
      auto                          gr_itr = gr_table.find(account.value);
      if (gr_itr != gr_table.end()) {
         gr = *gr_itr;
      }
   }
   return gr;
}

[[eosio::action, eosio::read_only]] eosiosystem::gifted_ram api::giftedram(const name account)
{
   return get_gifted_ram(get_config(), account);
}

vector<eosio::multisig::proposal> api::get_msig_proposals(const api::config_row config, const name account)
{
   vector<eosio::multisig::proposal> msig_rows;
   eosio::multisig::proposals        msig_table(config.system_contract_msig, account.value);
   auto                              msig_itr = msig_table.begin();
   while (msig_itr != msig_table.end()) {
      msig_rows.push_back(*msig_itr);
      msig_itr++;
   }
   return msig_rows;
}

[[eosio::action, eosio::read_only]] vector<eosio::multisig::proposal> api::proposals(const name account)
{
   return get_msig_proposals(get_config(), account);
}

eosiosystem::rex_balance api::get_rex_balance(const api::config_row config, const name account)
{
   eosiosystem::rex_balance rexbal;
   if (eosiosystem::system_contract::rex_system_initialized()) {
      eosiosystem::rex_balance_table rexbal_table(config.system_contract, config.system_contract.value);
      auto                           rex_itr = rexbal_table.find(account.value);
      if (rex_itr != rexbal_table.end()) {
         rexbal = *rex_itr;
         // Balances should use the system token regardless of what the table responds with
         rexbal.vote_stake = asset(rex_itr->vote_stake.amount, config.system_token_symbol);
      }
   }
   return rexbal;
}

[[eosio::action, eosio::read_only]] eosiosystem::rex_balance api::rexbal(const name account)
{
   return get_rex_balance(get_config(), account);
}

eosiosystem::rex_fund api::get_rex_fund(const api::config_row config, const name account)
{
   eosiosystem::rex_fund rexfund;
   if (eosiosystem::system_contract::rex_system_initialized()) {
      eosiosystem::rex_fund_table rexfund_table(config.system_contract, config.system_contract.value);
      auto                        rexfund_itr = rexfund_table.find(account.value);
      if (rexfund_itr != rexfund_table.end()) {
         rexfund = *rexfund_itr;
         // Balances should use the system token regardless of what the table responds with
         rexfund.balance = asset(rexfund_itr->balance.amount, config.system_token_symbol);
      }
   }
   return rexfund;
}

[[eosio::action, eosio::read_only]] eosiosystem::rex_fund api::rexfund(const name account)
{
   return get_rex_fund(get_config(), account);
}

eosiosystem::voter_info api::get_voter_info(const api::config_row config, const name account)
{
   eosiosystem::voter_info   vote;
   eosiosystem::voters_table voters_table(config.system_contract, config.system_contract.value);
   auto                      voter_itr = voters_table.find(account.value);
   if (voter_itr != voters_table.end()) {
      vote = *voter_itr;
   }
   return vote;
}

[[eosio::action, eosio::read_only]] eosiosystem::voter_info api::votes(const name account)
{
   return get_voter_info(get_config(), account);
}

eosiosystem::abi_hash get_contract_hash(const api::config_row config, const name account)
{
   eosiosystem::abi_hash                                  result;
   eosio::multi_index<"abihash"_n, eosiosystem::abi_hash> abihash_table(config.system_contract,
                                                                        config.system_contract.value);
   auto                                                   abihash_itr = abihash_table.find(account.value);
   if (abihash_itr != abihash_table.end() && abihash_itr->owner == account) {
      result = *abihash_itr;
   }
   return result;
}

[[eosio::action, eosio::read_only]] get_account_response api::account(
   const name account, const optional<vector<antelope::token_definition>> tokens, const optional<bool> zerobalances)
{
   check(is_account(account), "account does not exist");

   auto config       = get_config();
   auto contracthash = get_contract_hash(config, account);
   auto balance      = get_system_token_balance(config, account);
   auto giftedram    = get_gifted_ram(config, account);
   auto refund       = get_refund_request(config, account);
   auto delegations  = get_delegated_bandwidth(config, account);
   auto proposals    = get_msig_proposals(config, account);
   auto rexbal       = get_rex_balance(config, account);
   auto rexfund      = get_rex_fund(config, account);
   auto vote         = get_voter_info(config, account);
   auto created      = get_account_creation_time(account);

   vector<antelope::token_balance> balances;
   if (tokens.has_value() && !tokens->empty()) {
      balances = api::balances(account, tokens.value(), zerobalances.value_or(true));
   }

   return get_account_response{.account      = account,
                               .balance      = balance,
                               .balances     = balances,
                               .contracthash = contracthash.hash,
                               .delegations  = delegations,
                               .giftedram    = giftedram,
                               .proposals    = proposals,
                               .refund       = refund,
                               .rexbal       = rexbal,
                               .rexfund      = rexfund,
                               .vote         = vote,
                               .created      = created};
}

[[eosio::action, eosio::read_only]] get_available_response api::available(const name account)
{
   return get_available_response{.account = account, .available = !is_account(account)};
}

eosiosystem::eosio_global_state api::get_global(const api::config_row config)
{
   eosiosystem::global_state_singleton global_table(config.system_contract, config.system_contract.value);
   auto                                global = global_table.get_or_default();
   return global;
}

[[eosio::action, eosio::read_only]] eosiosystem::eosio_global_state api::global() { return get_global(get_config()); }

eosiosystem::exchange_state api::get_rammarket(const api::config_row config)
{
   eosiosystem::exchange_state ram;
   eosiosystem::rammarket      rammarket_table(config.system_contract, config.system_contract.value);
   auto                        rammarket_itr = rammarket_table.find(config.system_ramcore_symbol.raw());
   if (rammarket_itr != rammarket_table.end()) {
      ram = *rammarket_itr;
      // Balances should use the system token regardless of what the table responds with
      ram.quote.balance = asset(rammarket_itr->quote.balance.amount, config.system_token_symbol);
   }
   return ram;
}

[[eosio::action, eosio::read_only]] eosiosystem::exchange_state api::ram() { return get_rammarket(get_config()); }

eosiosystem::rex_pool api::get_rex_pool(const api::config_row config)
{
   eosiosystem::rex_pool       rex;
   eosiosystem::rex_pool_table rex_table(config.system_contract, config.system_contract.value);
   auto                        rex_itr = rex_table.begin();
   if (rex_itr != rex_table.end()) {
      rex = *rex_itr;
      // Balances should use the system token regardless of what the table responds with
      rex.total_lent       = asset(rex_itr->total_lent.amount, config.system_token_symbol);
      rex.total_unlent     = asset(rex_itr->total_unlent.amount, config.system_token_symbol);
      rex.total_rent       = asset(rex_itr->total_rent.amount, config.system_token_symbol);
      rex.total_lendable   = asset(rex_itr->total_lendable.amount, config.system_token_symbol);
      rex.namebid_proceeds = asset(rex_itr->namebid_proceeds.amount, config.system_token_symbol);
   }
   return rex;
}

[[eosio::action, eosio::read_only]] eosiosystem::rex_pool api::rex() { return get_rex_pool(get_config()); }

eosiosystem::powerup_state api::get_powerup(const api::config_row config)
{
   eosiosystem::powerup_state_singleton powerup_table(config.system_contract, 0);
   auto                                 powerup = powerup_table.get_or_default();
   // Balances should use the system token regardless of what the table responds with
   powerup.cpu.min_price   = asset(powerup.cpu.min_price.amount, config.system_token_symbol);
   powerup.cpu.max_price   = asset(powerup.cpu.max_price.amount, config.system_token_symbol);
   powerup.net.min_price   = asset(powerup.net.min_price.amount, config.system_token_symbol);
   powerup.net.max_price   = asset(powerup.net.max_price.amount, config.system_token_symbol);
   powerup.min_powerup_fee = asset(powerup.min_powerup_fee.amount, config.system_token_symbol);
   return powerup;
}

[[eosio::action, eosio::read_only]] eosiosystem::powerup_state api::powerup() { return get_powerup(get_config()); }

[[eosio::action, eosio::read_only]] get_network_response api::network()
{
   auto config = get_config();

   antelope::token_definition def = {
      .contract = config.system_token_contract,
      .symbol   = config.system_token_symbol,
   };

   return get_network_response{.global  = get_global(config),
                               .ram     = get_rammarket(config),
                               .rex     = get_rex_pool(config),
                               .powerup = get_powerup(config),
                               .token   = get_system_token(config, true)};
}

antelope::token_distribution api::get_token_distribution(const antelope::token_definition def)
{
   auto config   = get_config();
   auto rex_pool = get_rex_pool(config);

   antelope::token_distribution distribution = {
      .circulating = asset(0, def.symbol),
      .locked      = asset(0, def.symbol),
      .max         = asset(0, def.symbol),
      .staked      = rex_pool.total_lendable,
      .supply      = asset(0, def.symbol),
   };

   eosio::token::stats stats_table(def.contract, def.symbol.code().raw());
   auto                stats_itr = stats_table.find(def.symbol.code().raw());
   if (stats_itr != stats_table.end()) {
      distribution.supply      = stats_itr->supply;
      distribution.max         = stats_itr->max_supply;
      distribution.circulating = distribution.supply;
   }

   // Consider tokens in the system contract as locked
   eosio::token::accounts system_balances(def.contract, config.system_contract.value);
   auto                   system_balance_itr = system_balances.find(def.symbol.code().raw());
   if (system_balance_itr != system_balances.end()) {
      distribution.locked += system_balance_itr->balance;
      distribution.circulating -= system_balance_itr->balance;
   }

   // Consider tokens in the token contract as locked
   if (config.system_contract.value != config.system_token_contract.value) {
      eosio::token::accounts contract_balances(def.contract, config.system_token_contract.value);
      auto                   contract_balance_itr = contract_balances.find(def.symbol.code().raw());
      if (contract_balance_itr != contract_balances.end()) {
         distribution.locked += contract_balance_itr->balance;
         distribution.circulating -= contract_balance_itr->balance;
      }
   }

   return distribution;
}

[[eosio::action, eosio::read_only]] antelope::token api::distribution(const antelope::token_definition definition)
{
   return {.id = definition, .distribution = get_token_distribution(definition)};
}

[[eosio::action, eosio::read_only]] vector<antelope::token_balance>
api::balances(const name account, const vector<antelope::token_definition> tokens, const bool zerobalances = true)
{
   auto config = get_config();

   vector<antelope::token_balance> balances;
   check(tokens.size() > 0, "tokens must not be empty");

   for (const auto& requested : tokens) {
      const antelope::token_definition id = {
         .chain    = config.chain_id,
         .contract = requested.contract,
         .symbol   = requested.symbol,
      };
      const antelope::token   token   = {.id = id};
      antelope::token_balance balance = {.token = token};

      eosio::token::accounts _accounts(token.id.contract, account.value);
      auto                   balance_itr = _accounts.find(token.id.symbol.code().raw());

      if (balance_itr != _accounts.end()) {
         balance.balance = balance_itr->balance;
         balances.push_back(balance);
      } else if (zerobalances) {
         balance.balance = asset(0, token.id.symbol);
         balances.push_back(balance);
      }
   }

   return balances;
}

[[eosio::action, eosio::read_only]] eosiosystem::abi_hash api::contracthash(const name account)
{
   auto config = get_config();
   return get_contract_hash(config, account);
}

[[eosio::action]] void api::setconfig(const checksum256 chain_id,
                                      const name        system_contract,
                                      const name        system_contract_msig,
                                      const name        system_token_contract,
                                      const symbol      system_token_symbol,
                                      const symbol      system_ramcore_symbol,
                                      const symbol      system_ram_symbol,
                                      const symbol      system_rex_symbol,
                                      const bool        gifted_ram_enabled)
{
   require_auth(get_self());
   config_table _config(get_self(), get_self().value);
   auto         config          = _config.get_or_default();
   config.chain_id              = chain_id;
   config.system_contract       = system_contract;
   config.system_contract_msig  = system_contract_msig;
   config.system_token_contract = system_token_contract;
   config.system_token_symbol   = system_token_symbol;
   config.system_ramcore_symbol = system_ramcore_symbol;
   config.system_ram_symbol     = system_ram_symbol;
   config.system_rex_symbol     = system_rex_symbol;
   config.gifted_ram_enabled    = gifted_ram_enabled;
   _config.set(config, get_self());
}

} // namespace api

// DEBUG (used on testnet)
#ifdef DEBUG
#include "debug.cpp"
#endif
