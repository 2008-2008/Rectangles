// The rectangle class.

#include "Rect.h"

// Constructor
// index is the rectangle number. We use it to generate combinations of input rectangles.
// tlx, tly are coordinates of the top left point.
// w is width. h is height.
// We prefer initialization over assignment.
CRect::CRect(const unsigned int index, const long tlx, const long tly, const long w, const long h)
	: CPoint(tlx, tly), width(w), height(h), included(false),
	x1(tlx), x2(tlx + width), y1(tly), y2(tly+h)
{
	// Put the index into the component set.
	components.insert(index);
}

// Destructor
CRect::~CRect()
{}

// Prints an output rectangle.
// If printComponents is true, components will be printed.
void CRect::Print(const bool printComponents) const
{
	if (printComponents)
	{
		cout << "Between rectangle {";
		set<unsigned int>::iterator obj;
		bool first = true;
		for (obj = components.begin(); obj != components.end(); obj++)
		{
			if (first)
			{
				cout << (*obj + 1); // The requirements cound objects starting at 1.
				first = false;
			}
			else
				cout << "," << (*obj + 1);
		}
		cout << "} at (" << x << "," << y << "), w=" << width << ", h=" << height << "." << endl;
	}
	else
		cout << "Rectangle at (" << x << "," << y << "), w=" << width << ", h=" << height << "." << endl;
}

#ifdef _DEBUG
void CRect::dbgPrint() const
{
	cerr << "Included components - ";
	set<unsigned int>::iterator obj;
	for (obj = components.begin(); obj != components.end(); obj++)
		cerr << *obj << "," ;
	cerr << endl;

	cerr << "\t" << x1 << "\t" << x2 << "\t" << y1 << "\t" << y2 << "\t" << included << endl; 
}
#endif