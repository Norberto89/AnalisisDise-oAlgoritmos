SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic


# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------


CPLEXDIR      = /opt/ibm/ILOG/CPLEX_Studio1263/cplex
CONCERTDIR    = /opt/ibm/ILOG/CPLEX_Studio1263/concert

# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++
CC  = gcc
JAVAC = javac

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -DNDEBUG -DIL_STD -pthread -O3 -std=c++11
COPT  = -xtarget=ultra -xarch=v9 -KPIC -pthread -O3 -std=c++11 
JOPT  = -classpath $(CPLEXDIR)/lib/cplex.jar -O

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXJARDIR   = $(CPLEXDIR)/lib/cplex.jar
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -xarch=v9 -lnsl  
CLNFLAGS  = -L$(CPLEXLIBDIR) -lcplex -lm -xarch=v9 -lnsl 
JAVA      = java -d64 -Djava.library.path=$(CPLEXDIR)/bin/ultrasparc64_9_9 -classpath $(CPLEXJARDIR):


all:
	make all_cpp

execute: all
	make execute_cpp

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

EXDIR         = /home/norberto/Escritorio/Heur_SSP2-Alea
EXSRC         = $(EXDIR)
EXINC         = $(EXDIR)
EXDATA        = $(EXDIR)

CFLAGS  = $(COPT)  -I$(CPLEXINCDIR)
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 
JCFLAGS = $(JOPT)


#------------------------------------------------------------
#  make all      : to compile the examples. 
#  make execute  : to compile and execute the examples.
#------------------------------------------------------------


CPP_EX = main

all_cpp: $(CPP_EX)

execute_cpp: $(CPP_EX)
	 ./main

# ------------------------------------------------------------

clean :
	/bin/rm -rf *.o *~ *.class
	/bin/rm -rf $(C_EX) $(CPP_EX)
	/bin/rm -rf *.mps *.ord *.sos *.lp *.sav *.net *.msg *.log *.clp

# ------------------------------------------------------------
#
# The examples
#

main: main.o SubP.o CovProb.o GrammarWTW.o Heuristic.o SSP.o SSP_Solution.o
	$(CCC) $(CCFLAGS) main.o SubP.o CovProb.o GrammarWTW.o Heuristic.o SSP.o SSP_Solution.o -o main $(CCLNFLAGS)
main.o: $(EXSRC)/main.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRC)/main.cpp -o main.o
SubP.o: $(EXSRC)/SubP.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRC)/SubP.cpp -o SubP.o
CovProb.o: $(EXSRC)/CovProb.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRC)/CovProb.cpp -o CovProb.o
GrammarWTW.o: $(EXSRC)/GrammarWTW.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRC)/GrammarWTW.cpp -o GrammarWTW.o
Heuristic.o: $(EXSRC)/Heuristic.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRC)/Heuristic.cpp -o Heuristic.o
SSP.o: $(EXSRC)/SSP.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRC)/SSP.cpp -o SSP.o
SSP_Solution.o: $(EXSRC)/SSP_Solution.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRC)/SSP_Solution.cpp -o SSP_Solution.o


