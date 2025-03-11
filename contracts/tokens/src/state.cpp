namespace tokens {

[[eosio::action]] void tokens::setconfig(const bool enabled)
{
   require_auth(get_self());
   config_table _config(get_self(), get_self().value);
   auto         config = _config.get_or_default();
   config.enabled      = enabled;
   _config.set(config, get_self());
}

} // namespace tokens