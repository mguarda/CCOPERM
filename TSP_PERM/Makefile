IDIR =../BASE
DIR =.
CC=g++
CFLAGS=-I$(IDIR) -I.

ODIR=obj

_OBJ = main.o rand31pmc.o toolbox.o colony.o plant.o problem.o solution.o SolutionPerm.o cco.o Tsp.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp
	$(CC) -O0 -g3 -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(IDIR)/%.cpp
	$(CC) -O0 -g3 -Wall -c -o $@ $< $(CFLAGS)

EJECUTABLE_TSP_PERM: $(OBJ)
	g++ -o ../$@ $^ $(CFLAGS) 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

