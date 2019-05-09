#include "./telosvesting.hpp"

using namespace eosio;
using namespace std;

void telosvesting::setconfig(uint64_t default_vesting_period_microseconds) {
  require_auth(get_self());
  cfg.set(config{.default_vesting_period =
                     microseconds(default_vesting_period_microseconds)},
          get_self());
}

void telosvesting::on_transfer(name from, name to, asset quantity,
                               string memo) {
  if (from == get_self()) {
    // we're sending tokens, do nothing additional
    return;
  }

  check(to == get_self(), "contract is not involved in this transfer");
  check(quantity.symbol.is_valid(), "invalid quantity");
  // prevent RAM DoS by restricting access
  check(quantity.amount >= 10000, "must vest more than 1 token");
  check(quantity.symbol == EOS_SYMBOL || quantity.symbol == TLOS_SYMBOL,
        "invalid token symbol");

  name vest_to = name(memo);
  check(is_account(vest_to),
        "must provide an existing account as memo that can withdraw the funds "
        "after the vesting period");

  config c = get_config();
  vests_t vests(get_self(), vest_to.value);
  // _self must pay during notify
  vests.emplace(get_self(), [&](vest &v) {
    v.id = vests.available_primary_key();
    v.quantity = quantity;
    v.matures_at = current_time_point() + c.default_vesting_period;
  });
}

void telosvesting::withdraw(eosio::name to) {
  // anyone can initiate a withdraw
  vests_t vests(get_self(), to.value);
  auto time_index = vests.get_index<"time"_n>();

  auto it = time_index.lower_bound(0);
  check(it != time_index.end(), "no vestings found");
  auto now = current_time_point();
  check(it->matures_at < now, "no vestings found");

  action(permission_level{get_self(), "active"_n}, "eosio.token"_n,
         "transfer"_n,
         make_tuple(get_self(), to, it->quantity,
                    std::string("Withdraw from vesting contract")))
      .send();
}

void telosvesting::changevest(eosio::name to, uint64_t vest_id, uint64_t new_matures_at_unix_timestamp) {
  require_auth(get_self());
  vests_t vests(get_self(), to.value);
  auto it = vests.find(vest_id);
  check(it != vests.end(), "vesting does not exist");
  
  time_point new_matures_at = time_point(microseconds(new_matures_at_unix_timestamp * 1e6));
  check(new_matures_at > current_time_point() - days(1), "cannot set vesting end date so far into the past");
  
  vests.modify( it, same_payer, [&]( auto& v ) {
    v.matures_at = new_matures_at;
  });
}

void telosvesting::testreset(eosio::name scope) {
  require_auth(_self);
  vests_t vests(get_self(), scope.value);

  auto itr = vests.begin();
  while (itr != vests.end()) {
    itr = vests.erase(itr);
  }
}
