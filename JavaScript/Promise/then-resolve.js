// Point: Show the different cases of `then` (that fulfills).

/**
 * Output:
 * promise1 =  Promise { <pending> }
 * promise2 =  Promise { <pending> }
 * promise3 =  Promise { <pending> }
 * promise4 =  Promise { <pending> }
 * promise5 =  Promise { <pending> }
 * promise6 =  Promise { <pending> }
 * promise1 =  Promise { 'promise1(resolved)' }
 * (node:11517) UnhandledPromiseRejectionWarning: Error: promise1(resolved) is not the expected value
 *     at /home/ywen/yaobin/github/robin_on_rails/JavaScript/Promise/then-resolve.js:69:11
 *     at processTicksAndRejections (internal/process/task_queues.js:95:5)
 * (Use `nodejs --trace-warnings ...` to show where the warning was created)
 * (node:11517) UnhandledPromiseRejectionWarning: Unhandled promise rejection. This error originated either by throwing inside of an async function without a catch block, or by rejecting a promise which was not handled with .catch(). To terminate the node process on unhandled promise rejection, use the CLI flag `--unhandled-rejections=strict` (see https://nodejs.org/api/cli.html#cli_unhandled_rejections_mode). (rejection id: 1)
 * (node:11517) [DEP0018] DeprecationWarning: Unhandled promise rejections are deprecated. In the future, promise rejections that are not handled will terminate the Node.js process with a non-zero exit code.
 * promise1 =  Promise { 'promise1(resolved)' }
 * promise2 =  Promise { 'promise1(resolved)' }
 * promise3 =  Promise { undefined }
 * promise4 =  Promise { null }
 * promise5 =  Promise { 'promise1 resolved value is promise1(resolved)' }
 * promise6 =  Promise {
 *   <rejected> Error: promise1(resolved) is not the expected value
 *       at /home/ywen/yaobin/github/robin_on_rails/JavaScript/Promise/then-resolve.js:69:11
 *       at processTicksAndRejections (internal/process/task_queues.js:95:5)
 * }
 */

const promise1 = new Promise((resolve, reject) => {
  setTimeout(() => {
    resolve('promise1(resolved)')
    console.log("promise1 = ", promise1)
  }, 0)
})
console.log("promise1 = ", promise1)

const promise2 = promise1.then(null, null)

// At this moment, `promise1` is not resolved yet, so `promise2` is still in
// the state of `pending`.
console.log("promise2 = ", promise2)

const promise3 = promise1.then(
  // Handle resolved
  (value) => {
    // Do not return anything.
  }
)

// At this moment, `promise1` is not resolved yet, so `promise3` is still in
// the state of `pending`.
console.log("promise3 = ", promise3)

const promise4 = promise1.then(
  // Handle resolved
  (value) => {
    return null
  }
)

// At this moment, `promise1` is not resolved yet, so `promise4` is still in
// the state of `pending`.
console.log("promise4 = ", promise4)

const promise5 = promise1.then(
  // Handle resolved
  (value) => {
    return `promise1 resolved value is ${value}`
  }
)

// At this moment, `promise1` is not resolved yet, so `promise5` is still in
// the state of `pending`.
console.log("promise5 = ", promise5)

const promise6 = promise1.then(
  // Handle resolved but will throw an error.
  (value) => {
    throw new Error(`${value} is not the expected value`)
  }
)

// At this moment, `promise1` is not resolved yet, so `promise5` is still in
// the state of `pending`.
console.log("promise6 = ", promise6)

setTimeout(() => {
  // Now we have waited enough time. All the promises should be resolved or
  // rejected.

  // `promise1` is `resolved` with the value "promise1(resolved)" because this
  // is what is passed to the function `resolve`.
  console.log("promise1 = ", promise1)

  // See https://promisesaplus.com/#point-43: "If `onFulfilled` is not a
  // function and `promise1` is fulfilled, `promise2` must be fulfilled with
  // the same value as `promise1`."
  //
  // Because `onFulfilled` is `null` and `promise1` is fulfilled, `promise2` is
  // thus fulfilled with the same value as `promise1`, which is
  // "promise1(resolved)".
  console.log("promise2 = ", promise2)

  // See https://promisesaplus.com/#point-64: "If x is not an object or
  // function, fulfill promise with x."
  //
  // `onFulfilled` is a function but returns nothing (which means it returns
  // `undefined`). Therefore, the `x` as the spec says in this case is
  // `undefined` which is neither an object nor a function, so `promise3` is
  // fulfilled with the value of `x` itself which is `undefined`.
  console.log("promise3 = ", promise3)

  // The same reason as `promise3` but with a different value `null`.
  console.log("promise4 = ", promise4)

  // The same reason as `promise3` but with a different value "promise1
  // resolved value is promise1(resolved)".
  console.log("promise5 = ", promise5)

  // See https://promisesaplus.com/#point-42: "If either `onFulfilled` or
  // `onRejected` throws an exception `e`, `promise2` must be rejected with `e`
  // as the reason."
  //
  // Because `onFulfilled` throws the error "promise1(resolved) is not the
  // expected value", `promise6` is rejected with this reason.
  console.log("promise6 = ", promise6)
}, 10)
