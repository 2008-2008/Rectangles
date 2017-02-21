// Solution to the rectangles problem.
// See the problem in file C__proficiency_test.pdf.

// This solution uses SimpleJSON to read in the input rectangles.
// This is at https://github.com/simplejson/simplejson
// The following files are added from there to this project.
//	JSON.h, JSONValue.h, JSON.cpp, JSONValue.cpp.

#include "stdafx.h"
#include <vector>
#include <string>
#include "Sect.h"

// Main Console function.
int _tmain(int argc, _TCHAR* argv[])
{
	// Sect is the class that pupulates the input rectangles and finds intersections.
	// There is only one instance of this class.
	CSect sect;

	// Read in input rectangles from the JSON file.
	if ( ! (sect.Init()) )
		cerr << "Fatal Error 1: Unable to init rectangles from input Json. Terminating." << endl ;

#ifdef _DEBUG
	sect.dbgPrint();
#endif

	// Keep on getting the next successive combination of the input rectangles.
	// If this combination of rectangles intersect, the intersecting rectangle
	// is printed to cout.

	while (sect.NextCombo())
	{
		// We have the next combination.
		// Create a set of indexes of the input rectangles in this combination.
		set<unsigned int> s = sect.GetSet();

		// We have the set of input rectangles in s.
		// Create their intersection.

		bool found;
		CRect rect = sect.GetXsect(s, found);

		if (found)
		{
#ifdef _DEBUG
			rect.dbgPrint();
#endif

			// Print the output intersecting rectangle.
			rect.Print(true);
		}
	}

	// All done. Exit.
	exit(0);
}

