
/* This class represents a point in terms of X and Y coordinates.

X axis is a horizontal line.
Y axis is a verticle line.

We use the same convention used in screen coordinates in that 
the x coordinate increases left to right, and y coordinate increases
top to bottom.

Assumption: All coordinates, widths and heights are integer values.
If this changes, change corresponding variables from type long to type double.

*/

class CPoint
{
	friend class CSect;	// CSect functions heavily use x,y.
	friend class CRect;	// x,y  are needed for printing.

	long x;		// X coordinate
	long y;		// Y coordinate

public:
	// Ctor
	CPoint(long x1, long y1);

	// Dtor
	~CPoint();
};
