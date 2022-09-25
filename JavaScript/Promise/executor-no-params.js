// Point: a promise doesn't need to accept `resolve` and `reject`, although
// this makes the promise practically useless.

/**
 * Output:
 * promise1 =  Promise { <pending> }
 * promise1 =  Promise { <pending> }
 */

const promise1 = new Promise(() => {
  setTimeout(() => {}, 0)
})

console.log("promise1 = ", promise1)

setTimeout(() => {
  console.log("promise1 = ", promise1)
}, 10)
