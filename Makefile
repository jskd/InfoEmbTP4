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

clean:
	(cd tempsExecution; make clean)
	(cd tempsContext; make clean)
	(cd impliciteFIFO; make clean)
	(cd expliciteFIFO; make clean)

tar: clean
	tar -zcvf LefrancSkodaTP4.tar.gz *
