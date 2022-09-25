# GPG

Programs:
- `gpg`
- `gpgsm`
- `gpgconf`
- `gpg-connect-agent`

Environment variables:
- `GPG_AGENT_INFO`

Directories & Files:
- `~/.gnupg`

[How does GPG agent work?](https://unix.stackexchange.com/q/188668/162971):

> (`gpg-agent`) performs actions using the key, on behalf of the client process. (The client provides the agent with something to sign, encrypt, or decrypt, and the agent does so.) When using an agent, your SSH and GPG client software never gains access to the actual key.
>
>  gpg-agent works fine with SSH agent forwarding. I use it every day. The SSH client handles the forwarding, gpg-agent is not really involved in that. What is not supported is for GPG itself to talk to an agent remotely.

[Using gpg-agent Effectively](https://eklitzke.org/using-gpg-agent-effectively)
