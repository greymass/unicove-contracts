import {APIClient, Asset} from '@wharfkit/antelope'
import {SigningRequest} from '@wharfkit/signing-request'
import zlib from 'pako'

import {Contract} from '../../codegen/api'
import ABICache from '@wharfkit/abicache'

const client = new APIClient({
    url: 'https://eos.greymass.com',
})

const contract = new Contract({
    client,
})

interface Token {
    key: string
    symbol: string
    account: string
    chain: string
    supply: {
        circulating: number
        max: number
        precision: number
        last_update: string
    }
    metadata: {
        name: string
        website: string
        logo: string
        created_at: string
        desc: string
    }
    chain_rank: string
}

async function fetchTokens(): Promise<Token[]> {
    const url =
        'https://raw.githubusercontent.com/greymass/antelope-tokens.json/refs/heads/main/tokens.json'
    const response = await fetch(url)
    if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`)
    }
    return response.json()
}

const tokens = await fetchTokens()

const action = {
    tokens: [],
}

tokens
    .filter((token) => {
        return token.chain === 'eos'
    })
    .forEach((token) => {
        action.tokens.push({
            contract: token.account,
            symbol: Asset.Symbol.from(`${token.supply.precision},${token.symbol.toUpperCase()}`),
        })
    })

const abiProvider = new ABICache(client)

const request = await SigningRequest.create(
    {
        action: contract.action('addtokens', action),
    },
    {abiProvider, zlib}
)

console.log(String(request))
