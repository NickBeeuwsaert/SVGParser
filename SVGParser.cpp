#include "SVGParser.h"
#include <sstream>
#include <string>
#include <iostream>
#include <cctype>

using namespace std;

namespace SVGParser {

	SVGCommandType getSVGCommand(char c){
		switch(c){
			case 'm':
				return SVG_MOVE_TO_REL;
			break;
			case 'M':
				return SVG_MOVE_TO_ABS;
			break;
			
			case 'l':
				return SVG_LINE_TO_REL;
			break;
			case 'L':
				return SVG_LINE_TO_ABS;
			break;
			
			case 'c':
				return SVG_CUBIC_CURVE_TO_REL;
			break;
			case 'C':
				return SVG_CUBIC_CURVE_TO_ABS;
			break;
			
			case 'q':
				return SVG_QUADRATIC_CURVE_TO_REL;
			break;
			case 'Q':
				return SVG_QUADRATIC_CURVE_TO_ABS;
			break;
			
			case 'a':
				return SVG_ARC_TO_REL;
			break;
			case 'A': 
				return SVG_ARC_TO_ABS;
			break;
			
			case 't':
				return SVG_SMOOTH_QUADRATIC_CURVE_TO_REL;
			break;
			case 'T': 
				return SVG_SMOOTH_QUADRATIC_CURVE_TO_ABS;
			break;
			
			case 's':
				return SVG_SMOOTH_CUBIC_CURVE_TO_REL;
			break;
			case 'S': 
				return SVG_SMOOTH_CUBIC_CURVE_TO_ABS;
			break;
			
			case 'h':
				return SVG_HORIZONTAL_LINE_TO_REL;
			break;
			case 'H':
				return SVG_HORIZONTAL_LINE_TO_ABS;
			break;
			
			case 'v':
				return SVG_VERTICAL_LINE_TO_REL;
			break;
			case 'V':
				return SVG_VERTICAL_LINE_TO_ABS;
			break;
			
			case 'z':
			case 'Z':
				return SVG_CLOSE_PATH;
			break;
			default:
				return SVG_INVALID;
			break;
		}
	};
	int getCommandLength(SVGCommandType t){
		switch(t){
			case SVG_MOVE_TO_REL:
			case SVG_LINE_TO_REL:
			case SVG_MOVE_TO_ABS:
			case SVG_LINE_TO_ABS:
				return 2;
			break;
			case SVG_HORIZONTAL_LINE_TO_REL:
			case SVG_HORIZONTAL_LINE_TO_ABS:
			case SVG_VERTICAL_LINE_TO_REL:
			case SVG_VERTICAL_LINE_TO_ABS:
				return 1;
			break;
			
			
			case SVG_QUADRATIC_CURVE_TO_REL:
			case SVG_QUADRATIC_CURVE_TO_ABS:
				return 4;
			break;
			case SVG_CUBIC_CURVE_TO_REL:
			case SVG_CUBIC_CURVE_TO_ABS:
				return 6;
			break;
			
			case SVG_SMOOTH_CUBIC_CURVE_TO_REL:
			case SVG_SMOOTH_CUBIC_CURVE_TO_ABS:
				return 4;
			break;
			
			case SVG_SMOOTH_QUADRATIC_CURVE_TO_REL:
			case SVG_SMOOTH_QUADRATIC_CURVE_TO_ABS:
				return 2;
			break;
			
			case SVG_ARC_TO_REL:
			case SVG_ARC_TO_ABS:
				return 7;
			break;
			case SVG_CLOSE_PATH:
			case SVG_INVALID:
			default:
				return 0;
			break;
		}
	};
	bool isAbsolute(SVGCommandType t){
		if(t == SVG_CLOSE_PATH) return false;
		return t%2;
	}
	SVGPath createCommandChunk(SVGCommandType t, vector<float> elements){
		int commandLength = getCommandLength(t);
		SVGPath chunk;
		bool abs = isAbsolute(t);
		if(t==SVG_CLOSE_PATH) chunk.push_back(new SVGClosePath());
		if(commandLength > 0)
		for(int i = 0; i < int(elements.size() / commandLength); i++){
			switch(t){
				case SVG_LINE_TO_REL:
				case SVG_LINE_TO_ABS:
					chunk.push_back(new SVGLineTo(elements[i*commandLength+0],elements[i*commandLength+1],abs));
				break;
				case SVG_MOVE_TO_REL:
				case SVG_MOVE_TO_ABS:
					chunk.push_back(new SVGMoveTo(elements[i*commandLength+0],elements[i*commandLength+1],abs));
				break;
				case SVG_CUBIC_CURVE_TO_REL:
				case SVG_CUBIC_CURVE_TO_ABS:
					chunk.push_back(new SVGCubicCurveTo(elements[i*commandLength+0],elements[i*commandLength+1],
														elements[i*commandLength+2],elements[i*commandLength+3],
														elements[i*commandLength+4],elements[i*commandLength+5],abs));
				break;
				
				case SVG_SMOOTH_CUBIC_CURVE_TO_REL:
				case SVG_SMOOTH_CUBIC_CURVE_TO_ABS:
					chunk.push_back(new SVGSmoothCubicCurveTo(elements[i*commandLength+0],elements[i*commandLength+1],
														elements[i*commandLength+2],elements[i*commandLength+3],abs));
				break;
				
				case SVG_QUADRATIC_CURVE_TO_REL:
				case SVG_QUADRATIC_CURVE_TO_ABS:
					chunk.push_back(new SVGQuadraticCurveTo(elements[i*commandLength+0],elements[i*commandLength+1],
													    	elements[i*commandLength+2],elements[i*commandLength+3],abs));
				break;
				
				case SVG_SMOOTH_QUADRATIC_CURVE_TO_REL:
				case SVG_SMOOTH_QUADRATIC_CURVE_TO_ABS:
					chunk.push_back(new SVGSmoothQuadraticCurveTo(elements[i*commandLength+0],elements[i*commandLength+1],abs));
				break;
				
				case SVG_ARC_TO_REL:
				case SVG_ARC_TO_ABS:
					chunk.push_back(new SVGArcTo(elements[i*commandLength+0],elements[i*commandLength+1],//rx, ry
											     elements[i*commandLength+2],//rotation, 
											     int(elements[i*commandLength+3])==1,int(elements[i*commandLength+4])==1, //large arc, sweep
											     elements[i*commandLength+5],elements[i*commandLength+6], //end x, end y
											     abs));
				break;
				case SVG_HORIZONTAL_LINE_TO_REL:
				case SVG_HORIZONTAL_LINE_TO_ABS:
					chunk.push_back(new SVGHLineTo(elements[i*commandLength+0],abs));
				break;
				case SVG_VERTICAL_LINE_TO_REL:
				case SVG_VERTICAL_LINE_TO_ABS:
					chunk.push_back(new SVGVLineTo(elements[i*commandLength+0],abs));
				break;
			}
			//This here is funky.
			if(t==SVG_MOVE_TO_REL || t == SVG_MOVE_TO_ABS) t = SVGCommandType(int(t) + 2);
		}
		return chunk;
	}
	SVGPath parsePath(istream &ss){
		SVGPath commandArray;
		//istringstream ss(path);
		bool absolute = false;
		SVGCommandType command = SVG_INVALID;
		vector<float> elements;
		
		while(ss.good()){
			char c = ss.get();
			SVGCommandType cmd = getSVGCommand(c);

			if(cmd != SVG_INVALID){
				SVGPath chunk = createCommandChunk(command, elements);
				commandArray.insert(commandArray.end(), chunk.begin(), chunk.end());
				command = cmd;
				elements.clear();
				
			}else if(!(isspace(c) || c==',')){//its a number...
				float x;
				ss.unget();
				ss >> x;
				elements.push_back(x);
			}// else its a separator
		}
		SVGPath chunk = createCommandChunk(command, elements);
		commandArray.insert(commandArray.end(), chunk.begin(), chunk.end());
		return commandArray;
	}
	
	SVGCommand::SVGCommand(float _x, float _y, bool abs) : x(_x), y(_y), absolute(abs) {}
	SVGLineTo::SVGLineTo(float x, float y, bool abs) : SVGCommand(x, y, abs){
	}
	
	const string SVGLineTo::toString() const {
		stringstream s;
		s << (absolute?"L":"l") << x << "," << y;
		return s.str();
	}
	string SVGLineTo::toString2(){
		stringstream s;
		s << "Line to (" <<(absolute?"absolute":"relative") << ") " << x << "x" << y;
		return s.str();
	}
	
	SVGCommandType SVGLineTo::getType(){
		return absolute?SVG_LINE_TO_ABS:SVG_LINE_TO_REL;
	}
	
	SVGHLineTo::SVGHLineTo(float x, bool abs) : SVGCommand(x, 0, abs){
	}
	const string SVGHLineTo::toString() const {
		stringstream s;
		s << (absolute?"H":"h") << x;
		return s.str();
	}
	string SVGHLineTo::toString2(){
		stringstream s;
		s << "Horizontal Line (" << (absolute?"absolute":"relative") << ") " << x <<"px";
		return s.str();
	}
	
	SVGCommandType SVGHLineTo::getType(){
		return absolute?SVG_HORIZONTAL_LINE_TO_ABS:SVG_HORIZONTAL_LINE_TO_REL;
	}
	
	SVGVLineTo::SVGVLineTo(float y, bool abs) : SVGCommand(0, y, abs){
	}
	
	const string SVGVLineTo::toString() const {
		stringstream s;
		s << (absolute?"V":"v") << y;
		return s.str();
	}
	string SVGVLineTo::toString2(){
		stringstream s;
		s << "Vertical Line (" << (absolute?"absolute":"relative") << ") " << y <<"px";
		return s.str();
	}
	SVGCommandType SVGVLineTo::getType(){
		return absolute?SVG_VERTICAL_LINE_TO_ABS:SVG_VERTICAL_LINE_TO_REL;
	}
	
	
	
	
	SVGMoveTo::SVGMoveTo(float x, float y, bool abs) : SVGCommand(x, y, abs){
	}
	const string SVGMoveTo::toString() const {
		stringstream s;
		s << (absolute?"M":"m") << x << "," << y;
		return s.str();
	}
	string SVGMoveTo::toString2(){
		stringstream s;
		s << "Move to (" << (absolute?"absolute":"relative") << ") to " << x << "x" << y;
		return s.str();
	}
	SVGCommandType SVGMoveTo::getType(){
		return absolute?SVG_MOVE_TO_ABS:SVG_MOVE_TO_REL;
	}
	
	
	SVGCubicCurveTo::SVGCubicCurveTo(float _x0, float _y0,
									 float _x1, float _y1, 
									 float x, float y, bool abs) : SVGCommand(x, y, abs),
									 								x0(_x0), y0(_y0),
									 								x1(_x1), y1(_y1) {
									 								}
									 								
	const string SVGCubicCurveTo::toString() const {
		stringstream s;
		s << (absolute?"C":"c") << x0 << "," << y0 << " " << x1 << "," << y1 << " " << x << "," << y;
		return s.str();
	}
	string SVGCubicCurveTo::toString2(){
		stringstream s;
		s << "Cubic Curve (" << (absolute?"absolute":"relative") << ") control points: " << x0 << "," << y0 << " " << x1 << "," << y1 << " to " << x << "x" << y;
		return s.str();
	}
	SVGCommandType SVGCubicCurveTo::getType(){
		return absolute?SVG_CUBIC_CURVE_TO_ABS:SVG_CUBIC_CURVE_TO_REL;
	}
	
	SVGSmoothCubicCurveTo::SVGSmoothCubicCurveTo(
									 float _x1, float _y1, 
									 float x, float y, bool abs) : SVGCommand(x, y, abs),
									 								x1(_x1), y1(_y1) {
									 								}
									 								
	const string SVGSmoothCubicCurveTo::toString() const {
		stringstream s;
		s << (absolute?"S":"s") << x1 << "," << y1 << " " << x << "," << y;
		return s.str();
	}
	string SVGSmoothCubicCurveTo::toString2(){
		stringstream s;
		s << "Smooth cubic curve to (" << (absolute?"absolute":"relative") << ") control point: " << x1 << "," << y1 << " to " << x << "x" << y;
		return s.str();
	}
	SVGCommandType SVGSmoothCubicCurveTo::getType(){
		return absolute?SVG_SMOOTH_CUBIC_CURVE_TO_ABS:SVG_SMOOTH_CUBIC_CURVE_TO_REL;
	}
	
	
	SVGQuadraticCurveTo::SVGQuadraticCurveTo(float _x0, float _y0,
											 float x, float y, bool abs) : SVGCommand(x, y, abs), x0(_x0), y0(_y0) {
											 }
 	const string SVGQuadraticCurveTo::toString() const {
		stringstream s;
		s << (absolute?"Q":"q") << x0 << "," << y0 << " " << x << "," << y;
		return s.str();
	}
	string SVGQuadraticCurveTo::toString2(){
		stringstream s;
		s << "quadratic curve (" << (absolute?"absolute":"relative") << ") control point: " << x0 << "," << y0 << " to " << x << "x" << y;
		return s.str();
	}
	SVGCommandType SVGQuadraticCurveTo::getType(){
		return absolute?SVG_QUADRATIC_CURVE_TO_ABS:SVG_QUADRATIC_CURVE_TO_REL;
	}
	SVGSmoothQuadraticCurveTo::SVGSmoothQuadraticCurveTo(
											 float x, float y, bool abs) : SVGCommand(x, y, abs) {
											 }
 	const string SVGSmoothQuadraticCurveTo::toString() const {
		stringstream s;
		s << (absolute?"T":"t") <<  x << "," << y;
		return s.str();
	}
	string SVGSmoothQuadraticCurveTo::toString2(){
		stringstream s;
		s << "Smooth quadratic curve (" << (absolute?"absolute":"relative") << ") to " <<  x << "x" << y;
		return s.str();
	}
	SVGCommandType SVGSmoothQuadraticCurveTo::getType(){
		return absolute?SVG_SMOOTH_QUADRATIC_CURVE_TO_ABS:SVG_SMOOTH_QUADRATIC_CURVE_TO_REL;
	}
	
	SVGArcTo::SVGArcTo(float rx, float ry, float rotxaxis, bool large, bool sweep, float x, float y, bool abs) : SVGCommand(x, y, abs) {
		this->rx = rx;
		this->ry = ry;
		
		this->rot = rotxaxis;
		
		this->large = large;
		this->sweep = sweep;
	}
	const string SVGArcTo::toString() const {
		stringstream s;
		s << (absolute?"A":"a") <<  rx << "," << ry << " " << rot << " " << large << "," << sweep << " " << x << "," <<y;
		return s.str();
	}
	
	string SVGArcTo::toString2(){
		stringstream s;
		s << "Arc (" << (absolute?"absolute":"relative") <<  "x-radii: " << rx << " y-radii: " << ry << " x-rotation: " << rot << " large-arc: " << large << " sweep: " << sweep << " to: " << x << "x" <<y;
		return s.str();
	}
	
	SVGCommandType SVGArcTo::getType(){
		return absolute?SVG_ARC_TO_ABS:SVG_ARC_TO_REL;
	}
	
	SVGClosePath::SVGClosePath(bool abs):SVGCommand(0,0,abs){
		
	}

	const string SVGClosePath::toString() const {
		stringstream s;
		s << (absolute?"Z":"z");
		return s.str();
	}
	string SVGClosePath::toString2(){
		stringstream s;
		s << "Close path";
		return s.str();
	}
	SVGCommandType SVGClosePath::getType(){
		return SVG_CLOSE_PATH;
	}
    std::ostream& operator<< (std::ostream& out, const SVGParser::SVGPath& path){
        for(SVGParser::SVGPath::const_iterator it = path.begin(); it != path.end(); it++){
            out << (*it)->toString();
        }
        return out;
    }
    std::ostream& operator<< (std::ostream& out, const SVGParser::SVGCommand* cmd){
        out << cmd->toString();
        return out;
    }
    SVGPath toAbsolute(SVGPath path){
        float x = 0, y = 0;
        SVGPath newPath;
        for(SVGPath::iterator it = path.begin(); it != path.end(); it++){
            SVGCommand *cmd = *it;
            bool absolute = cmd->absolute;
            switch(cmd->getType()){
                case SVG_MOVE_TO_REL:
                    newPath.push_back(new SVGMoveTo(x+=cmd->x, y+=cmd->y, true));
                break;
                case SVG_LINE_TO_REL:
                    newPath.push_back(new SVGLineTo(x+=cmd->x, y+=cmd->y, true));
                break;
                case SVG_CUBIC_CURVE_TO_REL:
                    newPath.push_back(new SVGCubicCurveTo(
                                        x+((SVGCubicCurveTo*)cmd)->x0, y+((SVGCubicCurveTo*)cmd)->y0,
                                        x+((SVGCubicCurveTo*)cmd)->x1, y+((SVGCubicCurveTo*)cmd)->y1,
                                        x+=cmd->x, y+=cmd->y, true));
                break;
                case SVG_QUADRATIC_CURVE_TO_REL:
                    newPath.push_back(new SVGQuadraticCurveTo(
                                        x+((SVGQuadraticCurveTo*)cmd)->x0, y+((SVGQuadraticCurveTo*)cmd)->y0,
                                        x+=cmd->x, y+=cmd->y, true));
                break;
                case SVG_SMOOTH_CUBIC_CURVE_TO_REL:
                    newPath.push_back(new SVGSmoothCubicCurveTo(
                                        x+((SVGSmoothCubicCurveTo*)cmd)->x1, y+((SVGSmoothCubicCurveTo*)cmd)->y1,
                                        x+=cmd->x, y+=cmd->y, true));
                break;
                case SVG_SMOOTH_QUADRATIC_CURVE_TO_REL:
                    newPath.push_back(new SVGSmoothQuadraticCurveTo(
                                        x+=cmd->x, y+=cmd->y, true));
                break;
                case SVG_HORIZONTAL_LINE_TO_REL:
                    newPath.push_back(new SVGHLineTo(
                                        x+=cmd->x, true));
                break;
                case SVG_VERTICAL_LINE_TO_REL:
                    newPath.push_back(new SVGVLineTo(
                                        y+=cmd->y, true));
                break;
                case SVG_ARC_TO_REL:
                    newPath.push_back(new SVGArcTo(
                                        ((SVGArcTo*)cmd)->rx,  ((SVGArcTo*)cmd)->ry,
                                        ((SVGArcTo*)cmd)->rot, 
                                        ((SVGArcTo*)cmd)->large, ((SVGArcTo*)cmd)->sweep, 
                                        x+=cmd->x, y+=cmd->y, true));
                break;
                default: //is is a absolute command, update the coords
                    if(cmd->getType() == SVG_HORIZONTAL_LINE_TO_ABS){
                        x = cmd->x;
                    }else if(cmd->getType() == SVG_VERTICAL_LINE_TO_ABS){
                        y = cmd->y;
                    }else{
                        x = cmd->x;
                        y = cmd->y;
                    }
                    newPath.push_back(cmd);
                break;
            }
            //x += cmd->x;
            //y += cmd->y;
        }
        return newPath;
    }
    SVGPath toRelative(SVGPath path){
        SVGPath newPath;
        //Convert all path elements to be relative...
        return path;
    }
};

