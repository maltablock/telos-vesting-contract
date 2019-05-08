/* eslint-disable no-underscore-dangle */
const { api } = require(`../config`)
const { sendTransaction } = require(`../utils`)

const { CONTRACT_ACCOUNT } = process.env

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

const getLastVestingEntry = async () =>
    api.rpc
        .get_table_rows({
            json: true,
            code: CONTRACT_ACCOUNT,
            scope: CONTRACT_ACCOUNT,
            table: `vests`,
            lower_bound: 0,
            upper_bound: -1,
            limit: 9999,
        })
        .then(result => result.rows.pop())

describe(`contract`, () => {
    beforeEach(async () => {
        await sendTransaction({ name: `testreset` })
    })

    test(`setconfig`, async () => {
        expect.assertions(1)
        const newVestingTime = 5 * 24 * 60 * 60 * 1e6
        await sendTransaction({
            name: `setconfig`,
            data: {
                default_vesting_time_microseconds: newVestingTime,
            },
        })

        const config = await getConfig()
        expect(config.default_vesting_time._count).toBe(newVestingTime)
    })
})
