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

## How to export a key

`gpg --export <key ID or fingerprint>`  in binary mode.

`gpg --armor --export <key ID or fingerprint>` creates ASCII-armored output.

## How to list the details of a key

`gpg --export <key ID> | gpg --list-packets`. For example:

```
# off=0 ctb=99 tag=6 hlen=3 plen=397
:public key packet:
	version 4, algo 1, created 1566997176, expires 0
	pkey[0]: [3072 bits]
	pkey[1]: [17 bits]
	keyid: F0EA47D829353AC7
# off=400 ctb=b4 tag=13 hlen=2 plen=45
:user ID packet: "Yaobin Wen <yaobin.wen@minevisionsystems.com>"
# off=447 ctb=89 tag=2 hlen=3 plen=462
:signature packet: algo 1, keyid F0EA47D829353AC7
	version 4, created 1615289882, md5len 0, sigclass 0x13
	digest algo 10, begin of digest fb b6
	hashed subpkt 27 len 1 (key flags: 03)
	hashed subpkt 11 len 4 (pref-sym-algos: 9 8 7 2)
	hashed subpkt 21 len 5 (pref-hash-algos: 10 9 8 11 2)
	hashed subpkt 22 len 3 (pref-zip-algos: 2 3 1)
	hashed subpkt 30 len 1 (features: 01)
	hashed subpkt 23 len 1 (keyserver preferences: 80)
	hashed subpkt 33 len 21 (issuer fpr v4 20603564BF79EA203C1E6668F0EA47D829353AC7)
	hashed subpkt 2 len 4 (sig created 2021-03-09)
	subpkt 16 len 8 (issuer key ID F0EA47D829353AC7)
	data: [3070 bits]

...
...
```
