
#include <eosio.system/eosio.system.hpp>
#include <eosio.token/eosio.token.hpp>

namespace tokens {

tokens::config_row tokens::get_config()
{
   config_table _config(get_self(), get_self().value);
   return _config.get_or_default();
}

[[eosio::action]] void tokens::regtoken(const name&                      issuer,
                                        const asset&                     maximum_supply,
                                        const std::vector<distribution>& distribution,
                                        const asset&                     payment)
{
   auto config = get_config();

   auto sym = maximum_supply.symbol;
   check(maximum_supply.is_valid(), "invalid supply");
   check(maximum_supply.amount > 0, "max-supply must be positive");

   stats statstable(get_self(), sym.code().raw());
   auto  existing = statstable.find(sym.code().raw());
   check(existing == statstable.end(), "token with symbol already exists");

   check(payment.symbol == config.system_token.symbol, "payment must use system token");
   check(payment.amount == config.price.amount, "incorrect payment amount");
   if (config.price.amount > 0) {
      //   if (config.privileged) {
      //      token::transfer_action transfer_act{config.system_token.contract,
      //                                          {{issuer, eosiosystem::system_contract::active_permission}}};
      //      transfer_act.send(issuer, config.fees_account, payment,
      //                        std::string("regtoken payment for ") + sym.code().to_string());
      //   } else {
      //      // Will need to build a deposit system if a privileged account is not available
      //      check(false, "privileged actions are disabled and payment cannot be accepted.");
      //   }
   }

   statstable.emplace(issuer, [&](auto& s) {
      s.supply.symbol = maximum_supply.symbol;
      s.max_supply    = maximum_supply;
      s.issuer        = issuer;
   });

   add_token(issuer, get_self(), sym);

   auto sum = asset(0, sym);
   for (const auto& dist : distribution) {
      check(dist.quantity.symbol == sym, "distribution symbol mismatch");
      sum += dist.quantity;
      distribute(dist.receiver, dist.quantity);
   }
   check(sum == maximum_supply, "sum of distribution must equal maximum supply");
}

void tokens::distribute(const name& to, const asset& quantity)
{
   auto sym = quantity.symbol;
   check(sym.is_valid(), "invalid symbol name");

   stats statstable(get_self(), sym.code().raw());
   auto  existing = statstable.find(sym.code().raw());
   check(existing != statstable.end(), "token with symbol does not exist, create token before issue");
   const auto& st = *existing;

   require_auth(st.issuer);
   check(quantity.is_valid(), "invalid quantity");
   check(quantity.amount > 0, "must issue positive quantity");

   check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
   check(quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

   statstable.modify(st, same_payer, [&](auto& s) { s.supply += quantity; });

   add_balance(to, quantity, st.issuer);

   logdistribute_action logdistribute_act{get_self(), {{get_self(), eosiosystem::system_contract::active_permission}}};
   logdistribute_act.send(st.issuer, to, quantity);
}

void tokens::logdistribute(const name& issuer, const name& receiver, const asset& quantity)
{
   require_auth(get_self());
   require_recipient(issuer);
   require_recipient(receiver);
}

void tokens::add_token(const name issuer, const name contract, const symbol symbol)
{
   token_table tokens(get_self(), get_self().value);
   tokens.emplace(issuer, [&](auto& row) {
      row.id       = tokens.available_primary_key();
      row.contract = contract;
      row.symbol   = symbol;
   });
}

[[eosio::action]] void tokens::setconfig(const bool                       enabled,
                                         const asset                      price,
                                         const antelope::token_definition system_token,
                                         const name                       fees_account)
{
   require_auth(get_self());
   config_table _config(get_self(), get_self().value);
   auto         config = _config.get_or_default();
   config.enabled      = enabled;
   config.price        = price;
   config.system_token = system_token;
   config.fees_account = fees_account;
   _config.set(config, get_self());
}

} // namespace tokens