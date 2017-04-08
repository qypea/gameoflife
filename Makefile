

CXXFLAGS = -I /usr/X11/include -L /usr/X11/lib -Wall -Wextra -Werror -g -O3
LDLIBS = -lX11

TARGETS=gameoflife

all: $(TARGETS)

gameoflife: main.o x11_display.o fps_clock.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGETS) *.o
