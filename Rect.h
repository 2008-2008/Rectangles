// CRect class.

#include <iostream>
#include "Point.h"
#include <set>

using namespace std;

/* The inherited Point class is the top-left point.

Assumption: All rectangles are aligned to X and Y axes.There are no rectangles
that are 'at angle' with X and Y axes.
*/

class CRect : CPoint
{
	friend class CSect;	// CSect functions heavily use x1,x2,y1,y2,etc.

	// The original representation
	long width ;	// Width
	long height ;	// height

	// A representation that we find useful for manipulation in finding intersections
	long x1 ;		// x coordinate of the top edge
	long x2 ;		// x coordinate of the bottom edge
	long y1 ;		// y coordinate of the left edge
	long y2 ;		// y coordinate of the right edge

	// IMPORTANT: All rectangles, input and output, are populated so that
	// x1 < x2 and y1 < y2.
	// Assumption: When rectangles intersect each other only in a point or in a line
	//	are not considered valid intersections and are not printed in output.

	bool included;	// This applies to input rectangles only.
					// If set, this rectangle is included in the current
					// combinations of rectangles for which we are trying to
					// find intersection.

	set<unsigned int> components;	// This applies to output rectangles only.
					// It contains a set of input rectangles whose intersection
					// this output rectangle is. We number input rectangles starting at 0.

public:
	// Ctor
	CRect(const unsigned int index, const long tlx, const long tly, const long w, const long h);

	// Dtor
	~CRect();

	// Checks the value of included.
	inline bool Included() const { return included; }

	// Set and reset the 'included' marker.
	inline void SetIncluded() { included = true;  }
	inline void ResetIncluded() { included = false; }

	void Print(const bool printIndex=false) const;

#ifdef _DEBUG
	void dbgPrint() const;
#endif
};