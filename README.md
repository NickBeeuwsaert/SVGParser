SVGParser
=========

Why, HELLO there! How are you? That is GREAT. I am well. Anyhow, you are probably here to read about SVGParser. 

What is this thing?
===================

It parses SVG paths. What did you expect from a name like "SVGParser"?

---

How do I use it?
================
Simple! First you can include the SVGParser Library

    #include "SVGParser/SVGParser.h"

And then you use the namespace (or not, it just will make your code cleaner if you DO do this(haha DOODOO))
    
    using namespace SVGParser;

Then you are pretty much set! you can parse SVG paths! YAY! 

    istream *mystream;

    mystream = new istringstream("M0,0 10,0 0,10 10,10z");
    /* OR */
    mystream = new ifstream("mySVGPath.txt");
    
    SVGPath myPath = parsePath(*mystream);
    //SVGPath is just a typedef for vector<SVGCommand*>
    
    //Loop through the path and do things
    for(SVGPath::iterator it = myPath.begin(); it != myPath.end(); it++){
        SVGCommand *cmd = *it;
        /* Do things with the path*/
        switch(cmd->getType()){
            case SVG_MOVE_TO_REL:
            case SVG_MOVE_TO_ABS://or any of the other values in the SVGCommandType enum
                SVGMoveTo *movetocmd = (SVGMoveTo*)cmd;
                // ...
            break;
            default:
                // unused path data?
            break;
        }
    }

---
But! What if I want to create my own paths?
===========================================
I got your back, bro! Check it out:


    SVGPath myNewPath;
    myNewPath.push_back(new SVGMoveTo(10,10, true)); //start at 10,10 absolute
    myNewPath.push_back(new SVGLineTo(10,0)); //line right 10 units (relative)
    myNewPath.push_back(new SVGLineTo(0,10, false)); // line down 10 units (relative)
    myNewPath.push_back(new SVGLineTo(-10,0, false)); //line 10 unites left
    myNewPath.push_back(new SVGClosePath());
    
    //print it out
    for(SVGPath::iterator it = myNewPath.begin(); it != myNewPath.end(); it++){
        cout << (*it)->toString();
        //OR
        cout << *it;
    }
    // OR 
    cout << myNewPath << endl;

---
That's great and all, but can I draw with this also?
====================================================
NO

---

Documentation!
===
SVG Command initializers are in the same  order as in an actual SVG path. Here is a handy dandy table.

| SVG Command | SVGParser |
|---|---|
| Move To (M or m) x y | `SVGMoveTo(x, y[, isAbsolute=false]);` |
| Line To (L or l) x y| `SVGLineTo(x, y[, isAbsolute=false]);` |
| Horizontal Line To (H or h) | `SVGHLineTo(x[, isAbsolute=false]);` |
| Vertical Line To (V or v) | `SVGVLineTo(y[, isAbsolute=false]);` |
| Quadratice Curve To (Q or q) | `SVGQuadraticCurveTo(cp1x, cp1y, x, y[, isAbsolute=false]);` |
| Smooth Quadratice Curve To (T ort) | `SVGSmoothQuadraticCurveTo(x, y[, isAbsolute=false]);` |
| Cubic Curve To (C or c) | `SVGCubicCurveTo(cp1x, cp1y, cp2x, cp2y, x, y[, isAbsolute=false]);` |
| Smooth Cubic Curve To (S or s) | `SVGSmoothCubicCurveTo(cp1x, cp1y, x, y[, isAbsolute=false]);` |
| Arc To (A or a) | `SVGArcTo(rx, ry, xrot, largeArc, sweep, x, y[, isAbsolute=false]);` |
| Close Path (Z or z) | `SVGClosePath();` |
