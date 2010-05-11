IFLAGS = -I.

# load library flags
LFLAGS =  
         
#GCXXFLAGS = -O3 -g0 -Wall -pipe -fPIC $(IFLAGS)
GCXXFLAGS = -O3 -g3 -Wall -pipe -fPIC  $(IFLAGS) -D_DEBUG 

all: swigs 

###############################################################################
.SUFFIXES : .o .c .cpp .cc

.cpp.o .cc.o:
	${CXX} -c ${GCXXFLAGS} ${IFLAGS} -o $@ $<
.c.o:
	${CC} -c ${CCFLAGS} ${IFLAGS} -o $@ $<
###############################################################################
install: swigs 
	python setup.py install

swigs: TST.i
	swig -python -c++ TST.i


clean:
	-rm -rf *.so
	-rm -rf *.pyc
	-rm -rf *.cxx
	-rm -rf TST.py
	-rm -rf build	
	python setup.py clean

