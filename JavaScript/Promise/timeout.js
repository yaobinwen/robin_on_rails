let cancel1 = false
let cancel2 = false
let cancel3 = false

function is_service_ready() {
  return false
}

// This promise never resolves because `is_service_ready()` always returns
// `false`. As a result, this promise needs external cancelation in order to
// stop appropriately.
function check_service_ready_forever_as_Promise(delay_ms) {
  let p1_service_ready = new Promise((resolve, reject) => {
    function check() {
      if (cancel1) {
        reject("p1_service_ready: canceled")
        return
      }
  
      if (is_service_ready()) {
        resolve()
        return
      }
  
      setTimeout(check, delay_ms)
    }
  
    check()
  })

  return p1_service_ready
}

// This promise implements its own timeout mechanism. So its internal timeout
// and the timeout that's imposed by `TimeoutPromise` will race.
function check_service_ready_timeout_as_Promise(delay_ms, timeout_ms) {
  let p2_service_ready_timeout = new Promise((resolve, reject) => {
    function check(delay_ms = 10, timeout_ms = 1000, i = 0) {
      if (cancel2) {
        reject("p2_service_ready_timeout: canceled")
        return
      }
  
      if (i * delay_ms >= timeout_ms) {
        reject("p2_service_ready_timeout: timeout")
        return
      }
  
      if (is_service_ready()) {
        resolve()
        return
      }
  
      setTimeout(check, delay_ms, delay_ms, timeout_ms, i+1)
    }
  
    check(delay_ms, timeout_ms)
  })

  return p2_service_ready_timeout
}

function multiplication_chart_as_Promise(N = 9) {
  let p3_multiplication_chart = new Promise((resolve, reject) => {
    let chart = new Array(N)
    for (let i = 1; i <= N; ++i) {
      let row = new Array(N)
      for (let j = 1; j <=N; ++j) {
        if (cancel3) {
          reject("p3_multiplication_chart: canceled")
          return
        }
        row[j-1] = i * j
      }
      chart[i-1] = row
    }

    resolve(chart)
  })

  return p3_multiplication_chart
}

// NOTE(ywen): For demo purpose, the code does not do error handling (such as
// validating the input arguments).
//
// Arguments:
// - timeout_ms: The timeout in milliseconds.
// - prom: The promise to run.
// - on_fulfill: The callback when `prom` can fulfill before timeout. It has
//    the same signature as the `then`'s `onFulfillment` callback:
//    `value => {...}`.
// - on_reject: The callback when `prom` is rejected before timeout. It has the
//    same signature as the `then`'s `onRejection` callback: `reason => {...}`.
// - on_timeout: The callback to handle the timeout event. It has the signature
//    of `(promise, timeout_ms) => {...}`.
//
// Return:
// - A promise that is fulfilled/rejected when `prom` is fulfilled/rejected.
function TimeoutPromise(timeout_ms, prom, on_fulfill, on_reject, on_timeout) {
  let tid = -1
  let timeout = new Promise((undefined, reject) => {
    tid = setTimeout(() => {
      on_timeout(prom, timeout_ms)
      reject("Timeout")
    }, timeout_ms)
  })

  let p = Promise.race([prom, timeout])
    .then(on_fulfill)
    .catch(on_reject)
    .finally(() => {
      clearTimeout(tid)
    })

  return p
}

function on_reject(reason) {
  cancel1 = true
  cancel2 = true
  cancel3 = true

  console.error(reason)
}

TimeoutPromise(
  // timeout_ms
  2500,
  // prom
  Promise.allSettled([
    check_service_ready_forever_as_Promise(),
    check_service_ready_timeout_as_Promise(10, 1000),
  ]),
  // on_fulfill
  (results) => { console.info(results) },
  // on_reject
  (reason) => {
    // When one promise gets rejected, we need to cancel the others because
    // otherwise they will keep running until resolved.
    cancel1 = true
    cancel2 = true
    console.error("rejected: ", reason)
  },
  // on_timeout
  (prom, timeout_ms) => {
    cancel1 = true
    cancel2 = true
    console.error("promise timeout: ", prom, timeout_ms)
  },
)

TimeoutPromise(
  // timeout_ms
  2500,
  // prom
  multiplication_chart_as_Promise(3),
  // on_fulfill
  (value) => {
    console.info("(p3_multiplication_chart) chart is ready: ", value)
  },
  // on_reject
  (reason) => {
    console.error("(p3_multiplication_chart) rejected: ", reason)
  },
  // on_timeout
  (prom, timeout_ms) => {
    cancel3 = true
    console.error(
      "(p3_multiplication_chart) promise timeout: ", prom, timeout_ms,
    )
  },
)
