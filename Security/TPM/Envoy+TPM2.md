# Envoy Proxy + TPM2

- Envoy Proxy HSM support, timeline:
  - 2017-06-13: Created the issue [HSM support #1106](https://github.com/envoyproxy/envoy/issues/1106) to ask about HSM.
  - 2018-01-13: Announced that someone would take up the task and start a design.
  - 2019-03-11: Created the issue [Support for BoringSSL asynchronous private key operations #6248](https://github.com/envoyproxy/envoy/issues/6248).
  - 2019-03-20: Created the PR [tls: support BoringSSL private key async functionality #6326](https://github.com/envoyproxy/envoy/pull/6326).
    - "Two example private key method providers are included in the tests."
  - 2019-06-06: The first commit of [aruba/envoy-extn-pkm-provider](https://github.com/aruba/envoy-extn-pkm-provider).
  - 2019-06-12: Suggested that HSM support "can probably be done nicely with the BoringSSL Private Key Methods once #6326 lands."
    - "After some more investigation it seems that we might as well do a generic PKCS#11 private key method provider. This would give us the most flexibility regarding the actual backend (just load the SO/DLL implementing the PKCS#11 API) and also be pretty cross-platform. We can also use something like SoftHSM for testing."
  - 2019-08-23: [aruba/envoy-extn-pkm-provider](https://github.com/aruba/envoy-extn-pkm-provider) was finished.
  - 2019-10-07: Mentioned [aruba/envoy-extn-pkm-provider](https://github.com/aruba/envoy-extn-pkm-provider).
