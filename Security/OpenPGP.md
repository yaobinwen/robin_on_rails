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

## Certificates and Certificate Authority  (CA)

Certificates are good for **organizations**. CA is **centralized**.

[**X.509**](https://tools.ietf.org/html/rfc5280) is a standard defining the format of public key certificates. X.509 certificates are used in many Internet protocols, including TLS/SSL, which is the basis for HTTPS.

## Web of Trust (WOT)

This section assumes the use of GnuPG (GPG) as the implementation of OpenPGP.

`Trust` means the **certainty of one's identity**, not the trustworthiness of the person.

WOT is based on **individual relationships** among the users. It is **distributed**. When using the certificates issued by the CA, you rely on the CA's effort to verify the other one's identity; when using WOT, you rely on another individual's effort to verify the other one's identity.

As [1] says,

> By signing another person's key, you are affirming that you have verified that person's identity.

Therefore, the more signatures you obtain, the more likely other people will believe that the key actually belongs to you.

Not every signer is the same. If your key is signed by a trustworthy person, other people will more likely trust the key; in contrast, if your key is signed by a well-known scum, other people will more likely choose to ignore this signature.

Therefore, the point is:

- Get more people to sign your key.
- Get more trustworthy people to sign your key.

Because the private key is always kept secret to its owner, when you sign somebody's key, it is the **public key** you are signing. Your signature is then attached to this **public key**.

GPG stores the public keys in the keyring on the local computer. Therefore, when you want to get somebody's signature, you follow these steps:

- Export your public key from the local keyring to a file.
- Send this file to the signer.
- The signer verifies the key and your identity to make sure you are the authentic owner of the key.
- The signer signs the public key so a new signature is attached to the key.
- Because the new signature exists in the copy of your public key that the signer has, the signer needs to send the public key with his/her signature back to you.
- Upon receipt, you import the public key along with the new signature into your keyring.
- Later when somebody asks for your public key, you can send the public key with this new signature to that person.

The inconvenience is: you must send the public key to everyone that asks for it; if later you need to revoke the key, you need to send everyone who already has your key a message saying your previous key is revoked.

Of course, you can publish your key on your own website so you don't have to send the key every time but the users of your key must come to get your key. When this becomes a more common practice, the idea of `key server` becomes feasible.

A key server is essentially no different than a personal website. It just helps people by letting them not have to remember every person's personal website. When you want to find somebody's public key, you go and search for it on the key server. When you sign somebody's key, you can simply push the key with your signature back to the server (instead of to its owner directly). When you need to revoke your key, you just publish the revocation certificate to the key server, and the key server will match the revocation certificate with your public key, and tag the key as "REVOKED". When you need to refresh the public keys of others, you don't have to send them messages one by one to ask if there is any update. You simply ask the server for the updates, and the server will send all the available updates back to you.

## References

- [1] [PGP & GPG: Email for the Practical Paranoid](https://www.amazon.com/PGP-GPG-Email-Practical-Paranoid/dp/1593270712)
- [2] [PGP Web of Trust: Core Concepts Behind Trusted Communication](https://www.linux.com/learn/pgp-web-trust-core-concepts-behind-trusted-communication)
- [3] [PGP Web of Trust: Delegated Trust and Keyservers](https://www.linuxfoundation.org/blog/2014/02/pgp-web-of-trust-delegated-trust-and-keyservers/)
- [4] [The GNU Privacy Handbook](https://www.gnupg.org/gph/en/manual/book1.html)
- [5] `PGP pathfinder`: https://pgp.cs.uu.nl/
- [6] [Wotsap: Web of trust statistics and pathfinder](https://www.lysator.liu.se/~jc/wotsap/)
