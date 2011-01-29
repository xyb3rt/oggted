all: oggted

CXX?=g++
PREFIX?=/usr/local
CXXFLAGS+= -I$(PREFIX)/include -Wall -pedantic
LDFLAGS+= -L$(PREFIX)/lib
LIBS+= -ltag

CPPFILES=$(wildcard *.cpp)
OBJFILES=$(CPPFILES:.cpp=.o)

oggted:	$(OBJFILES)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -c -o $@ $<

install: all
	install -D -m 0755 oggted $(PREFIX)/bin/oggted

clean:
	rm -f oggted *.o

tags: *.h *.cpp
	ctags $^

cscope: *.h *.cpp
	cscope -b
