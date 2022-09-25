// Output:
// promise1 resolved value: 101
// promise2 resolved value: 102
// promise3 rejected reason: Error: Unexpected value from promise2: 102
// promise4 resolved value: promise3 rejection gets handled
// promise5 rejected reason: Error: Unexpected value from promise4: promise3 rejection gets handled
// promise6 rejected reason: Error: Something else wrong happens...
// promise1 =  Promise { 101 }
// promise2 =  Promise { 102 }
// promise3 =  Promise {
//   <rejected> Error: Unexpected value from promise2: 102
//       at /home/ywen/yaobin/github/robin_on_rails/JavaScript/Promise/then-chained.js:23:11
// }
// promise4 =  Promise { 'promise3 rejection gets handled' }
// promise5 =  Promise {
//   <rejected> Error: Unexpected value from promise4: promise3 rejection gets handled
//       at /home/ywen/yaobin/github/robin_on_rails/JavaScript/Promise/then-chained.js:48:11
// }
// promise6 =  Promise {
//   <rejected> Error: Something else wrong happens...
//       at /home/ywen/yaobin/github/robin_on_rails/JavaScript/Promise/then-chained.js:64:11
// }
// promise7 =  Promise { undefined }

const promise1 = new Promise((resolve, reject) => {
  setTimeout(() => {
    resolve(101)
  }, 500)
})

const promise2 = promise1.then(
  // Handle resolved value.
  value => {
    console.log(`promise1 resolved value: ${value}`)
    return value + 1
  },
  // Handle rejected reason.
  reason => {
    console.debug(`promise1 rejected reason: ${reason}`)
  }
)

const promise3 = promise2.then(
  // Handle resolved value.
  value => {
    console.log(`promise2 resolved value: ${value}`)
    throw new Error(`Unexpected value from promise2: ${value}`)
  },
  // Handle rejected reason.
  reason => {
    console.debug(`promise2 rejected reason: ${reason}`)
  }
)

const promise4 = promise3.then(
  // Handle resolved value.
  value => {
    console.log(`promise3 resolved value: ${value}`)
    return value + 3
  },
  // Handle rejected reason.
  reason => {
    console.debug(`promise3 rejected reason: ${reason}`)
    return "promise3 rejection gets handled"
  }
)

const promise5 = promise4.then(
  // Handle resolved value.
  value => {
    console.log(`promise4 resolved value: ${value}`)
    throw new Error(`Unexpected value from promise4: ${value}`)
  },
  // Handle rejected reason.
  reason => {
    console.debug(`promise4 rejected reason: ${reason}`)
  }
)

const promise6 = promise5.then(
  // Handle resolved value.
  value => {
    console.log(`promise5 resolved value: ${value}`)
  },
  // Handle rejected reason.
  reason => {
    console.debug(`promise5 rejected reason: ${reason}`)
    throw new Error("Something else wrong happens...")
  }
)

const promise7 = promise6.then(
  // Handle resolved value.
  value => {
    console.log(`promise6 resolved value: ${value}`)
  },
  // Handle rejected reason.
  reason => {
    console.debug(`promise6 rejected reason: ${reason}`)
  }
)

setTimeout(() => {
  // Now we have waited enough time. All the promises should be resolved or
  // rejected.

  console.log("promise1 = ", promise1)
  console.log("promise2 = ", promise2)
  console.log("promise3 = ", promise3)
  console.log("promise4 = ", promise4)
  console.log("promise5 = ", promise5)
  console.log("promise6 = ", promise6)
  console.log("promise7 = ", promise7)
}, 1500)
