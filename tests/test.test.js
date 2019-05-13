/* eslint-disable no-underscore-dangle */
const { api, keys } = require(`../config`)
const { sendTransaction } = require(`../utils`)

const { CONTRACT_ACCOUNT } = process.env

// running three actions sometimes goes beyond default 5s timeout
jest.setTimeout(20000)

const getConfig = async () =>
    api.rpc
        .get_table_rows({
            json: true,
            code: CONTRACT_ACCOUNT,
            scope: CONTRACT_ACCOUNT,
            table: `config`,
            lower_bound: 0,
            upper_bound: -1,
            limit: 9999,
        })
        .then(result => result.rows.pop())

const getLastVestingEntry = async account =>
    api.rpc
        .get_table_rows({
            json: true,
            code: CONTRACT_ACCOUNT,
            scope: account,
            table: `vests`,
            lower_bound: 0,
            upper_bound: -1,
            limit: 9999,
        })
        .then(result => result.rows.pop())

describe(`contract`, () => {
    const newVestingPeriod = 5 * 24 * 60 * 60 * 1e6

    beforeAll(async () => {
        await sendTransaction({
            name: `testreset`,
            data: {
                scope: `testvest2`,
            },
        })
    })

    test(`setconfig`, async () => {
        expect.assertions(1)

        await sendTransaction({
            name: `setconfig`,
            data: {
                default_vesting_period_microseconds: newVestingPeriod,
            },
        })

        const config = await getConfig()
        expect(Number.parseInt(config.default_vesting_period._count, 10)).toEqual(newVestingPeriod)
    })

    test(`transfer`, async () => {
        expect.assertions(2)
        const quantity = `1.0000 EOS`
        await sendTransaction({
            account: `eosio.token`,
            name: `transfer`,
            actor: `testvest1`,
            data: {
                from: `testvest1`,
                to: CONTRACT_ACCOUNT,
                quantity,
                memo: `testvest2`,
            },
        })

        const vest = await getLastVestingEntry(`testvest2`)
        expect(vest).toMatchObject({ quantity })
        const expectedMaturesAt = new Date(Date.now() + newVestingPeriod / 1000)
        const maturesAt = new Date(vest.matures_at)
        expect(maturesAt.toDateString()).toMatch(expectedMaturesAt.toDateString())
    })

    test(`withdraw + changevest`, async () => {
        expect.assertions(2)

        let failed = false
        try {
            await sendTransaction({
                name: `withdraw`,
                actor: `testvest2`,
                data: {
                    to: `testvest2`,
                },
            })
        } catch (error) {
            failed = true
        }
        expect(failed).toBe(true)

        const vest = await getLastVestingEntry(`testvest2`)
        await sendTransaction({
            name: `changevest`,
            actor: CONTRACT_ACCOUNT,
            data: {
                to: `testvest2`,
                vest_id: vest.id,
                new_matures_at_unix_timestamp: Math.floor(Date.now() / 1e3 - 10), // 10 seconds in past
            },
        })

        await sendTransaction({
            name: `withdraw`,
            actor: `testvest2`,
            data: {
                to: `testvest2`,
            },
        })

        expect(true).toBe(true)
    })
})
