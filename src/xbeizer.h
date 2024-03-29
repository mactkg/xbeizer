//
//  xbeizer.h
//  
//
//  Created by 健太 原 on 2/10/12.
//  Copyright (c) 2012 Tokyo Tech High School of Science and Technoligy. All rights reserved.
//

#ifndef _xbeizer_h
#define _xbeizer_h

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void drawFillArc(Display* dpy, Drawable w, GC gc, int x, int y, int r);

void drawPointDetail(Display* dpy, Drawable w, GC gc, XPoint* up, int num);

void drawBeizerCurve(Display* dpy, Drawable w, GC gc, XPoint* up, XPoint* bp, float t, int l);

XPoint getBeizerPoint(XPoint* p, float t, int l, int n);

XPoint dividePoints(XPoint p1, XPoint p2, float t);

short divideValue(short n1, short n2, float t);

#endif