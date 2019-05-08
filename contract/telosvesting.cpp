#include "./telosvesting.hpp"

using namespace eosio;
using namespace std;

void telosvesting::setconfig(uint64_t default_vesting_time_microseconds) {
  require_auth(_self);
  cfg.set(config{.default_vesting_time = microseconds(default_vesting_time_microseconds)}, _self);
}

void telosvesting::on_transfer(name from, name to, asset quantity,
                               string memo) {
  if (from == _self) {
    // we're sending money, do nothing additional
    return;
  }

  check(to == _self, "contract is not involved in this transfer");
  check(quantity.symbol.is_valid(), "invalid quantity");
  check(quantity.amount > 0, "only positive quantity allowed");
  check(quantity.symbol == EOS_SYMBOL, "only EOS tokens allowed");

  config c = get_config();
}

void telosvesting::testreset() {
  require_auth(_self);
  // auto itr = games.begin();
  // while(itr != games.end()) {
  //     itr = games.erase(itr);
  // }
}
