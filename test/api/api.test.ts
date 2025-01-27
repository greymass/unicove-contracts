import {beforeEach, describe, expect, test} from 'bun:test'

import {apiContract, contracts, resetContracts} from '../helpers'
import {Serializer} from '@wharfkit/antelope'

describe(apiContract, () => {
    beforeEach(async () => {
        await resetContracts()
    })

    describe('action::addtoken', () => {
        describe('success', () => {
            test('add tokens', async () => {
                await contracts.api.actions.addtoken(['eosio.token', '4,EOS']).send()
                await contracts.api.actions.addtoken(['scrap', '0,SCRAP']).send()

                const rows = await contracts.api.tables.tokens().getTableRows()
                expect(rows).toHaveLength(2)
                expect(Serializer.objectify(rows[0])).toMatchSnapshot()
                expect(Serializer.objectify(rows[1])).toMatchSnapshot()
            })
        })
        describe('error', () => {
            test('require contract auth', async () => {
                const action = contracts.api.actions
                    .addtoken(['eosio.token', '4,EOS'])
                    .send('alice')
                expect(action).rejects.toThrow('missing required authority api')
            })
        })
    })

    describe('action::removetoken', () => {
        describe('success', () => {
            test('remove tokens', async () => {
                await contracts.api.actions.addtoken(['eosio.token', '4,EOS']).send()
                await contracts.api.actions.addtoken(['scrap', '0,SCRAP']).send()

                const rows = await contracts.api.tables.tokens().getTableRows()
                expect(rows).toHaveLength(2)
                expect(Serializer.objectify(rows[0])).toMatchSnapshot()
                expect(Serializer.objectify(rows[1])).toMatchSnapshot()

                await contracts.api.actions.removetoken([1]).send()

                const rows2 = await contracts.api.tables.tokens().getTableRows()
                expect(rows2).toHaveLength(1)
                expect(Serializer.objectify(rows[0])).toMatchSnapshot()

                await contracts.api.actions.removetoken([0]).send()

                const rows3 = await contracts.api.tables.tokens().getTableRows()
                expect(rows3).toHaveLength(0)
            })
        })
        describe('error', () => {
            test('require contract auth', async () => {
                await contracts.api.actions.addtoken(['eosio.token', '4,EOS']).send()
                const action = contracts.api.actions.removetoken([0]).send('alice')
                expect(action).rejects.toThrow('missing required authority api')
            })
        })
    })
})
