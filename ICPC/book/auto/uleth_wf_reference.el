(TeX-add-style-hook "uleth_wf_reference"
 (lambda ()
    (TeX-add-symbols
     '("myimage" 2)
     '("code" 2)
     "directed"
     "undirected"
     "both")
    (TeX-run-style-hooks
     "graphicx"
     "calc"
     "lstlinebgrd"
     "setspace"
     "fancyhdr"
     "fullpage"
     "xcolor"
     "usenames"
     "dvipsnames"
     "multicol"
     "listings"
     "color"
     "enumerate"
     "amsthm"
     "amstext"
     "latexsym"
     "amsfonts"
     "amssymb"
     "pslatex"
     "amsmath"
     "latex2e"
     "art10"
     "article"
     "landscape"
     "10pt")))

