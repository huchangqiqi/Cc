(TeX-add-style-hook
 "t2"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("ctexart" "a4paper")))
   (TeX-run-style-hooks
    "latex2e"
    "ctexart"
    "ctexart10"
    "xeCJK"
    "fontspec"
    "xunicode"
    "xltxtra"
    "titlesec")
   (TeX-add-symbols
    "yh"))
 :latex)

