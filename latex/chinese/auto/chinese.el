(TeX-add-style-hook
 "chinese"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "12pt" "a4paper")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("geometry" "top=3cm" "bottom=3cm" "left=3cm" "right=3cm") ("titlesec" "indentafter" "pagestyles")))
   (add-to-list 'LaTeX-verbatim-environments-local "minted")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperref")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperimage")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperbaseurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "nolinkurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art12"
    "geometry"
    "graphicx"
    "latexsym"
    "pifont"
    "units"
    "amsmath"
    "amsfonts"
    "amssymb"
    "marvosym"
    "indentfirst"
    "titlesec"
    "xcolor"
    "multicol"
    "rotating"
    "soul"
    "xltxtra"
    "xeCJK"
    "hyperref"
    "fancyhdr"
    "minted")
   (TeX-add-symbols
    '("cfbox" 2)
    '("code" 1)
    '("ziju" 1))
   (LaTeX-add-xcolor-definecolors
    "currentcolor"
    "#1"))
 :latex)

