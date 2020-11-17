# Cross-Domain Security

## Same-Origin Policy (SOP)

The "same-origin policy", as its name suggests, means information access (read/write; send/receive) should be restricted on the same origin. However, in reality, this is not always practical because the "web" only exists when cross-site hyperlinks are allowed. If accessing cross-site links were disallowed, the "web" would not exist anymore.

The **general principle** is: "documents retrieved from distinct origins are isolated from each other". [1]

> For example, if a document retrieved from http://example.com/doc.html tries to access the DOM of a document retrieved from https://example.com/target.html, the user agent will disallow access because the origin of the first document, (http, example.com, 80), does not match the origin of the second document. [1]

The **purpose**: "let users visit untrusted web sites without those web sites interfering with the user's session with honest web sites". [1]

Therefore, broadly speaking: [1]

- "There is no single same-origin policy." [1] Instead, SOP has different implications under different circumstances.
- Sending (writing) information to another origin is allowed.
  - However, technically speaking, this exposes the risk of `cross-site request forgery (CSRF)` (see below) and `clickjacking` (see below).
- Receiving (reading) information from another origin is not allowed so that:
  - Malicious sites won't be able to send a request to a trusted site and receive confidential information.
  - A trusted site may be lured to send a request to a malicious site. SOP guarantees that the trusted site doesn't process the content that is returned by the malicious site.

Both [2] and [3] provide more details but I like the list in [3]:

- `iframes`
  - **Embedding** a cross-site origin via `src` attribute is **usually allowed**, depending on `X-Frame-Options` (see below).
  - Using JavaScript to access a document embedded in an `iframe` from another origin is **disallowed** (because this is a case of reading).
- CSS
  - Cross-origin CSS can be embedded using a <link> element or an @import in a CSS file.
  - The correct `Content-Type` header may be required.
- forms
  - Submitting data to a cross-site origin is **allowed**.
  - The submission is done via `action` attribute.
- images
  - Embedding cross-origin images is **allowed**.
  - Reading cross-origin images is **disallowed** (e.g., loading a cross-origin image into a `canvas` element using JavaScript).
- multimedia
  - Cross-origin video and audio can be embedded using `<video>` and `<audio>` elements.
- script
  - Cross-origin scripts can be embedded.
  - Access to certain APIs (such as cross-origin `fetch` requests) might be **disallowed**.

## Clickjacking

According to [4]:

> Clickjacking, also known as a "UI redress attack", is when an attacker uses multiple transparent or opaque layers to trick a user into clicking on a button or link on another page when they were intending to click on the top level page. Thus, the attacker is "hijacking" clicks meant for their page and routing them to another page, most likely owned by another application, domain, or both.

> Using a similar technique, keystrokes can also be hijacked. With a carefully crafted combination of stylesheets, iframes, and text boxes, a user can be led to believe they are typing in the password to their email or bank account, but are instead typing into an invisible frame controlled by the attacker.

> For example, imagine an attacker who builds a web site that has a button on it that says "click here for a free iPod". However, on top of that web page, the attacker has loaded an iframe with your mail account, and lined up exactly the "delete all messages" button directly on top of the "free iPod" button. The victim tries to click on the "free iPod" button but instead actually clicked on the invisible "delete all messages" button. In essence, the attacker has "hijacked" the user's click, hence the name "Clickjacking".

## `X-Frame-Option` Header

According to [5]:

> The X-Frame-Options HTTP response header can be used to indicate whether or not a browser should be allowed to render a page in a `<frame>`, `<iframe>`, `<embed>` or `<object>`. Sites can use this to avoid click-jacking attacks, by ensuring that their content is not embedded into other sites.

> The added security is provided only if the user accessing the document is using a browser that supports X-Frame-Options.

## TODO

- [ ] CSRF
- [ ] Content Security Policy (CSP)

## References

- [1] [W3C: Same Origin Policy](https://www.w3.org/Security/wiki/Same_Origin_Policy)
- [2] [MDN: Same-origin policy](https://developer.mozilla.org/en-US/docs/Web/Security/Same-origin_policy)
- [3] [Same-origin policy](https://web.dev/same-origin-policy/)
- [4] [OWASP: Clickjacking](https://owasp.org/www-community/attacks/Clickjacking)
