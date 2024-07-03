# WiFi

## WiFi signal strength

See [WiFi Signal Strength: A No-Nonsense Guide](https://techgrid.com/blog/wifi-signal-strength):

> To transmit a signal, your access point uses either the 2.4 or the 5 GHz band. And within these bands are smaller bands called WiFi channels.
>
> To help your visualize—imagine that the different bands are like highways and the channels are different lanes within that highway.
>
> ![5Ghz band "highway"](https://techgrid.com/hubfs/WiFi-Bands-vs-Channels-2.png)

Three ways to measure signal strength:

- mW (milliwatts):
  - the most accurate measurement
  - but not convenient because it ends up being a bunch of decimal places (e.g., `0.00000001mW`)
- dBm (decibel-milliwatts)
  - the most commonly used
- RSSI (received signal strength indicator)
  - Doesn't seem to have a widely accepted standard, so kind of vendor-specific.

## WEP

Note that WEP (including OSA and SKA, see below) is considered obsolete entirely and should not be used in setting up new WiFi networks.

In `hostapd.conf`, WEP can be configured to use one or two authentication algorithms that are defined in IEEE 802.11:
- Open System Authentication (OSA): any device can communicate with the access point.
- Shared Key Authentication (SKA): SKA devices must use a WEP key for authentication.

```ini
# IEEE 802.11 specifies two authentication algorithms. hostapd can be
# configured to allow both of these or only one. Open system authentication
# should be used with IEEE 802.1X.
# Bit fields of allowed authentication algorithms:
# bit 0 = Open System Authentication
# bit 1 = Shared Key Authentication (requires WEP)
auth_algs=3
```

## WPA-PSK vs WPA2-PSK

Ref: [What is WPA2-PSK?](https://www.ninjaone.com/it-hub/endpoint-security/what-is-wpa2-psk/)

- WPA-PSK is the predecessor to WPA2-PSK.
- WPA-PSK uses Temporal Key Integrity Protocol (TKIP) for encryption, which has been found to have several vulnerabilities.
- WPA2-PSK uses Advanced Encryption Standard (AES), which is a more robust and secure encryption method.
- WPA2-PSK vulnerabilities:
  - "KRACK" Attack: The [Key Reinstallation AttaCK (KRACK)](https://www.krackattacks.com/) is a severe vulnerability in the WPA2 protocol that allows attackers within range of a victim to exploit these weaknesses using key reinstallation attacks. This can potentially compromise encrypted information.
  - Brute Force Attack: If a weak password is used for the network, attackers can use brute force methods to guess the pre-shared key.
  - Outdated Hardware: Older network devices may not properly support WPA2-PSK, making them more susceptible to attacks.
  - Insider Threats: If the pre-shared key is known by unauthorized individuals within the network’s range, they could gain unauthorized access to the network.
