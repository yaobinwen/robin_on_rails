function promise(delay_ms, resolution, res_value) {
  let p = new Promise((resolve, reject) => {
    setTimeout(() => {
      if (resolution === "fulfill") {
        resolve(res_value)
      } else if (resolution === "reject") {
        reject(res_value)
      } else if (resolution === "never_resolve") {
        // The promise will never resolve.
      } else {
        throw new Error(`unkown resolution "${resolution}"`)
      }
    }, delay_ms)
  })
  .then(value => {
    console.log(`fulfilled value: ${value}`)
    return value
  })
  .catch(reason => {
    console.error(`rejected reason: ${reason}`)
    throw new Error(reason)
  })

  return p
}

function run_all(promises) {
  return Promise.all(promises)
    .then(result => {
      console.info(`Promise.all: fulfilled result: ${result}`)
    })
    .catch(reason => {
      console.error(`Promise.all: rejected reason: ${reason}`)
    })
}

// Output:
//
// fulfilled value: 1
// fulfilled value: 2
// Promise.all: fulfilled result: 1,2
//
// Notes: `all()` is not fulfilled until all the input promises fulfill.
run_all([
  promise(100, "fulfill", 1),
  promise(110, "fulfill", 2),
])

// Output:
//
// fulfilled value: 3
// rejected reason: 4
// Promise.all: rejected reason: Error: 4
//
// Notes: `all()` is rejected if one of the input promises gets rejected.
run_all([
  promise(120, "fulfill", 3),
  promise(130, "reject", 4),
])

// Output:
//
// rejected reason: 5
// Promise.all: rejected reason: Error: 5
// rejected reason: 6
//
// Notes: `all()` is rejected (i.e. resolved) once one of the input promises is
// rejected, regardless if other promises are resolved or not.
run_all([
  promise(150, "reject", 5),
  promise(160, "reject", 6),
])

// Output:
//
// fulfilled value: 7
//
// Notes: `all()` is not fulfilled until all the input promises fulfill. If
// one promise never fulfill, `all()` will never fulfill either.
run_all([
  promise(170, "fulfill", 7),
  promise(180, "never_resolve", 8),
])

// Output:
//
// rejected reason: 10
// Promise.all: rejected reason: Error: 10
//
// Notes: Again, `all()` is rejected once one of the input promiese is
// rejected. The other promises may take much longer to fulfill or never
// resolve, but that doesn't matter.
run_all([
  promise(190, "never_resolve", 9),
  promise(200, "reject", 10),
])

function run_allSettled(promises) {
  return Promise.allSettled(promises)
    .then(result => {
      console.info("Promise.allSettled: fulfilled result: ", result)
    })
    .catch(() => {
      throw new Error("Promise.allSettled catch block should not be called")
    })
}

// Output:
//
// fulfilled value: A
// fulfilled value: B
// Promise.allSettled: fulfilled result:  [
//   { status: 'fulfilled', value: 'A' },
//   { status: 'fulfilled', value: 'B' }
// ]
//
// Notes: `allSettled()` is fulfilled when all the input promises are settled.
run_allSettled([
  promise(500, "fulfill", "A"),
  promise(510, "fulfill", "B"),
])

// Output:
//
// rejected reason: C
// fulfilled value: D
// Promise.allSettled: fulfilled result:  [
//   {
//     status: 'rejected',
//     reason: Error: C
//         at /home/ywen/yaobin/github/robin_on_rails/JavaScript/Promise/API/all_allSettled/all.js:21:11
//   },
//   { status: 'fulfilled', value: 'D' }
// ]
//
// Notes: `allSettled()` is fulfilled when all the input promises are settled.
run_allSettled([
  promise(520, "reject", "C"),
  promise(530, "fulfill", "D"),
])

// Output:
//
// rejected reason: E
// rejected reason: F
// Promise.allSettled: fulfilled result:  [
//   {
//     status: 'rejected',
//     reason: Error: E
//         at /home/ywen/yaobin/github/robin_on_rails/JavaScript/Promise/API/all_allSettled/all.js:21:11
//   },
//   {
//     status: 'rejected',
//     reason: Error: F
//         at /home/ywen/yaobin/github/robin_on_rails/JavaScript/Promise/API/all_allSettled/all.js:21:11
//   }
// ]
//
// Notes: `allSettled()` is fulfilled when all the input promises are settled.
run_allSettled([
  promise(540, "reject", "E"),
  promise(550, "reject", "F"),
])

// Output:
//
// fulfilled value: G
//
// Notes: `allSettled()` is not fulfilled until all the input promises are
// settled.
run_allSettled([
  promise(560, "fulfill", "G"),
  promise(570, "never_resolve", "H"),
])

// Output:
//
// rejected reason: J
//
// Notes: `allSettled()` is not fulfilled until all the input promises are
// settled.
run_allSettled([
  promise(580, "never_resolve", "I"),
  promise(590, "reject", "J"),
])
