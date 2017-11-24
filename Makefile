CC = gcc

BIN_DIR = bin/
SRC_DIR = src/

pdf:
	pdflatex lefranc_skoda_tp4.tex -o lefranc_skoda_tp4.pdf
	rm *.aux *.log

bench-exp-thread:
	(cd expliciteFIFO; make bench-thread)

bench-exp-processus:
	(cd expliciteFIFO; make bench-processus)

bench-imp-thread:
	(cd impliciteFIFO; make bench-thread)

bench-imp-processus:
	(cd impliciteFIFO; make bench-processus)

bench-cont-thread:
	(cd tempsContext; make bench-thread)

bench-cont-processus:
	(cd tempsContext; make bench-processus)

bench-exec-thread:
	(cd tempsExecution; make bench-thread)

bench-exec-processus:
	(cd tempsExecution; make bench-processus)


#tar: clean
#	tar -zcvf PooCAv_Project_G7.tar.gz .gitignore Makefile LICENSE projetv1.pdf README.md UML_v1.pdf deliverables/ bin/ src/
