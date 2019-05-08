cmake_minimum_required(VERSION 3.5)
project(telosvesting VERSION 1.0.0)

find_package(eosio.cdt)

# add contract
add_contract( telosvesting telosvesting telosvesting.cpp )

### add the path to where the ricardian contracts/clauses are found
target_compile_options( telosvesting.wasm PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR} )
