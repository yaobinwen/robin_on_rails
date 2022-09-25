// Point: a promise doesn't need to call `resolve` and `reject`, although
// this makes the promise practically useless.

/**
 * Output:
 * promise1 =  Promise { <pending> }
 * promise1 =  Promise { <pending> }
 */

const promise1 = new Promise((resolve, reject) => {
  setTimeout(() => {
    // We do not call `resolve` or `reject` here. As a result, the promise will
    // remain "pending".
  }, 0)
})

console.log("promise1 = ", promise1)

setTimeout(() => {
  console.log("promise1 = ", promise1)
}, 10)
