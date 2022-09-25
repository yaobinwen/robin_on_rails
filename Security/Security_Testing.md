# Security Testing

## What to Test

This section largely refers to [1].

### Access to Application

What to test:

- 1). Authentication: "who you are"
- 2). Authorization: "what you can do"

### Data Protection

What to test:

- 1). "a user can view or utilize only the data which he/she is supposed to use" [1]. This is partly related with the access testing.
- 2). "how that data is stored in the DB": "All the sensitive data must be encrypted to make it secure." [1]
- 3). If needed, data transmission must be protected properly (such as using `https`).

- [What Is Database Security Testing – Complete Guide](https://www.softwaretestinghelp.com/database-security-testing/)

### Brute-Force Attack

What to test: Make sure the app is immune from brute-force attach. For example, if a hacker tries to log in repeatedly, the app should be able to detect this behavior and act accordingly (such as suspend the account for some time).

- [Testing for Brute Force (OWASP-AT-004)](https://wiki.owasp.org/index.php/Testing_for_Brute_Force_(OWASP-AT-004))

### SQL Injection And XSS (Cross-Site Scripting)

- [SQL Injection Testing Tutorial (Example and Prevention of SQL Injection Attack)](https://www.softwaretestinghelp.com/sql-injection-how-to-test-application-for-sql-injection-attacks/)
- [SQL Injection](https://owasp.org/www-community/attacks/SQL_Injection)

### Service Access Points (Sealed And Secure Open)

What to test: The public service access points must be "accommodating enough to fulfill all users’ requests and secure enough to cope with any security-trial". [1]

### Session Management

What to test: Make sure the sessions expire in a reasonable period.

### Error handling

???

### Specific Risky Functionalities

- payments
- file uploads

## What to Test (Else)

### OWASP Top Ten (2017)

[Top 10 Web Application Security Risks (2017)](https://owasp.org/www-project-top-ten/)([Chinese version](https://wiki.owasp.org/images/d/dc/OWASP_Top_10_2017_%E4%B8%AD%E6%96%87%E7%89%88v1.3.pdf))

> 1. **Injection**. Injection flaws, such as SQL, NoSQL, OS, and LDAP injection, occur when untrusted data is sent to an interpreter as part of a command or query. The attacker’s hostile data can trick the interpreter into executing unintended commands or accessing data without proper authorization.
> 2. **Broken Authentication**. Application functions related to authentication and session management are often implemented incorrectly, allowing attackers to compromise passwords, keys, or session tokens, or to exploit other implementation flaws to assume other users’ identities temporarily or permanently.
> 3. **Sensitive Data Exposure**. Many web applications and APIs do not properly protect sensitive data, such as financial, healthcare, and PII. Attackers may steal or modify such weakly protected data to conduct credit card fraud, identity theft, or other crimes. Sensitive data may be compromised without extra protection, such as encryption at rest or in transit, and requires special precautions when exchanged with the browser.
> 4. **XML External Entities (XXE)**. Many older or poorly configured XML processors evaluate external entity references within XML documents. External entities can be used to disclose internal files using the file URI handler, internal file shares, internal port scanning, remote code execution, and denial of service attacks.
> 5. **Broken Access Control**. Restrictions on what authenticated users are allowed to do are often not properly enforced. Attackers can exploit these flaws to access unauthorized functionality and/or data, such as access other users’ accounts, view sensitive files, modify other users’ data, change access rights, etc.
> 6. **Security Misconfiguration**. Security misconfiguration is the most commonly seen issue. This is commonly a result of insecure default configurations, incomplete or ad hoc configurations, open cloud storage, misconfigured HTTP headers, and verbose error messages containing sensitive information. Not only must all operating systems, frameworks, libraries, and applications be securely configured, but they must be patched/upgraded in a timely fashion.
> 7. **Cross-Site Scripting (XSS)**. XSS flaws occur whenever an application includes untrusted data in a new web page without proper validation or escaping, or updates an existing web page with user-supplied data using a browser API that can create HTML or JavaScript. XSS allows attackers to execute scripts in the victim’s browser which can hijack user sessions, deface web sites, or redirect the user to malicious sites.
> 8. **Insecure Deserialization**. Insecure deserialization often leads to remote code execution. Even if deserialization flaws do not result in remote code execution, they can be used to perform attacks, including replay attacks, injection attacks, and privilege escalation attacks.
> 9. **Using Components with Known Vulnerabilities**. Components, such as libraries, frameworks, and other software modules, run with the same privileges as the application. If a vulnerable component is exploited, such an attack can facilitate serious data loss or server takeover. Applications and APIs using components with known vulnerabilities may undermine application defenses and enable various attacks and impacts.
> 10. **Insufficient Logging & Monitoring**. Insufficient logging and monitoring, coupled with missing or ineffective integration with incident response, allows attackers to further attack systems, maintain persistence, pivot to more systems, and tamper, extract, or destroy data. Most breach studies show time to detect a breach is over 200 days, typically detected by external parties rather than internal processes or monitoring.

## References

- [1] [Security Testing (A Complete Guide)](https://www.softwaretestinghelp.com/how-to-test-application-security-web-and-desktop-application-security-testing-techniques/)
