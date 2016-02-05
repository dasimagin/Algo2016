all: \
	test \
	clean\

test: test.tex
	pdflatex test.tex
	open test.pdf

clean:
	rm *.{log,aux}
