# Robin on Rails: Track My Study of Various Software/Hardware Technologies

(Last updated on 2022-11-20)

## 1. Overview

Is it possible to keep track of the latest development of every technology in the software development industry?

No. Impossible nowadays. A lot of people will say: **Don't try!**

Being a programmer requires continuous learning of various technologies. However, as the article [_How to stay sane in today's world of tech_](https://unixsheikh.com/articles/how-to-stay-sane-in-todays-world-of-tech.html) says:

> If you try to master a single skill in the tech industry today, in order to become an expert, you will loose the ability to know a lot of different skills and technologies and hence loose job or consulting opportunities. On the other hand, if you try to know and become good at many different things, you will never become really good (expert level) at a single thing.
>
> That's just a hard fact of how much the technology has changed and progressed.

Nonetheless, I need to learn a lot of different things in order to do my work well. Hence this repository. This repository is not a new one. It was initially created in 2018 to keep my notes of learning. Since then, I have made notes on various subjects. These notes are helpful whenever I want to quickly refresh my memory on a particular subject.

But when I work on this repository, the question _How to stay sane in today's world of tech_ keeps echoing in my mind. In the world of new things popping up and existing things changing all the time, I really need to focus on the ones that benefit me the most in the context that I am living, short term and long term.**Learning the fundamentals**, i.e., the things that change less frequently, is probably the most economical way. I just need to strike a balance between learning the things I need right now and learning the things that can solidify my foundation and benefit me in the far future. Meanwhile, I'm also asking myself these questions:

1. How to make the best out of learning for the short term?
2. What else in addition to learning the fundamentals?

## 2. How to Use This Repository

As of 2022-11-20, this repository has a lot of sub-directories and [issues](https://github.com/yaobinwen/robin_on_rails/issues652a6db61eaf4f5d9131da104673d24fac9f9299). My plan is:

- I will list the subjects I want to focus on in this `README` file. But this `README` file does not track the progress.
- I will need to keep updating this `README` file when I have a new subject interested in or I don't want to track an existing subject anymore.
- I will gradually reorganize the issues so they can be used to track the progress of my learning.
- I will gradually reorganize the sub-directories so subjects I care about will have a sub-directory for the notes, and the unimportant subjects will be put in a separate place, probably in the category of "miscellaneous". I don't want to delete them because sometimes I may still want to make notes of random interesting things.

## 3. How to Study

In general, study in whatever time that's available in whatever forms (e.g., text, podcast, video).

When I get a few days of free time, like when I'm on vacation, I may arrange a series of study sessions that I call "tech dashes" to learn continuously. See the tag [`tech-dash`](https://github.com/yaobinwen/robin_on_rails/issues?q=is%3Aissue+is%3Aopen+label%3Atech-dash) in "Issues". The [`Tech_Dashes.md`](./Tech_Dashes.md) briefly records what I do for each tech dash.

The output of study is in two places:

1. Some organized or unorganized notes in this repository.
2. An article in my [tech blog](https://yaobinwen.github.io/) that discusses a particular topic.

## 4. Subjects of Interest

### 4.1 Hardware

- How are atomic operations implemented on the hardware level?
  - I took some notes [here](./Hardware/Atomic.md) but need to study further.

### 4.2 Operating Systems

- [Computer Systems: A Programmers Perspective 2nd Edition](http://csapp.cs.cmu.edu/2e/home.html)
- [Linux ELF format](https://man7.org/linux/man-pages/man5/elf.5.html)
- [Understanding the Linux Kernel (3e)](https://www.amazon.com/Understanding-Linux-Kernel-Third-Daniel/dp/0596005652): [tracker](https://github.com/yaobinwen/robin_on_rails/issues/68)

### 4.3 Mathematics & Computer Science

- Calculus
- Linear Algebra
- [B-Tree](https://en.wikipedia.org/wiki/B-tree)
  - [10.2 B Trees and B+ Trees. How they are useful in Databases](https://youtu.be/aZjYr87r1b8)
  - [How Database B-Tree Indexing Works](https://dzone.com/articles/database-btree-indexing-in-sqlite)
- Floating point numbers: math and tricks (in [this series of articles](https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/))

### 4.4 Security

- CVEs
  - [How The Tables Have Turned: An analysis of two new Linux vulnerabilities in nf_tables](https://blog.dbouman.nl/2022/04/02/How-The-Tables-Have-Turned-CVE-2022-1015-1016/) (about CVE-2022-1015 and CVE-2022-1016)
  - [The Discovery and Exploitation of CVE-2022-25636](https://nickgregory.me/linux/security/2022/03/12/cve-2022-25636/)
- OAuth 2.0
- [OpenVPN](https://openvpn.net/)
- Transport Layer Security (TLS)
- [WireGuard](https://www.wireguard.com/)
- [Securing Debian Manual 3.19](https://www.debian.org/doc/manuals/securing-debian-manual/index.en.html)
- [Strong Distribution HOWTO](https://www.cryptnet.net/fdp/crypto/strong_distro.html): "This document describes the protocol and methods for the cryptographically strong distribution of free software on the GNU/Linux platform."

### 4.5 Databases

- [PostgreSQL](https://www.postgresql.org/)
  - Read: [PostgreSQL 13 official documentation](https://www.postgresql.org/docs/13/index.html): [tracker](https://github.com/yaobinwen/robin_on_rails/issues/131)
- [SQLite](https://www.sqlite.org/index.html):
  - [Atomic Commit In SQLite](https://www.sqlite.org/atomiccommit.html)
  - [Write-Ahead Logging](https://www.sqlite.org/wal.html)

### 4.6 Others

- Ansible
- [Debian Policy Manual](https://www.debian.org/doc/debian-policy/)
- [dpkg](https://github.com/yaobinwen/dpkg)
- Docker
  - Study the answer to my question [_In my Docker container, why can I still bind the port 1 without `NET_BIND_SERVICE` capability?_](https://stackoverflow.com/q/65853364/630364).
- Envoy
- Go (programming language): [tracker](https://github.com/yaobinwen/robin_on_rails/issues/133)
- JavaScript
  - [The Modern JavaScript Tutorial](https://javascript.info/): [tracker](https://github.com/yaobinwen/robin_on_rails/issues/9)
  - [Series of "How JavaScript Works"](https://blog.sessionstack.com/how-does-javascript-actually-work-part-1-b0bacc073cf): [tracker](https://github.com/yaobinwen/robin_on_rails/issues/9#issuecomment-401534229)
- [Material UI](https://mui.com/):
  - [Official documentation](https://mui.com/material-ui/getting-started/installation/): [tracker](https://github.com/yaobinwen/robin_on_rails/issues/136)
- Python
  - [Real Python](https://realpython.com/) articles: [tracker](https://github.com/yaobinwen/robin_on_rails/issues/132)
  - [CPython Internals](https://github.com/zpoint/CPython-Internals)
  - [Official documentation](https://docs.python.org/3/)
  - Python async I/O programming: [tracker](https://github.com/yaobinwen/robin_on_rails/issues/134)
- React.js:
  - [Tutorial: Intro to React](https://reactjs.org/tutorial/tutorial.html): [tracker](https://github.com/yaobinwen/robin_on_rails/issues/8)
  - [Official documentation](https://reactjs.org/docs/getting-started.html): [tracker](https://github.com/yaobinwen/robin_on_rails/issues/8)
- [Software Bill of Materials (SBOM)](https://www.ntia.gov/SBOM)
- Terraform
- UI design:
  - [Design and code Windows apps](https://docs.microsoft.com/en-us/windows/apps/design/)
  - [iOS Design Themes](https://developer.apple.com/design/human-interface-guidelines/platforms/overview)

## 5. Blogs to Read

- [酷壳](https://coolshell.cn/)
- [科技爱好者周刊](https://github.com/ruanyf/weekly): 记录每周值得分享的科技内容 (by 阮一峰)
- [Technology Short Take](https://blog.scottlowe.org/) by Scott Lowe.
- [unixsheikh.com](https://unixsheikh.com/index.html)
