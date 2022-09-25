// Point: The passed in `resolve` and `reject` are internal functions.

// Output:
// resolve =  [Function (anonymous)]
// reject =  [Function (anonymous)]
const promise1 = new Promise((resolve, reject) => {
  console.log("resolve = ", resolve)
  console.log("reject = ", reject)
  setTimeout(() => {
    resolve('Resolved!')
  }, 0)
})
