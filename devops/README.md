# DevOps

## Bird's-eye View

[DevOps Roadmap in 2018](https://github.com/kamranahmedse/developer-roadmap#-devops-roadmap):

![DevOps Roadmap in 2018](https://github.com/kamranahmedse/developer-roadmap/blob/master/images/devops.png)

[DevOps-Student-Roadmap](https://github.com/lucassha/DevOps-Student-Roadmap):

![DevOps-Student-Roadmap](https://camo.githubusercontent.com/84c2faf3a081a531485909d25119723d7672f6c5/68747470733a2f2f692e696d6775722e636f6d2f707967396d48312e706e67)

## Jenkins

- [How to Upgrade Jenkins to New Version and Update Plugins](https://www.thegeekstuff.com/2016/06/upgrade-jenkins-and-plugins)
- How to set up the Swarm mode:
  - This post ([Jenkins Slave Nodes – using the Swarm Plugin](http://www.donaldsimpson.co.uk/2013/03/18/jenkins-slave-nodes-using-the-swarm-plugin/)) describes the process to set up both the master and the slave nodes.
  - [This answer](https://stackoverflow.com/a/34078581/630364) mentions the use of `username` and `password` to connect the slave to the master. Note the `username` and `password` are one of the Jenkins accounts.
  - [This email](https://groups.google.com/d/msg/jenkinsci-users/bQLIJwoVPzU/ifsgQKVu0GIJ) shows how to configure the master side: under "Manage Jenkins" -> "Configure Global Security" -> "Agent" -> Enabled the "TCP port for JNLP slave agents" to "Random" (or a fixed one if appropriate).
- How to share variables among stages:
  - Use [`readFile`](https://stackoverflow.com/a/44101004/630364)
  - Use [`sh` in `environment` section](https://stackoverflow.com/a/43881731/630364). Note that you must have an agent to run the script (`agent` can't be `none`).

## References

- [What Is Immutable Infrastructure](https://www.digitalocean.com/community/tutorials/what-is-immutable-infrastructure)
- [How To Use Blue-Green Deployments to Release Software Safely](https://www.digitalocean.com/community/tutorials/what-is-immutable-infrastructure)
- [CI/CD Tools Comparison: Jenkins, GitLab CI, Buildbot, Drone, and Concourse](https://www.digitalocean.com/community/tutorials/ci-cd-tools-comparison-jenkins-gitlab-ci-buildbot-drone-and-concourse)
- [Twelve-Factor App](https://www.12factor.net/): A methodology for building software-as-a-service apps
- [DevOps术语、工具集、技能图谱最全汇总](https://mp.weixin.qq.com/s?__biz=MzIzNjUxMzk2NQ==&mid=2247489629&idx=1&sn=f1986df12783bf9f76f47077189a1544)
