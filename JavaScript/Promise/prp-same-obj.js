/**
 * https://promisesaplus.com/#point-48: "If promise and x refer to the same
 * object, reject promise with a TypeError as the reason."
 */

const promise1 = new Promise((resolve, reject) => {
  setTimeout(() => {
    resolve(promise1);
    console.log("promise1 = ", promise1)
  }, 0);
});
console.log("promise1 = ", promise1)

const promise2 = promise1.then(
  // Handle resolved value
  value => {
    throw new Error("Wrong! promise1 should have been rejected.")
  },
  // Handle rejected reason.
  reason => {
    if (reason instanceof TypeError) {
      return "Success!"
    } else {
      throw new TypeError(`Wrong! The rejected reason should be a 'TypeError'`)
    }
  }
)

setTimeout(() => {
  console.log("promise2 = ", promise2)
}, 0)
