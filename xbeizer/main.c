//
//  main.c
//  xbeizer
//
//  Created by 健太 原 on 2/10/12.
//  Copyright (c) 2012 Tokyo Tech High School of Science and Technoligy. All rights reserved.
//

#include "xbeizer.h"

#define BORDER 2
#define WIDTH  500
#define HIGHT 350

XPoint dividePoints(XPoint p1, XPoint p2, float t);
short divideValue(short n1, short n2, float t);

int main(int argc, char **argv)
{
	Display *dpy;
	Window w,quit;
	Window root;
	int    screen;
	unsigned long black, white;
	GC       gc;
	XEvent   e;
	int  i;
    Cursor csr;
    
    //*********************************************
    
	dpy = XOpenDisplay("");
    
	root = DefaultRootWindow (dpy);
	screen = DefaultScreen (dpy);
	white = WhitePixel (dpy, screen);
	black = BlackPixel (dpy, screen);
    
	w = XCreateSimpleWindow(dpy, root, 100, 100, WIDTH, HIGHT, BORDER, black, white);
	/* Make Sub Window */
	quit = XCreateSimpleWindow(dpy, w, 10, 3, 30, 12, BORDER, black, white);
    
	gc = XCreateGC(dpy, w, 0, NULL);
    
    csr = XCreateFontCursor(dpy, 60);
    
    //*********************************************
    
	XSelectInput(dpy, w, ButtonPressMask);
	XSelectInput(dpy, quit, ButtonPressMask);
    
	XMapWindow(dpy, w);
	XMapSubwindows(dpy, w);
    
	XSetForeground(dpy,gc,black);
	while(1){
        /* event loop */
        XNextEvent(dpy, &e);
        switch(e.type){
            case ButtonPress : if(e.xany.window == quit) return 0;
                XDrawString(dpy,quit,gc,4,10,"Exit",4);
                XDrawLine(dpy, w, gc, 50,175,450,175);
                XDrawLine(dpy, w, gc, 50,50,50,300);
                for(i=0;i<400;i++){
                    XDrawPoint(dpy, w, gc, 50+i,175-100*sin(i/100.0));
                }
        }
	}
}



//*********************************************

XPoint dividePoints(XPoint p1, XPoint p2, float t){
    XPoint ans;
    ans.x = divideValue(p1.x, p2.x, t);
    ans.y = divideValue(p1.y, p2.y, t);
    return ans;
}

//*********************************************

short divideValue(short n1, short n2, float t){
    short n;
    if (n1 > n2) {
        n = n1 - n2;
        return n1 + n * t;
    } else if(n2 > n1) {
        n = n2 - n1;
        return n2 + n * t;
    } else {
        return n1;
    }
}
