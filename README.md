# Unicove Smart Contract(s)

Smart contracts providing readonly actions to serve as APIs.

Initially these contracts are being designed to meet the needs of Unicove.com and to experiment with the limitations of readonly APIs. Ideally these contracts would mature into system level contracts which are capable of meeting the needs of many wallets, explorers, and other types of applications.

## Contracts

- `api`: A collection of readonly actions to replace the native chain APIs.
- `tokens`: Unused currently, but the start of a token index.

## Developing

This project makes use of `make` to coordinate the different build tools (Antelope CDT, Bun, VeRT, Wharf, etc). These will all need to be installed prior to development.

A number of the important commands are:

- `make test`: Compiles the contract with the `DEBUG` flag, generates the contract TypeScript code using Wharf, and then uses Bun to run the VeRT unit tests.
- `make build`: Compiles the contract with the `DEBUG` flag.
- `make build/production`: Compiles the contract for production.
- `make codegen`: Generates the contract TypeScript code against the currently compiled contract.
