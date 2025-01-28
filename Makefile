BIN := ./node_modules/.bin
SHELL := /bin/bash

# CONTRACT BUILD

build: build/api/production

build/debug: build/api/debug

build/production: build/api/production

build/api:
	make -C contracts/api build

build/api/debug:
	make -C contracts/api build/debug

build/api/production:
	make -C contracts/api build/production

# TESTNET

testnet:
	make -C contracts/api testnet

# MAINNET

mainnet:
	make -C contracts/api mainnet

# UNIT TESTS

test/api: build/api/debug node_modules codegen
	bun test

node_modules:
	make -C contracts/api node_modules

.PHONY: jscheck
jscheck: node_modules
	@${BIN}/eslint test --ext .ts --max-warnings 0 --format unix && echo "Ok"

test: build/debug codegen node_modules 
	bun test

# CODEGEN

.PHONY: codegen
codegen: codegen/api

codegen/api:
	npx @wharfkit/cli generate --json ./contracts/api/build/api.abi --file ./codegen/api.ts api