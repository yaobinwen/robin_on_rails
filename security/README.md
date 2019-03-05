# Security

## Overview

This folder has notes about security. See [1] for glossary.

[13] provides a series of cheat sheets about security.

## Cookies

See [11].

## SSL/TLS Certificate

The article [2] covers this topic clearly and concisely. The takeaway is:

- > **Secure Sockets Layer (SSL)** and **Transport Layer Security (TLS)** are protocols that provide secure communications over a computer network or link.
- > TLS is based on SSL and was developed as a replacement in response to known vulnerabilities in SSLv3.
- > A digital certificate provides a link between a public key and an entity (business,domain name etc) that has been verified (signed) by a trusted third party ( A certificate authority).
- > You get a digital certificate from a **recognized** Certificate Authority (CA).
- > Now when someone wants your public keys, you send them the certificate, they verify the signature on the certificate, and if it verifies, then they can trust your keys.

The article [3] explains the topic with greater details.

## Federated Identity

The Wikipedia article [4] explains this topic well.

> A federated identity in information technology is the means of linking a person's electronic identity and attributes, stored across multiple distinct identity management systems.

The article explains why federated identity is needed in the first place:

> Centralized identity management solutions were created to help deal with user and data security where the user and the systems they accessed were within the same network – or at least the same "domain of control". Increasingly however, users are accessing external systems which are fundamentally outside their domain of control, and external users are accessing internal systems. The increasingly common separation of user from the systems requiring access is an inevitable by-product of the decentralization brought about by the integration of the Internet into every aspect of both personal and business life. Evolving identity management challenges, and especially the challenges associated with cross-company, cross-domain access, have given rise to a new approach to identity management, known now as "federated identity management".

The referenced paper [5] also explains the problem to be solved by federated identity. See the _Introduction_ section of this white paper.

> Federation is enabled through the use of open industry standards and/or openly published specifications, such that multiple parties can achieve interoperability for common use-cases. Typical use-cases involve things such as cross-domain, web-based single sign-on, cross-domain user account provisioning, cross-domain entitlement management and cross-domain user attribute exchange.

The [Liberty Alliance project](http://www.projectliberty.org/liberty/about/) is such an endeavor, as it is "to enable a networked world based on open standards where consumers, citizens, businesses and governments can more easily conduct online transactions while protecting the privacy and security of identity information."

## Single Sign-on (SSO)

See the Wikipedia article [6].

> Single sign-on (SSO) is a property of access control of multiple related, yet independent, software systems. With this property, a user logs in with a single ID and password to gain access to any of several related systems. ... it is best to refer to systems requiring authentication for each application but using the same credentials from a directory server as Directory Server Authentication and systems where a single authentication provides access to multiple applications by passing the authentication token seamlessly to configured applications as single sign-on.
>
> As different applications and resources support different authentication mechanisms, single sign-on must internally store the credentials used for initial authentication and translate them to the credentials required for the different mechanisms.

## Token Authentication

Refer to [9].

The process by which an application confirms user identity is called authentication. Also see [1], Page 25.

> In this method, tokens are generated for your users after they present verifiable credentials. The initial authentication could be by  username/password credentials, API keys or even tokens from another service.

![Authentication](https://stormpath.com/wp-content/uploads/2016/05/Cookie-v-Token-Diagram-v1-3-1024x536.png)

The article provides a good example:

> Let’s try an analogy: When you attend a conference or convention, you’re issued credentials specific to that event, often in the form of a lanyard. How did you score that lanyard? Well, you presented a valid, government-issued ID on the first day, they compared your ID, your face, and your registration. All matched, and the lanyard was yours. You’ve been authenticated. When you return to that convention the next day (a new session), or try to attend a gated sub-event (an authorization request), it’s your event credentials that are checked, not your actual ID.
>
> In a session ID world, each gatekeeper would need a list of every single attendee and their registrations (permissions!), and would check your valid, government-issued ID against that list not just each day, but every time you entered a new area or session. That sounds exhausting, right?
>
> In token land, gatekeepers authenticate your identity and authorize your access based on the custom event credential you’re wearing around your neck. No additional ID needed, no enormous list to check every individual against. The lanyard carries all the information!

The article [10] explains what a token consists of. A token is a [Base64](https://en.wikipedia.org/wiki/Base64) encoded string. It looks like this (the example is from [12] [3.1.  Example JWT](https://tools.ietf.org/html/rfc7519#section-3.1))

```text
eyJ0eXAiOiJKV1QiLA0KICJhbGciOiJIUzI1NiJ9
.
eyJpc3MiOiJqb2UiLA0KICJleHAiOjEzMDA4MTkzODAsDQogImh0dHA6Ly9leGFt
cGxlLmNvbS9pc19yb290Ijp0cnVlfQ
.
dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXk
```

and can be decoded into a structured representation:

```json
{
  "iss":"joe",
  "exp":1300819380,
  "http://example.com/is_root":true
}
```

The token also contains the signature that the user signs with his/her private key. Therefore, when the server receives this token, decodes it into the structure representation, and sees the claimed user's identity, it can further uses the signature and the user's public key to verify whether this token is actually sent by the authentic user.

[JSON Web Token (JWT)](https://en.wikipedia.org/wiki/JSON_Web_Token) is a JSON-based open standard (See [12]) for creating access tokens that assert some number of claims.

## OAuth 2.0

See [7]:

- "OAuth is an open standard for authorization and anyone can implement it."
- "OAuth is **not** an API or a service."
- OAuth 1.0a and OAuth 2.0 are **completely different** with **no** backwards compatibility.
- **QUESTION**: "Basic Authentication is still used as a primitive form of API authentication for server-side applications: instead of sending a username and password to the server with each request, the user sends an API key ID and secret."
  - What are the "API key ID" and "secret"?
- The _password anti-pattern_ (See [8]):
  - Suppose the user is using some web service (such as Facebook), on which his/her private information is protected by the username and password.
  - Before OAuth was invented, if a third-party (desktop/web) application wants to access the web service through its public APIs, the third-party application must ask the user to provide the username and password. This creates several problems:
    - The user can't easily revoke the access permissions from each third-party application. Even if the third-party application says it has deleted the stored username and password, the user doesn't know if they are actually deleted or not.
    - With the username and password, the third-party application gains the full access of the web service. There is no access scope control.
  - "What users need is a granular authorization system that will allow them to selectively grant revokable privileges to individual applications without having to supply a global password."
  - Before OAuth was invented, there were already such vendor-specific implementations. However, this variety required the third-party application developers implement support for all such implementations. The developers needed a widely-accepted standard, hence OAuth.
- "To create a better system for the web, federated identity was created for single sign-on (SSO)."
- "OAuth is a delegated authorization framework for REST/APIs. It enables apps to obtain limited access (scopes) to a user’s data without giving away a user’s password."
- **"You can think of this like hotel key cards, but for apps. If you have a hotel key card, you can get access to your room. How do you get a hotel key card? You have to do an authentication process at the front desk to get it. After authenticating and obtaining the key card, you can access resources across the hotel."**

## Tools

- [Keycloak](https://www.keycloak.org/index.html): "Keycloak is an open source Identity and Access Management solution aimed at modern applications and services. It makes it easy to secure applications and services with little to no code."

## References

- [1] [Internet Security Glossary, Version 2](https://tools.ietf.org/html/rfc4949)
- [2] [_SSL and SSL Certificates Explained For Beginners_](http://www.steves-internet-guide.com/ssl-certificates-explained/)
- [3] [_Survival guides - TLS/SSL and SSL (X.509) Certificates_](http://www.zytrax.com/tech/survival/ssl.html)
- [4] [Wikipedia entry: Federated Identity](https://en.wikipedia.org/wiki/Federated_identity)
- [5] [LIBERTY ALLIANCE PROJECT WHITE PAPER: Liberty ID-WSF People Service – federated social identity](http://www.projectliberty.org/liberty/content/download/387/2720/file/Liberty_Federated_Social_Identity.pdf)
- [6] [Wikipedia entry: Single sign-on](https://en.wikipedia.org/wiki/Single_sign-on)
- [7] [What the Heck is OAuth?](https://developer.okta.com/blog/2017/06/21/what-the-heck-is-oauth)
- [8] [OAuth and OAuth WRAP: defeating the password anti-pattern](https://arstechnica.com/information-technology/2010/01/oauth-and-oauth-wrap-defeating-the-password-anti-pattern/)
- [9] [Token Authentication: The Secret to Scalable User Management](https://stormpath.com/blog/token-authentication-scalable-user-mgmt)
- [10] [Token Based Authentication for Single Page Apps (SPAs)](https://stormpath.com/blog/token-auth-spa)
- [11] [What Are Cookies? Computer Cookies Explained](http://www.whatarecookies.com/)
- [12] [RFC 7519: JSON Web Token (JWT)](https://tools.ietf.org/html/rfc7519)
- [13] [OWASP Cheat Sheet Series](https://www.owasp.org/index.php/OWASP_Cheat_Sheet_Series)
- [13] [What is OAuth? Definition and How it Works](https://www.varonis.com/blog/what-is-oauth/)
- [14] [OAuth for Dummies](https://marktrapp.com/blog/2009/09/17/oauth-dummies/)
- [15] [OAuth 2.0 Playground](https://developers.google.com/oauthplayground/)
