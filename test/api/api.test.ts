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
                /*
                [[eosio::action]] void api::setconfig(const checksum256 chain_id,
                                      const name        system_contract,
                                      const name        system_contract_msig,
                                      const name        system_token_contract,
                                      const symbol      system_token_symbol,
                                      const symbol      system_ramcore_symbol,
                                      const symbol      system_ram_symbol,
                                      const symbol      system_rex_symbol,
                                      const bool        gifted_ram_enabled)
                */
                await contracts.api.actions
                    .setconfig([
                        '73e4385a2708e6d7048834fbc1079f2fabb17b3c125b146af438971e90716c4d',
                        'foo',
                        'foo.msig',
                        'foo.token',
                        '4,FOO',
                        '4,RAMCORE',
                        '4,RAM',
                        '4,REX',
                        true,
                    ])
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
                expect(
                    Asset.Symbol.from(rows[0].system_token_symbol).equals('0,UNKNOWN')
                ).toBeTrue()
            })
        })
        describe('error', () => {
            test('require contract auth', async () => {
                const action = contracts.api.actions
                    .setconfig([
                        '73e4385a2708e6d7048834fbc1079f2fabb17b3c125b146af438971e90716c4d',
                        'foo',
                        'foo.msig',
                        'foo.token',
                        '4,FOO',
                        '4,RAMCORE',
                        '4,RAM',
                        '4,REX',
                        true,
                    ])
                    .send('alice')
                expect(action).rejects.toThrow('missing required authority api')
            })
        })
    })
})
