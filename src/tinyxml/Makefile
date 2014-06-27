INCLUDE =-I./  -I../../include/tinyxml/ \


CFLAGS = -fPIC

OUTPUT  = libtinyxml.a

OBJS    = \
		tinyxml.o tinyxmlerror.o  tinyxmlparser.o
		  
	
all:$(OUTPUT) 

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDE) -c $^ 

.o:
	$(CXX) $(CFLAGS) $(INCLUDE) -o $@ $^

$(OUTPUT): $(OBJS)
	ar r $@ $^

clean:
	rm -f *.o *.~ *.bak
	rm -f $(OUTPUT)

dep:
	mkdep $(INCLUDE) $(SOURCES) > .depend

install:
	cp -fv libtinyxml.a ../../libs

ifneq ($(wildcard .depend),)
	include .depend
endif
