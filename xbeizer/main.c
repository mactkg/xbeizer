//
//  main.c
//  xbeizer
//
//  Created by 健太 原 on 2/10/12.
//  Copyright (c) 2012 Tokyo Tech High School of Science and Technoligy. All rights reserved.
//

#include "xbeizer.h"

//#define DEBUG

#define BORDER 2
#define WIDTH  500
#define HEIGHT 500
#define NANOTIME 1000

int main(int argc, char **argv){
	Display *dpy;
	Window w,quit,clearButton;
	Window root;
    struct timespec wait;
	int    screen;
	unsigned long black, white;
	GC       gc;
	XEvent   e;
	int  i;
    float t;
    int pointNum;
    XPoint usrPoints[20];
    XPoint bufPoints[20];
    XPoint selectedPoints[20];
    
    //********************************test data****
    
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
    
    
    //*************************************init****
    
	dpy = XOpenDisplay("");
    
	root = DefaultRootWindow (dpy);
	screen = DefaultScreen (dpy);
	white = WhitePixel (dpy, screen);
	black = BlackPixel (dpy, screen);
    
	w = XCreateSimpleWindow(dpy, root, 100, 100, WIDTH, HEIGHT, BORDER, black, white);
	//pmap = XCreatePixmap(dpy, w, WIDTH, HEIGHT, DefaultDepth(dpy, 0));

    /* Make Sub Window */
	quit = XCreateSimpleWindow(dpy, w, 10, 3, 30, 12, BORDER, black, white);
    clearButton = XCreateSimpleWindow(dpy, w, 52, 3, 37, 12, BORDER, black, white);

    
	gc = XCreateGC(dpy, w, 0, NULL);
    
    pointNum = 0;
    t = 0;
    wait.tv_sec = 0;
    wait.tv_nsec = NANOTIME;
    
    //*********************************************
    
	XSelectInput(dpy, w, ButtonPressMask | StructureNotifyMask | PointerMotionMask);
    XSelectInput(dpy, quit, ButtonPressMask);
    XSelectInput(dpy, clearButton, ButtonPressMask);

    
	XMapWindow(dpy, w);
	XMapSubwindows(dpy, w);
    XSetForeground(dpy, gc, white);
    XFillRectangle(dpy, w, gc, 0, 0, WIDTH, HEIGHT);
	XSetForeground(dpy,gc,black);

    //*************************************loop****
	while(1){
        if(XEventsQueued(dpy, QueuedAfterReading)){
            XDrawString(dpy, quit, gc, 4, 10, "Exit", 4);
            XDrawString(dpy, clearButton, gc, 4, 10, "Clear", 5);
            //***********************************event****
            XNextEvent(dpy, &e);
            switch(e.type){
                case ButtonPress : 
                    if(e.xany.window == quit) return 0;
                    drawPointDetail(dpy, w, gc, bufPoints, 4);

            }
        } else {
            //********************************animation****
            if (t < 1) {
                nanosleep(&wait, NULL);
                drawBeizerCurve(dpy, w, gc, usrPoints, bufPoints, t, 4);
                t += 0.00001;
            }
        }
        XFlush(dpy);
	}
    return 0;
}

//*********************************************

void drawPointDetail(Display* dpy, Drawable w, GC gc, XPoint* up, int num){
    int i = 0;
    for(; i < num; i++){
        XPoint p = up[i];
        char str[20];
        sprintf(str, "x:%hd y:%hd",p.x, p.y);
        if (p.x < WIDTH / 2) {
            XDrawString(dpy, w, gc, p.x, p.y-3, str, (int)strlen(str));
        } else {
            XDrawString(dpy, w, gc, p.x-25, p.y-3, str, (int)strlen(str));
        }
        XFillArc(dpy, w, gc, p.x-3, p.y-3, 6, 6, 0, 360*64);
    }
}

//*********************************************

void drawBeizerCurve(Display* dpy, Drawable w, GC gc, XPoint* up, XPoint* bp, float t, int l){
#ifdef DEBUG
    printf("called: drawBeizerCurve");
#endif
    int i;
    for (i = 0; i < 20; i++) {
        up[i] = bp[i];
    }
    XPoint p = _getBeizerPoint(up, t, l-1, 0);
    XDrawPoint(dpy, w, gc, p.x, p.y);
}


//*********************************************

XPoint getBeizerPoint(XPoint* up, XPoint* bp, float t, int l){

    int i;
    for (i = 0; i < 20; i++) {
        up[i] = bp[i];
    }
#ifdef DEBUG
    printf("called! : get BeizerPoint\n");
#endif
    return _getBeizerPoint(up, t, l-1, 0);
}

XPoint _getBeizerPoint(XPoint* p, float t, int l, int n){

#ifdef DEBUG
    printf("called! : _get BeizerPoint\n");
#endif
    if (n >= l/2) {
#ifdef DEBUG
        printf("RETURN! : p[%d].x %hd, p[%d].y %hd /// p2[%d].x %hd, p[%d].y %hd\n",n ,p[n].x, n, p[n].y,n+1 ,p[n+1].x ,n+1, p[n+1].y);
#endif
        return dividePoints(p[n], p[n+1], t);
    } else {
#ifdef DEBUG
        printf("call2 : p[%d].x %hd, p[%d].y %hd /// p2[%d].x %hd, p[%d].y %hd\n",n ,p[n].x, n, p[n].y,n+1 ,p[n+1].x ,n+1, p[n+1].y);
#endif
        p[n+1] = dividePoints(p[n], p[n+1], t);
#ifdef DEBUG
        printf("call2 : p[%d].x %hd, p[%d].y %hd /// p2[%d].x %hd, p[%d].y %hd\n",l-n ,p[l-n].x, l-n, p[l-n].y,l-n-1 ,p[l-n-1].x ,l-n-1, p[l-n-1].y);
#endif
        p[l-n-1] = dividePoints(p[l-n-1], p[l-n], t);
        return _getBeizerPoint(p, t, l, n+1);
    }
}

//*********************************************

XPoint dividePoints(XPoint p1, XPoint p2, float t){
    XPoint ans;
    ans.x = divideValue(p1.x, p2.x, t);
    ans.y = divideValue(p1.y, p2.y, t);
#ifdef DEBUG
    printf("divided : x %hd, y %hd\n", ans.x, ans.y);
#endif
    return ans;
}

//*********************************************

short divideValue(short n1, short n2, float t){
    short n;
    n = n2 - n1;
    return n1 + n * t;
}