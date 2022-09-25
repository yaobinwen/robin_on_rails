// Point: Demonstrate https://promisesaplus.com/#point-36: "then may be called
// multiple times on the same promise."

// Output:
// promise1 =  Promise { <pending> }
// promise1 =  Promise { <rejected> 'promise1(reject)' }
// promise1 rejected promise1(reject): call then the first time
// promise1 rejected promise1(reject): call then the second time
// promise1 rejected promise1(reject): call then the third time
// promise1 =  Promise { <rejected> 'promise1(reject)' }
// promise2 =  Promise { undefined }
// promise3 =  Promise { undefined }
// promise4 =  Promise { undefined }

const promise1 = new Promise((resolve, reject) => {
  setTimeout(() => {
    reject('promise1(reject)')
    console.log("promise1 = ", promise1)
  }, 0)
})
console.log("promise1 = ", promise1)

const promise2 = promise1.then(
  // Handle resolved value.
  null,
  // Handle rejected reason.
  reason => {
    console.log(`promise1 rejected ${reason}: call then the first time`)
  }
)

const promise3 = promise1.then(
  // Handle resolved value.
  null,
  // Handle rejected reason.
  reason => {
    console.log(`promise1 rejected ${reason}: call then the second time`)
  }
)

const promise4 = promise1.then(
  // Handle resolved value.
  null,
  // Handle rejected reason.
  reason => {
    console.log(`promise1 rejected ${reason}: call then the third time`)
  }
)

setTimeout(() => {
  // Now we have waited enough time. All the promises should be resolved or
  // rejected.

  console.log("promise1 = ", promise1)
  console.log("promise2 = ", promise2)
  console.log("promise3 = ", promise3)
  console.log("promise4 = ", promise4)
}, 10)
