# DevOps

## Bird's-eye View

### Periodic Table of DevOps Tools (v3)

[Periodic Table of DevOps Tools (v3)](https://xebialabs.com/periodic-table-of-devops-tools/)

### DevOps Roadmap in 2018

[DevOps Roadmap in 2018](https://github.com/kamranahmedse/developer-roadmap#-devops-roadmap):

![DevOps Roadmap in 2018](https://github.com/kamranahmedse/developer-roadmap/blob/master/images/devops.png)

### DevOps-Student-Roadmap

[DevOps-Student-Roadmap](https://github.com/lucassha/DevOps-Student-Roadmap):

![DevOps-Student-Roadmap](https://camo.githubusercontent.com/84c2faf3a081a531485909d25119723d7672f6c5/68747470733a2f2f692e696d6775722e636f6d2f707967396d48312e706e67)

## Testinfra

Its official introduction says:

> With [Testinfra](https://testinfra.readthedocs.io/en/latest/index.html) you can write unit tests in Python to test actual state of your servers configured by management tools like [Salt](https://saltstack.com/), [Ansible](https://www.ansible.com/), [Puppet](https://puppet.com/), [Chef](https://www.chef.io/) and so on.
>
>Testinfra aims to be a [Serverspec](https://serverspec.org/) equivalent in python and is written as a plugin to the powerful [Pytest](https://docs.pytest.org/en/latest/) test engine

## Jenkins

- [How to Upgrade Jenkins to New Version and Update Plugins](https://www.thegeekstuff.com/2016/06/upgrade-jenkins-and-plugins)
- How to set up the Swarm mode:
  - This post ([Jenkins Slave Nodes – using the Swarm Plugin](http://www.donaldsimpson.co.uk/2013/03/18/jenkins-slave-nodes-using-the-swarm-plugin/)) describes the process to set up both the master and the slave nodes.
  - [This answer](https://stackoverflow.com/a/34078581/630364) mentions the use of `username` and `password` to connect the slave to the master. Note the `username` and `password` are one of the Jenkins accounts.
  - [This email](https://groups.google.com/d/msg/jenkinsci-users/bQLIJwoVPzU/ifsgQKVu0GIJ) shows how to configure the master side: under "Manage Jenkins" -> "Configure Global Security" -> "Agent" -> Enabled the "TCP port for JNLP slave agents" to "Random" (or a fixed one if appropriate).
- How to share variables among stages:
  - Use [`readFile`](https://stackoverflow.com/a/44101004/630364)
  - Use [`sh` in `environment` section](https://stackoverflow.com/a/43881731/630364). Note that you must have an agent to run the script (`agent` can't be `none`).

## Other CI/CD Tools

See [3]:

- [Buildbot](https://buildbot.net/)
- [Drone](https://drone.io/)
- [Concourse](https://concourse-ci.org/)

## System Administration

**NOTE:** The following information assumes a Linux operating system unless specified otherwise.

- How to check the hardware specification:
  - Use `dmidecode`. For example:
    - `dmidecode -t17` to show each RAM slot information.
    - `dmidecode -t16` to show the entire memory array information, including its maximum capacity.
  - Use [crucial.com](http://www.crucial.com/) to check the information.

## Immutable Infrastructure

The article [9] explains the advantages of immutable infrastructure well.

In the "mutable infrastructure" paradigm, a server is "upgraded" from version 1 to version 2 by running a set of instructions. Because any instruction may fail, this introduces additional states between version 1 and version 2. However:

- Usually, only the version 1 and version 2 are well tested. The development team has no knowledge of whether a transient state works well or not. This may impact the user's experience of the service and bring risk to the system.
- Secondly, the additional in-between states introduced by mutating a server introduces unnecessary complexity because instead of managing only two well-defined states, the development team must handle much more states of the servers.
- Think about the case when the team must manage 1000 servers, with each server possibly landing in any of the in-between states.
- Even if the probability of success is high on one server, when there are many servers, the probability of success of all the servers is still low enough to be cautious.

In the "immutable infrastructure" paradigm, the team only need to deal with two well-defined states. The deployment can be done in the "blue-green" deployment model, which can guarantee a smooth transition of the user traffic from the earlier version of the service to the newer version.

## References

- [1] [What Is Immutable Infrastructure](https://www.digitalocean.com/community/tutorials/what-is-immutable-infrastructure)
- [2] [How To Use Blue-Green Deployments to Release Software Safely](https://www.digitalocean.com/community/tutorials/what-is-immutable-infrastructure)
- [3] [CI/CD Tools Comparison: Jenkins, GitLab CI, Buildbot, Drone, and Concourse](https://www.digitalocean.com/community/tutorials/ci-cd-tools-comparison-jenkins-gitlab-ci-buildbot-drone-and-concourse)
- [4] [Twelve-Factor App](https://www.12factor.net/): A methodology for building software-as-a-service apps
- [5] [DevOps术语、工具集、技能图谱最全汇总](https://mp.weixin.qq.com/s?__biz=MzIzNjUxMzk2NQ==&mid=2247489629&idx=1&sn=f1986df12783bf9f76f47077189a1544)
- [6] [2018 State of DevOps Report](https://puppet.com/resources/whitepaper/state-of-devops-report)
- [7] [A Comprehensive Guide to DevOps & It’s Tools Ecosystem](https://medium.com/@BangBitTech/a-comprehensive-guide-to-devops-its-tools-ecosystem-83d739cdf543)
- [8] [https://opensource.com/article/20/2/devops-beginners](https://opensource.com/article/20/2/devops-beginners)
- [9] [What is Mutable vs. Immutable Infrastructure?](https://www.hashicorp.com/resources/what-is-mutable-vs-immutable-infrastructure/)
