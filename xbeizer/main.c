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
#define HIGHT 500

int main(int argc, char **argv){
	Display *dpy;
	Window w,quit;
	Window root;
	int    screen;
	unsigned long black, white;
	GC       gc;
	XEvent   e;
	int  i;
    float t;
    Cursor csr;
    XPoint usrPoints[20];
    XPoint bufPoints[20];
    
    usrPoints[0].x = 20;
    usrPoints[0].y = 100;
    usrPoints[1].x = 60;
    usrPoints[1].y = 200;
    usrPoints[2].x = 100;
    usrPoints[2].y = 200;
    usrPoints[3].x = 140;
    usrPoints[3].y = 100;
    for (i = 0; i < 20; i++) {
        bufPoints[i] = usrPoints[i];
    }
    
    
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
    
    //************************************test*****
    
    
    t = 0;

    //******************************loop***********
	while(1){
        
        if(XEventsQueued(dpy, QueuedAfterReading)){
            //***************************event************
            XNextEvent(dpy, &e);
            switch(e.type){
                case ButtonPress : if(e.xany.window == quit) return 0;
                    XDrawString(dpy,quit,gc,4,10,"Exit",4);
                    XDrawRectangle(dpy, w, gc, bufPoints[0].x-1, bufPoints[0].y-1, 2, 2);
                    XDrawRectangle(dpy, w, gc, bufPoints[1].x-1, bufPoints[1].y-1, 2, 2);
                    XDrawRectangle(dpy, w, gc, bufPoints[2].x-1, bufPoints[2].y-1, 2, 2);
                    XDrawRectangle(dpy, w, gc, bufPoints[3].x-1, bufPoints[3].y-1, 2, 2);
                    XDrawLine(dpy, w, gc, bufPoints[0].x, bufPoints[0].y, bufPoints[1].x, bufPoints[1].y);
                    XDrawLine(dpy, w, gc, bufPoints[1].x, bufPoints[1].y, bufPoints[2].x, bufPoints[2].y);
                    XDrawLine(dpy, w, gc, bufPoints[2].x, bufPoints[2].y, bufPoints[3].x, bufPoints[3].y);
                    XDrawLine(dpy, w, gc, 80, 300, 80, 20);


            }
        } else {
            //***************************animation*********
            
            usleep(5);
            XFlush(dpy); 
            if (t < 1) {
                for (i = 0; i < 20; i++) {
                    usrPoints[i] = bufPoints[i];
                }
                XPoint p = getBeizerPoint(usrPoints, t, 4);
                XDrawPoint(dpy, w, gc, p.x, p.y);
                t += 0.00001;
            }
        }
	}
}

//*********************************************

XPoint getBeizerPoint(XPoint* p, float t, int l){
    printf("called! : get BeizerPoint\n");
    return _getBeizerPoint(p, t, l-1, 0);
}

XPoint _getBeizerPoint(XPoint* p, float t, int l, int n){
    printf("called! : _get BeizerPoint\n");
    if (n >= l/2) {
        printf("RETURN! : p[%d].x %hd, p[%d].y %hd /// p2[%d].x %hd, p[%d].y %hd\n",n ,p[n].x, n, p[n].y,n+1 ,p[n+1].x ,n+1, p[n+1].y);
        return dividePoints(p[n], p[n+1], t);
    } else {
        printf("call2 : p[%d].x %hd, p[%d].y %hd /// p2[%d].x %hd, p[%d].y %hd\n",n ,p[n].x, n, p[n].y,n+1 ,p[n+1].x ,n+1, p[n+1].y);
        p[n+1] = dividePoints(p[n], p[n+1], t);
        printf("call2 : p[%d].x %hd, p[%d].y %hd /// p2[%d].x %hd, p[%d].y %hd\n",l-n ,p[l-n].x, l-n, p[l-n].y,l-n-1 ,p[l-n-1].x ,l-n-1, p[l-n-1].y);
        p[l-n-1] = dividePoints(p[l-n-1], p[l-n], t);
        return _getBeizerPoint(p, t, l, n+1);
    }
}

//*********************************************

XPoint dividePoints(XPoint p1, XPoint p2, float t){
    XPoint ans;
    ans.x = divideValue(p1.x, p2.x, t);
    ans.y = divideValue(p1.y, p2.y, t);
    printf("divided : x %hd, y %hd\n", ans.x, ans.y);
    return ans;
}

//*********************************************

short divideValue(short n1, short n2, float t){
    short n;
    n = n2 - n1;
    return n1 + n * t;
}