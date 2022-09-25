// Point: Show the different cases of `then` (that rejects).

/**
 * Output:
 * promise1 =  Promise { <pending> }
 * promise2 =  Promise { <pending> }
 * promise3 =  Promise { <pending> }
 * promise4 =  Promise { <pending> }
 * promise5 =  Promise { <pending> }
 * promise6 =  Promise { <pending> }
 * promise1 =  Promise { <rejected> 'promise1(rejected)' }
 * Analyzing the rejection reason promise1(rejected)...
 * (node:8624) UnhandledPromiseRejectionWarning: promise1(rejected)
 * (Use `nodejs --trace-warnings ...` to show where the warning was created)
 * (node:8624) UnhandledPromiseRejectionWarning: Unhandled promise rejection. This error originated either by throwing inside of an async function without a catch block, or by rejecting a promise which was not handled with .catch(). To terminate the node process on unhandled promise rejection, use the CLI flag `--unhandled-rejections=strict` (see https://nodejs.org/api/cli.html#cli_unhandled_rejections_mode). (rejection id: 1)
 * (node:8624) [DEP0018] DeprecationWarning: Unhandled promise rejections are deprecated. In the future, promise rejections that are not handled will terminate the Node.js process with a non-zero exit code.
 * (node:8624) UnhandledPromiseRejectionWarning: Error: something else wrong happens...
 *     at /home/ywen/yaobin/github/robin_on_rails/JavaScript/Promise/then-reject.js:42:11
 *     at processTicksAndRejections (internal/process/task_queues.js:95:5)
 * (node:8624) UnhandledPromiseRejectionWarning: Unhandled promise rejection. This error originated either by throwing inside of an async function without a catch block, or by rejecting a promise which was not handled with .catch(). To terminate the node process on unhandled promise rejection, use the CLI flag `--unhandled-rejections=strict` (see https://nodejs.org/api/cli.html#cli_unhandled_rejections_mode). (rejection id: 2)
 * promise1 =  Promise { <rejected> 'promise1(rejected)' }
 * promise2 =  Promise { <rejected> 'promise1(rejected)' }
 * promise3 =  Promise { undefined }
 * promise4 =  Promise { null }
 * promise5 =  Promise { 'Analyzing the rejection reason promise1(rejected)...' }
 * promise6 =  Promise {
 *   <rejected> Error: something else wrong happens...
 *       at /home/ywen/yaobin/github/robin_on_rails/JavaScript/Promise/then-reject.js:42:11
 *       at processTicksAndRejections (internal/process/task_queues.js:95:5)
 * }
 */

const promise1 = new Promise((resolve, reject) => {
  setTimeout(() => {
    reject('promise1(rejected)')
    console.log("promise1 = ", promise1)
  }, 0)
})
console.log("promise1 = ", promise1)

const promise2 = promise1.then(null, null)
console.log("promise2 = ", promise2)

const promise3 = promise1.then(
  null, // No handler for resolved value.
  (reason) => {
    // Do not return anything.
  }
)
console.log("promise3 = ", promise3)

const promise4 = promise1.then(
  null, // No handler for resolved value.
  (reason) => {
    return null
  }
)
console.log("promise4 = ", promise4)

const promise5 = promise1.then(
  null, // No handler for resolved value.
  (reason) => {
    return `Analyzing the rejection reason ${reason}...`
  }
)
console.log("promise5 = ", promise5)

const promise6 = promise1.then(
  null, // No handler for resolved value.
  (reason) => {
    console.log(`Analyzing the rejection reason ${reason}...`)
    throw new Error("something else wrong happens...")
  }
)
console.log("promise6 = ", promise6)

setTimeout(() => {
  // Now we have waited enough time. All the promises should be resolved or
  // rejected.

  // `promise1` is `rejected` with the value "promise1(rejected)" because this
  // is what is passed to the function `reject`.
  console.log("promise1 = ", promise1)

  // https://promisesaplus.com/#point-44: "If onRejected is not a function and
  // promise1 is rejected, promise2 must be rejected with the same reason as
  // promise1."
  //
  // Because `onRejected` is null and promise1 was rejected, promise2 is thus
  // rejected with the same reason.
  console.log("promise2 = ", promise2)

  // This result is determined by two rules:
  // 1). https://promisesaplus.com/#point-41: "If either onFulfilled or
  // onRejected returns a value x, run the Promise Resolution Procedure
  // [[Resolve]](promise2, x)."
  //
  // Although the rejection handler doesn't have a `return` statement, it
  // returns `undefined` by default. Therefore, the result is determined by
  // the second rule:
  // 2). https://promisesaplus.com/#point-64: "If x is not an object or
  // function, fulfill promise with x."
  //
  // Therefore, promise3 is fulfilled with `undefined`.
  console.log("promise3 = ", promise3)

  // Same reasoning as promise3.
  console.log("promise4 = ", promise4)

  // Same reasoning as promise3.
  console.log("promise5 = ", promise5)

  // https://promisesaplus.com/#point-42: "If either onFulfilled or onRejected
  // throws an exception e, promise2 must be rejected with e as the reason."
  //
  // Because `onRejected` throws an exception here, promise6 was rejected with
  // the exception as the reason.
  console.log("promise6 = ", promise6)
}, 10)
