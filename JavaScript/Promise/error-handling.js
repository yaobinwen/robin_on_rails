// Skip the first two arguments because they are `node` and this file path.
const CMD_ARGS = process.argv.slice(2)
const EXPECTED_ARG_NUM = 1

if (CMD_ARGS.length !== 1) {
  throw new Error(
    "Too many input arguments. " +
    `Expect ${EXPECTED_ARG_NUM} but got ${CMD_ARGS.length}`
  )
}

const INPUT_ARG = parseInt(CMD_ARGS[0])
if (isNaN(INPUT_ARG)) {
  throw new Error("The input argument should be an integer")
}

console.log(`Input argument: ${INPUT_ARG}`)

const DIVIDEND = 1000

console.log(`Calculating "${DIVIDEND} / ${INPUT_ARG}"...`)

// When the promise is just created, it uses `reject` to handle the errors.
const range_checker = new Promise((resolve, reject) => {
  if (INPUT_ARG !== 0) {
    if (-DIVIDEND <= INPUT_ARG && INPUT_ARG <= DIVIDEND) {
      resolve(INPUT_ARG)
    } else {
      reject(`"${INPUT_ARG}" is out of the range [-${DIVIDEND}, ${DIVIDEND}]`)
    }
  } else {
    reject("Cannot divide by zero")
  }
})

const even_division_checker = range_checker.then((value) => {
  let remainder = DIVIDEND % value
  if (remainder === 0) {
    return value
  } else {
    throw new Error(`"${DIVIDEND}" is not evenly divisible by "${value}"`)
  }
})

const divider = even_division_checker.then((value) => {
  return DIVIDEND / value
})

const result_printer = divider.then((value) => {
  console.log(`${DIVIDEND} / ${INPUT_ARG} = ${value}`)
})

const error_handler = result_printer.catch((reason) => {
  console.error(`Failed: ${reason.message}`)
})
