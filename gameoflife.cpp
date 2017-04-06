#include <X11/Xlib.h> // preceede most other headers!

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

   long eventMask = StructureNotifyMask | PropertyChangeMask;
   XSelectInput(dsp, win, eventMask);

   XEvent evt;
   while (1) {
      if (evt.type == MapNotify || evt.type == ConfigureNotify)
      {
         GC gc = XCreateGC(dsp, win,
                           0,        // mask of values
                           NULL);   // array of values

         if (evt.type == ConfigureNotify)
         {
            width = evt.xconfigure.width;
            height = evt.xconfigure.height;
         }

         XSetBackground(dsp, gc, white);
         XSetForeground(dsp, gc, black);
         XDrawLine(dsp, win, gc, 10, 10, width - 10, height - 10); //from-to
         XDrawLine(dsp, win, gc, 10, height - 10, width - 10, 10); //from-to
      }

      FD_ZERO(&in_fds);
      FD_SET(x11_fd, &in_fds);

      // TODO: Set timer such that we're capped at 30fps, not slower than 30fps
      tick_timer.tv_usec = 1000000 / 30;
      tick_timer.tv_sec = 0;

      // TODO: Check response code maybe?
      select(x11_fd + 1, &in_fds, NULL, NULL, &tick_timer);

      if (XPending(dsp))
      {
         XNextEvent(dsp, &evt);   // calls XFlush()
         if (evt.type == DestroyNotify)
         {
            break;
         }
      }
   }

   XDestroyWindow(dsp, win);
   XCloseDisplay(dsp);

   return 0;
}
