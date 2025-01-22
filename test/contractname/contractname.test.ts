import {beforeEach, describe, test} from 'bun:test'

import {contractnameContract, contracts, resetContracts} from '../helpers'

describe(contractnameContract, () => {
    beforeEach(async () => {
        await resetContracts()
    })

    describe('action::foo', () => {
        test('success', async () => {
            await contracts.contractname.actions.foo(['bar']).send()
        })
    })
})
