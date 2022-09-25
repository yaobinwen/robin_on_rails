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

## Garbage Collection

[Garbage Collection](https://en.wikipedia.org/wiki/Garbage_collection_%28computer_science%29) is a form of automatic memory management that can be implemented in various strategies:
- [Tracing](https://en.wikipedia.org/wiki/Tracing_garbage_collection)
- [Reference counting](https://en.wikipedia.org/wiki/Reference_counting)
- [Escape analysis](https://en.wikipedia.org/wiki/Escape_analysis)

## Notes: What Every Computer Scientist Should Know About Floating Point Arithmetic

Reference:
- [1] [What Every Computer Scientist Should Know About Floating Point Arithmetic](https://cr.yp.to/2005-590/goldberg.pdf)
- [2] [What Every Computer Scientist Should Know About Floating Point Arithmetic](https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html)
- [3] [What Every Computer Scientist Should Know About Floating Point Arithmetic](https://www.itu.dk/~sestoft/bachelor/IEEE754_article.pdf)

### Page 156

The expression `UPPER_INT(log2(e_max - e_min + 1)) + UPPER_INT(log2(beta^p)) + 1` should be calculating the maximum required number of bits to represent the numbers which have the base _beta_ and the precision _p_.

### Page 157

> ... Thus when _beta_ = 2, the number 0.1 lies strictly between two floating-point numbers and is exactly representable by neither of them.

1). The choices of the above-mentioned "two floating-point numbers" depend on the chosen precision _p_.
2). In Page 156, the paragraph right under the section title "Floating-point Formats", the example uses _beta_ = 2 and _p_ = 24 as the example to approximately represent `dec(0.1)` as `bin(1.10011001100110011001101 × 2^(-4))`.
3). However:
  - `bin(1.10011001100110011001101 × 2^(-4))` is exactly `dec(0.10000000149011611938)`,
  - `bin(1.10011001100110011001100 × 2^(-4))` is exaclty `dec(0.09999999403953552246)`.

Therefore, when _p_ = 24, `dec(0.1)` lies between `dec(0.09999999403953552246)` and `dec(0.10000000149011611938)`.

> ... A natural way to represent 0 is with `1.0 × beta^(e_min - 1)`, since this preserves the fact that the numerical ordering of nonnegative real numbers corresponds to the lexicographic ordering of their floating-point representations. (Footnote: This assumes the usual arrangement where the exponent is stored to the left of the significand.)...

1). The "floating-point representations" mean the binary representation of the real numbers.
2). The "lexicographic ordering" is the dictionary order.

### Page 158: Relative Error and Ulps

"Ulp" is "units in the last place" in which the "last place" is the last place of the **approximate number**. The paper uses the approximate number `3.14 × 10^(-2)` and the real number `0.0314159` as the example. Here:

- 1). When calculating the "ulp", we need to either use the `x beta^e` notation or the normal notation for both numbers.
- 2). If we use the `× beta^e` notation, the error is `|3.14 × 10^(-2) - 3.14159 × 10^(-2)| = 0.00159 × 10^(-2) = 0.159 × 10^(-4)`. Here, the last place in the approximate number is `0.04 × 10^(-2)`, so its unit is `0.01 × 10^(-2) = 1.0 × 10^(-4)`. Compare `1.0 × 10^(-4)` and the error `0.159 × 10^(-4)`, we can see the error is `0.159 * (1.0 × 10^(-4))`. This is why the paper says "... is in error by .159 units in the last place."

### Page 159

> ... Since numbers of the form _d.dd...dd × beta^e_ all have the same absolute error, but have values that range between _beta^e_ and _beta × beta^e_, the relative error ranges between _((beta / 2)*beta^(-p)) × beta^e / beta^e_ and _((beta / 2)*beta^(-p)) × beta^e / beta^(e+1)_.

- 1). `beta^e` and `beta^(e+1)` are used as the denominators when calculating the relative error, so, according to the definition of the relative error, we know that `beta^e` and `beta^(e+1)` are the real values of the lower-bound and upper-bound numbers.
- 2). According to the context, the form _d.dd...dd × beta^e_ should be a normalized representation, so its leading _d_ must be non-zero. Therefore, the leading _d_ could be any number of 1, 2, ..., beta-1, while the following _d_ could be any number of 0, 1, 2, ..., beta-1. Therefore, assuming _beta_ is 10, its lowest value is _1.00..00 × 10^e_ and the highest value is _9.99..99 × 10^e_.

However, the equation `(2)` indicates an inclusiveness on both ends of `(1/2)ulp`. It's easier to understand why it's inclusive on the left-hand side because of _1.00..00 × 10^e_ in the example in 2) above, but _9.99..99 × 10^e_ is not equal to _10 × 10^e_. **Why is it inclusive on the right-hand side, too?**

> ... we can say that when a real number is rounded to the closest floating-point number, the relative error is always bounded by `e`, which is referred to as _machine epsilon_.

Note that **"rounded to the closest floating-point number"** is an important precondition.

> In general, when the base is _beta_, a fixed relative error expressed in ulps can wobble by a factor of up to _beta_. And conversely, as equation (2) above shows, a fixed error of .5 ulps results in a relative error that can wobble by _beta_.

Using the example in this paragraph in the paper, we can see that although the relative errors can be the same (aka "fixed"), when expressed in ulps, the ulps can wobble by a factor of up to _beta_ (0.5ulps vs 4.0ulps in the example).

### Page 160 (1)

> The correct answer is .17, so the computed difference is off by 30 ulps and is wrong in every digit!

**Question:** But according to my calculation, the error is just 0.3 ulp. Who is wrong?

In the given example, the computed difference is `0.02 × 10^1` while the correct answer is `0.17`. So the error is:

`error = 0.02 × 10^1 - 0.017 × 10^1 = 0.003 × 10^1` (or, `error = 0.2 - 0.17 = 0.03`)

Since the "last place" in the `ulp` is about the computed value, not the true value, so we need to look at the last place of the value `0.02 × 10^1`, namely, `0.2`. The unit of the last place is `0.1` (ulp), so we need to solve the `x` that makes `0.03 = x * ulp = x * 0.1`. The result is `x = 0.3`. Not 30.

### Page 160 (2): The Proof of Theorem 1

We can use a concrete example to see the proof more clearly. Suppose `beta = 16`, `p = 6`. I deliberately avoid `beta = 10` because it has been too commonly used.

Also note that, in the proof, the `x` and `y` notations represent exact values, not the approximate floating-point values. The author's words may (or may not) be a little bit misleading because he says "Here y has p digits..." but so far in the paper the words "p digits" are always associated with the floating-point format. Nonetheless, you won't be misled if you remember that in the examples before this proof, the author has used `x` and `y` to denote the exact values and `x~` and `y~` to denote the approximate values.

With `beta = 16` and `p = 6`, we have `rho = F` (`hex(F)` is `dec(15)` in hexadecimal notation). Therefore:

- `x = 1` (The paper says `1.00...0` but in fact that is equivalent to `1`.)
- `y = 0.FFFFFF` as the paper says "y has _p_ digits (all equal to _rho_)".
- The exact difference `diff = x - y = 1.000000 - 0.FFFFFF = 0.000001 = 1 × 16^(-6) = 1 × beta^(-p) = beta^(-p)`.

When computing the difference using only _p_ digits, we get:

- `x~ = 1.00000 × 16^0` (totally 6 digits)
- `y~ = 0.FFFFF × 16^0` (totally 6 digits including the leading zero, and the right-most F is shifted off)
- The approximate difference: `diff~ = x~ - y~ = 0.00001 × 16^0 = 1 × 16^(-5) = 1 × 16^(-6+1) = 1 × beta^(-p+1) = beta^(-p+1)`.

The error between the two differences is:

`diff~ - diff = beta^(-p+1) - beta^(-p) = beta^(-p) * beta - beta^(-p) = beta^(-p) * (beta - 1)`.

Then the relative error (recall that it is the error divided by the exact value) is:

`rel_err = (diff~ - diff) / diff = beta^(-p) * (beta - 1) / beta^(-p) = beta - 1`.

### Page 161 (1)

> When `beta=2`, the relative error can be as large as the result, ...

When `beta` is 2, it is the binary system. When `p = 1`, namely, we use one digit for calculation, then `y = 0.1` would be converted to `y~ = 0`. So the above example becomes:

- `x = 1.0`
- `y = 0.1`
- The exact difference: `diff = x - y = 1.0 - 0.1 = 0.1 = 1 × 2^(-1)`.
- `x~ = 1`
- `y~ = 0`
- The approximate difference: `diff~ = x~ - y~ = 1 - 0 = 1 = 1 × 2^0`.
- The error between the differences: `error = diff~ - diff = 1 × 2^0 - 1 × 2^(-1) = 10 × 2^(-1) - 1 × 2^(-1) = 1 × 2^(-1)`.
- The relative error is: `rel_err = 1 × 2^(-1) / (1 x 2^(-1)) = 1 = 2 - 1 = beta - 1`.

Here the "result" in "as large as the result" refers to the approximate difference `1 × 2^(0) = 1`. Because the relative error `beta - 1` is also `1` (because `beta` is 2), the author thus says "can be as large as the result".

> ... and when `beta=10`, it can be 9 times larger.

If you repeat the calculation above using `beta = 10` and `p = 1`, you will get `diff~ = 1 × 2^0 = 1` and `rel_erro = beta - 1 = 9`. Therefore, it can be 9 times larger than the result.

> Or to put it another way, when `beta=2`, equation (3) shows that the number of contaminated digits is `log2(1/epsilon) = log2(2^p) = p`. That is, all of the p digits in the result are wrong!

Go back to Page 159. The sentence right above equation (3) says "If the relative error in a computation is `n*epsilon`". When `beta=2`, we had shown above that its maximal relative error can be `beta - 1 = 2 - 1 = 1`. Therefore, in this case, `n*epsilon = 1`, so we have `n = 1 / epsilon`. So we have the equation (3) `log2(n) = log2(1/epsilon)`.

Go back to Page 159 to find the definition of `epsilon` right below equation (2): `Setting epsilon = (beta / 2) * beta^(-p)`. Then we know `1 / epsilon = 2 * beta^(p-1)`. Because `beta` happens to be `2`, we further have `2 * beta^(p-1) = 2 * 2^(p-1) = 2^p`.

Taken all of them up, we have `log2(n) = log2(1/epsilon) = log2(2^p) = p`. In other words: we have _p_ contaminated digits in the result, or "all of the p digits in the result are wrong".
