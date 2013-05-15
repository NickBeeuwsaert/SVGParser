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

And then you use the namespace (or not, it just will make your code cleaner if you DO do this(haha DOO DOO))
    
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
    
    //Also you could build your own SVGPath
    SVGPath myNewPath;
    myNewPath.push_back(new SVGMoveTo(10,10, true)); //start at 10,10 absolute
    myNewPath.push_back(new SVGLineTo(10,0)); //line right 10 units (relative)
    myNewPath.push_back(new SVGLineTo(0,10, false)); // line down 10 units (relative)
    myNewPath.push_back(new SVGLineTo(-10,0, false)); //line 10 unites left
    myNewPath.push_back(new SVGClosePath());
    
    //print it out
    for(SVGPath::iterator it = myNewPath.begin(); it != myNewPath.end(); it++){
        cout << (*it)->toString();
    }

