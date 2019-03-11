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

## Birthday Problem

[Birthday problem](https://en.wikipedia.org/wiki/Birthday_problem):

> It may well seem surprising that a group of just 23 individuals is required to reach a probability of 50% that two individuals in the group have the same birthday.
>
> The goal is to compute P(A), the probability that at least two people in the room have the same birthday. However, it is simpler to calculate P(A'), the probability that no two people in the room have the same birthday. Then, because A and A' are the only two possibilities and are also mutually exclusive, P(A) = 1 − P(A').

