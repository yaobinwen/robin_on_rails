!function(){"use strict";var a=window.document,b={STYLES:"https://c.disquscdn.com/next/embed/styles/lounge.a6306b99a258593b69a6bcf61dc83760.css",RTL_STYLES:"https://c.disquscdn.com/next/embed/styles/lounge_rtl.988da989767c5626a1f9c610e0f70551.css","lounge/main":"https://c.disquscdn.com/next/embed/lounge.bundle.3c359b914364cd453831c854d654c1f8.js","remote/config":"https://disqus.com/next/config.js","common/vendor_extensions/highlight":"https://c.disquscdn.com/next/embed/highlight.6fbf348532f299e045c254c49c4dbedf.js"};window.require={baseUrl:"https://c.disquscdn.com/next/current/embed/embed",paths:["lounge/main","remote/config","common/vendor_extensions/highlight"].reduce(function(a,c){return a[c]=b[c].slice(0,-3),a},{})};var c=a.createElement("script");c.onload=function(){require(["common/main"],function(a){a.init("lounge",b)})},c.src="https://c.disquscdn.com/next/embed/common.bundle.c817aead0064c1fa134fcaa6055ca127.js",a.body.appendChild(c)}();