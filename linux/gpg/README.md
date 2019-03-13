# README

## Why GPG Subkeys

The article [Subkeys](https://wiki.debian.org/Subkeys) explains the rationale and benefits of using GPG subkeys.

## HOWTOs

### How to list available keys

`gpg --list-keys`

### How to get the key ID (fingerprint) of a public key:

`gpg --with-fingerprint --with-colons ./public.gpg.key`

## How to get the key's expiration time

`gpg --list-keys --with-colons <key ID>`

This generates the output as follows:

```
tru::1:1551992548:0:3:1:5
pub:u:4096:1:E5C897F603EE4229:2019-03-07:::u:Yaobin Wen (Test) <yaobin.wen@test.com>::scESC:
sub:u:4096:1:86679E445FF1F4E3:2019-03-07::::::e:
sub:e:4096:1:7B0743494DD5684D:2019-03-07:2019-03-08:::::s:
```

If the key has an expiration date, it is shown after its creation date. In the case above, it is `2019-03-08`.

`gpg --list-keys --with-colons --fixed-list-mode <key ID>`

This generates the output as follows:

```
tru::1:1551992548:0:3:1:5
pub:u:4096:1:E5C897F603EE4229:1551983322:::u:::scESC:
uid:u::::1551983322::CE5334ABA5C64A7AB3674EF75AF04F08197E22CF::Yaobin Wen (Test) <yaobin.wen@test.com>:
sub:u:4096:1:86679E445FF1F4E3:1551983322::::::e:
sub:e:4096:1:7B0743494DD5684D:1551983550:1552069950:::::s:
```

The expiration time is in the format of [Unix Epoch time](https://en.wikipedia.org/wiki/Unix_time). It is `1552069950` in the case above. Use the [Epoch Converter](https://www.epochconverter.com/) tool to convert it to a human-readable date and time which has the hour, minute, and second.
