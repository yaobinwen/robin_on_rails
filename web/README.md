# Web

## Overview

This document contains the notes taken from learning web development.

## Front-end

### Prologue

I must write a prologue for the front-end development because of its complexity. There are so many technologies, hence terminologies, that people keep talking about but are confusing to the novice developers. Therefore, laying a solid foundation before diving into the what-I-call **`front-end technology hell`** is important as it gives you a clear mind so you won't lose it.

Please excuse me for making mistakes because I'm also learning everything from scratch. I'll keep updating this document to add more information or correct mistakes.

Alrighty. Let's begin.

According to [Wikipedia: Front-end development](https://en.wikipedia.org/wiki/Front-end_web_development), the front-end development deals with four parts:

- `Hyper Text Markup Language(HTML)`: Describe the content and the arrangement of a web page.
- `Cascading Style Sheets (CSS)`: Describe the visual effect of the web page content defined by HTML.
- `JavaScript`: Implemnet the user interaction of the web page with the user so the web page content, arrange, and visual effect can be dynamically changed as the user interacts with it.
- `WebAssembly`: I didn't know this technology until I read the Wikipedia page. I know nothing about it so can't provide a reasonable summary of my own. Please read the Wikipedia page for more details. I put it here just to make things complete.

Personally (aka "I could be wrong"), I think HTML and CSS are easy to learn. They are just a fixed set of tags that you can look up in the standards ([HTML](https://www.w3.org/standards/techs/html#completed) and [CSS](https://www.w3.org/standards/techs/css#completed)). The function of each tag is clearly defined. Your task is to learn the existence of a tag that can meet your need.

JavaScript is the most interesting one.

I'm not a historian. Neither do I want to include too many historical details here. But initially JavaScript code was mostly embedded in the web pages shown to the users and was executed by the browser to add dynamic interaction with the users. This is called the **"client-side JavaScript"**. In the early days, JavaScript was mostly run on the browser (aka the client) side by the 
JavaScript engine in the browser.

[Chrome V8](https://developers.google.com/v8/) is a JavaScript engine developed for Google Chrome browser, but it's also used by [Node.js](https://nodejs.org/en/) to execute JavaScript on the server-side, as said in its official website:

> Node.js® is a JavaScript runtime built on Chrome's V8 JavaScript engine. Node.js uses an event-driven, non-blocking I/O model that makes it lightweight and efficient. Node.js' package ecosystem, [npm](https://www.npmjs.com/), is the largest ecosystem of open source libraries in the world.

Thus start our front-end development journey along with Node.js and npm.

(Note that Chrome V8 is surely not the only JavaScript engine but [one of many](https://en.wikipedia.org/wiki/JavaScript_engine#JavaScript_engines); Node.js is surely not the only JavaScript runtime for web development either. However, because I am currently working with technologies so I'll focus on them for now, but I don't mean any offense to those who use other technologies.)

### Perspectives

I suggest the following perspectives to look at and understand the entire web development ecosystem:

- **Technical:** Include the programming languages, libraries and modules, specifications, etc..
- **Software Engineering:** Include the engineering tools for the best practices such as the linters and CI tools.

## Tools & Libraries

- [Express](http://expressjs.com/): Fast, unopinionated, minimalist web framework for Node.js.
- [Grunt](https://gruntjs.com/): The JavaScript task runner for automation.

## Terms & Concepts

- [HTTP cookies](https://developer.mozilla.org/en-US/docs/Web/HTTP/Cookies)
- [MIME types](https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types)
- [Polyfill](https://en.wikipedia.org/wiki/Polyfill_(programming))
- [Ponyfill](https://github.com/sindresorhus/ponyfill)
  - [Polyfill, Ponyfill & Prollyfill](https://kikobeats.com/polyfill-ponyfill-and-prollyfill/)
- [RESTful](https://en.wikipedia.org/wiki/Representational_state_transfer)

## Aspects to Take Care

When working on a web application, one must take care of the following aspects to develop an application of high quality:

- Availability
- Backward compatibility: What if a client of newer version is communicatinng to an older version of server? Is this allowed? Consider versioning.
- Operation
- Performance
  - Caching
  - Load balancing
- Scalability
- Security
- User experience

(More to be added)

Here are some links for this topic:
- [ ] [5 Challenges need to be overcome in Web Application Development](https://medium.theuxblog.com/5-challenges-need-to-be-overcome-in-web-application-development-c03a67726ff1)
- [ ] [Considerations for Designing Distributed Systems](https://msdn.microsoft.com/en-us/library/dd129908.aspx)
- [ ] [A Thorough Introduction to Distributed Systems](https://hackernoon.com/a-thorough-introduction-to-distributed-systems-3b91562c9b3c)

## Misc.

- [How to Become a Great Front-End Engineer](https://philipwalton.com/articles/how-to-become-a-great-front-end-engineer/)
- [Fun stuff: The Super Tiny Compiler](https://github.com/jamiebuilds/the-super-tiny-compiler)
- [Brython](https://brython.info/): A Python 3 implementation for client-side web programming, designed to replace Javascript as the scripting language for the Web.

## References

- [frontend-dev-bookmarks](https://github.com/dypsilon/frontend-dev-bookmarks): "Manually curated collection of resources for frontend web developers."
- [frontend-guide](https://github.com/grab/front-end-guide): Study guide and introduction to the modern front end stack. Some other interesting articles are:
  - [A Study Plan To Cure JavaScript Fatigue](https://medium.freecodecamp.org/a-study-plan-to-cure-javascript-fatigue-8ad3a54f2eb1)
  - [How it feels to learn JavaScript in 2016](https://hackernoon.com/how-it-feels-to-learn-javascript-in-2016-d3a717dd577f)
