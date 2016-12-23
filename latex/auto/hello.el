(TeX-add-style-hook
 "hello"
 (lambda ()
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art10"))
 :latex)

