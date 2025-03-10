namespace accounts {

template <typename T>
void accounts::clear_table(T& table, uint64_t rows_to_clear)
{
   auto itr = table.begin();
   while (itr != table.end() && rows_to_clear--) {
      itr = table.erase(itr);
   }
}

void accounts::reset_singletons()
{
   //    config_table _config(get_self(), get_self().value);
   //    config_row   default_row;
   //    _config.set(default_row, get_self());
}

void accounts::wipe_singletons()
{
   //    config_table _config(get_self(), get_self().value);
   //    _config.remove();
}

void accounts::wipe_tables()
{
   //    token_table accounts(get_self(), get_self().value);
   //    clear_table(accounts, -1);
}

[[eosio::action]] void accounts::wipe()
{
   require_auth(get_self());

   wipe_singletons();
   wipe_tables();
}

[[eosio::action]] void accounts::reset()
{
   require_auth(get_self());

   reset_singletons();
   wipe_tables();
}

} // namespace accounts