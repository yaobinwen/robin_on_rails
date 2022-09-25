// Point: Demonstrate https://promisesaplus.com/#point-36: "then may be called
// multiple times on the same promise."

// Output:
// promise1 =  Promise { <pending> }
// promise1 =  Promise { 'promise1(resolved)' }
// promise1 resolved promise1(resolved): call then the first time
// promise1 resolved promise1(resolved): call then the second time
// promise1 resolved promise1(resolved): call then the third time
// promise1 =  Promise { 'promise1(resolved)' }
// promise2 =  Promise { undefined }
// promise3 =  Promise { undefined }
// promise4 =  Promise { undefined }

const promise1 = new Promise((resolve, reject) => {
  setTimeout(() => {
    resolve('promise1(resolved)')
    console.log("promise1 = ", promise1)
  }, 0)
})
console.log("promise1 = ", promise1)

const promise2 = promise1.then(
  // Handle resolved value.
  value => {
    console.log(`promise1 resolved ${value}: call then the first time`)
  }
)

const promise3 = promise1.then(
  // Handle resolved value.
  value => {
    console.log(`promise1 resolved ${value}: call then the second time`)
  }
)

const promise4 = promise1.then(
  // Handle resolved value.
  value => {
    console.log(`promise1 resolved ${value}: call then the third time`)
  }
)

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

  console.log("promise4 = ", promise4)
}, 10)
