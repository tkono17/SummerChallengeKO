LATEX=uplatex
DVIPDF=xdvipdfmx
NAME=SummerChallenge2019
#TEXOPT = -output-format=pdf -kanji=utf8 -interaction=nonstopmode --src-specials
TEXOPT=

.PHONY: all clean

all: $(NAME).pdf

clean:
	@rm -fr $(NAME).pdf *.dvi *.aux *.toc *.log *.out

$(NAME).dvi: $(NAME).tex
	$(LATEX) $<
	$(LATEX) $<
	$(LATEX) $<

$(NAME).pdf: $(NAME).dvi
	$(DVIPDF) $<

