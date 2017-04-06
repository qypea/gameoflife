

CXXFLAGS = -I /usr/X11/include -L /usr/X11/lib
LDLIBS = -lX11

TARGETS=gameoflife

all: $(TARGETS)

gameoflife: gameoflife.cpp

clean:
	rm -f $(TARGETS)
