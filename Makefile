#https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html

SRC=$(PWD)/src
INCLUDE=$(PWD)/include
BUILDDIR=$(PWD)/build
OUTDIR=$(BUILDDIR)/bin
TEMPDIR=$(BUILDDIR)/tmp

BinarySerchTree: bst.o main.o
	g++ main.o bst.o -o BinarySerchTree

bst.o: BST.cpp
	g++ -std=c++11 -c $^ -o $@

main.o: main.cpp
	g++ -std=c++11 -c $^ -o $@



.PHONY: clean run

run:
	make
	./BinarySerchTree
	
clean:
	rm BinarySerchTree main.o bst.o

#Creazione cartelle se non esistono

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(TEMPDIR):
	mkdir -p $(TEMPDIR)
	
