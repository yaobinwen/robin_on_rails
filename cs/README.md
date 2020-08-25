# README

## Overview

This folder has the notes about computer science that I had not a chance to learn before.

## Entropy

Here are some references about the explanation of entropy from the perspective of physics:

- [What Is An Intuitive Way To Understand Entropy?](https://www.forbes.com/sites/quora/2016/10/10/what-is-an-intuitive-way-to-understand-entropy/)
- [Entropy Sites — A Guide](http://entropysite.oxy.edu/)
  - [Teaching Entropy Is Simple — If You Discard "Disorder"](http://entropysite.oxy.edu/teaching_entropy.html)

## Randomness

A [pseudorandom number generator (PRNG)](https://en.wikipedia.org/wiki/Pseudorandom_number_generator) does not generate truly random sequence, because the sequence is completely determined by an initial value which is called PRNG's seed. This property is important in practice, because it allows reproduction for easier testing and debugging.

Note that many common PRNGs fail statistical pattern-detection tests which include:

> - Shorter than expected periods for some seed states (such seed states may be called 'weak' in this context);
> - Lack of uniformity of distribution for large quantities of generated numbers;
> - Correlation of successive values;
> - Poor dimensional distribution of the output sequence;
> - The distances between where certain values occur are distributed differently from those in a random sequence distribution.

Therefore, finding a PRNG of good quality is important because otherwise the result which is based on a sequence of low quality can be misleading.

The first PRNG to avoid major problems and still run fairly quickly was the [Mersenne Twister](https://en.wikipedia.org/wiki/Mersenne_Twister).

## Random Numbers: Modulo Bias

Modulo bias happens when we try to get a smaller set of random numbers from a larger set of random numbers (which are uniformly distributed) by applying the modulo operator.

Suppose we have the following:

- A (pseudo)random number generator `rand(0, MAX)`: [0, 1, 2, ..., MAX-1, MAX], evenly distributed.
- We want to get random numbers in a smaller range `[0, N-1]` where `N-1 < MAX`.

One way is to use modulo operator to get the wanted random numbers: `rand(0, MAX) % N`.

Suppose `MAX = M * N + r` where `0 <= r < N`. Below is what actually happens:

| result of % | 0 | 1 | 2 | 3 | ... | r | ... | N - 1 |
|:-----------:|:-:|:-:|:-:|:-:|:---:|:-:|:---:|:-----:|
|             | 0 | 1 | 2 | 3 | ... | r | ... | N - 1 |
|             | N | N + 1 | N + 2 | N + 3 | ... | N + r | ... | 2N - 1 |
|             | 2N | 2N + 1 | 2N + 2 | 2N + 3 | ... | 2N + r | ... | 3N - 1 |
|             | 3N | 3N + 1 | 3N + 2 | 3N + 3 | ... | 3N + r | ... | 4N - 1 |
|             | ... | ... | ... | ... | ... | ... | ... | ... |
| (row M - 1) | (M-1)N | (M-1)N + 1 | (M-1)N + 2 | (M-1)N + 3 | ... | (M-1)N + r | ... | MN - 1 |
| (last row)  | MN | MN + 1 | MN + 2 | MN + 3 | ... | MN + r |

Because of the last row, the numbers of `0, 1, 2, 3, ..., r` have a slightly higher probability to appear in the result set than the numbers of `r + 1, r + 2, ..., N - 1`. This is caused by modulo bias.

We can make two conclusions from the table:

- When `MAX % N == N - 1` (in other words, the `last row` doesn't exist), it's still safe to use `%` to get the smaller set of random numbers.
- Or, if we can somehow remove the `last row`, we will get an evenly distributed set of random numbers.

To remove the `last row`, we need to get a modified version of `rand(0, MAX)`:

- 1). Calculate `r`: `r = MAX % N`
- 2). Get the new "MAX": `MAX' = MAX - (r + 1) = MAX - (MAX % N + 1)` (because the last row has `r + 1` numbers)
  - A caveat: When `MAX % N` is already `N - 1`, namely, the last row doesn't exist in the first place, then `MAX - (MAX % N + 1)` actually removes the entire `row M - 1`. This is still correct but unnecessary, so the modified version of calculating the new "MAX" is `MAX'' = MAX - (MAX % N + 1) % N`.

Then we can define the modified version of `rand` as follows:

```python
# `rand` returns a number in [0, MAX].
def rand_(MAX_, rand):
  x = rand()
  while x > MAX_:
    x = rand()
  return x
```

Because we only remove the last row (`r + 1` numbers) from `MAX` numbers, if `MAX` is significantly larger than `r + 1`, then `rand_` is very likely to only run one iteration in the `while` loop to get the desired result.

Finally, we calculate `n = x % N` to get the number `n` that falls between `[0, N - 1]`.

References:
- [1] [Why do people say there is modulo bias when using a random number generator?](https://stackoverflow.com/a/10984975/630364)
- [2] [Generating Random Numbers without Modulo Bias](https://funloop.org/post/2013-07-12-generating-random-numbers-without-modulo-bias.html)
- [3] [Generating Random Numbers without Modulo Bias, Redux](https://funloop.org/post/2015-02-27-removing-modulo-bias-redux.html)

## Birthday Problem

[Birthday problem](https://en.wikipedia.org/wiki/Birthday_problem):

> It may well seem surprising that a group of just 23 individuals is required to reach a probability of 50% that two individuals in the group have the same birthday.
>
> The goal is to compute P(A), the probability that at least two people in the room have the same birthday. However, it is simpler to calculate P(A'), the probability that no two people in the room have the same birthday. Then, because A and A' are the only two possibilities and are also mutually exclusive, P(A) = 1 − P(A').

## Fast Multiplication

There are many different multiplication methods, and the ["grade school" algorithm](https://en.wikipedia.org/wiki/Multiplication_algorithm#Long_multiplication) is just one of them. See the Wikipedia article [Multiplication algorithm](https://en.wikipedia.org/wiki/Multiplication_algorithm).

The [Karatsuba algorithm](https://en.wikipedia.org/wiki/Karatsuba_algorithm) is a fast and popular one published in 1962. The [Toom–Cook multiplication](https://en.wikipedia.org/wiki/Toom%E2%80%93Cook_multiplication) is a faster generalization of Karatsuba's method, and the [Schönhage–Strassen algorithm](https://en.wikipedia.org/wiki/Sch%C3%B6nhage%E2%80%93Strassen_algorithm) (1971) is even faster for sufficiently large n.

The article [Mathematicians Discover the Perfect Way to Multiply](https://www.quantamagazine.org/mathematicians-discover-the-perfect-way-to-multiply-20190411) introduces the potentially (see Note [1]) fastest multiplication algorithm that was discovered in March, 2019, in the paper [Integer multiplication in time O(n log n)](https://hal.archives-ouvertes.fr/hal-02070778/document).

Note [1]: "At the end of February, a team of computer scientists at Aarhus University [posted a paper](https://arxiv.org/abs/1902.10935) arguing that if another unproven conjecture is also true, this is indeed the fastest way multiplication can be done."
