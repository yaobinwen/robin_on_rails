# OpenPGP

## Glossary

| Name | Description | Notes |
|-----:|:------------|:------|
| OpenPGP | An encryption standard. Not software. | https://www.openpgp.org/ |
| PGP | An implementation of `OpenPGP`. ||
| GPG | Gnu Privacy Guard. An implementation of `OpenPGP`. | https://www.gnupg.org/ |
| Plaintext | Text before encryption. ||
| Ciphertext | Encrypted text. ||
| Code | "a general term for any method of concealing the contents of a message" ||
| Cipher | A specific type of "code" that transforms each character in some way. ||
| Hash | "a specialized mathematical computation performed on a message" ||
| Cryptanalysis | Another name for "attack" ||
| Keyring | A "database" that stores all the (private/public) keys, like a real keyring. ||

## Key Length Does Not Determine Security

The longer key (such as a 128-bit key) is harder to be guessed than a shorter key (such as a 40-bit key).

Most cryptanalysis experts **attack the algorithm** instead of the key because the algorithm may have weaknesses that enable people to decrypt the ciphertext without using the key or accidentally reveal part of the key.

Therefore, choosing a stronger algorithm is more important than choosing a longer key.

## Web of Trust (WOT)

`Trust` means the certainty of one's identity, not the trustworthiness of the person.

WOT is implemented in two forms:

| Name | Suited for | Structure| Notes |
|-----:|:-----------|:--------:|:------|
| Certificates | Organizations | Centralized | Described in the X.509 standard (see below). |
| Trust relationnships among users | Individuals | Distributed | Described in OpenPGP. |

X.509:
- https://en.wikipedia.org/wiki/X.509
- https://tools.ietf.org/html/rfc5280

X.509 is a standard defining the format of public key certificates. X.509 certificates are used in many Internet protocols, including TLS/SSL, which is the basis for HTTPS.

[PGP Web of Trust: Core Concepts Behind Trusted Communication](https://www.linux.com/learn/pgp-web-trust-core-concepts-behind-trusted-communication)

[PGP Web of Trust: Delegated Trust and Keyservers](https://www.linuxfoundation.org/blog/2014/02/pgp-web-of-trust-delegated-trust-and-keyservers/)

**QUESTION**: How does revocation certificate works?

[The GNU Privacy Handbook](https://www.gnupg.org/gph/en/manual/book1.html)
