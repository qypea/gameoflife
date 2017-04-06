#include <X11/Xlib.h> // preceede most other headers!
#include <stdio.h>

int main() {
   Display *dsp = XOpenDisplay(NULL);
   if(!dsp){
      return 1;
   }

   int screenNumber = DefaultScreen(dsp);
   unsigned long white = WhitePixel(dsp, screenNumber);
   unsigned long black = BlackPixel(dsp, screenNumber);

   unsigned long width = 200;
   unsigned long height = 200;

   Window win = XCreateSimpleWindow(dsp,
                                    DefaultRootWindow(dsp),
                                    50, 50,   // origin
                                    width, height, // size
                                    0, black, // border
                                    white);  // backgd
   XMapWindow(dsp, win);

   int x11_fd = ConnectionNumber(dsp);
   fd_set in_fds;

   struct timeval tick_timer;
   struct timeval fps;
   fps.tv_usec = 1000000 / 30;
   fps.tv_sec = 0;
   tick_timer = fps;

   long eventMask = StructureNotifyMask | PropertyChangeMask
                     | KeyPressMask;
   XSelectInput(dsp, win, eventMask);

   XEvent evt;
   while (1) {
      FD_ZERO(&in_fds);
      FD_SET(x11_fd, &in_fds);

      int stat = select(x11_fd + 1, &in_fds, NULL, NULL, &tick_timer);
      // Note: If input driven then remaining time is in tick_timer
      bool redraw = false;

      // Deal with events
      if (XPending(dsp))
      {
         XNextEvent(dsp, &evt);   // calls XFlush()
         if (evt.type == KeyPress)
         {
            fprintf(stderr, "exiting\n");
            break;
         }

         if (evt.type == ConfigureNotify)
         {
            fprintf(stderr, "configured\n");
            width = evt.xconfigure.width;
            height = evt.xconfigure.height;
            redraw = true;
         }

         if (evt.type == MapNotify)
         {
            fprintf(stderr, "mapped\n");
            redraw = true;
         }
      }

      if (stat == 0 || redraw)
      {
         tick_timer = fps; // reset fps timer

         GC gc = XCreateGC(dsp, win,
                           0,        // mask of values
                           NULL);    // array of values

         XSetForeground(dsp, gc, white);
         XFillRectangle(dsp, win, gc, 0, 0, width, height);
         XSetForeground(dsp, gc, black);
         XDrawLine(dsp, win, gc, 10, 10, width - 10, height - 10); //from-to
         XDrawLine(dsp, win, gc, 10, height - 10, width - 10, 10); //from-to
      }
  }

   XDestroyWindow(dsp, win);
   XCloseDisplay(dsp);

   return 0;
}
