COMP=c++ 
ROOTLIBS = `root-config --glibs --cflags` -lMinuit 
INC= -I.. -I. -I./include  -I${CLHEP}/include 
ROOTINC= -I${ROOTSYS}/include
LIBS= -L.  ${ROOTLIBS} -L${CLHEP}/lib -L${CLHEP}/lib 
SELECTIONLIB = Selection.o 
EXE = main_finalSelection main_ele_isolation_ID

# ********** TEMPLATE *************
# mainProg: mainProg.o $(SELECTIONLIB)
#	$(COMP) $(INC) $(ROOTINC) $(LIBS) $(ROOTLIBS) -o $@  $(SELECTIONLIB) $@.o
# *********************************

all: ${EXE}

main_finalSelection: main_finalSelection.o $(SELECTIONLIB)
	$(COMP) $(INC) $(ROOTINC) $(LIBS) -o $@  $(SELECTIONLIB) $@.o

main_ele_isolation_ID: main_ele_isolation_ID.o $(SELECTIONLIB)
	$(COMP) $(INC) $(ROOTINC) $(LIBS) -o $@  $(SELECTIONLIB) $@.o

clean:
	rm -f *.o *.lo core core.*
	rm -f *~	
	rm -f *.exe
	rm -f $(EXE)
.cpp.o:
	$(COMP) -c $(INC) $(ROOTINC) -o $@ $<

.cc.o:
	$(COMP) -m32 -c $(INC) $(ROOTINC) -o $@ $<

.cxx.o:
	$(COMP) -c $(INC) $(ROOTINC) -o $@ $<

.C.o:
	$(COMP) -c $(INC) $(ROOTINC) -o $@ $<


