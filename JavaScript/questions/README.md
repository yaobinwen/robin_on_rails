# JavaScript (also React.js) Questions

## What is type coercion?

Type coercion in JavaScript refers to the automatic or implicit conversion of values from one data type to another. This conversion happens when operators or functions are applied to values of different types, often without the developer explicitly requesting it.

- [JavaScript type coercion explained](https://www.freecodecamp.org/news/js-type-coercion-explained-27ba3d9a2839/)
- [JS Comparison Table](https://dorey.github.io/JavaScript-Equality-Table/)
- [JS Comparison Table (unified)](https://dorey.github.io/JavaScript-Equality-Table/unified/)

Three types of conversion

The first rule to know is there are only three types of conversion in JavaScript:
- to string
- to boolean
- to number

Secondly, conversion logic for primitives and objects works differently, but both primitives and objects can only be converted in those three ways.

String conversion: To explicitly convert values to a string apply the `String()` function. Implicit coercion is triggered by the binary `+` operator, when any operand is a string:

```javascript
String(123) // explicit
123 + ''    // implicit
```

Boolean conversion: To explicitly convert a value to a boolean apply the `Boolean()` function. Implicit conversion happens in logical context, or is triggered by logical operators (`||`, `&&`, `!`).

```javascript
Boolean(2)          // explicit
if (2) { ... }      // implicit due to logical context
!!2                 // implicit due to logical operator
2 || 'hello'        // implicit due to logical operator
```

Numeric conversion: For an explicit conversion just apply the `Number()` function, same as you did with `Boolean()` and `String()`. Implicit conversion is tricky, because it's triggered in more cases:
- comparison operators (`>`, `<`, `<=`, `>=`)
- bitwise operators (`|` `&` `^` `~`)
- arithmetic operators (`-` `+` `*` `/` `% ). Note, that binary `+` does not trigger numeric conversion, when any operand is a string.
- unary `+` operator
- loose equality operator `==` (incl. `!=`).
  - Note that `==` does not trigger numeric conversion when both operands are strings.

There are two special rules to remember:
- When applying `==` to `null` or `undefined`, numeric conversion does not happen. `null` equals only to `null` or `undefined`, and does not equal to anything else.
- `NaN` does not equal to anything even itself.

## What is the difference between `==` and `===`?

`==` (Equality Operator):
- Performs **type coercion** if the operands are of different types.
- Converts the operands to the same type before making the comparison.
- Can lead to unexpected results due to type conversion.

Examples:

```javascript
console.log(1 == '1'); // true
console.log(true == 1); // true
console.log(null == undefined); // true
```

`===` (Strict Equality Operator):
- Does not perform type coercion.
- Requires both the type and value of the operands to be the same for the comparison to be true.
- Generally recommended for most comparisons to avoid unexpected type coercion.

Examples:

```javascript
console.log(1 === '1'); // false
console.log(true === 1); // false
console.log(null === undefined); // false
```

## Event loop; call stack; task queue

It allows the JavaScript runtime to perform non-blocking operations by offloading operations like I/O to the system and running them asynchronously. The event loop continuously checks the call stack and the task queue to determine what code to execute next.

### Event Loop

The event loop monitors the call stack and the task queue. When the call stack is empty, the event loop picks the first task from the task queue and pushes it onto the call stack for execution.

Call Stack: Synchronous code runs directly in the call stack.

Task Queue (or Callback Queue):
- The task queue is a data structure that holds tasks (callbacks or events) to be executed after the current script execution completes.
- Tasks in the task queue are added by asynchronous operations, such as `setTimeout`, promises, or event handlers.
- When asynchronous operations complete, their corresponding callbacks are pushed into the task queue.
- The event loop will process these tasks only when the call stack is empty.

Interaction
- Synchronous Functions:
  - Functions are called and executed in the call stack.
  - They run to completion before the next function call.
- Asynchronous Functions:
  - When an asynchronous function is called, it initiates the operation and returns immediately.
  - The callback is placed in the task queue when the asynchronous operation completes.
- Event Loop Operation:
  - Continuously checks if the call stack is empty.
  - If the call stack is empty, it takes the first task from the task queue and pushes it onto the call stack.
  - This process ensures that asynchronous tasks are executed only after the current synchronous code completes.

## What are the differences between variables created using `let`, `var` or `const`?

Can you change a property of an object defined via const? How you can change this behavior?

## Others

Examples:

- What is the difference between synchronous and asynchronous functions?
- Can you offer a use case for the new arrow => function syntax? How does this new syntax differ from other functions?
- What is the definition of a higher-order function?
- What is a promise? Where and how would you use promise?
