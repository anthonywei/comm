INCLUDE =-I./  -I../../include/net/ \
		 -I../../include/time/ \
		 -I../../include/serialize 


CFLAGS = -fPIC -std=c++0x -Wno-deprecated

OUTPUT  = libnetbase.a

OBJS    = \
		net_svr_reactor.o  svr_reactor_facade.o  tcp_handle_base.o\
		epoll_svr_reactor.o svr_reactor_facade_epoll.o
		  
	
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
	cp -fv libnetbase.a	../../libs

ifneq ($(wildcard .depend),)
	include .depend
endif
