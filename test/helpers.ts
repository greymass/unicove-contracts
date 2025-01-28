import {Blockchain} from '@proton/vert'
import {TimePointSec} from '@greymass/eosio'

import * as ApiContract from '../codegen/api.ts'

export const api = ApiContract

export const blockchain = new Blockchain()
export const alice = 'alice'
export const bob = 'bob'
blockchain.createAccounts(bob, alice)

export const apiContract = 'api'

export const contracts = {
    api: blockchain.createContract(apiContract, `./contracts/api/build/api`, true),
}

export async function resetContracts() {
    await contracts.api.actions.reset().send()
}

export function advanceTime(seconds: number) {
    const newDate = new Date(blockchain.timestamp.toMilliseconds() + seconds * 1000)
    blockchain.setTime(TimePointSec.from(newDate))
}
