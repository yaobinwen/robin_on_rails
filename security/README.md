# Security

## Overview

This folder has notes about security.

## SSL/TLS Certificate

The article [_SSL and SSL Certificates Explained For Beginners_](http://www.steves-internet-guide.com/ssl-certificates-explained/) covers this topic clearly and concisely. The takeaway is:

- > **Secure Sockets Layer (SSL)** and **Transport Layer Security (TLS)** are protocols that provide secure communications over a computer network or link.
- > TLS is based on SSL and was developed as a replacement in response to known vulnerabilities in SSLv3.
- > A digital certificate provides a link between a public key and an entity (business,domain name etc) that has been verified (signed) by a trusted third party ( A certificate authority).
- > You get a digital certificate from a **recognized** Certificate Authority (CA).
- > Now when someone wants your public keys, you send them the certificate, they verify the signature on the certificate, and if it verifies, then they can trust your keys.

The article [_Survival guides - TLS/SSL and SSL (X.509) Certificates_](http://www.zytrax.com/tech/survival/ssl.html) explains the topic with greater details.