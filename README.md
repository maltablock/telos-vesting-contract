# Telos-Vesting

An EOS/Telos vesting contract where tokens can be transfered to other accounts who can then only withdraw the tokens after a specified amount.

## Actions:

* `on_transfer`: Requires the account receiving the tokens as memo
* `changevest`: Allows changing the vesting end date
* `setconfig`: Allows changing the default configuration including default vesting period
* `withdraw`: Allows withdrawing the funds if vesting period is over

# Setup

Bootstrapped with [generator-eos](https://github.com/MrToph/generator-eos)

TL;DR:

```bash
cd build
cmake ../contract
cd ..
# set HTTP endpoint in .development.env
npm run init # creates initial accounts for contract + testing
npm run compile # compiles contract
npm run deploy # sets contract code and abi configured through .development.env
npm test
```

## eosio-cpp

Requires `eosio-cpp (>=v1.6.1)` to be installed from the [eosio.cdt](https://github.com/EOSIO/eosio.cdt) package to compile the smart contract.
Also needs `cmake` for compiling the smart contract.

(MacOS: `brew install cmake`)

## Local Blockchain setup

Run any EOS version with nodeos.

Change the EOS http endpoint in `.development.env` to your one (here `http://127.0.0.1:7777`).

When starting from an empty blockchain, one must first deploy the `eosio.token` contract and do other initialization steps by running:

```bash
npm run init_blockchain # deploys eosio.token
```

To create necessary accounts for the contract and testing, run:

```bash
npm run init # creates accounts
```

## Compiling

This template uses `cmake` to build the contract. Run the following commands once to setup the process:

```
cd build
cmake ../contract
```

Now you can run `npm run compile` which will run `make` to create the `.wasm` and `.abi` in `/build`.

## Deployment

```
npm run deploy
```

## Testing the smart contract

```
npm test
```
