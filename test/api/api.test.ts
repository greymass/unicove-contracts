import {beforeEach, describe, expect, test} from 'bun:test'

import {apiContract, contracts, resetContracts} from '../helpers'
import {Asset, Name} from '@wharfkit/antelope'

describe(apiContract, () => {
    beforeEach(async () => {
        await resetContracts()
    })

    describe('action::setconfig', () => {
        describe('success', () => {
            test('set config', async () => {
                await contracts.api.actions
                    .setconfig(['foo', 'foo.msig', 'foo.token', '4,FOO'])
                    .send()
                const rows = await contracts.api.tables.config().getTableRows()
                expect(rows).toHaveLength(1)
                expect(Name.from(rows[0].system_contract).equals('foo')).toBeTrue()
                expect(Name.from(rows[0].system_contract_msig).equals('foo.msig')).toBeTrue()
                expect(Name.from(rows[0].system_token_contract).equals('foo.token')).toBeTrue()
                expect(Asset.Symbol.from(rows[0].system_token_symbol).equals('4,FOO')).toBeTrue()
            })
            test('defaults', async () => {
                const rows = await contracts.api.tables.config().getTableRows()
                expect(rows).toHaveLength(1)
                expect(Name.from(rows[0].system_contract).equals('eosio')).toBeTrue()
                expect(Name.from(rows[0].system_contract_msig).equals('eosio.msig')).toBeTrue()
                expect(Name.from(rows[0].system_token_contract).equals('eosio.token')).toBeTrue()
                expect(Asset.Symbol.from(rows[0].system_token_symbol).equals('4,EOS')).toBeTrue()
            })
        })
        describe('error', () => {
            test('require contract auth', async () => {
                const action = contracts.api.actions
                    .setconfig(['eosio', 'eosio.msig', 'eosio.token', '4,EOS'])
                    .send('alice')
                expect(action).rejects.toThrow('missing required authority api')
            })
        })
    })
})
