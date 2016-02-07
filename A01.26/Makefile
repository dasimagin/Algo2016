all: \
	o-notation-exercices \
	o-notation-tasks \
	o-notation-solutions \
	clean \

o-notation-exercices: o-notation-exercices.tex
	pdflatex o-notation-exercices.tex
	# open o-notation-exercices.pdf

o-notation-tasks: o-notation-tasks.tex
	pdflatex o-notation-tasks.tex
	# open o-notation-tasks.pdf

o-notation-solutions: o-notation-solutions.tex
	pdflatex o-notation-solutions.tex
	# open o-notation-solutions.pdf

clean:
	rm *.{log,aux}
