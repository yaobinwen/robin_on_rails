# JavaScript

## References

I found some very good JavaScript references:

- [Modern JS Cheatsheet](https://mbeaudru.github.io/modern-js-cheatsheet/)
- [ECMAScript 6 — New Features: Overview & Comparison](http://es6-features.org/)
- [Node.js ECMAScript compatibility tables](https://node.green/)
- [JavaScript Garden](http://bonsaiden.github.io/JavaScript-Garden): A Guide to JavaScript's Quirks and Flaws

## Anonymous Function

An example of anonymous function is [here](https://gist.github.com/iam-peekay/e29fa76b94ff56163446#file-anonymousclosurewithglobal-js), which is used in the article [Mod-001]:

```javascript
var global = 'Hello, I am a global variable :)';

(function () {
  // We keep these variables private inside this closure scope

  var myGrades = [93, 95, 88, 0, 55, 91];

  var average = function() {
    var total = myGrades.reduce(function(accumulator, item) {
      return accumulator + item}, 0);

    return 'Your average grade is ' + total / myGrades.length + '.';
  }

  var failing = function(){
    var failingGrades = myGrades.filter(function(item) {
      return item < 70;});

    return 'You failed ' + failingGrades.length + ' times.';
  }

  console.log(failing());
  console.log(global);
}());

// 'You failed 2 times.'
// 'Hello, I am a global variable :)'
```

As [Mod-001] says:

> With this construct, our anonymous function has its own evaluation environment or “closure”, and then we immediately evaluate it. This lets us hide variables from the parent (global) namespace.

[Mod-001] reminds us to pay attention to the surrounding parentheses as they are required. Read [this post](https://stackoverflow.com/questions/1634268/explain-the-encapsulated-anonymous-function-syntax) for more details.

## Bind

- [Bind-001] [Understanding JavaScript Bind](https://www.smashingmagazine.com/2014/01/understanding-javascript-function-prototype-bind/)

## Code Style

Look at [JavaScript Standard Style](https://standardjs.com/), a "JavaScript style guide, linter, and formatter".

## Module

- [Mod-001] [JavaScript Modules: A Beginner’s Guide](https://medium.freecodecamp.org/javascript-modules-a-beginner-s-guide-783f7d7a5fcc)
  - This article introduces how to implement the **"module pattern"** in JavaScript and it provides very good examples.
- [Mod-002] [JavaScript Modules Part 2: Module Bundling](https://medium.freecodecamp.org/javascript-modules-part-2-module-bundling-5020383cf306)

## Testing

- [An Overview of JavaScript Testing in 2018](https://medium.com/welldone-software/an-overview-of-javascript-testing-in-2018-f68950900bc3)

## Types

This method is the right way to test an object's type in Javascript:

```javascript
function objectIsType(obj, type) {
    var class = Object.prototype.toString.call(obj).slice(8, -1);
    return obj !== undefined && obj !== null && class === type;
}
```
