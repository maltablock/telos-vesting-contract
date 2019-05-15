const { sendTransaction, getErrorDetail } = require(`../utils`)

const { CONTRACT_ACCOUNT } = process.env

async function action() {
    try {
        await sendTransaction({
            account: `eosio.token`,
            name: `transfer`,
            actor: `testvest1`,
            data: {
                from: `testvest1`,
                to: CONTRACT_ACCOUNT,
                quantity: `1.0000 EOS`,
                memo: `testvest2`,
            },
        })
        console.log(`SUCCESS`)
    } catch (error) {
        console.error(`${getErrorDetail(error)}`)
    }
}

action()
