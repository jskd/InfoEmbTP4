CC = gcc

BIN_DIR = bin/
SRC_DIR = src/

pdf:
	pdflatex lefranc_skoda_tp4.tex -o lefranc_skoda_tp4.pdf 
	rm *.aux *.log

#tar: clean
#	tar -zcvf PooCAv_Project_G7.tar.gz .gitignore Makefile LICENSE projetv1.pdf README.md UML_v1.pdf deliverables/ bin/ src/
