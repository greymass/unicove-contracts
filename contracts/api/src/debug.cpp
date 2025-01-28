namespace api {

template <typename T>
void api::clear_table(T& table, uint64_t rows_to_clear)
{
   auto itr = table.begin();
   while (itr != table.end() && rows_to_clear--) {
      itr = table.erase(itr);
   }
}

void api::reset_singletons()
{
   config_table _config(get_self(), get_self().value);
   config_row   default_row;
   _config.set(default_row, get_self());
}

void api::wipe_singletons()
{
   config_table _config(get_self(), get_self().value);
   _config.remove();
}

void api::wipe_tables()
{
   token_table tokens(get_self(), get_self().value);
   clear_table(tokens, -1);
}

[[eosio::action]] void api::wipe()
{
   require_auth(get_self());

   wipe_singletons();
   wipe_tables();
}

[[eosio::action]] void api::reset()
{
   require_auth(get_self());

   reset_singletons();
   wipe_tables();
}

} // namespace api