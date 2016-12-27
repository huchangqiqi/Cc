(TeX-add-style-hook
 "hello"
 (lambda ()
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art10")
   (LaTeX-add-labels
    "sec:introduction"
    "sec:top-matter"
    "sec:article-information"))
 :latex)

