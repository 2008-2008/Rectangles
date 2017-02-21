// Intersections class
#ifdef WIN32
#include <Windows.h>	// Visual Studio doesn't recognize min(), max() without 
						// including this file.
#endif

#include <iostream>

#include "Sect.h"
#include "JSON.h"
#include "JSONValue.h"

/******************************************************************************************************************
IMPORTANT: This class creates a combination of input n rectangles, whose
intersection we are trying to find.

If there are n input rectangle, there are the following number of combinations
of two or more rectangles.

2**n			- all possible combinations
minus
n				- these combinations have only one rectangle, thus no intersection
minus
1				- this combination has zero ractangles, thus no intersection

We will call this number Ncombo for ease of discussion.

Now, there are various ways we can generate these combinations.

***Method 1***: We can have an integer number (or long) varying from 0 (0000) to 2**n (11111) and collect
the rectangles who have a 1 in their corresponding bit location. This is simple method, however

*** this will impose a limit on the number of input rectangles ***

to 128 rectangles (this number is processor/compiler dependent). So, we have not chosen this method.

***Method 2***: In each Rect object, we have a marker called 'bool included'. If the object is included, we set it to true.
If the object is not included, we reset it to false.

We start by setting the first object. This is done in CSect::Init().

To get the next combination, we find the first object where included=false, set it to true and reset all the
prior objects with included=false. This gives us the next combination to process.

We keep on doing this in a loop until we reach the point where all objects are already
marked with included=true. (This is the point where all combinations of objects have been exhausted.)

The con of this Method 2 is that it requires a bit more coding. Depending on the requirements, either Method 1
or Method 2 can be implemented.
******************************************************************************************************************/

// Constructor
// file is the input file in which rectangles are stored in JSON format.
CSect::CSect()
{}

// Destructor
CSect::~CSect()
{}

// Initializes all elements in the inList vector
// Function returns true on success, false on failure.
bool CSect::Init()
{
	int index = 0;

	// Start the printout.
	cout << "Input:" << endl;

	// Get input JSON into a string.
	string input;
	while (!cin.eof())
	{
		string line;
		getline(cin, line);
		input += line;
	}

	// At this time, the entire input file is in string 'input'.
	// Process the json.
	const char * cInput = input.c_str();	// Access to char string.
	JSONValue *value;
	if ((value = JSON::Parse(cInput)) == NULL)
	{
		cerr << "Fatal Error 2: Unable to parse input json. Terminating.";
		exit(2);
	}
	if ( ! (value->IsObject()) )
	{
		cerr << "Fatal Error 3: Input json is not valid. Terminating.";
		exit(3);
	}

	// We have parsed the json successfully. Now, get each rectangle
	// and add it to our inList vector.

	JSONObject root;
	root = value->AsObject();

	if (root.find(L"rects") != root.end() && root[L"rects"]->IsArray())
	{
		JSONArray array = root[L"rects"]->AsArray();
		for (unsigned int i = 0; i < array.size(); i++)
		{
			JSONValue *row = array[i];

			// Extract values of x,y,w and h.
			// Check if values actually are numeric.
			bool numeric = true;
			long x, y, w, h;
			if (row->Child(L"x")->IsNumber())
				x = row->Child(L"x")->AsNumber();
			else numeric = false;
			if (row->Child(L"y")->IsNumber())
				y = row->Child(L"y")->AsNumber();
			else numeric = false;
			if (row->Child(L"w")->IsNumber())
				w = row->Child(L"w")->AsNumber();
			else numeric = false;
			if (row->Child(L"h")->IsNumber())
				h = row->Child(L"h")->AsNumber();
			else numeric = false;

			if (!numeric)
			{
				cerr << "Fatal Error 4: Non-numeric values detected in input json. Terminating.";
				exit(4);
			}

			// Insert this input rectangle into inList.
			// CRect *rect = new CRect(i, x, y, w, h);
			// inList.push_back(*rect);
			inList.emplace_back(i, x, y, w, h);
			
			// Print this rectangle.
			inList[i].Print();
			// free(rect);

		}
	}

	// End printout of input rectangles.
	cout << endl << endl << "Intersections: " << endl;

	// Are there at least 2 object ? Otherwise, we have no intersecting rectangles
	if (inList.size() < 2)
		return false;

	// We start by 'including' the first object, because
	// an intersection requires at least 2 objests.
	// When NextCombo() will called the first time, it will mark the second object.
	// That way, we will try the first combination as the first 2 objects.

	// Yes there are 2 elements. Mark the second element, so NextCombo() will return the first 2 elements.
	vector<CRect>::iterator obj = inList.begin();
	(++obj)->SetIncluded();

	// Success.

	return true;
}

// Finds the next combination of rectangles to intersect.
// This means := this function sets/resets the 'included' of the rectangles in the next combination.

// IMPORTANT: See comments at the top of this file.

// Function returns true if the next combination can be found (and marked).
// Returns false, if there is NO next combination available. (All combinations
// have been exhausted.)
bool CSect::NextCombo()
{
	// Find the first non-included object.

	vector<CRect>::iterator obj1, obj2;
	bool found = false;
	for (obj1 = inList.begin(); obj1 != inList.end(); obj1++)
	{
		if (!(obj1->Included()))
		{
			// We found the first non-included object.
			found = true;

			// Set this object to included.
			obj1->SetIncluded();

			// Set all objects before it to not-included.
			for (obj2 = inList.begin(); obj2 != obj1; obj2++)
				obj2->ResetIncluded();

			break;
		}
	}

	if (found)
		return true;
	else
		return false;
}

// Returns a set of indexes of input rectangles included in the current combination.
set<unsigned int> CSect::GetSet()
{
	set<unsigned int> s;
		// We will collect the indexes of the 'included' rectangles in this set.
	
	vector<CRect>::iterator obj;
	unsigned int i = 0;

	for (obj = inList.begin(); obj != inList.end(); obj++)
	{
		if (obj->Included())
			s.insert(i);
		i++;
	}

	return s;
}

/******************************************************************************************************************
IMPORTANT: We can think of several methods to find intersection of a given set of n rectangles.

***Method 1***: Start with the first 2 rectangles in the set. Find their intersection Isub12. Add rectangle 3
to the mix, and find intersection of Isub12 and I3. Keep on doing it until we have exhausted all rectangles in the
set. If at any time in iteration the rectangles don't intersect, we return 'not-found'.

The above method will re-calculate intersection of rectangle 1 and 2, for example, for any combination that contains
1 and 2. Thus lot of repeat computations.

***Method 2***: We hash-store all the previously tried combinations in a map - map<set<unsigned int>, CRect>. Here 
set<unsigned int> is the combination of input rectangles. CRect is the resulting intersection.

It is possible that some combinations may not have a valid intersection. We store this case too. It has value.
For example if {1,2} are not intersecting, any further combinations such as {1,2,3}, {1,2,4} are not going
to intersect either. So, we save repeat computations in this case too.

Then each time, for a given set {1,2,3,4}, we always remove the last element 4 and check the value of {1,2,3}.
(We remove the last element, because we generate combinations from the front. Thus for the given set {1,2,3,4},
{1,2,3} is more likely to be in the hash store. {2,3,4} is less likely to be in hash store.)

This method saves a lot of re-computing, but it has a memory problem.

Consider the input of 1000 rectangles. For this, the hash storage is of the order of magnitude
(2**1000). Can't do this.

***Method 3***: This is a compromise between Method 1 and Method 2. We hash-store prior
calculations only up to X input rectangles, say X=30. That's about 1 Gig elements in the hash table.
What the value of X is will depend on the processing environment (CPU, memory) and current load.
So, instead of hardcoding X, we would store it in a config file.

Unfortunately, this is a coding test and we do not want to muddy the waters too much by adding code that is farther
from the gist of the requirements. So, albeit we had started to implement Method 2/3, we backed out of it and decided
to stick with Method 1. (We=I=Mike Deo. I typically write comments using 'we' because I am part of a team.)
******************************************************************************************************************/


// Gets the intersection rectangle for the given set of input rectangles.
// The argument, s, is the set of indexes of the input rectangles in inList.
// The second argument, found, returns a value indicating if we found a valid intersection.
//		If true, we found a valid intersection.
//			In this case, the intersection rectangle is sent back in the return value.
//		If false, we did not find a valid intersection.

CRect CSect::GetXsect(set<unsigned int> s, bool& found)
{
	CRect rect(0,0,0,0,0);	// We use this as a stand-by output when needed.

	// We need minimum 2 rectangles to find intersection.
	// Some of the combinations we will send, will have less that 2 rectangles.
	// These are valid combinations. So, do not exit in case of this error.
	if (s.size() < 2)
	{
		found = false;
		return rect;
	}

	// From here on, s has 2 or more elements.
	// We iteratively take 2 elements from set s, get the corresponding rectangles and find their intersection.
	// If found a valid intersection, we keep going by adding the next ractangle if it exists.
	// If a valid intersection is not found, we terminate and return with found=false.
	//		This is because if, for example, rectangles {1,2,3} are not intersecting,
	//		any further combination eg. rectangles {1,2,3,4} won't have a valid intersection either.
	
	found = true;

	// We use the variable rect as the resulting intersection at any stage. We keep on intersecting it
	// the next ractangle in the set iteratively.
	// When a valid intersection is not found in any iteration, we reset 'found' to false
	// and return. If we come out of the below while loop without finding an invalid case,
	// 'found' will remain true and we will return rect as the intersection of ALL rectangles in this set.

	// Get the first 2 rectangles.
	unsigned int first = *(s.begin());
	s.erase(first);
	rect = inList.at(first);

	// Each time, insert the included rectangles in rect.components.
	// We need this for output.
	rect.components.insert(first);

	unsigned int nextIndex = *(s.begin());
	s.erase(nextIndex);
	CRect nextRect = inList.at(nextIndex);
		// We use nextRect as the next rectangle in the set at any stage of the iteration.
	rect.components.insert(nextIndex);

	// Now, find the intersection of first 2 rectangles.
	CRect rectXsect = GetXsect2(rect, nextRect, found);

	if (!found)
		// The first 2 rectangles in this set don't intersect. That means the entire
		// set does not have a valid intersection.
		// No point in going further.
		return rect;

	// Get the coordinates of intersection back into rect.
	// We do this so that the rect.components will be preserved.
	rect.x1 = rectXsect.x1; rect.x2 = rectXsect.x2;
	rect.y1 = rectXsect.y1; rect.y2 = rectXsect.y2;

	// Now, keep trying the remaining elements in this set one by one.
	while (s.size()>0)
	{
		// Get the next rectangle.
		nextIndex = *(s.begin());
		s.erase(nextIndex);
		nextRect = inList.at(nextIndex);
		rect.components.insert(nextIndex);

		// Intersect the next rectangle with rect.
		rectXsect = GetXsect2(rect, nextRect, found);

		if (!found)
			// This set does not have a valid intersection
			return rect;

		// Get the coordinates of intersection back into rect.
		rect.x1 = rectXsect.x1; rect.x2 = rectXsect.x2;
		rect.y1 = rectXsect.y1; rect.y2 = rectXsect.y2;
	}

	// If we come here, that means that all the rectangles in this set have a valid
	// intersection, which is already in rect. We will return success.

	// We should also populate CPoint (which is the top-left point) and
	// width and height.
	rect.x = rect.x1;
	rect.y = rect.y1;
	rect.width = (rect.x2 - rect.x1);
	rect.height = (rect.y2 - rect.y1);

	return rect;
}

// Gets the intersecting rectangle of rect1 and rect2.
// The last argument, found, is set to true if there is valid intersection.
// Else it is set to false before returning.
CRect CSect::GetXsect2(const CRect rect1, const CRect rect2, bool& found)
{
	CRect rect(0, 0, 0, 0, 0);	// Output value.
	found = true;

	// It is IMPORTANT to note that for an input rectangle, x1 < x2 AND y1 < y2.
	// That's the way we have assigned x1,x2,y1,y2 when constructing input rectangles
	// and also in this function for intersecting rectangles.

	rect.x1 = max(rect1.x1, rect2.x1);
	rect.x2 = min(rect1.x2, rect2.x2);
	rect.y1 = max(rect1.y1, rect2.y1);
	rect.y2 = min(rect1.y2, rect2.y2);

	if (
		(rect.x1 >= rect.x2)	// rect1 and rect2 are not intersecting along x axis or just touching.
								// We consider 'just touching' case to be invalid.
		||
		(rect.y1 >= rect.y2)	// rect1 and rect2 are not intersecting along y axis or just touching.
		)
		found = false;

	return rect;
}

#ifdef _DEBUG
// Prints the state of the set for debugging purposes.
void CSect::dbgPrint()
{
	cerr << "State of input rectangles -" << endl;
	vector<CRect>::iterator obj;
	for (obj = inList.begin(); obj != inList.end(); obj++)
		obj->dbgPrint();
}
#endif