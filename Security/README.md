# README

## Overview

This folder has notes about security.

**Security is omnipresent**: every technology deserves an evaluation from two perspectives of security:

- 1). Whether the technolog can provide enough protection to the assets. In other words, does this technology have "holes" that intruders can make use of to gain unauthorized access?
- 2). Whether the technology can be used as a vehicle by the intruders to gain unauthorized access to the protected assets?
  - For example: [_UNICODE SECURITY CONSIDERATIONS_](http://unicode.org/reports/tr36/). Improperly used, the Unicode can be used to confuse the victims and make them think they are accessing a legitimate website but in fact not. 

## The First Principle

I believe the information security industry starts with a very simple idea which is **"authorized access is granted to protected data and unauthorized access is restricted"** [1]. This idea is the seed of all of the so many, simple or complex, security technologies. Unfortunately, most of the online tutorials or books fail to bridge the so simple essence and the so many complex technologies. As a result, the beginners reads the essential idea maybe once or twice, and then immediately plunged into the ocean of various security technologies and quickly get lost or have no clear mind why something should be done this way and not that way.

I wrote an article [_Thinking in Security_](https://yaobinwen.github.io/2021/02/14/Thinking-in-security.html), trying to show the essence of all the complex security technologies. I summarized the variety into two models which are essentially equivalent to the statement I mentioned above.

## The Bridge

What is the bridge between the simple essential idea and the various security technologies? It is the **"security requirements"**: the product/service buyers list their security requirements, and the providers implement the product/service using the appropriate security technologies to deliver the required security.

Simple, right? Not at all, because, typically, the buyers would say "I want the product/service to be **secure enough**" and their security requirements would list everything that is needed to achieve the "sufficient security" but: how to define whether something is secure **enough**?

Figuring out "how secure is enough" is hard, because **"being secure enough" is about not only the security but also the cost and the convenience**. It's like the triangle: "security, cost, and convenience - pick any two of them". Therefore, to define "enough", one must know:

- 1). The risks that may cause damage or loss to the business (i.e., a risk assessement). The damage or loss can be transformed into a monetary value and expressed in ranges. For example: "[0, 1 million)", "[1 million, 10 million)", "[10 million, unlimited)", etc..
- 2). The cost of building the security to avoid each range of damage/loss. You don't want to spend 1 million dollars to build something that gains only 1 thousand dollars.
- 3). The convenience loss of adding additional security measures. More security measures mean less convenience. If it's too convenient, the users may refuse to use the product or work around the measures.

We can also figure out how each security technology affects the three above-mentioned aspects. Then we will know what specific technologies should be used for a particular product or service in a particular use context.

There is no such a thing called "universally secure". The security must be measured in a particular context.

The analysis of the three aspects of the user's collective security requirements and those of each security technology is the bridge.

## How to Learn Security

Therefore, we need to learn security from three perspectives:

- 1). Learn what each security technology provides and sacrifices.
- 2). Learn how to evaulate the "risk", "cost", and "convenience" of each security technology.
- 3). Learn security testing: how to ensure the product correctly implements the security requirements.

## References

- [1] [Security Testing (A Complete Guide)](https://www.softwaretestinghelp.com/how-to-test-application-security-web-and-desktop-application-security-testing-techniques/)
