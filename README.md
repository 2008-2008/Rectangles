DESCRIPTION
-----------
This project reads an input set of rectangles in JSON format and
finds the intersecting rectangle for all possible combinations
of input rectangles.

A sample input file is included - rectangles.json.


CREATE PROJECT DIRECTORY
------------------------
This project was originally built in Visual C++ 2013.

This solution uses SimpleJSON to read in the input rectangles.
This is at https://github.com/simplejson/simplejson
Copy the following files from there to your project directory -
	- JSON.h, JSONValue.h, JSON.cpp, JSONValue.cpp.

Copy the project files from here into your project's directory.


TO COMPILE
----------
This project was originally built in Visual C++ 2013.

You can use any IDE to build the project.

The main() function is in file rectangles.cpp.



TO RUN
------
From command line, or from your IDE (I used Visual C++), run this command

rectangles.exe < rectangles.json > stdout.txt 2> stderr.txt

