#!/bin/bash
SHELL := /bin/bash
TEST_FILES := $(shell find src -name '*.ts')
BIN := ./node_modules/.bin

SRC_CONTRACT_NAME = noact
MAINNET_NODE_URL = https://eos.greymass.com
MAINNET_CONTRACT_ACCOUNT = noact.gm
TESTNET_NODE_URL = https://jungle4.greymass.com
TESTNET_CONTRACT_ACCOUNT = noact.gm

build: | build/dir
	cdt-cpp -O=2 -abigen -abigen_output=build/${SRC_CONTRACT_NAME}.abi -o build/${SRC_CONTRACT_NAME}.wasm src/${SRC_CONTRACT_NAME}.cpp -R src -I include -I ../common/include  -D DEBUG

build/debug: | build/dir
	cdt-cpp -O=2 -abigen -abigen_output=build/${SRC_CONTRACT_NAME}.abi -o build/${SRC_CONTRACT_NAME}.wasm src/${SRC_CONTRACT_NAME}.cpp -R src -I include -I ../common/include -D DEBUG

build/production: | build/dir
	cdt-cpp -O=2 -abigen -abigen_output=build/${SRC_CONTRACT_NAME}.abi -o build/${SRC_CONTRACT_NAME}.wasm src/${SRC_CONTRACT_NAME}.cpp -R src -I include -I ../common/include -D DEBUG

build/dir:
	mkdir -p build

clean:
	rm -rf build

testnet: build/debug
	cleos -u $(TESTNET_NODE_URL) set contract $(TESTNET_CONTRACT_ACCOUNT) \
		build/ ${SRC_CONTRACT_NAME}.wasm ${SRC_CONTRACT_NAME}.abi

mainnet: build/production
	cleos -u $(MAINNET_NODE_URL) set contract $(MAINNET_CONTRACT_ACCOUNT) \
		build/ ${SRC_CONTRACT_NAME}.wasm ${SRC_CONTRACT_NAME}.abi