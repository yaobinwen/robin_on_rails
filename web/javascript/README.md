# JavaScript

## References

I found some very good JavaScript references:

- [MDN JavaScript](https://developer.mozilla.org/en-US/docs/Web/JavaScript)
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

## Concurrency

- [Concurrency model and Event Loop](https://developer.mozilla.org/en-US/docs/Web/JavaScript/EventLoop)
- [The Node.js Event Loop, Timers, and process.nextTick()](https://nodejs.org/en/docs/guides/event-loop-timers-and-nexttick/)
- [What you should know to really understand the Node.js Event Loop](https://medium.com/the-node-js-collection/what-you-should-know-to-really-understand-the-node-js-event-loop-and-its-metrics-c4907b19da4c)
- [Understanding Javascript Function Executions — Call Stack, Event Loop , Tasks & more](https://medium.com/@gaurav.pandvia/understanding-javascript-function-executions-tasks-event-loop-call-stack-more-part-1-5683dea1f5ec)
- [The JavaScript Event Loop: Explained](https://blog.carbonfive.com/2013/10/27/the-javascript-event-loop-explained/)
- [Handling IO — NodeJS Event Loop Part 4](https://jsblog.insiderattack.net/handling-io-nodejs-event-loop-part-4-418062f917d1)
- [Why setImmediate() execute before fs.readFile() in Nodejs Event Loop's works?](https://stackoverflow.com/a/47727402/630364): This is an awesome answer but may have mistakes. See Rueian Oneecom's comments.
- [Philip Roberts: Help, I’m stuck in an event-loop.](https://vimeo.com/96425312)

## Module

- [Mod-001] [JavaScript Modules: A Beginner’s Guide](https://medium.freecodecamp.org/javascript-modules-a-beginner-s-guide-783f7d7a5fcc)
  - This article introduces how to implement the **"module pattern"** in JavaScript and it provides very good examples.
- [Mod-002] [JavaScript Modules Part 2: Module Bundling](https://medium.freecodecamp.org/javascript-modules-part-2-module-bundling-5020383cf306)

## Testing

- [An Overview of JavaScript Testing in 2018](https://medium.com/welldone-software/an-overview-of-javascript-testing-in-2018-f68950900bc3)

## Types

This method is the right way to test an object's type in Javascript (from `JavaScript Garden`):

```javascript
function is(type, obj) {
    var clas = Object.prototype.toString.call(obj).slice(8, -1);
    return obj !== undefined && obj !== null && clas === type;
}
```

## Versioning

As a novice JavaScript developer, I myself am constantly confused by the JavaScript (or ECMAScript) versioning. Fortunately, some good articles can help clarify:

- [JavaScript Versions](https://www.w3schools.com/js/js_versions.asp)
- [ES6 (ES2015) And Beyond: Understanding JavaScript Versioning](https://www.sitepoint.com/javascript-versioning-es6-es2015/)
- [JavaScript — WTF is ES6, ES8, ES 2017, ECMAScript… ?](https://codeburst.io/javascript-wtf-is-es6-es8-es-2017-ecmascript-dca859e4821c)
- [ES6 vs ES2015 - What to call a JavaScript version?](https://bytearcher.com/articles/es6-vs-es2015-name/)
- [ES5, ES6, ES2016, ES.Next: What's going on with JavaScript versioning?](https://benmccormick.org/2015/09/14/es5-es6-es2016-es-next-whats-going-on-with-javascript-versioning/?utm_medium=email)
