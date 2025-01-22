import {beforeEach, describe, test} from 'bun:test'

import {apiContract, contracts, resetContracts} from '../helpers'

describe(apiContract, () => {
    beforeEach(async () => {
        await resetContracts()
    })

    describe('action::getaccount', () => {
        test('success', async () => {
            await contracts.api.actions.getaccount(['bar']).send()
        })
    })
})
