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
- Reduce the number of handoffs (by minimizing the team's dependencies on external resources).
  - The "Wait Time" (%Busy / %Idle) makes sense in the context of handoffs: the more handoffs, the more wait time.
  - Even within the team, we may want to minimize the handoffs among team members in order to deploy the work to production as soon as possible. That means we want to make every team member be capable of doing every thing on the value stream so each team member can work independently. In reality, I don't think this is always possbile because we want to introduce review to some stages of the work.
- Continually identify and elevate our constraints.
- Eliminate hardships and waste in the value stream.

> Quote_P15-01: The First Way requires the fast and smooth flow of work from Development to Operations, to deliver value to customers quickly. **We optimize for this global goal instead of local goals**, such as Development feature completion rates, test find/fix ratios, or Ops availablility measures.

> Quote_P18-01: Nothing can be worked on until it is represented first in a work card, reinforcinng that all work must be made visible.

> Quote_P18-02: Although it may be tempting to start new work (i.e., "It's better to be doing something than nothing"), a far better action would be to find out what is causing the delay and help fix that problem.

## Chapter 03: The Second Way: The Principles of Feedback

It is better to **swarm** to solve the problem when they occur. Do not bypass the problem just to keep the work going because you will lose the problem's context which is important to figure out the root cause.

> **This practice of swarming seems contrary to common management practice, as we are deliberately allowing a local problem to disrupt operations globally. However, swarming enables learning.** It prevents the loss of critical information due to fading memories or changing circumstances. This is especially critical in complex systems, where many problems occur because of some unexpected, idiosyncratic interaction of people, processes, products, places, and circumstances - as time passes, it becomes impossible to reconstruct exactly what was going on when the problem occurred.

We should allow "everyone in our value stream to find and fix problems in their area of control as part of our daily work". We should **NOT** let people who are remote to the actual work area to make the critical desicion because, firstly, those people don't have the first-hand context and knowledge, and secondly, the additional approval process harms the efficiency.

## Chapter 04: The Third Way: The Principles of Continual Learning and Experimentation

We must create a culture that "instead of looking for human error, we look for how we can redesign the system to prevent the accident from happening again."

> "By removing blame, you remove fear; by removing fear, you enable honesty; and honesty enables prevention." (Bethany Macri, engineer at Etsy)

We must reserve time to pay down the technical debt and learn new things:

> We improve daily work by explicitly reserving time to pay down technical debt, fix defects, and refactor and improve problematic areas of our code and environments - we do this by reserving cycles in each development interval, or by scheduling _kaizen blitzes_, which are periods when engineers self-organize into teams to work on fixing any problem they want.

We must share the learned knowledge out in some active way, not just writing it down in some document and hoping others would read it:

> When teams or individuals have experiences that create expertise, our goal is to convert that tacit knowledge (i.e., knowledge that is difficult to transfer to another person by means of writing it down or verbalizing) into explicit, codified knowledge, which becomes someone else's expertise through practice.

> The leader's role is to create the conditions so their team can discover greatness in their daily work.

## Chapter 05: Selecting Which Value Stream to Start With

"greenfield" vs "brownfield"

"systems of record" (doing it right) vs "systems of engagement" (doing it fast)

> **Our goal is to find those teams that already believe in the need for DevOps principles and practices, and who possess a desire and demonstrated ability to innovate and improve their own processes.**
>
> Especially in the early stages, **we will not spend much time trying to convert the more conservative groups**. Instead, we will focus our energy on creating successes with less risk-averse groups and build out our base from there (a process that is discussed further in the next section). **Even if we have the highest levels of executive sponsorship, we will avoid the big bang approach (i.e., starting everywhere all at once)**, choosing instead to focus our efforts in a few areas of the organization, ensuring that those initiatives are successful, and expanding from there.

> We specifically bypass dangerous political battles that could jeopardize our initiative.

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
