(TeX-add-style-hook
 "spacemacs"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "11pt")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("inputenc" "utf8") ("fontenc" "T1") ("ulem" "normalem")))
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "nolinkurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperbaseurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperimage")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperref")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art11"
    "inputenc"
    "fontenc"
    "graphicx"
    "grffile"
    "longtable"
    "wrapfig"
    "rotating"
    "ulem"
    "amsmath"
    "textcomp"
    "amssymb"
    "capt-of"
    "hyperref")
   (LaTeX-add-labels
    "sec:orgbfe9ff6"
    "sec:org4e06a58"
    "sec:org7a5e2c4"
    "sec:orgac0ab14"
    "sec:orgf4c1d22"
    "sec:orgb9dbcdb"
    "sec:org6f79e7f"
    "sec:org9c69f49"
    "sec:orgbd9728f"
    "sec:org52121ba"
    "sec:org6f8728d"
    "sec:org4e49942"
    "sec:org1f48d1b"
    "sec:org531aa44"
    "sec:orga2198ff"
    "sec:orgee72d50"
    "sec:org8036ac9"
    "sec:orgc871085"
    "sec:orge772532"
    "sec:org169217b"
    "sec:org0783293"
    "sec:orgc944986"
    "sec:org014a41e"
    "sec:org3f156b7"
    "sec:org83593d9"
    "sec:org822cf99"
    "sec:orge3a09dd"
    "sec:orgf3da10d"
    "sec:org762e600"
    "sec:org8db7f35"
    "sec:org11e70d9"
    "sec:orge51446f"
    "sec:orgacd2172"))
 :latex)

