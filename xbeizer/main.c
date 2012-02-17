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
    float t;
    Cursor csr;
    XPoint usrPoints[20];
    XPoint bufPoints[20];
    
    usrPoints[0].x = 20;
    usrPoints[0].y = 100;
    usrPoints[1].x = 200;
    usrPoints[1].y = 400;
    usrPoints[2].x = 400;
    usrPoints[2].y = 50;
    for (i = 0; i < 20; i++) {
        bufPoints[i] = usrPoints[i];
    }
    for (t = 0; t < 1; t+=0.01) {
        XPoint p = getBeizerPoint(bufPoints, t, 2);
        printf("Returned : %hd, %hd\n\n", p.x, p.y);
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
                    XDrawRectangle(dpy, w, gc, bufPoints[0].x, bufPoints[0].y, 3, 3);
                    XDrawRectangle(dpy, w, gc, bufPoints[1].x, bufPoints[1].y, 3, 3);
                    XDrawRectangle(dpy, w, gc, bufPoints[2].x, bufPoints[2].y, 3, 3);


            }
        } else {
            //***************************animation*********
            usleep(500);
            XFlush(dpy); 
            if (t < 1) {
                XPoint p = getBeizerPoint(bufPoints, t, 3);
                XDrawPoint(dpy, w, gc, p.x, p.y);
                t += 0.0001;
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
    if (l - n == 2) {
        printf("call : p[%d].x %hd, p[%d].y %hd\n       p2[%d].x %hd, p[%d].y %hd\n",n ,p[n].x, n, p[n].y,n+1 ,p[n+1].x ,n+1, p[n+1].y);
        printf("call : p[%d].x %hd, p[%d].y %hd\n       p2[%d].x %hd, p[%d].y %hd\n",l-n ,p[l-n].x, l-n, p[l-n].y,l-n-1 ,p[l-n-1].x ,l-n-1, p[l-n-1].y);
        return dividePoints(p[l-n], dividePoints(p[n], p[n+1], t), t);
    } else {
        printf("called! : _get BeizerPoint\n");
        printf("call : p[%d].x %hd, p[%d].y %hd\n       p2[%d].x %hd, p[%d].y %hd\n",n ,p[n].x, n, p[n].y,n+1 ,p[n+1].x ,n+1, p[n+1].y);
        p[n+1] = dividePoints(p[n], p[n+1], t);
        printf("call : p[%d].x %hd, p[%d].y %hd\n       p2[%d].x %hd, p[%d].y %hd\n",l-n ,p[l-n].x, l-n, p[l-n].y,l-n-1 ,p[l-n-1].x ,l-n-1, p[l-n-1].y);
        p[l-n-1] = dividePoints(p[l-n], p[l-n-1], t);
        
        if (n >= l/2) {
            return dividePoints(p[n+1], p[l-n+1], t);
        } else {
            return _getBeizerPoint(p, t, l, n-1);
        }
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
    if (n1 > n2) {
        n = n1 - n2;
        return n2 + n * t;
    } else if(n2 > n1) {
        n = n2 - n1;
        return n1 + n * t;
    } else {
        return n1;
    }
}