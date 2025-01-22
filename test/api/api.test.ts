import {beforeEach, describe, test} from 'bun:test'

import {api, apiContract, contracts, resetContracts} from '../helpers'
import {APIClient, Serializer} from '@wharfkit/antelope'

describe(apiContract, () => {
    beforeEach(async () => {
        await resetContracts()
    })

    describe('action::getaccount', () => {
        test('success', async () => {
            const contract = new api.Contract({
                account: 'api.gm',
                client: new APIClient({url: 'https://jungle4.greymass.com '}),
            })
            const result = await contract.readonly('getaccount', {account: 'babyproofing'})
            console.log(Serializer.objectify(result))
        })
    })
})
