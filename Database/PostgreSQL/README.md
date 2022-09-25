# PostgreSQL (Postgres)

## 1. Start Postgres

For testing and learning purpose, one can start [a Postgres Docker container](https://hub.docker.com/_/postgres):

```
docker run --name ywen-postgres -e POSTGRES_PASSWORD=test -p 5432:5432 -d --rm postgres:13
```

This command creates a Docker container whose name is `ywen-postgres`, which maps to the host port 5432. The Postgres instance has the default user `postgres` and its password is `test`.

Then one needs to use the following connection options to access the database. Use `createdb` for example:

```
createdb --host localhost --port 5432 --username postgres --password ywen-db
```

## 2. Architectural Overview

Postgres is in the client-server model. Every time a client connects the server, the server program forks a new process that's dedicated to this client.

## 3. `psql`

To access the database using `psql`, run:

```
psql --host localhost --port 5432 --username postgres --password ywen-db
```

The prompt `=>` is for a regular user; the prompt `=#` is for the superuser.

One can enter SQL statements directly, such as `SELECT version();`, `SELECT 2 + 2;`.

`psql` has a number of internal commands, too. Type `\?` for a list of the internal commands; type `\h` for the help information about SQL commands; type `\q` to quit `psql`.

## 4. SQL

### 4.1 Transactions

Transactions begin with `BEGIN` and end with `COMMIT` (or `ROLLBACK`):

```sql
BEGIN;
UPDATE accounts SET balance = balance - 100.00
    WHERE name = 'Alice';
-- etc etc
COMMIT;
```

> PostgreSQL actually treats every SQL statement as being executed within a transaction. If you do not issue a `BEGIN` command, then each individual statement has an implicit `BEGIN` and (if successful) `COMMIT` wrapped around it. A group of statements surrounded by `BEGIN` and `COMMIT` is sometimes called a _transaction block_.

One can use `SAVEPOINT` to define intermediate states inside a transaction so, if needed, one can roll back to a particular save point without having to roll back the entire transaction. For example:

```sql
BEGIN;
UPDATE accounts SET balance = balance - 100.00
    WHERE name = 'Alice';
SAVEPOINT my_savepoint;
UPDATE accounts SET balance = balance + 100.00
    WHERE name = 'Bob';
-- oops ... forget that and use Wally's account
ROLLBACK TO my_savepoint;
UPDATE accounts SET balance = balance + 100.00
    WHERE name = 'Wally';
COMMIT;
```

A save point can be rolled back more than once, but the save points after this save point will be lost.
