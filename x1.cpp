#include <X11/Xlib.h> // preceede most other headers!

int main(){

  Display *dsp = XOpenDisplay( NULL );
  if( !dsp ){ return 1; }


  int screenNumber = DefaultScreen(dsp);
  unsigned long white = WhitePixel(dsp,screenNumber);
  unsigned long black = BlackPixel(dsp,screenNumber);

  unsigned long width = 200;
  unsigned long height = 200;

  Window win = XCreateSimpleWindow(dsp,
                               DefaultRootWindow(dsp),
                               50, 50,   // origin
                               width, height, // size
                               0, black, // border
                               white );  // backgd

  XMapWindow( dsp, win );


  long eventMask = StructureNotifyMask;
  XSelectInput( dsp, win, eventMask );

  XEvent evt;
  do{
   if (evt.type == MapNotify) {
     GC gc = XCreateGC( dsp, win,
                        0,        // mask of values
                        NULL );   // array of values
     XSetForeground( dsp, gc, black );


     XDrawLine(dsp, win, gc, 10, 10, width - 10, height - 10); //from-to
     XDrawLine(dsp, win, gc, 10,height - 10,width - 10, 10); //from-to


     eventMask = ButtonPressMask|ButtonReleaseMask;
     XSelectInput(dsp,win,eventMask); // override prev
   }

    XNextEvent( dsp, &evt );   // calls XFlush()
  }while( evt.type != ButtonRelease );

  XDestroyWindow( dsp, win );
  XCloseDisplay( dsp );

  return 0;
}
