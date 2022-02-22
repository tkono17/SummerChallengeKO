#!/usr/bin/env perl
$latex            = "uplatex -synctex=1 -interaction=nonstopmode -shell-escape -halt-on-error";
$latex_silent     = "uplatex -synctex=1 -interaction=batchmode -shell-escape -halt-on-error";
$lualatex         = "lualatex -synctex=1 -interaction=nonstopmode -shell-escape -halt-on-error";
$xelatex          = "xelatex -synctex=1 -interaction=nonstopmode -shell-escape -halt-on-error";
$bibtex           = "upbibtex";
$biber            = "biber --bblencoding=utf8 -u -U --output_safechars";
$dvipdf           = "dvipdfmx %O -o %D %S";
$makeindex        = "upmendex %O -o %D %S";
$max_repeat       = 5;
$pdf_previewer    = ":"; # do nothing
$pvc_view_file_via_temporary = 0;