# Blockchain, Bitcoin, and The Related Technologies

## Overview

This document has my notes of learning the blockchain-related technologies.

## Prerequisite Knowledge

In order to understand the blockchain technology better, it is highly recommended to learn about the knowledge below before you proceed.

### Systems, and Distributed Systems

Whenever we are talking about a "system", we are referring to a group of units that collaborate for a common goal. We see many systems in our everyday life, such as the home security system, the central air-conditioning system, the immune system in our bodies. In the realm of information technology, we might have seen a student information management system in schools.

The units that form the system may perform different functions, but they work together to serve the same purpose. In other words, a group of units that don't share the common goal don't form a system.

The units of a system may reside at the same geographical location. For example, a simple student information management system may consist of the units such as user input, data validation, database, and so on, and they may run in the same physical computer. In this case, these units form a "local" system.

When the units of a system reside at geographically different locations, they form a "distributed" system. For example, the immune system spread out in our entire body instead of a single location. A more complex student information management system may have the user input unit running on the user's terminal (such as inside the user's browser), the data validation running on another computer, and the database running in a third computer and is securely locked in a dedicated room.

A distributed system may be complicated as a whole, but its units may not be complicated at all. In fact, many such systems have each unit perform simple and clearly defined functions, and the complexity arises from their interaction. A typical example is our brain: each neuron acts in a very simple way, but as a whole they create a very complex system.

In the example of the brain, it might be arguable whether it is a "local" or a "distributed" system. It kind of depends on how you define "geographically different locations". In an IT system, we usually define it as "running on different computers", even if those computers are put in the same room.

A distributed system can be organized in different structures, two of which are client-server structure and peer-to-peer structure. The Bitcoin system is a peer-to-peer structure, which is what we are going to discuss immediately.

### Client-server and Peer-to-peer (P2P)

A distributed system that is organized in the client-server structure has a central server that all the clients connect to. In such a structure, the clients don't communicate with each other directly. Instead, they communicate with the server; the server may forward messages from one client to another, making it possible for the two clients communicate indirectly. The client-server model can be depicted using the following diagram.

![Client-server model](https://upload.wikimedia.org/wikipedia/commons/f/fb/Server-based-network.svg)

(**TODO:** Replace the figure.)

In contrast, a peer-to-peer (P2P) structure is one in which the participants can connect to one another directly, hence communicate directly without the need of a central server. Note that a participant does not have to connect to every other participants in a P2P network. They connect on a needed base. A peer-to-peer model can be depicted as follows.

![Peer-to-peer model](https://upload.wikimedia.org/wikipedia/en/f/fa/Unstructured_peer-to-peer_network_diagram.png)

(**TODO:** Replace the figure.)

When a participant in a peer-to-peer network wants to broadcast a message to all its peers, it can't send the message to every other peer because it only connects to a limited set of them. Therefore, this participant simply broadcasts the message to the peers that it connects to, then its connected peers broadcast the received message to all the peers that they connect to, and this process continues on until all the participants receive the message. This kind of broadcasting process is called "flooding".

In the case of flooding, when two participants broadcast their messages to the entire network at exactly the same time, the other participants may receive these two messages at different times, depending on how many intermediate levels there are between the sender and the receiver.

(**TODO:** Add a figure.)

### Hash Function and Computational Power

Another term you definitely have heard frequently about Bitcoin is "mining". We will leave the discussion of "mining" later, but in this section, we will talk about the mathematical foundation of "mining" - the **hash function**.

In mathematics, a generic function `f` can be described as `y = f(x)` where `x` is the input and `y` is the output. When we learn about functions in high schools or colleges, `x` and `y` are usually both numeric. For example, we may define a function that turns an integer into an even number: `y = 2 * x`, where `x` is an integer.

In computer science, the input `x` is not necessarily numeric. It could also be a piece of text, or a combination of both. A piece of text, also called a "string of characters", or simply "string", is usually referred to by putting it in between a pair of quotation marks. For example, `"Hello, this is a string of characters."` is a string.

A numeric value can be turned into a string. For example, the number `12` can be turned into the string `"12"`. But the reverse is not always true. The string `"12"` can be read and interpreted as the number `12`, but usually, you can't turn the word `"Hello"` into a numeric value because there is no meaningful interpretation.

However, a hash function, in computer science, is designed to turn literally anything into a numeric value. Note that when we are talking about "a hash function", we are not talking about one specific function but a family of them. There are many well-known hash functions world widely, one of which is called `SHA-256`; in fact, you can also design your own.

Totally depending on the design, a hash function may take a numeric value as its input and output another numeric value; alternatively, a hash function may take a string of characters as input and output a numeric value.

The hash function is a quite fundamental part of the blockchain technology, and is the key to understand the "mining" process and why blockchain is secure. But you don't have to learn how to design and implement a hash function. You only need to remember two very important characteristics of hash functions: randomness of output, and the sensitivity of input.

The output of a hash function looks completely random from the observer's perspective. You won't be able to find a pattern to connect the input and the output. For example, when given a normal function like `y = 2 * x`, the input `1` produces the output `2`. When you increase the input by three times, which is `3`, you can confidentally expect the output to be increased by three times, too, which is `6`. When given a hash function, say, `y = H(x)`, and told that the input `1` generates the output `1000`, you can't predict what `2` would generate. It might generate `39`, or `7`, or `19287345`, or some other arbitrary number. With that said, when you are given an input, you may predict the output only when you also know the implementation details. The other way, however, is not true: given the output of a hash function, you may not figure out the input that generates the output even if you know all the implementation details. The technical term for this characteristic is called "one-way function".

The "one-way function" means if you want to find the input that generates a specific output, the practically only way is to enumerate all possible inputs and see which input generates the expected output. This is labor-intensive work. When done by computers, the time the work takes depends on how fast your CPU is and how many computers you have. The faster the CPU is, the earlier you can find an appropriate input. The more computers you have, you can divide the entire input space into smaller spaces and let all the computers process them simultaneously, hence the shorter time it takes to find an appropriate answer. The speed of the CPU and the number of the computers determine how powerful your computational capability is, hence "computational power".

Note that a hash function should produce a stable output when given the same input regardless what time the hash function is applied, on which computer, or in which country. In other words, given the same input and the same hash function, the output is always the same. In the above-mentioned example `y = H(x)`, if you already know `1000 = H(1)`, you won't be able to predict what `2` generates before the first time you feed it into `H`. However, if you apply `H` on `2` and it returns `39`, you can confidentally say that `2` always results in `39` through `H` no matter you apply the function today or one week later, on your own computer or your friend's computer, in China or United States. In sum, the hash function only uses the input data to calculate the output.

A hash function is usually input sensitive which means the alternation of even a little bit of the input may produce a completely different output. For example, the earlier mentioned well-known hash function `SHA-256`, turns the input `"Hello, world!` into the number `22331814027392488307105736075480205742348666473969333634173732071459215699411`, but if you replace the ending exclamation point with a period, making it `"Hello, world."`, the output will become `112519442519776671206630282554703694312805327356566406491744467208085897409263`. Later you will see that the input sensitivity is also how the blockchain secure the integrity of the data.

## Blockchain Technology Explained

### Blockchain and Bitcoin

Why is the technology called "blockchain"? Because it is a "chain" of "blocks" which looks like this:


```
+-----+     +-----+     +-----+     +-----+                 +-----+
|     |     |     |     |     |     |     |                 |     |
| 1st <-----+ 2nd <-----+ 3rd <-----+ 4th <-----+ ... <-----+ Nth |
|     |     |     |     |     |     |     |                 |     |
+-----+     +-----+     +-----+     +-----+                 +-----+
```

Later, I will explain what every "block" contains and how they are "chained" together.

Blockchain is the fundamental technology to implement cryptocurrencies, the most well known one of which is Bitcoin. Bitcoin is similar to the fiat currency like the US dollars that we use every day, and the blockchain is the mechanism and rules that everybody follows to prevent this currency from being misused. Throughout this article, I'll use Bitcoin to explain how the blockchain works.

### The Motive of Blockchain

Why was the blockchain technology invented? Because its inventor, [Satoshi Nakamoto](https://en.wikipedia.org/wiki/Satoshi_Nakamoto), wanted to remove the central trusted financial institute, i.e. the banks, to allow the buyers to pay **directly** to the sellers **without the double-spending issue**.

If we use cash in our everyday life, the double-spending issue is usually avoided because once we give a bill to somebody else, we don't own that bill anymore and can't spend the same bill again. However, a person may use counterfeits to do the double-spending. If this person has only one $100 dollar bill, he can only buy goods that's worth $100. However, he may use a counterfeit of a $100 bill to buy more goods. Now he owns goods that's worth $200 in total. Alternatively, when he buys goods from the seller, he may pay the seller with paper instead of the real bills by playing malicious tricks. He could then use the real bills again.

When it comes to the online banking, the double-spending issue is usually avoided because the trusted central authority, the banks, tracks the amount of money each party owns. A central authority may suffer from two problems:

- The central authority is the single failure point. When it fails to function properly, its clients won't be able to do business.
- The central authority increases the cost because it needs to hire a lot of people to keep it functioning properly.

The blockchain technology aims to solve these two problems:

- The distributed nature of the blockchain technology eliminates the single-point failure issue.
- The blockchain is structured in the peer-to-peer structure which allows two users exchange money directly.

### How Blockchain Works

In essence, the central trusted financial institute does two tasks:

- maintaining a central ledger that tracks the amount of money owned by each customer;
- verifying the payment to guarantee no money is spent twice.

As you will learn later, the blockchain itself is the "ledger" that tracks the amount of money owned by each user. Instead of having a central institute maintaining the blockchain(the ledger), every user maintains a copy of the blockchain(the ledger). Without the central financial institute, the buyer can pay to the seller directly. After the payment is done, they both modify their own copy of the blockchain(the ledger) to reflect their most recent financial status. The blockchain technology implements a mechanism to make sure they do the recording honestly.

Like the fiat currency users, the bitcoin users form their own network, and bitcoin is only useable inside this network. The exchange of bitcoin and fiat currency is out of the scope of this article. With that said, only the bitcoin users need to maintain their own copy of the blockchain(the ledger), because they need to record every amount of bitcoin they pay or receive. The users who don't use bitcoin don't need to maintain the blockchain(the ledger).

In order to join the bitcoin network and start to use it, a user must run a piece of software called the "bitcoin wallet." In other words, if the bitcoin network has N users, there will be N "bitcoin wallets". This wallet provides the following functions:

- It provides the interface that its owner, the bitcoin user, pays or receives bitcoins to or from other users.
- It maintains the blockchain(the ledger) so the user knows how many bitcoins he and everybody else has. (Remember that the blockchain tracks the amount of money owned by "each user". In other words, "all the users.")

We will soon explain how the blockchain(the ledger) is maintained from an individual user's perspective. Later we will explain how the technology guarantees the honesty. But before we dive in any further, we need to understand how the bitcoins are created in the first place, how they are exchanged among users, and how to determine how many bitcoins a user owns. 

The term "bitcoin" had been used in two ways in the paragraphs above. It can be used as a collective noun to refer to this particular cryptocurrency, in which case it is uncountable. Meanwhile, it can also be used to refer to a countable unit of this cryptocurrency. In fact, the minimal unit of bitcoin (used collectively) is called "Satoshi" and it cannot be divided further. 100,000,000 Satoshi make up 1 bitcoin; 200,000,000 Satoshi make up 2 bitcoins.

Immediately, the term "Satoshi" has two meanings, too: it is the first name of the blockchain technology inventor, and it is the minimal unit of bitcoin.

When the bitcoin network was launched by Satoshi Nakamoto in 2009, the entire network probably had only him as the only user. Therefore, his "bitcoin wallet" was the only software running at that time. After some special work called "mining", which I will explain later, by his bitcoin wallet, the first batch of bitcoin, totally 50, were created **out of nothing** and added to Satoshi's wallet, officially introducing them into the bitcoin network. Satoshi could then use these bitcoins to buy goods from other bitcoin users who joined the network later and were willing to do business using bitcoins. From then on, these 50 bitcoins started to spread in the bitcoin network as more and more purchases were done.

It might initially sound peculiar to say the bitcoins were created **out of nothing**. Alright, technically, the bitcoins are created by the blockchain system which is **outside the control of the users**. Think about the gold or seashells that were once used as money. They were not created by their users. Instead, some people mined the gold out of sand or picked up the seashells at the shore, both of which were the products of the nature, a system outside the control of their users. The blockchain system is the environment, or the "nature", that all the bitcoin users live in and "mined" the bitcoins out, liking gold mining. This is how bitcoins are created.

The exchange of bitcoins between two users is called a "transaction" which consists of:

- the ID of the buyer who pays the bitcoins, or, "from";
- the ID of the seller who receives the bitcoins, or, "to";
- the number of exchanged bitcoins, or, "amount".

Remember that I said earlier that a user always has a bitcoin wallet to use. In other words, the users and the wallets are a one-on-one match. Therefore, we can use the ID of the wallet to identify its owner, the user, assuming that the user maintains his wallet securely.

Therefore, the amount of the bitcoins that a user owns can be deduced from all the transactions he gets involved in. For example, when Satoshi got his first 50 bitcoins, he got them in a transaction that looks like this:

```
  From: The blockchain system
    To: Satoshi Nakamoto's bitcoin wallet
Amount: 50 bitcoins
```

Later, when a new user called Alice joined the bitcoin network, she had no bitcoin initially. When Satoshi purchased something from her and paid with bitcoin, say, 10, there would be another transaction:

```
  From: Satoshi Nakamoto's bitcoin wallet
    To: Alice's bitcoin wallet
Amount: 10 bitcoins
```

So far, there were two transactions that Satoshi got involved in and one transaction that Alice got involved in. We can figure out how many bitcoins they owned from these transactions:

- Satoshi owned 40 bitcoins, which was 0 + 50 - 10. He initially had no bitcoins. Later he "mined" 50 bitcoins. Then he paid 10 to Alice.
- Alice owned 10 bitcoins, which was 0 + 10. She initially had no bitcoins, either. Later she received 10 bitcoins from Satoshi by selling something to him.

Therefore, tracking the amount of bitcoins of all the users is equivalent to tracking all the transactions. Because the ledger is the facility that tracks the amount of money every user has, the ledger can achieve this goal by tracking all the transactions. Because the blockchain is the ledger, the blockchain tracks all the transactions. Because every user has a copy of the blockchain(the ledger), and because the bitcoin wallet is the entity that maintains the owner's copy of the blockchain, the bitcoin wallet software actually tracks (a copy) all the transactions.

If you look back at the blockchain figure at the beginning of this article, you should now understand that:

- It is the collection of all the transactions made by all the users. They are somehow recorded in the form of chained blocks.
- It is stored inside the bitcoin wallet.

I'm then going to explain how the transactions are recorded as a chain of blocks.

As I said earlier, the blockchain tracks the number of bitcoins every user has by tracking all the transactions, which is done by recording the transactions in the blockchain. The transaction recording is not done by the users manually, but by the bitcoin wallet, the software, automatically.

Earlier I said "After the payment is done, they both modify their own copy of the blockchain(the ledger) to reflect their most recent financial status," which may suggest that both users make the changes independently. Technically, this is precise. The transaction recording is done as a collective effort of all the users in the entire bitcoin network. Let's assume that when Satoshi bought something from Alice, another two users, Bob and Caroline, had already joined the network, too. This would be what happened when the bitcoin payment happened:

- When Satoshi paid 10 bitcoins to Alice by operating his bitcoin wallet, the wallet generated the transaction and broadcasted it to the entire bitcoin network.
- Because the transaction was broadcasted, Alice's, Bob's and Caroline's wallets all received this transaction. Satoshi's wallet of course knew the transaction, too.
- Instead of modifying their own copy of the blockchain using the transaction information, they started to compete for the right to record this transaction by doing a piece of special work called "mining". For now, all you need to know is:
  - Mining is time consuming that takes about 10 minutes to finish on average.
  - However, it's "on average", which means some "miners" may finish earlier than others.
- The blockchain system makes the rule that only the winning "miner" has the right to record the transaction. Let's say in this example, Caroline's wallet won the "mining" competition. Upon her wallet finished the "mining" work, it recorded the transaction into a data structure called a "block" and immediately broadcasted this "block" along with the necessary information that proved she finished the "mining" successfully, aka Proof-of-Work, to the entire bitcoin network.
- Because this "block" was broadcasted, Alice's, Bob's and Satoshi's wallets all received this "block". Their wallets independently verified the "block" and the Proof-of-Work. If the verification succeeded, they would stop their own "mining" work, acknowledged Caroline's wallet's work, and incorporated this "block" into their own copy of the blockchain.
- Now, all the four users' copies of the blockchain were modified and all reflected the latest and correct financial status of everyone.

The time-consuming "mining" is the key to the security of the blockchain, and I'm going to talk about it later.

In the simple example above, the "block" that Caroline's wallet generated only contained one transaction between Satoshi and Alice. In reality, such a block could contain hundreds of transactions because there are a large number of users doing business frequently, thus generating many transactions all the time. For example, when the wallets of Alice, Bob, Caroline and Satoshi were "mining", another three transactions happened among them. These transactions were broadcasted to everyone else, too, in the same way as the first transaction. However, because these four wallets had already started "mining", they put the three new transactions aide temporarily in order to finish the current work first. Once Caroline's wallet broadcasted her mined block and got incorporated into everyone's copy of the blockchain, they came back, picked up the three transactions, and started to compete for the right to record them again. Once one of them won, the created block would include all the three new transactions and then be broadcasted to everybody else.

Now you should also understand what a "block" contains: from the perspective of an individual bitcoin wallet, it contains the transactions between two time points:

- Time point #1: when the wallet started to "mine" the previous block;
- Time point #2: when the wallet finishes incorporating the block into its own copy of the blockchain.

Now we know how the blocks are created. How are they chained?

We had learned earlier that every user's wallet has a full copy of the entire blockchain(the ledger). For most of the time, the blockchain is straightforward and doesn't have any fork. Therefore, when the wallet incorporates a new block into the blockchain, it simply adds the new block to the tip of the blockchain, making the blockchain one block longer than before.

However, there are occasions in which the blockchain may have forks. When a fork occurs to the blockchain, the wallet will always treat the longer forked chain as the "trustable" blockchain. It doesn't matter if the longer chain records the correct transactions or not. As long as a forked chain is longer than the other, it is considered the "truth". In other words, if the attacker can somehow create a fork that contains false information and make this fork grow faster, hence longer, than the fork that contains the true information, the blockchain system will treat this false fork as "truth".

Therefore, maintaining the security of the blockchain is to make sure the honest users control enough computational power so the forked chain of true truth always grows faster than the potentially malicious fork.

Let's use some examples to show how the attackers may succeed or fail.

Let's still consider the small bitcoin network that only had Alice, Bob, Caroline, and Satoshi, all of whom were honest users. In this case, the blockchain was always secure and trustable.

Later, a new user Sutler joined the network. He was an honest user at the beginning and sold some personal stuff to Alice and Bob in two transactions and obtained totally 15 bitcoins. Once he realized how convenient bitcoin is, however, he started to plot to crack the blockchain system to obtain more bitcoins illegally.

### Additional Details

#### How The Genesis Block Was Created

According to the ["Genesis block" page on Bitcoin Wikipedia](https://en.bitcoin.it/wiki/Genesis_block), the genesis block is "almost always hard-coded into the software of the applications that utilize its block chain."

- [1] [Why can’t the genesis block coinbase be spent?](https://bitcoin.stackexchange.com/a/10019/60763)
- [2] [The first 50BTC block reward can't be spent. Why?](https://www.reddit.com/r/Bitcoin/comments/1nc13r/the_first_50btc_block_reward_cant_be_spend_why/)

## Attacks on Blockchain

### Sybil Attack

According to the [Wikipedia page](https://en.wikipedia.org/wiki/Sybil_attack):

> The Sybil attack in computer security is an attack wherein a reputation system is subverted by forging identities in peer-to-peer networks. ... In a Sybil attack, the attacker subverts the reputation system of a peer-to-peer network by creating a large number of pseudonymous identities, using them to gain a disproportionately large influence.

The [Bitcoin Wikipedia page](https://en.bitcoin.it/wiki/Weaknesses#Sybil_attack) explains the potential issues the Sybil attack can cause. For example:

> - The attacker can refuse to relay blocks and transactions from everyone, effectively disconnecting you from the network
> - The attacker can relay only blocks that they create, effectively putting you on a separate network and then also leaving you open to double-spending attacks

### 51% Attack

The white paper [1] says:

> The incentive may help encourage nodes to stay honest. If a greedy attacker is able to assemble more CPU power than all the honest nodes, he would have to choose between using it to defraud people by stealing back his payments, or using it to generate new coins. He ought to find it more profitable to play by the rules, such rules that favour him with more new coins than everyone else combined, than to undermine the system and the validity of his own wealth.

### Others

There seems to be many forms (or names) of attacks:

- [Finney attack](https://bitcoin.stackexchange.com/a/4963/60763)
- [Race attack](https://bitcoin.stackexchange.com/a/74283/60763)
- [Double-spend attack](https://bitcoin.stackexchange.com/a/74283/60763)
- [Eclipse attack vs. Sybil attack](https://bitcoin.stackexchange.com/q/61151/60763)

Charlie Lee said on [this tweet](https://twitter.com/satoshilite/status/999738656414814208?lang=en):

> If they 51% attack and mine empty blocks and orphan all other blocks. Rest of miners will leave. People can't move coins, so no one buys and price drops. This makes it even less profitable to mine. It likely won't recover until community hard forks to another PoW.

[This answer](https://bitcoin.stackexchange.com/a/49038/60763) says:

> Also, keep in mind that it's not just miners that would have to be fooled into accepting a block with insufficient work. Full nodes also verify blocks as they receive them, and will reject blocks that do not have nonces that meet their own calculated difficulty requirement.

It suggests that when a block is received by a node, the node does not simply uses the target recorded in the received block to validate it. Rather, the node uses its own target to validate the block because, if the block is an honest one, they both should have the same target.

[This answer](https://bitcoin.stackexchange.com/a/43823/60763) says:

> ... and the target stored inside the block header **has to** match the value determined by history.

This can be proven by [these two lines of code](https://github.com/bitcoin/bitcoin/blob/v0.17.0/src/validation.cpp#L3235-L3236):

```cpp
static bool ContextualCheckBlockHeader(const CBlockHeader& block, CValidationState& state, const CChainParams& params, const CBlockIndex* pindexPrev, int64_t nAdjustedTime)
{
    // ...
    // Check proof of work
    const Consensus::Params& consensusParams = params.GetConsensus();
    if (block.nBits != GetNextWorkRequired(pindexPrev, &block, consensusParams))
        return state.DoS(100, false, REJECT_INVALID, "bad-diffbits", false, "incorrect proof of work");
    // ...
}
```

Because of this check, the malicious miner can't use an arbitrary target to gain advantage.


## Questions

- [ ] If the 50 BTC in the genesis block can't be spent, how did the initial batch of users get the usable bitcoins?
- [ ] When a new user joins the bitcoin network, how does he get the copy of the previously constructed blockchain?
- [ ] How exactly does double-spending happen? I can't seem to understand how it could happen in the real world.

## References

- [1] [Bitcoin: A Peer-to-Peer Electronic Cash System](https://bitcoin.org/bitcoin.pdf)
- [2] [Mastering Bitcoin (2nd Edition, Print 2)](https://github.com/yaobinwen/bitcoinbook/tree/second_edition_print2)
- [3] [Blockchain: how a 51% attack works (double spend attack)](https://hackernoon.com/what-is-a-51-attack-or-double-spend-attack-aa108db63474)
