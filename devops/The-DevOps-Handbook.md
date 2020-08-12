# The DevOps Handbook

## Preface

The **blue-green deployment pattern** (page _xii_): 

- See Martin Fowler's article [1].
- But it was introduced in the book [Continuous Delivery](https://www.amazon.com/gp/product/0321601912).
- This deployment pattern reduces the downtime: the traffic is not routed to the idle deployment until it is fully ready.
- This deployment pattern reduces the risk: "if something unexpected happens with your new version on Green, you can immediately roll back to the last version by switching back to Blue." [2]
- When database is involved, it is better to "to separate the deployment of schema changes from application upgrades": [1]
  - 1). "apply a database refactoring to change the schema to support both the new and old version of the application."
  - 2). Deploy the database.
  - 3). "check everything is working fine so you have a rollback point".
  - 4). "deploy the new version of the application".
  - 5). "when the upgrade has bedded down remove the database support for the old version".

---

On page _xv_, it mentions _ITIL(IT Infrastructure Library)_, or _ITSM(IT Service Management)_. See [4] for a book about this area.

---

Self-service Infrastructure

Think of "self-service infrastructure" as "vending machines".

## Chapter 02: The First Way: The Principles of Flow

To implement the First Way, we need to:

- Optimize for the entire value stream, not just part of it. See [Quote_P15-01].
- Make our work visible.
  - All work must be logged in the task tracking system. See [Quote_P18-01].
- Limit work in process (WIP).
  - Avoid multi-tasking as much as possible. If you find multi-tasking is "inevitable", ask the question: what causes the necessicity of multi-tasking?
  - When idling, figure out what causes the delay of work from upstream work centers. See [Quote_P18-02].
- Reduce batch sizes.
  - This practice requires continuous integration/deployment. In other words, as long as the team is doing CI/CD, the batch sizes are usually small.

> Quote_P15-01: The First Way requires the fast and smooth flow of work from Development to Operations, to deliver value to customers quickly. **We optimize for this global goal instead of local goals**, such as Development feature completion rates, test find/fix ratios, or Ops availablility measures.

> Quote_P18-01: Nothing can be worked on until it is represented first in a work card, reinforcinng that all work must be made visible.

> Quote_P18-02: Although it may be tempting to start new work (i.e., "It's better to be doing something than nothing"), a far better action would be to find out what is causing the delay and help fix that problem.

## References

- [1] [Blue Green Deployment](https://martinfowler.com/bliki/BlueGreenDeployment.html)
- [2] [Using Blue-Green Deployment to Reduce Downtime and Risk](https://docs.cloudfoundry.org/devguide/deploy-apps/blue-green.html)
- [3] [Teaching an elephant to dance](https://www.redhat.com/en/engage/teaching-an-elephant-to-dance-20180321): This looks like a good book to read.
- [4] [ITIL Service Design: 2011](https://www.amazon.com/ITIL-Service-Design-Lou-Hunnebeck/dp/0113313055)
- [5] [Google: Site Reliability Engineering](https://landing.google.com/sre/)
- [6] Mentioned books:
  - [6.1] [Toyota Kata: Managing People for Improvement, Adaptiveness and Superior Results](https://www.amazon.com/Toyota-Kata-Managing-Improvement-Adaptiveness/dp/0071635238) (Mike Rother) (Page 6)
  - [6.2] [Value Stream Mapping: How to Visualize Work and Align Leadership for Organizational Transformation](https://www.amazon.com/Value-Stream-Mapping-Organizational-Transformation/dp/0071828915) (Karen Martin; Mike Osterling) (Page 7)
  - [6.3] [Kanban: Successful Evolutionary Change for Your Technology Business](https://www.amazon.com/Kanban-Esencial-Condensado-Spanish-Anderson/dp/0984521453) (David J. Anderson) (Page 18)
  - [6.4] [Lean Thinking: Banish Waste and Create Wealth in Your Corporation, Revised and Updated](https://www.amazon.com/Lean-Thinking-Corporation-Revised-Updated/dp/0743249275) (James P. Womack; Daniel T. Jones) (Page 19)
