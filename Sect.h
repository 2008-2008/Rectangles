// The intersections class

#include <vector>
#include <map>
#include "Rect.h"

using namespace std;

class CSect
{
	vector<CRect> inList;	// Input list of rectangles

public:
	// Ctor
	CSect();

	// Dtor
	~CSect();

	// This initializes the various elements (input rectangles)
	// in the inList.
	bool Init();

	// Finds the next combination of input rectangles to process.
	// This function sets/resets the 'included' flag of the input rectangles
	// included in this combination.
	bool NextCombo();

	// Creates a set of indexes of input rectangles included in a given combination.
	set<unsigned int> GetSet();

	// Gets the intersection rectangle for the *** given set of input rectangles ***.
	CRect GetXsect(set<unsigned int> s, bool& found);

	// Gets the intersection rectangle for *** given 2 rectangles ***.
	CRect GetXsect2(const CRect rect1, const CRect rect2, bool& found);

#ifdef _DEBUG
	// Prints state of the input collection for degugging purposes.
	void dbgPrint();
#endif


};