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


  long eventMask = StructureNotifyMask|PropertyChangeMask;
  XSelectInput( dsp, win, eventMask );

  XEvent evt;
  do{
   if (evt.type == MapNotify || evt.type == ConfigureNotify) {
     GC gc = XCreateGC( dsp, win,
                        0,        // mask of values
                        NULL );   // array of values
     XSetForeground( dsp, gc, black );

     if (evt.type == ConfigureNotify) {
       width = evt.xconfigure.width;
       height = evt.xconfigure.height;
     }

     XDrawLine(dsp, win, gc, 10, 10, width - 10, height - 10); //from-to
     XDrawLine(dsp, win, gc, 10,height - 10,width - 10, 10); //from-to

   }

    XNextEvent( dsp, &evt );   // calls XFlush()
  }while( evt.type != DestroyNotify );

  XDestroyWindow( dsp, win );
  XCloseDisplay( dsp );

  return 0;
}
