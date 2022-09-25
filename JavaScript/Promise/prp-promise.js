/**
 * https://promisesaplus.com/#point-49: "If x is a promise, adopt its state: "
 */

const promise1 = new Promise((resolve, reject) => {
  setTimeout(() => {
    resolve("promise1(resolved)");
    console.log("promise1 = ", promise1)
  }, 0);
});
console.log("promise1 = ", promise1)

const promise2 = promise1.then(
  // Handle resolved value.
  value => {
    // Return another promise `p` which keeps pending for one second. During
    // the second, `promise2` is in `pending` state, too.
    const p = new Promise((resolve, reject) => {
      setTimeout(() => {
        resolve(`promise2(internal)(${value})`)
      }, 500)
    })
    return p
  },

  // Handle rejected reason.
  null,
)

// At this moment, `promise2` is also pending.
// See https://promisesaplus.com/#point-50: "If x is pending, promise must
// remain pending until x is fulfilled or rejected."
console.log("promise2 = ", promise2)

const promise3 = promise1.then(
  // Handle resolved value.
  value => {
    return Promise.resolve(`promise3(internal)(${value})`)
  },

  // Handle rejected reason.
  null,
)

const promise4 = promise1.then(
  // Handle resolved value.
  value => {
    return Promise.reject(`promise4(internal)(${value})`)
  },

  // Handle rejected reason.
  null,
)

setTimeout(() => {
  console.log("promise1 = ", promise1)
  console.log("promise2 = ", promise2)

  // https://promisesaplus.com/#point-51: "If/when x is fulfilled, fulfill
  // promise with the same value."
  console.log("promise3 = ", promise3)

  // https://promisesaplus.com/#point-52: "If/when x is rejected, reject
  // promise with the same reason."
  console.log("promise4 = ", promise4)
}, 1000)
