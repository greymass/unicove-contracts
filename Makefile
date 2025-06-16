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

build/delphihelper:
	make -C contracts/delphihelper build

build/delphihelper/debug:
	make -C contracts/delphihelper build/debug

build/delphihelper/production:
	make -C contracts/delphihelper build/production

build/noact:
	make -C contracts/noact build

build/noact/debug:
	make -C contracts/noact build/debug

build/noact/production:
	make -C contracts/noact build/production

build/tokens:
	make -C contracts/tokens build

build/tokens/debug:
	make -C contracts/tokens build/debug

build/tokens/production:
	make -C contracts/tokens build/production

# TESTNET

testnet:
	make -C contracts/api testnet

testnet/noact:
	make -C contracts/noact testnet

testnet/tokens:
	make -C contracts/tokens testnet

# MAINNET

mainnet/api:
	make -C contracts/api mainnet

mainnet/delphihelper:
	make -C contracts/delphihelper mainnet

mainnet/noact:
	make -C contracts/noact mainnet

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
	npx @wharfkit/cli generate --json ./contracts/api/build/api.abi --file ./codegen/api.ts unicove.gm