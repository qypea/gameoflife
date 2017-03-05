

CXXFLAGS = -I /usr/X11/include -L /usr/X11/lib
LDLIBS = -lX11

TARGETS=x1

all: $(TARGETS)

x1: x1.cpp

clean:
	rm -f $(TARGETS)
