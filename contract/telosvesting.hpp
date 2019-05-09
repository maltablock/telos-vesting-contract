#include <string>

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>

#define EOS_SYMBOL symbol("EOS", 4)
#define TLOS_SYMBOL symbol("TLOS", 4)

// remove this when compiling for production
#define TEST

CONTRACT telosvesting : public eosio::contract {
 public:
  telosvesting(eosio::name receiver, eosio::name code,
               eosio::datastream<const char *> ds)
      : contract(receiver, code, ds), cfg(receiver, receiver.value) {}

  TABLE config { eosio::microseconds default_vesting_period; };
  typedef eosio::singleton<"config"_n, config> config_t;

  struct [[eosio::table("vests")]] vest {
    uint64_t id;
    // eosio::name to; // will be scope
    eosio::time_point matures_at;
    eosio::asset quantity;

    auto primary_key() const { return id; }
    uint64_t by_time() const { return matures_at.time_since_epoch().count(); }
  };
  typedef eosio::multi_index<
      "vests"_n, vest,
      eosio::indexed_by<"time"_n,
                        eosio::const_mem_fun<vest, uint64_t, &vest::by_time>>>
      vests_t;

  config get_config() {
    config c =
        cfg.get_or_default(config{.default_vesting_period = eosio::days(180)});
    return c;
  }

#ifdef TEST
  ACTION testreset(eosio::name scope);
#endif
  ACTION withdraw(eosio::name to);
  ACTION changevest(eosio::name to, uint64_t vest_id,
                    uint64_t new_matures_at_unix_timestamp);
  ACTION setconfig(uint64_t default_vesting_period_microseconds);
  [[eosio::on_notify("eosio.token::transfer")]] void on_transfer(
      eosio::name from, eosio::name to, eosio::asset quantity,
      std::string memo);

 private:
  config_t cfg;
};
