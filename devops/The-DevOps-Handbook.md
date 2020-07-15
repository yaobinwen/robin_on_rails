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

## References

- [1] [Blue Green Deployment](https://martinfowler.com/bliki/BlueGreenDeployment.html)
- [2] [Using Blue-Green Deployment to Reduce Downtime and Risk](https://docs.cloudfoundry.org/devguide/deploy-apps/blue-green.html#implementations)
- [3] [Teaching an elephant to dance](https://www.redhat.com/en/engage/teaching-an-elephant-to-dance-20180321): This looks like a good book to read.
