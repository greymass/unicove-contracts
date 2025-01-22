BIN := ./node_modules/.bin
SHELL := /bin/bash

# CONTRACT BUILD

build: build/contractname/production

build/debug: build/contractname/debug

build/production: build/contractname/production

build/contractname:
	make -C contracts/contractname build

build/contractname/debug:
	make -C contracts/contractname build/debug

build/contractname/production:
	make -C contracts/contractname build/production

# UNIT TESTS

test/contractname: build/contractname/debug node_modules codegen
	bun test

node_modules:
	make -C contracts/contractname node_modules

.PHONY: jscheck
jscheck: node_modules
	@${BIN}/eslint test --ext .ts --max-warnings 0 --format unix && echo "Ok"

test: build/debug codegen node_modules 
	bun test

# CODEGEN

.PHONY: codegen
codegen: codegen/contractname

codegen/contractname:
	npx @wharfkit/cli generate --json ./contracts/contractname/build/contractname.abi --file ./codegen/contractname.ts contractname