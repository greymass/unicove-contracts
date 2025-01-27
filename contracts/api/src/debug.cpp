namespace api {

// @debug
template <typename T>
void api::clear_table(T& table, uint64_t rows_to_clear)
{
   auto itr = table.begin();
   while (itr != table.end() && rows_to_clear--) {
      itr = table.erase(itr);
   }
}

// @debug
[[eosio::action]] void
api::cleartable(const name table_name, const optional<name> scope, const optional<uint64_t> max_rows)
{
   require_auth(get_self());
   const uint64_t rows_to_clear = (!max_rows || *max_rows == 0) ? -1 : *max_rows;
   const uint64_t value         = scope ? scope->value : get_self().value;

   // list out all the tables that could be cleared
   //
   //    api::table1 _table1(get_self(), value);
   //    api::table2 _table2(get_self(), value);

   // Use an if/else if/else chain to determine which table to clear
   //
   //    if (table_name == "table1"_n)
   //       clear_table(_table1, rows_to_clear);
   //    else if (table_name == "table2"_n)
   //       clear_table(_table2, rows_to_clear);
   //    else
   //       check(false, "cleartable: [table_name] unknown table to clear");
}

// @debug
[[eosio::action]] void api::wipe()
{
   require_auth(get_self());

   api::token_table tokens(get_self(), get_self().value);
   clear_table(tokens, -1);

   // Define the tables to wipe
   //
   //    api::table1 _table1(get_self(), get_self().value);
   //    api::table2 _table2(get_self(), get_self().value);

   // Call clear on the tables to wipe
   //
   //    clear_table(_table1, -1);
   //    clear_table(_table2, -1);
}

} // namespace api