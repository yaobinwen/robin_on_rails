# LaTex

## References

- The official website seems to be [The LaTex Project](https://www.latex-project.org/).
- [_Comprehensive TeX Archive Network_](https://ctan.org), or "CTAN", provides all kinds of packages for TeX.
- [LaTeX vs. MiKTeX: The levels of TeX](https://www.tug.org/levels.html).
- [PDF: LATEX Math for Undergrads](https://tug.ctan.org/info/undergradmath/undergradmath.pdf): A cheat sheet for many common math symbols.
- I really need beautiful examples of TeX documents (provided by ChatGPT 4.0):
  - [LaTeX Templates](http://www.latextemplates.com/)
    - The [kaobook template](https://www.latextemplates.com/template/kaobook) looks fascinating to me.
  - [Overleaf Gallery](https://www.overleaf.com/gallery)
  - [ShareLaTeX Templates](https://www.sharelatex.com/templates/)
  - [TeXample](http://www.texample.net/)
  - [LaTeX Cookbook](http://latex-cookbook.net/)

## ! LaTeX Error: File `filename.sty' not found

Refer to [_! LaTeX Error: File filename .sty not found_](https://github.com/jankapunkt/latexcv/wiki/!-LaTeX-Error:-File--filename-.sty-not-found). Basically, the error means the mentioned `.sty` package is missing as part of the TeXLive distribution, and we need to find the TeXLive package that includes it. The method is as follows:

- `sudo apt update`
- `sudo apt-cache search <filename>`
  - Example: `sudo apt-cache search csquotes`
- It may return some results like below:

```
texlive-latex-extra - TeX Live: LaTeX additional packages
```

- Run `sudo apt install texlive-latex-extra`
