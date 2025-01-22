#!/bin/bash
SHELL := /bin/bash
TEST_FILES := $(shell find src -name '*.ts')
BIN := ./node_modules/.bin

SRC_CONTRACT_NAME = contractname
MAINNET_NODE_URL = https://eos.greymass.com
MAINNET_CONTRACT_ACCOUNT = contractname
TESTNET_NODE_URL = https://jungle4.greymass.com
TESTNET_CONTRACT_ACCOUNT = contractname

build: | build/dir
	cdt-cpp -abigen -abigen_output=build/${SRC_CONTRACT_NAME}.abi -o build/${SRC_CONTRACT_NAME}.wasm src/${SRC_CONTRACT_NAME}.cpp -R src -I include -D DEBUG

build/debug: | build/dir
	cdt-cpp -abigen -abigen_output=build/${SRC_CONTRACT_NAME}.abi -o build/${SRC_CONTRACT_NAME}.wasm src/${SRC_CONTRACT_NAME}.cpp -R src -I include -D DEBUG

build/production: | build/dir
	cdt-cpp -abigen -abigen_output=build/${SRC_CONTRACT_NAME}.abi -o build/${SRC_CONTRACT_NAME}.wasm src/${SRC_CONTRACT_NAME}.cpp -R src -I include

build/dir:
	mkdir -p build

clean:
	rm -rf build

testnet: build/debug
	cleos -u $(TESTNET_NODE_URL) set contract $(TESTNET_PLATFORM_ACCOUNT) \
		build/ ${SRC_CONTRACT_NAME}.wasm ${SRC_CONTRACT_NAME}.abi

mainnet: build/production
	cleos -u $(MAINNET_NODE_URL) set contract \
		--dont-broadcast --skip-sign --expiration 259200 --json-file msig.json \
		${MAINNET_PLATFORM_ACCOUNT} build/ ${SRC_CONTRACT_NAME}.wasm ${SRC_CONTRACT_NAME}.abi
	cleos -u $(MAINNET_NODE_URL) multisig propose_trx ${MAINNET_MSIG_PROPOSAL} ${MAINNET_MSIG_SIGNERS} \
		msig.json ${MAINNET_MSIG_PROPOSER} -p ${MAINNET_MSIG_PERMISSION}

test: build/debug node_modules build/contract.ts
	bun test

build/contract.ts:
	npx @wharfkit/cli generate --json ./build/${SRC_CONTRACT_NAME}.abi --file ./test/contracts/${SRC_CONTRACT_NAME}.ts ${SRC_CONTRACT_NAME}

codegen/dir:
	mkdir -p codegen

codegen/eosio.ts:
	npx @wharfkit/cli generate --url https://jungle4.greymass.com --file ./test/contracts/eosio.ts eosio

codegen/eosio.token.ts:
	npx @wharfkit/cli generate --url https://jungle4.greymass.com --file ./test/contracts/eosio.token.ts eosio.token

.PHONY: check
check: cppcheck jscheck

.PHONY: cppcheck
cppcheck:
	clang-format --dry-run --Werror src/*.cpp include/${SRC_CONTRACT_NAME}/*.hpp

.PHONY: jscheck
jscheck: node_modules
	@${BIN}/eslint test --ext .ts --max-warnings 0 --format unix && echo "Ok"

.PHONY: format
format: cppformat jsformat

.PHONY: cppformat
cppformat:
	clang-format -i src/*.cpp include/${SRC_CONTRACT_NAME}/*.hpp

.PHONY: jsformat
jsformat: node_modules
	@${BIN}/eslint src --ext .ts --fix

.PHONY: distclean
distclean: clean
	rm -rf node_modules/

node_modules:
	bun install
