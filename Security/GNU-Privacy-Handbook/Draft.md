# Draft

## References

- [1] [GPG Tutorial](https://futureboy.us/pgp.html)
- [2] [gpg(1) manual](http://manpages.ubuntu.com/manpages/focal/man1/gpg.1.html)

## Readings

- [New PGP key: RSA/RSA or DSA/Elgamal?](https://security.stackexchange.com/a/72586/80050)
- [Best encryption and signing algorithm for GnuPG: RSA/RSA or DSA/Elgamal?](https://superuser.com/a/541162/224429)
- [Recommendation for Key Management: Part 1 – General](https://csrc.nist.gov/publications/detail/sp/800-57-part-1/rev-5/final)
- [Encryption and Digital Signatures using GPG](https://cran.r-project.org/web/packages/gpg/vignettes/intro.html)

## What to Learn

- Learn the strength of different crypto algorithms, because you will need to use a strong algorithm and also know what algorithm to suggest others to use if you find them using a weaker algorithm.
- How to decide what kind of key to create?

```
   (1) RSA and RSA (default)
   (2) DSA and Elgamal
   (3) DSA (sign only)
   (4) RSA (sign only)
  (14) Existing key from card
```

- How to decide the key size? 3072 or 4096?

## Best Practices

From [1]:

> Use a *good* password for your user account and a *good* passphrase to protect your secret key. This passphrase is the weakest part of the whole system. Programs to do dictionary attacks on your secret keyring are very easy to write and so you should protect your "~/.gnupg/" directory very well.

From [1]:

> Keep in mind that, if this program is used over a network (telnet), it is *very* easy to spy out your passphrase!

See [_Forwarding `gpg-agent` to a Remote System over SSH_](https://yaobinwen.github.io/2022/02/20/Forwarding-gpg-agent.html).

## 1. Generate the key pair.

**NOTE:**
- Both Alice and Brandon must create their own key pairs because signing requires private keys.
- Name must be at least 5 characters long.

`gpg --full-gen-key` gives finer control:

```
ywen@ywen-vm-focal:~/keys/alice$ gpg --full-gen-key
gpg: WARNING: unsafe permissions on homedir '/home/ywen/keys/alice/gpg-home'
gpg (GnuPG) 2.2.19; Copyright (C) 2019 Free Software Foundation, Inc.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Please select what kind of key you want:
   (1) RSA and RSA (default)
   (2) DSA and Elgamal
   (3) DSA (sign only)
   (4) RSA (sign only)
  (14) Existing key from card
Your selection? 1
RSA keys may be between 1024 and 4096 bits long.
What keysize do you want? (3072) 4096
Requested keysize is 4096 bits
Please specify how long the key should be valid.
         0 = key does not expire
      <n>  = key expires in n days
      <n>w = key expires in n weeks
      <n>m = key expires in n months
      <n>y = key expires in n years
Key is valid for? (0) 1y
Key expires at Sat 04 Mar 2023 11:27:32 PM EST
Is this correct? (y/N) y

GnuPG needs to construct a user ID to identify your key.

Real name: alice
Email address: alice@ywen.com
Comment:
You selected this USER-ID:
    "alice <alice@ywen.com>"

Change (N)ame, (C)omment, (E)mail or (O)kay/(Q)uit? e
Email address: alice@example.com
You selected this USER-ID:
    "alice <alice@example.com>"

Change (N)ame, (C)omment, (E)mail or (O)kay/(Q)uit? O
We need to generate a lot of random bytes. It is a good idea to perform
some other action (type on the keyboard, move the mouse, utilize the
disks) during the prime generation; this gives the random number
generator a better chance to gain enough entropy.
We need to generate a lot of random bytes. It is a good idea to perform
some other action (type on the keyboard, move the mouse, utilize the
disks) during the prime generation; this gives the random number
generator a better chance to gain enough entropy.
gpg: key 9CF0B1DD45414C71 marked as ultimately trusted
gpg: directory '/home/ywen/keys/alice/gpg-home/openpgp-revocs.d' created
gpg: revocation certificate stored as '/home/ywen/keys/alice/gpg-home/openpgp-revocs.d/51C6B2E842538F3C05BE25FA9CF0B1DD45414C71.rev'
public and secret key created and signed.

pub   rsa4096 2022-03-05 [SC] [expires: 2023-03-05]
      51C6B2E842538F3C05BE25FA9CF0B1DD45414C71
uid                      alice <alice@example.com>
sub   rsa4096 2022-03-05 [E] [expires: 2023-03-05]
```

## 2. Alice: Export public key

```
gpg: checking the trustdb
gpg: marginals needed: 3  completes needed: 1  trust model: pgp
gpg: depth: 0  valid:   1  signed:   1  trust: 0-, 0q, 0n, 0m, 0f, 1u
gpg: depth: 1  valid:   1  signed:   0  trust: 1-, 0q, 0n, 0m, 0f, 0u
gpg: next trustdb check due at 2023-03-05
```

## 3. Import the signed public key

When Alice receives her own public key that has been signed by Brandon, she can import it to her own keyring. But she must have Brandon's public key imported in order to show the complete information of the signer, otherwise "[User ID not found]".

```
/home/ywen/keys/alice/gpg-home/pubring.kbx
------------------------------------------
pub   rsa4096 2022-03-05 [SC] [expires: 2023-03-05]
      51C6B2E842538F3C05BE25FA9CF0B1DD45414C71
uid           [ultimate] alice <alice@example.com>
sig 3        9CF0B1DD45414C71 2022-03-05  alice <alice@example.com>
sig          8CC969F7F1A5D7B0 2022-03-05  [User ID not found]
sub   rsa4096 2022-03-05 [E] [expires: 2023-03-05]
sig          9CF0B1DD45414C71 2022-03-05  alice <alice@example.com>
```



[What is the meaning of GnuPG's --list-sigs output?](https://security.stackexchange.com/q/141501/80050)

[Quick'n easy gpg cheatsheet ](http://irtfweb.ifa.hawaii.edu/~lockhart/gpg/) which mentions the scenarios to export and import private keys.
