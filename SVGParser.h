#ifndef __SVGPARSER_H__
#define __SVGPARSER_H__
#include <vector>
#include <string>
using namespace std;
namespace SVGParser {
	enum SVGCommandType {SVG_INVALID = -1,
		//SVG_MOVE_TO_REL, SVG_LINE_TO_REL, SVG_QUADRATIC_CURVE_TO_REL, SVG_CUBIC_CURVE_TO_REL,
		//SVG_MOVE_TO_ABS, SVG_LINE_TO_ABS, SVG_QUADRATIC_CURVE_TO_ABS, SVG_CUBIC_CURVE_TO_ABS,
		SVG_MOVE_TO_REL, SVG_MOVE_TO_ABS,
		SVG_LINE_TO_REL, SVG_LINE_TO_ABS,
		SVG_HORIZONTAL_LINE_TO_REL, SVG_HORIZONTAL_LINE_TO_ABS,
		SVG_VERTICAL_LINE_TO_REL, SVG_VERTICAL_LINE_TO_ABS,
		SVG_QUADRATIC_CURVE_TO_REL, SVG_QUADRATIC_CURVE_TO_ABS,
		SVG_CUBIC_CURVE_TO_REL, SVG_CUBIC_CURVE_TO_ABS,
		SVG_SMOOTH_CUBIC_CURVE_TO_REL, SVG_SMOOTH_CUBIC_CURVE_TO_ABS,
		SVG_SMOOTH_QUADRATIC_CURVE_TO_REL, SVG_SMOOTH_QUADRATIC_CURVE_TO_ABS,
		SVG_ARC_TO_REL, SVG_ARC_TO_ABS,
		
		SVG_CLOSE_PATH};
	
	bool isAbsolute(SVGCommandType);
	class SVGCommand {
		public:
			SVGCommand(float, float, bool=false);
			static const int length;
			float x, y;
			bool absolute;
			virtual const string toString() const = 0;
			virtual SVGCommandType getType() = 0;
			virtual string toString2() = 0;
	};
	class SVGMoveTo : public SVGCommand {
		public:
			static const int length = 2;
			SVGMoveTo(float, float, bool=false);
			virtual const string toString() const;
			virtual SVGCommandType getType();
			virtual string toString2();
	};
	
	class SVGLineTo : public SVGCommand {
		public:
			static const int length = 2;
			SVGLineTo(float, float, bool=false);
			virtual const string toString() const;
			virtual SVGCommandType getType();
			virtual string toString2();
	};
	class SVGHLineTo : public SVGCommand {
		public:
			static const int length = 1;
			SVGHLineTo(float, bool=false);
			virtual const string toString() const;
			virtual SVGCommandType getType();
			virtual string toString2();
	};
	class SVGVLineTo : public SVGCommand {
		public:
			static const int length = 1;
			SVGVLineTo(float, bool=false);
			virtual const string toString() const;
			virtual SVGCommandType getType();
			virtual string toString2();
	};
	
	class SVGQuadraticCurveTo : public SVGCommand {
		public:
			SVGQuadraticCurveTo(float, float, float, float, bool=false);
			float x0, y0;
			virtual const string toString() const;
			virtual SVGCommandType getType();
			virtual string toString2();
	};
	
	class SVGCubicCurveTo : public SVGCommand {
		public:
			float x0, y0, x1, y1;
			SVGCubicCurveTo(float, float,float, float, float, float, bool=false);
			virtual const string toString() const;
			virtual SVGCommandType getType();
			virtual string toString2();
	};
	class SVGSmoothQuadraticCurveTo : public SVGCommand {
		public:
			SVGSmoothQuadraticCurveTo(float, float, bool=false);
			virtual const string toString() const;
			virtual SVGCommandType getType();
			virtual string toString2();
	};
	
	class SVGSmoothCubicCurveTo : public SVGCommand {
		public:
			float x1, y1;
			SVGSmoothCubicCurveTo(float, float, float, float, bool=false);
			virtual const string toString() const;
			virtual SVGCommandType getType();
			virtual string toString2();
	};
	
	class SVGArcTo : public SVGCommand {
		public:
			float rx, ry, rot;
            bool large, sweep;
			SVGArcTo(float, float, float, bool, bool, float, float, bool=false);
			virtual const string toString() const;
			virtual SVGCommandType getType();
			virtual string toString2();
	};
	
	class SVGClosePath : public SVGCommand {
		public:
			SVGClosePath(bool=false);
			virtual const string toString() const;
			virtual SVGCommandType getType();
			virtual string toString2();
	};
	typedef vector<SVGCommand*> SVGPath;
	SVGPath parsePath(istream&);
    SVGPath toAbsolute(SVGPath);
    SVGPath toRelative(SVGPath);
    std::ostream& operator<< (std::ostream&, const SVGParser::SVGPath&);
    std::ostream& operator<< (std::ostream&, const SVGParser::SVGCommand*);
};
#endif//__SVGPARSER_H__
