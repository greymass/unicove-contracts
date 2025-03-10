#include "accounts/accounts.hpp"

namespace accounts {

// @user
[[eosio::on_notify("*::transfer")]] void
accounts::on_transfer(const name from, const name to, const asset quantity, const string memo)
{
   // ignore RAM sales
   // ignore transfers not sent to this contract
   if (from == "eosio.ram"_n || to != get_self()) {
      return 0;
   }

   // ignore transfers sent from this contract to purchase RAM
   // otherwise revert transaction if sending EOS outside of this contract if RAM transfer is enabled
   if (from == get_self()) {
      if (to == "eosio.ram"_n || to == "eosio.ramfee"_n) {
         return 0;
      }
      // safety check to prevent sending EOS outside of this contract when RAM transfer is enabled
      if (FLAG_ENABLE_RAM_TRANSFER_ON_CLAIM) {
         check(false, "RAM transfer is enabled. Use `claim` to claim RAM bytes.");
      }
      return 0;
   }

   // validate incoming token transfer
   check(get_first_receiver() == "eosio.token"_n, "Only the eosio.token contract may send tokens to this contract.");
   check(quantity.symbol == EOS, "Only the system token is accepted for transfers.");
   check(!memo.empty(), ERROR_INVALID_MEMO);

   // validate memo
   const name receiver = utils::parse_name(memo);
   check(receiver.value, ERROR_INVALID_MEMO); // ensure receiver is not empty & valid Name type
   check(is_account(receiver), ERROR_ACCOUNT_NOT_EXISTS);

   if (FLAG_FORCE_RECEIVER_TO_BE_SENDER) {
      check(receiver == from, "Receiver must be the same as the sender.");
   }

   // contract purchase bytes and credit to receiver
   const int64_t bytes = eosiosystem::bytes_cost_with_fee(quantity);
   buy_ram(quantity);
   add_ram_bytes(receiver, bytes);
   return bytes;
}

} // namespace accounts

// DEBUG (used on testnet)
#ifdef DEBUG
#include "debug.cpp"
#endif
