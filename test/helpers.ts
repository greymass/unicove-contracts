import {Blockchain} from '@proton/vert'
import {TimePointSec} from '@greymass/eosio'

import * as ContractNameContract from '../codegen/contractname.ts'

export const ContractName = ContractNameContract

export const blockchain = new Blockchain()
export const alice = 'alice'
export const bob = 'bob'
blockchain.createAccounts(bob, alice)

export const contractnameContract = 'contractname'

export const contracts = {
    contractname: blockchain.createContract(
        contractnameContract,
        `./contracts/contractname/build/contractname`,
        true
    ),
}

export async function resetContracts() {
    await contracts.contractname.actions.wipe().send()
}

export function advanceTime(seconds: number) {
    const newDate = new Date(blockchain.timestamp.toMilliseconds() + seconds * 1000)
    blockchain.setTime(TimePointSec.from(newDate))
}
