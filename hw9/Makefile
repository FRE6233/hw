MD   = $(wildcard [a-z]*.md) # do not publish _* files
PDF  = $(MD:.md=.pdf)
HTML = $(MD:.md=.html)
DOCX = $(MD:.md=.docx)
FILES = $(MD:.md=)

HFLAGS = -s -t html5
HFLAGS += -M classoption=fleqn
HFLAGS += --css=math.css
HFLAGS += --katex

%.html: %.md $(CSS)
	pandoc $(HFLAGS) $< -o $@

%.pdf: %.md 
	pandoc $< -o $@

all: $(HTML) $(PDF)

clean:
	rm -f $(HTML) $(PDF)

