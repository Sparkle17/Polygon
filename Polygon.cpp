#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Geometry.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "Polygon projector" << endl;
	cout << argc << endl;
	if (argc < 4) {
		cout <<
			"usage: Polygon.exe X Y Z" << endl <<
			"where x, y, z - coordinates of the projected point" << endl <<
			"Polygon.txt - text file with coordinates of the polygon points" << endl <<
			"Polygon.txt file format:" << endl <<
			"first line: number of vertexes (n)" << endl <<
			"next n lines each contains three coordinates (x, y, z) separated by space" << endl;
		return 0;
	}

	try {
		ifstream inputFile("Polygon.txt", ifstream::in);
		if (inputFile.bad())
			throw runtime_error("can't open vertex file Polygon.txt");

		int numberOfPoints;
		inputFile >> numberOfPoints;
		if (numberOfPoints < 2)
			throw runtime_error("polygon must contain at least 2 points");
			
		vector<Point> points(numberOfPoints);
		for (auto& p : points) {
			if (inputFile.eof())
				throw runtime_error("invalid input file format");
			inputFile >> p.x >> p.y >> p.z;
		}

		Polygon poly(move(points));
		Point projected{ stod(argv[1]), stod(argv[2]), stod(argv[3]) };
		auto result = poly.calculateProjection(projected);
		cout << "Number of solutions: " << result.size() << endl;
		for (auto& sol : result)
			cout << sol.to_string() << endl;
	}
	catch (exception& e) {
		cout << "exception caught: " << e.what() << endl;
		return -1;
	}
	catch (...) {
		cout << "unknown exception caught" << endl;
		return -1;
	}
	return 0;
}
