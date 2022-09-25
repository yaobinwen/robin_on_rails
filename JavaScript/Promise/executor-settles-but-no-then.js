// Point: Promises don't have to call `then`.

/**
 * Output:
 * resolve =  [Function (anonymous)]
 * Promise { <pending> }
 * Promise { <pending> }
 * Promise { 'Resolved!' }
 * Promise { <rejected> 'Rejected!' }
 * (node:23245) UnhandledPromiseRejectionWarning: Rejected!
 * (Use `nodejs --trace-warnings ...` to show where the warning was created)
 * (node:23245) UnhandledPromiseRejectionWarning: Unhandled promise rejection. This error originated either by throwing inside of an async function without a catch block, or by rejecting a promise which was not handled with .catch(). To terminate the node process on unhandled promise rejection, use the CLI flag `--unhandled-rejections=strict` (see https://nodejs.org/api/cli.html#cli_unhandled_rejections_mode). (rejection id: 1)
 * (node:23245) [DEP0018] DeprecationWarning: Unhandled promise rejections are deprecated. In the future, promise rejections that are not handled will terminate the Node.js process with a non-zero exit code.
 */

const promise1 = new Promise((resolve, reject) => {
  console.log("resolve = ", resolve)
  setTimeout(() => {
    resolve('Resolved!')
    console.log("promise1 = ", promise1)
  }, 0)
})

console.log("promise1 = ", promise1)

const promise2 = new Promise((resolve, reject) => {
  setTimeout(() => {
    reject('Rejected!')
    console.log("promise2 = ", promise2)
  }, 0)
})

console.log("promise2 = ", promise2)
