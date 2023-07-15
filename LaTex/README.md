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
- [pgf – A Portable Graphic Format for TeX](https://github.com/pgf-tikz/pgf)

## Manually install a package

1. Download the package from [CTAN](https://ctan.org/). It's usually a `.zip` file.
2. Extract the files from the `.zip` file.
3. If the package doesn't contain the `.sty` file (usually only has the `.ins` and `.dtx` files), then generate the `.sty` file:
  - `latex <filename>.ins`
  - `latex <filename>.dtx`
4. Create the folder `/usr/local/share/texmf/tex/latex`.
5. Put the package folder (which should contain the `.sty` file now) into `/usr/local/share/texmf/tex/latex` (so the full path is `/usr/local/share/texmf/tex/latex/<pkg-folder>/<filename>.sty`).
6. `cd /usr/local/share/texmf` and run `sudo mktexlsr` to update the `ls-R` database.
  - This is needed because "`/usr/local/share/texmf/tex/` is not searched recursively by `kpathsea` (cf. `kpsepath tex`), the `ls-R` file at the root directory of this search path must be updated to make the system aware of the new package." ([ref](https://tex.stackexchange.com/a/39165/66282))

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
