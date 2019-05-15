# Telos-Vesting

A Telos/EOS vesting contract where tokens can be transfered to other accounts who can then only withdraw them after a specified amount of time has passed.

## Actions:

* `on_transfer`: Requires the account receiving the tokens as memo
* `changevest`: Allows changing the vesting end date
* `setconfig`: Allows changing the default configuration including default vesting period
* `withdraw`: Allows withdrawing the funds if vesting period is over

## Examples

```bash

```

# Development

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

# License

MIT Copyright (c) 2019 AirdropsDAC

Disclaimer: AirdropsDAC makes its contribution on a voluntary basis as a member of the EOSIO community under MIT license and is not responsible for ensuring the overall performance of the software or any related applications. We make no representation, warranty, guarantee or undertaking in respect of the releases described here, the related GitHub release, or any related documentation, whether expressed or implied, including but not limited to the warranties or merchantability, fitness for a particular purpose and noninfringement. In no event shall we be liable for any claim, damages or other liability, whether in an action of contract, tort or otherwise, arising from, out of or in connection with the software or documentation or the use or other dealings in the software or documentation. Any reference to any third party or third-party product, resource or service is not an endorsement or recommendation by AirdropsDAC. We are not responsible, and disclaim any and all responsibility and liability, for your use of or reliance on any of these resources. Third-party resources may be updated, changed or terminated at any time, so the information here may be out of date or inaccurate.
