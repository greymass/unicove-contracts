namespace tokens {

template <typename T>
void tokens::clear_table(T& table, uint64_t rows_to_clear)
{
   auto itr = table.begin();
   while (itr != table.end() && rows_to_clear--) {
      itr = table.erase(itr);
   }
}

[[eosio::action]] void tokens::reset(const std::vector<name> testaccounts)
{
   //    require_auth(get_self());

   config_table _config(get_self(), get_self().value);
   _config.remove();

   token_table _tokens(get_self(), get_self().value);
   auto        itr = _tokens.begin();
   while (itr != _tokens.end()) {
      tokens::stats _stats(get_self(), itr->symbol.code().raw());
      clear_table(_stats, -1);
      itr = _tokens.erase(itr);
   }

   for (auto account : testaccounts) {
      tokens::accounts _accounts(get_self(), account.value);
      clear_table(_accounts, -1);
   }
}

} // namespace tokens