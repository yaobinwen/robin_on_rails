# T

## 1. Create the primary key.

```
$ sudo -u tss tpm2_createprimary --hierarchy p --format pem --output primary.pub --key-context primary.ctx
```

Consider the other options:
- `-P, --hierarchy-auth=AUTH`: The authorization value for the hierarchy specified with -C.
- `-p, --key-auth=AUTH`: The authorization value for the primary object created.
- `-a, --attributes=ATTRIBUTES`: The  object attributes, optional.  Defaults to: TPMA_OBJECT_RESTRICTED|TPMA_OBJECT_DECRYPT|TPMA_OBJECT_FIXEDTPM|  TPMA_OBJECT_FIXEDPARENT|TPMA_OBJECT_SENSITIVEDATAORIGIN|  TPMA_OBJECT_USER‐WITHAUTH.

TODO:
- a). Note that the command above does not create a restricted key, but a primary key is supposed to be restricted. See `-a 'restricted|decrypt|fixedtpm|fixedparent|sensitivedataorigin|userwithauth|\noda'` (See the example in `tpm2_createprimary(1)`)

## 2. Save the primary key as a persistent object

```
$ sudo -u tss tpm2_evictcontrol -C p -c primary.ctx 0x81800001
persistent-handle: 0x81800001
action: persisted
```

Notes [1]: ChatGPT listed the following available persistent handles:

> In TPM 2.0, there are several types of persistent handles that can be used to store objects in the TPM's non-volatile memory. These handles include:
>
>     0x81000000 through 0x817FFFFF: These are user-defined handles that can be used for any purpose. They can be created using the tpm2_create command with the -C option set to the handle of an existing persistent object.
> 
>     0x81800000 through 0x81FFFFFF: These are reserved handles that are used by the TPM for various purposes.
> 
>     0x82000000 through 0x827FFFFF: These are platform-defined handles that are used to store objects associated with the platform, such as the endorsement key and the platform hierarchy.
> 
>     0x82800000 through 0x82FFFFFF: These are vendor-defined handles that can be used by vendors to store objects associated with their specific products or applications.
> 
> Note that not all of these handles may be available or accessible depending on the particular TPM implementation and configuration.

However, after trying some of the handles, `0x82000000` ~ `0x827FFFFF` didn't seem available to me. I'm not sure if it's because I'm using a TPM simulator or because I'm using it incorrectly. But `0x81800000` ~ `0x81FFFFFF` seem to work for me. See the command above.

TODO:
- a). Where to find the available persistent handles in the TPM 2.0 spec?
- b). How do I know what persistent handles are available for my TPM simulator?

## 3. Create CSR

```
$ 
```

3). 

## References

- [1] tpm2-tools
  - [1.1] [tpm2_createprimary(1)](https://tpm2-tools.readthedocs.io/en/latest/man/tpm2_createprimary.1/)
- [2] [tpm2-openssl](https://github.com/tpm2-software/tpm2-openssl)
  - [2.1] [Identity Certificates](https://github.com/tpm2-software/tpm2-openssl/blob/master/docs/certificates.md)
