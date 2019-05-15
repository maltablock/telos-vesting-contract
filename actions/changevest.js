const { sendTransaction, getErrorDetail } = require(`../utils`)

const { CONTRACT_ACCOUNT } = process.env

async function action() {
    try {
        const transaction = await sendTransaction({
            name: `changevest`,
            actor: CONTRACT_ACCOUNT,
            data: {
                to: `testvest2`,
                vest_id: `0`,
                new_matures_at: new Date().toDateString(),
            },
        })
        console.log(`SUCCESS`)
        console.log(
            transaction.processed.action_traces
                .map(trace => `${trace.console}${trace.inline_traces.map(t => `\n\t${t.console}`)}`)
                .join(`\n`),
        )
    } catch (error) {
        console.error(`${getErrorDetail(error)}`)
    }
}

action()
