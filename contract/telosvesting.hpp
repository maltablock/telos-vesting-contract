#include <string>

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>

#define EOS_SYMBOL symbol("EOS", 4)

CONTRACT telosvesting : public eosio::contract {
 public:
  telosvesting(eosio::name receiver, eosio::name code, eosio::datastream<const char *> ds)
      : contract(receiver, code, ds), cfg(receiver, receiver.value) {}

  TABLE config { eosio::microseconds default_vesting_time; };

  typedef eosio::singleton<"config"_n, config> config_t;

  config get_config() {
    config c =
        cfg.get_or_create(_self, config{.default_vesting_time = eosio::days(180)});
    return c;
  }

  ACTION testreset();
  ACTION setconfig(uint64_t default_vesting_time_microseconds);
  [[eosio::on_notify("eosio.token::transfer")]] void on_transfer(
      eosio::name from, eosio::name to, eosio::asset quantity,
      std::string memo);

 private:
  config_t cfg;
};
