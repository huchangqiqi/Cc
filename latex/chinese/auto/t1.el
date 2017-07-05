(TeX-add-style-hook
 "t1"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("ctexart" "UTF8")))
   (TeX-run-style-hooks
    "latex2e"
    "ctexart"
    "ctexart10"))
 :latex)

